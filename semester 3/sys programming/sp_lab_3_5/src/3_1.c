#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define SHM_SIZE 1024
#define MAX_USERS 10
#define MAX_COMMANDS 100

typedef enum {
    LEFT_BANK,
    RIGHT_BANK,
    IN_BOAT
} Location;

typedef struct {
    Location farmer;
    Location wolf;
    Location goat;
    Location cabbage;
    int boat_occupant; // 0 = пусто, 1 = волк, 2 = коза, 3 = капуста
    char message[256];
    int game_over;
    int success;
} GameState;

typedef struct {
    GameState state;
    int user_count;
    int active_users[MAX_USERS];
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

int check_danger() {
    if (shm->state.wolf == shm->state.goat && 
        shm->state.farmer != shm->state.wolf) {
        return 1;
    }
    if (shm->state.goat == shm->state.cabbage && 
        shm->state.farmer != shm->state.goat) {
        return 1;
    }
    return 0;
}

int check_win() {
    return (shm->state.wolf == RIGHT_BANK &&
            shm->state.goat == RIGHT_BANK &&
            shm->state.cabbage == RIGHT_BANK &&
            shm->state.farmer == RIGHT_BANK);
}

void init_game() {
    shm->state.farmer = LEFT_BANK;
    shm->state.wolf = LEFT_BANK;
    shm->state.goat = LEFT_BANK;
    shm->state.cabbage = LEFT_BANK;
    shm->state.boat_occupant = 0;
    shm->state.game_over = 0;
    shm->state.success = 0;
    shm->game_in_progress = 1;
    strcpy(shm->state.message, "Game started. All on left bank.");
}

void reset_game() {
    printf("Resetting game...\n");
    init_game();
}

void process_command(const char* command, int user_id) {
    if (strcmp(command, "reset") == 0) {
        reset_game();
        strcpy(shm->response, "Game reset to initial state");
        return;
    }
    
    if (shm->state.game_over && !shm->reset_requested) {
        snprintf(shm->response, sizeof(shm->response), 
                "Game already finished. %s Type 'reset' to start new game.", 
                shm->state.success ? "You won!" : "You lost!");
        return;
    }
    
    if (!shm->game_in_progress) {
        init_game();
    }
    
    char cmd[50], obj[50];
    int parsed = sscanf(command, "%49s %49s", cmd, obj);
    
    if (parsed < 1) {
        strcpy(shm->response, "Invalid command format");
        return;
    }
    
    if (strcmp(cmd, "take") == 0) {
        if (parsed < 2) {
            strcpy(shm->response, "Usage: take <wolf|goat|cabbage>");
            return;
        }
        
        if (shm->state.boat_occupant != 0) {
            strcpy(shm->response, "Boat is already occupied");
            return;
        }
        
        Location* target_obj = NULL;
        const char* obj_name = "";
        
        if (strcmp(obj, "wolf") == 0) {
            target_obj = &shm->state.wolf;
            obj_name = "wolf";
        } else if (strcmp(obj, "goat") == 0) {
            target_obj = &shm->state.goat;
            obj_name = "goat";
        } else if (strcmp(obj, "cabbage") == 0) {
            target_obj = &shm->state.cabbage;
            obj_name = "cabbage";
        } else {
            strcpy(shm->response, "Unknown object. Use wolf, goat or cabbage");
            return;
        }
        
        if (*target_obj != shm->state.farmer) {
            snprintf(shm->response, sizeof(shm->response), 
                    "%s is not on the same bank as farmer", obj_name);
            return;
        }
        
        *target_obj = IN_BOAT;
        if (strcmp(obj, "wolf") == 0) shm->state.boat_occupant = 1;
        else if (strcmp(obj, "goat") == 0) shm->state.boat_occupant = 2;
        else shm->state.boat_occupant = 3;
        
        snprintf(shm->response, sizeof(shm->response), 
                "Farmer took %s into boat", obj_name);
    }
    else if (strcmp(cmd, "put") == 0) {
        if (shm->state.boat_occupant == 0) {
            strcpy(shm->response, "Boat is empty");
            return;
        }
        
        Location* target_obj = NULL;
        const char* obj_name = "";
        
        switch(shm->state.boat_occupant) {
            case 1: target_obj = &shm->state.wolf; obj_name = "wolf"; break;
            case 2: target_obj = &shm->state.goat; obj_name = "goat"; break;
            case 3: target_obj = &shm->state.cabbage; obj_name = "cabbage"; break;
        }
        
        *target_obj = shm->state.farmer;
        shm->state.boat_occupant = 0;
        
        snprintf(shm->response, sizeof(shm->response), 
                "Farmer put %s on %s bank", 
                obj_name, 
                shm->state.farmer == LEFT_BANK ? "left" : "right");
    }
    else if (strcmp(cmd, "move") == 0) {
        shm->state.farmer = (shm->state.farmer == LEFT_BANK) ? RIGHT_BANK : LEFT_BANK;
        
        if (shm->state.boat_occupant != 0) {
            Location* target_obj = NULL;
            
            switch(shm->state.boat_occupant) {
                case 1: target_obj = &shm->state.wolf; break;
                case 2: target_obj = &shm->state.goat; break;
                case 3: target_obj = &shm->state.cabbage; break;
            }
            
            *target_obj = shm->state.farmer;
        }
        
        snprintf(shm->response, sizeof(shm->response), 
                "Moved to %s bank", 
                shm->state.farmer == LEFT_BANK ? "left" : "right");
        
        if (check_danger()) {
            shm->state.game_over = 1;
            shm->state.success = 0;
            strcat(shm->response, " - GAME OVER! Something got eaten!");
        }
        
        if (check_win()) {
            shm->state.game_over = 1;
            shm->state.success = 1;
            strcat(shm->response, " - CONGRATULATIONS! All objects moved safely!");
        }
    }
    else {
        snprintf(shm->response, sizeof(shm->response), 
                "Unknown command: %s. Use take, put, move or reset", cmd);
    }
}

void cleanup(int sig) {
    (void)sig;
    
    printf("\nCleaning up resources...\n");
    
    if (shmdt(shm) == -1) {
        perror("shmdt failed");
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID failed");
    }
    
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID failed");
    }
    
    printf("Server shutdown complete.\n");
    exit(EXIT_SUCCESS);
}

