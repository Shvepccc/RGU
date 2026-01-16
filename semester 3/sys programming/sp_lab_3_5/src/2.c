#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <dispatch/dispatch.h>  // Для семафоров в macOS

typedef enum {
    NONE,
    WOMEN,
    MEN
} Gender;

// Семафоры для macOS
static dispatch_semaphore_t gender_mutex;      // Защита переменных gender и counter
static dispatch_semaphore_t women_switch;      // Переключатель для женщин
static dispatch_semaphore_t men_switch;        // Переключатель для мужчин
static dispatch_semaphore_t capacity_sem;      // Ограничение вместимости

// Переменные состояния
static volatile Gender current_gender = NONE;
static volatile int women_waiting = 0;
static volatile int men_waiting = 0;
static volatile int inside_count = 0;

static int capacity_limit;

// Структура для передачи данных в поток
typedef struct {
    int id;
    int gender;  // 0 = женщина, 1 = мужчина
} PersonData;

void init_semaphores() {
    gender_mutex = dispatch_semaphore_create(1);  // Бинарный семафор как мьютекс
    women_switch = dispatch_semaphore_create(0);
    men_switch = dispatch_semaphore_create(0);
    capacity_sem = dispatch_semaphore_create(0);  // Инициализируем нулем
}

void cleanup_semaphores() {
    dispatch_release(gender_mutex);
    dispatch_release(women_switch);
    dispatch_release(men_switch);
    dispatch_release(capacity_sem);
}

void woman_wants_to_enter(int id) {
    // Увеличиваем счетчик ожидающих женщин
    dispatch_semaphore_wait(gender_mutex, DISPATCH_TIME_FOREVER);
    women_waiting++;
    
    // Если ванная пуста или уже используются женщинами и есть место
    if ((current_gender == NONE || current_gender == WOMEN) && inside_count < capacity_limit) {
        if (current_gender == NONE) {
            current_gender = WOMEN;
        }
        women_waiting--;
        inside_count++;
        printf("Woman %d entered, inside: %d, men waiting: %d\n", id, inside_count, men_waiting);
        
        // Даем доступ к capacity семафору для следующего посетителя
        dispatch_semaphore_signal(capacity_sem);
        dispatch_semaphore_signal(gender_mutex);
    } else {
        dispatch_semaphore_signal(gender_mutex);
        
        // Ждем разрешения войти
        dispatch_semaphore_wait(women_switch, DISPATCH_TIME_FOREVER);
        
        // После получения разрешения
        dispatch_semaphore_wait(gender_mutex, DISPATCH_TIME_FOREVER);
        women_waiting--;
        inside_count++;
        printf("Woman %d entered, inside: %d, men waiting: %d\n", id, inside_count, men_waiting);
        dispatch_semaphore_signal(gender_mutex);
    }
}

void man_wants_to_enter(int id) {
    // Увеличиваем счетчик ожидающих мужчин
    dispatch_semaphore_wait(gender_mutex, DISPATCH_TIME_FOREVER);
    men_waiting++;
    
    // Если ванная пуста или уже используются мужчинами и есть место
    if ((current_gender == NONE || current_gender == MEN) && inside_count < capacity_limit) {
        if (current_gender == NONE) {
            current_gender = MEN;
        }
        men_waiting--;
        inside_count++;
        printf("Man %d entered, inside: %d, women waiting: %d\n", id, inside_count, women_waiting);
        
        // Даем доступ к capacity семафору для следующего посетителя
        dispatch_semaphore_signal(capacity_sem);
        dispatch_semaphore_signal(gender_mutex);
    } else {
        dispatch_semaphore_signal(gender_mutex);
        
        // Ждем разрешения войти
        dispatch_semaphore_wait(men_switch, DISPATCH_TIME_FOREVER);
        
        // После получения разрешения
        dispatch_semaphore_wait(gender_mutex, DISPATCH_TIME_FOREVER);
        men_waiting--;
        inside_count++;
        printf("Man %d entered, inside: %d, women waiting: %d\n", id, inside_count, women_waiting);
        dispatch_semaphore_signal(gender_mutex);
    }
}

