#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define NUM_SEMAPHORES 3
#define KEY 1234

void handle_error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    int semid;
    struct sembuf sem_operations[NUM_SEMAPHORES];
    int i;
    
    semid = semget(KEY, NUM_SEMAPHORES, IPC_CREAT | 0666);
    if (semid == -1)
    {
        handle_error("Failed to create semaphore set");
    }
    
    for (i = 0; i < NUM_SEMAPHORES; i++)
    {
        if (semctl(semid, i, SETVAL, 1) == -1)
        {
            semctl(semid, 0, IPC_RMID);
            handle_error("Failed to initialize semaphore");
        }
    }
    
    for (i = 0; i < NUM_SEMAPHORES; i++)
    {
        sem_operations[i].sem_num = i;
        sem_operations[i].sem_op = -1;
        sem_operations[i].sem_flg = 0;
    }
    
    printf("Locking all semaphores...\n");
    if (semop(semid, sem_operations, NUM_SEMAPHORES) == -1)
    {
        semctl(semid, 0, IPC_RMID);
        handle_error("Failed to lock semaphores");
    }
    
    printf("All semaphores locked. Working...\n");
    sleep(2);
    
    for (i = 0; i < NUM_SEMAPHORES; i++)
    {
        sem_operations[i].sem_num = i;
        sem_operations[i].sem_op = 1;
        sem_operations[i].sem_flg = 0;
    }
    
    printf("Unlocking all semaphores...\n");
    if (semop(semid, sem_operations, NUM_SEMAPHORES) == -1)
    {
        semctl(semid, 0, IPC_RMID);
        handle_error("Failed to unlock semaphores");
    }
    
    printf("All semaphores unlocked.\n");
    
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        handle_error("Failed to remove semaphore set");
    }
    
    return 0;
}