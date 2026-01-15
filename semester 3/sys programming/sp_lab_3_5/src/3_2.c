#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define SHM_SIZE 1024

typedef struct {
    struct {
        int farmer;
        int wolf;
        int goat;
        int cabbage;
        int boat_occupant;
        char message[256];
        int game_over;
        int success;
    } state;
    int user_count;
    int active_users[10];
    char command_buffer[256];
    int command_ready;
    int response_ready;
    char response[256];
    int current_user_id;
    int reset_requested;
    int game_in_progress;
} SharedMemory;

key_t shm_key;
key_t sem_key;

int shmid;
int semid;
SharedMemory *shm;
int user_id;

void sem_lock(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    while (semop(sem_id, &sb, 1) == -1) {
        if (errno != EINTR) {
            perror("semop lock failed");
            exit(EXIT_FAILURE);
        }
    }
}

void sem_unlock(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop unlock failed");
        exit(EXIT_FAILURE);
    }
}

int register_user() {
    sem_lock(semid);
    
    for (int i = 0; i < 10; i++) {
        if (shm->active_users[i] == 0) {
            shm->active_users[i] = 1;
            shm->user_count++;
            user_id = i + 1;
            sem_unlock(semid);
            return user_id;
        }
    }
    
    sem_unlock(semid);
    return -1;
}

void send_command(const char* command) {
    sem_lock(semid);
    
    while (shm->command_ready) {
        sem_unlock(semid);
        usleep(10000);
        sem_lock(semid);
    }
    
    strncpy(shm->command_buffer, command, sizeof(shm->command_buffer) - 1);
    shm->command_buffer[sizeof(shm->command_buffer) - 1] = '\0';
    shm->current_user_id = user_id;
    shm->command_ready = 1;
    
    while (!shm->response_ready) {
        sem_unlock(semid);
        usleep(10000); // 10 мс
        sem_lock(semid);
    }
    
    printf("Server: %s\n", shm->response);
    shm->response_ready = 0;
    
    sem_unlock(semid);
}

void process_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open command file");
        return;
    }
    
    char line[256];
    int line_num = 1;
    
    printf("Processing commands from %s\n", filename);
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0 || line[0] == '#') {
            line_num++;
            continue;
        }
        
        printf("\n[Line %d] Executing: %s\n", line_num, line);
        send_command(line);
        
        sem_lock(semid);
        if (shm->state.game_over) {
            printf("Game finished. %s\n", 
                   shm->state.success ? "You won!" : "You lost!");
            sem_unlock(semid);
            
            printf("Type 'reset' to start new game or press Enter to exit: ");
            char choice[10];
            fgets(choice, sizeof(choice), stdin);
            choice[strcspn(choice, "\n")] = '\0';
            
            if (strcmp(choice, "reset") == 0) {
                send_command("reset");
            }
            break;
        }
        sem_unlock(semid);
        
        sleep(1);
        line_num++;
    }
    
    fclose(file);
}

void interactive_mode() {
    printf("\nInteractive mode. Commands:\n");
    printf("  take <wolf|goat|cabbage>\n");
    printf("  put\n");
    printf("  move\n");
    printf("  reset - start new game\n");
    printf("  status - show current state\n");
    printf("  exit - quit\n\n");
    
    char command[256];
    
    while (1) {
        printf("client> ");
        fflush(stdout);
        
        if (!fgets(command, sizeof(command), stdin)) {
            break;
        }
        
        command[strcspn(command, "\n")] = '\0';
        
        if (strlen(command) == 0) {
            continue;
        }
        
        if (strcmp(command, "exit") == 0) {
            break;
        }
        
        if (strcmp(command, "status") == 0) {
            sem_lock(semid);
            printf("\n=== CURRENT STATE ===\n");
            printf("Farmer: %s bank\n", 
                   shm->state.farmer == 0 ? "left" : 
                   shm->state.farmer == 1 ? "right" : "in boat");
            printf("Wolf: %s\n", 
                   shm->state.wolf == 0 ? "left bank" : 
                   shm->state.wolf == 1 ? "right bank" : "in boat");
            printf("Goat: %s\n", 
                   shm->state.goat == 0 ? "left bank" : 
                   shm->state.goat == 1 ? "right bank" : "in boat");
            printf("Cabbage: %s\n", 
                   shm->state.cabbage == 0 ? "left bank" : 
                   shm->state.cabbage == 1 ? "right bank" : "in boat");
            printf("Boat occupant: %s\n", 
                   shm->state.boat_occupant == 0 ? "empty" : 
                   shm->state.boat_occupant == 1 ? "wolf" :
                   shm->state.boat_occupant == 2 ? "goat" : "cabbage");
            printf("Message: %s\n", shm->state.message);
            printf("Game status: %s\n", 
                   shm->state.game_over ? 
                       (shm->state.success ? "WON" : "LOST") : 
                       "IN PROGRESS");
            printf("=====================\n");
            sem_unlock(semid);
            continue;
        }
        
        send_command(command);
        
        // Проверяем, не закончилась ли игра
        sem_lock(semid);
        if (shm->state.game_over && strcmp(command, "reset") != 0) {
            printf("Game finished. %s\n", 
                   shm->state.success ? "You won!" : "You lost!");
            printf("Use 'reset' to start new game.\n");
        }
        sem_unlock(semid);
    }
}

void cleanup() {
    printf("Cleaning up client resources...\n");
    
    if (shm) {
        sem_lock(semid);
        if (user_id > 0 && user_id <= 10) {
            shm->active_users[user_id - 1] = 0;
            shm->user_count--;
        }
        sem_unlock(semid);
        
        if (shmdt(shm) == -1) {
            perror("shmdt failed");
        }
    }
    
    printf("Client shutdown complete.\n");
}

int main(int argc, char* argv[]) {
    atexit(cleanup);
    
    shm_key = ftok("/tmp", 'R');
    sem_key = ftok("/tmp", 'S');
    
    if (shm_key == -1 || sem_key == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }
    
    shmid = shmget(shm_key, sizeof(SharedMemory), 0666);
    if (shmid == -1) {
        perror("shmget failed (is server running?)");
        exit(EXIT_FAILURE);
    }
    
    shm = (SharedMemory*)shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    
    semid = semget(sem_key, 1, 0666);
    if (semid == -1) {
        perror("semget failed");
        shmdt(shm);
        exit(EXIT_FAILURE);
    }
    
    user_id = register_user();
    if (user_id == -1) {
        printf("Server is full. Maximum 10 users.\n");
        shmdt(shm);
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to server as User %d\n", user_id);
    
    sem_lock(semid);
    if (shm->state.game_over) {
        printf("Previous game finished. Auto-resetting...\n");
        sem_unlock(semid);
        send_command("reset");
    } else {
        sem_unlock(semid);
    }
    
    if (argc == 2) {
        process_file(argv[1]);
    } else {
        interactive_mode();
    }
    
    return 0;
}