void woman_leaves(int id) {
    dispatch_semaphore_wait(gender_mutex, DISPATCH_TIME_FOREVER);
    
    inside_count--;
    printf("Woman %d left, inside: %d\n", id, inside_count);
    
    // Если ванная пуста, проверяем, кто ждет
    if (inside_count == 0) {
        if (men_waiting > 0) {
            // Переключаемся на мужчин
            current_gender = MEN;
            // Разрешаем войти мужчинам (но не больше capacity_limit)
            int to_admit = men_waiting < capacity_limit ? men_waiting : capacity_limit;
            for (int i = 0; i < to_admit; i++) {
                dispatch_semaphore_signal(men_switch);
            }
        } else if (women_waiting > 0) {
            // Остаемся с женщинами
            current_gender = WOMEN;
            int to_admit = women_waiting < capacity_limit ? women_waiting : capacity_limit;
            for (int i = 0; i < to_admit; i++) {
                dispatch_semaphore_signal(women_switch);
            }
        } else {
            current_gender = NONE;
        }
    } else if (inside_count < capacity_limit) {
        // Есть место, можно пустить еще одного того же пола
        if (current_gender == WOMEN && women_waiting > 0) {
            dispatch_semaphore_signal(women_switch);
        } else if (current_gender == MEN && men_waiting > 0) {
            dispatch_semaphore_signal(men_switch);
        }
    }
    
    dispatch_semaphore_signal(gender_mutex);
}

void man_leaves(int id) {
    dispatch_semaphore_wait(gender_mutex, DISPATCH_TIME_FOREVER);
    
    inside_count--;
    printf("Man %d left, inside: %d\n", id, inside_count);
    
    // Если ванная пуста, проверяем, кто ждет
    if (inside_count == 0) {
        if (women_waiting > 0) {
            // Переключаемся на женщин
            current_gender = WOMEN;
            // Разрешаем войти женщинам
            int to_admit = women_waiting < capacity_limit ? women_waiting : capacity_limit;
            for (int i = 0; i < to_admit; i++) {
                dispatch_semaphore_signal(women_switch);
            }
        } else if (men_waiting > 0) {
            // Остаемся с мужчинами
            current_gender = MEN;
            int to_admit = men_waiting < capacity_limit ? men_waiting : capacity_limit;
            for (int i = 0; i < to_admit; i++) {
                dispatch_semaphore_signal(men_switch);
            }
        } else {
            current_gender = NONE;
        }
    } else if (inside_count < capacity_limit) {
        // Есть место, можно пустить еще одного того же пола
        if (current_gender == WOMEN && women_waiting > 0) {
            dispatch_semaphore_signal(women_switch);
        } else if (current_gender == MEN && men_waiting > 0) {
            dispatch_semaphore_signal(men_switch);
        }
    }
    
    dispatch_semaphore_signal(gender_mutex);
}

void* person_thread(void* arg) {
    PersonData* data = (PersonData*)arg;
    int id = data->id;
    int is_woman = data->gender;
    
    if (is_woman) {
        printf("Woman (id: %d) wants to enter\n", id);
        woman_wants_to_enter(id);
    } else {
        printf("Man (id: %d) wants to enter\n", id);
        man_wants_to_enter(id);
    }
    
    // Проводим время в ванной
    sleep(1 + rand() % 3);
    
    if (is_woman) {
        woman_leaves(id);
    } else {
        man_leaves(id);
    }
    
    free(data);
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
    
    // Инициализируем семафоры
    init_semaphores();
    
    // Инициализируем capacity_sem с максимальным количеством разрешений
    for (int i = 0; i < capacity_limit; i++) {
        dispatch_semaphore_signal(capacity_sem);
    }
    
    pthread_t threads[20];
    PersonData* thread_data[20];
    
    // Создаем перемешанный список ID
    int thread_ids[20];
    for (int i = 0; i < 20; i++) {
        thread_ids[i] = i + 1;
    }
    
    // Перемешиваем ID
    for (int i = 0; i < 20; i++) {
        int j = rand() % 20;
        int temp = thread_ids[i];
        thread_ids[i] = thread_ids[j];
        thread_ids[j] = temp;
    }
    
    // Создаем потоки
    for (int i = 0; i < 20; i++) {
        thread_data[i] = (PersonData*)malloc(sizeof(PersonData));
        thread_data[i]->id = thread_ids[i];
        thread_data[i]->gender = rand() % 2;  // Случайный пол
        
        if (pthread_create(&threads[i], NULL, person_thread, thread_data[i]) != 0) {
            printf("Error creating thread %d\n", i);
            return EXIT_FAILURE;
        }
        
        // printf("Created %s thread with ID %d\n", 
        //        thread_data[i]->gender ? "woman" : "man", 
        //        thread_data[i]->id);
        
        usleep((rand() % 500) * 1000);  // Случайная задержка между созданиями потоков
    }
    
    // Ждем завершения всех потоков
    for (int i = 0; i < 20; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nAll threads completed successfully\n");
    printf("Bathroom simulation completed with capacity N = %d\n", capacity_limit);
    
    // Очищаем семафоры
    cleanup_semaphores();
    
    return EXIT_SUCCESS;
}