void print_state() {
    printf("\n=== CURRENT STATE ===\n");
    printf("Farmer: %s bank\n", 
           shm->state.farmer == LEFT_BANK ? "left" : 
           shm->state.farmer == RIGHT_BANK ? "right" : "in boat");
    printf("Wolf: %s\n", 
           shm->state.wolf == LEFT_BANK ? "left bank" : 
           shm->state.wolf == RIGHT_BANK ? "right bank" : "in boat");
    printf("Goat: %s\n", 
           shm->state.goat == LEFT_BANK ? "left bank" : 
           shm->state.goat == RIGHT_BANK ? "right bank" : "in boat");
    printf("Cabbage: %s\n", 
           shm->state.cabbage == LEFT_BANK ? "left bank" : 
           shm->state.cabbage == RIGHT_BANK ? "right bank" : "in boat");
    printf("Boat occupant: %s\n", 
           shm->state.boat_occupant == 0 ? "empty" : 
           shm->state.boat_occupant == 1 ? "wolf" :
           shm->state.boat_occupant == 2 ? "goat" : "cabbage");
    printf("Game status: %s\n", 
           shm->state.game_over ? 
               (shm->state.success ? "WON" : "LOST") : 
               "IN PROGRESS");
    printf("=====================\n");
}

int main() {
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    
    shm_key = ftok("/tmp", 'R');
    sem_key = ftok("/tmp", 'S');
    
    if (shm_key == -1 || sem_key == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }
    
    shmid = shmget(shm_key, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
    
    shm = (SharedMemory*)shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    
    semid = semget(sem_key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget failed");
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL failed");
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        exit(EXIT_FAILURE);
    }
    
    memset(shm, 0, sizeof(SharedMemory));
    init_game();
    shm->user_count = 0;
    shm->reset_requested = 0;
    
    printf("River Crossing Server started\n");
    printf("Shared memory ID: %d\n", shmid);
    printf("Semaphore ID: %d\n", semid);
    printf("Commands: take <obj>, put, move, reset\n");
    printf("Waiting for client commands...\n\n");
    print_state();
    
    while (1) {
        sem_lock(semid);
        
        if (shm->command_ready) {
            printf("\n[User %d] Command: %s\n", 
                   shm->current_user_id, shm->command_buffer);
            
            process_command(shm->command_buffer, shm->current_user_id);
            
            printf("Response: %s\n", shm->response);
            print_state();
            
            shm->command_ready = 0;
            shm->response_ready = 1;
            
            if (strcmp(shm->command_buffer, "reset") == 0) {
                shm->reset_requested = 0;
            }
        }
        
        sem_unlock(semid);
        
        usleep(100000);
    }
    
    return 0;
}