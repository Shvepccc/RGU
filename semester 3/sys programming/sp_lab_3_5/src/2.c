#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef enum {
    NONE,
    WOMEN,
    MEN
} Gender;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t women_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t men_cond = PTHREAD_COND_INITIALIZER;

static int inside = 0;
static int capacity_limit;
static Gender current_gender = NONE;

void woman_wants_to_enter() {
    pthread_mutex_lock(&mutex);
    
    while (current_gender == MEN || inside >= capacity_limit) {
        pthread_cond_wait(&women_cond, &mutex);
    }
    
    if (current_gender == NONE) {
        current_gender = WOMEN;
    }
    
    inside++;
    printf("Woman entered, inside: %d\n", inside);
    
    pthread_mutex_unlock(&mutex);
}

void man_wants_to_enter() {
    pthread_mutex_lock(&mutex);
    
    while (current_gender == WOMEN || inside >= capacity_limit) {
        pthread_cond_wait(&men_cond, &mutex);
    }
    
    if (current_gender == NONE) {
        current_gender = MEN;
    }
    
    inside++;
    printf("Man entered, inside: %d\n", inside);
    
    pthread_mutex_unlock(&mutex);
}

void woman_leaves() {
    pthread_mutex_lock(&mutex);
    
    inside--;
    printf("Woman left, inside: %d\n", inside);
    
    if (inside == 0) {
        current_gender = NONE;
        pthread_cond_broadcast(&men_cond);
    }
    
    pthread_cond_signal(&women_cond);
    pthread_mutex_unlock(&mutex);
}

void man_leaves() {
    pthread_mutex_lock(&mutex);
    
    inside--;
    printf("Man left, inside: %d\n", inside);
    
    if (inside == 0) {
        current_gender = NONE;
        pthread_cond_broadcast(&women_cond);
    }
    
    pthread_cond_signal(&men_cond);
    pthread_mutex_unlock(&mutex);
}

void* woman_thread(void* arg) {
    (void)arg;
    
    int id = *((int*)arg);
    printf("Woman %d wants to enter\n", id);
    
    woman_wants_to_enter();
    sleep(1 + rand() % 3);
    woman_leaves();
    
    return NULL;
}

void* man_thread(void* arg) {
    (void)arg;
    
    int id = *((int*)arg);
    printf("Man %d wants to enter\n", id);
    
    man_wants_to_enter();
    sleep(1 + rand() % 3);
    man_leaves();
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    capacity_limit = atoi(argv[1]);
    if (capacity_limit <= 0) {
        printf("Error: Capacity must be positive integer\n");
        return EXIT_FAILURE;
    }
    
    srand((unsigned int)time(NULL));
    
    pthread_t threads[20];
    int thread_ids[20];
    
    for (int i = 0; i < 20; i++) {
        thread_ids[i] = i + 1; 
    }
    
    for (int i = 0; i < 20; i++) {
        int j = rand() % 20;
        int temp = thread_ids[i];
        thread_ids[i] = thread_ids[j];
        thread_ids[j] = temp;
    }
    
    for (int i = 0; i < 20; i++) {
        if (rand() % 2 == 0) {
            if (pthread_create(&threads[i], NULL, woman_thread, &thread_ids[i]) != 0) {
                printf("Error creating woman thread %d\n", i);
                return EXIT_FAILURE;
            }
            printf("Created woman thread with ID %d\n", thread_ids[i]);
        } else {
            if (pthread_create(&threads[i], NULL, man_thread, &thread_ids[i]) != 0) {
                printf("Error creating man thread %d\n", i);
                return EXIT_FAILURE;
            }
            printf("Created man thread with ID %d\n", thread_ids[i]);
        }
        
        usleep((rand() % 500) * 1000);
    }
    
    for (int i = 0; i < 20; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nAll threads completed successfully\n");
    printf("Bathroom simulation completed with capacity N = %d\n", capacity_limit);
    
    return EXIT_SUCCESS;
}