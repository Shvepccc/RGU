#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_MSG_SIZE 256
#define MAX_GUID_SIZE 37
#define STATUS_QUEUE_ID 1235
#define MAX_PENDING 100

// Структуры для паттернов
typedef struct ValidationResult {
    char errors[10][100];
    int error_count;
} ValidationResult;

typedef struct ValidationCommand {
    void (*execute)(const char* data, ValidationResult* result);
    struct ValidationCommand* next;
} ValidationCommand;

typedef struct ValidationChain {
    ValidationCommand* head;
    ValidationCommand* tail;
} ValidationChain;

// Структуры для очередей
struct RequestMsg {
    long mtype;
    struct {
        int priority;
        char data[MAX_MSG_SIZE];
        char guid[MAX_GUID_SIZE];
        int status; // 0=queued, 1=processing, 2=completed
    } content;
};

struct StatusMsg {
    long mtype;
    struct {
        char guid[MAX_GUID_SIZE];
        int status;
        char result_file[256];
    } content;
};

// Глобальные переменные
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
ValidationChain* validation_chain = NULL;
int request_queue_id;
int status_queue_id;

// Функции для GUID v1
void generate_guid_v1(char* guid) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    // GUID v1 основан на времени
    unsigned long long timestamp = (unsigned long long)tv.tv_sec * 10000000ULL + 
                                  tv.tv_usec * 10ULL + 0x01B21DD213814000ULL;
    
    // Добавляем случайность
    unsigned int clock_seq = rand() & 0x3FFF;
    unsigned int node = (rand() << 16) | rand();
    
    snprintf(guid, MAX_GUID_SIZE,
             "%08llx-%04llx-%04llx-%04x-%08x%04x",
             timestamp >> 32,
             (timestamp >> 16) & 0xFFFF,
             timestamp & 0xFFFF,
             clock_seq | 0x8000, // версия 1
             node >> 16, node & 0xFFFF);
}

// Реализация паттерна "Цепочка обязанностей"
ValidationChain* create_validation_chain() {
    ValidationChain* chain = malloc(sizeof(ValidationChain));
    chain->head = NULL;
    chain->tail = NULL;
    return chain;
}

void add_validator(ValidationChain* chain, void (*validator)(const char*, ValidationResult*)) {
    ValidationCommand* cmd = malloc(sizeof(ValidationCommand));
    cmd->execute = validator;
    cmd->next = NULL;
    
    if (chain->head == NULL) {
        chain->head = cmd;
        chain->tail = cmd;
    } else {
        chain->tail->next = cmd;
        chain->tail = cmd;
    }
}

void execute_validation_chain(ValidationChain* chain, const char* data, ValidationResult* result) {
    ValidationCommand* current = chain->head;
    while (current != NULL) {
        current->execute(data, result);
        current = current->next;
    }
}

// Конкретные валидаторы (паттерн "Команда")
void validate_not_empty(const char* data, ValidationResult* result) {
    if (data == NULL || strlen(data) == 0) {
        strcpy(result->errors[result->error_count], "error: empty string");
        result->error_count++;
    }
}

void validate_format(const char* data, ValidationResult* result) {
    if (strchr(data, ';') == NULL) {
        strcpy(result->errors[result->error_count], "error: expected ';'");
        result->error_count++;
    }
}

void validate_age(const char* data, ValidationResult* result) {
    char* sep = strchr(data, ';');
    if (sep != NULL) {
        int age = atoi(sep + 1);
        if (age == 0 && strcmp(sep + 1, "0") != 0) {
            strcpy(result->errors[result->error_count], "error: age is not a number");
            result->error_count++;
        } else if (age < 0 || age > 120) {
            strcpy(result->errors[result->error_count], "error: age out of range");
            result->error_count++;
        }
    }
}

// Функции для работы с очередями
int create_queue(key_t key) {
    int qid = msgget(key, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("msgget");
        exit(1);
    }
    return qid;
}

void send_status_update(const char* guid, int status, const char* result_file) {
    struct StatusMsg status_msg;
    status_msg.mtype = 2; // Тип для статусных сообщений
    
    strncpy(status_msg.content.guid, guid, MAX_GUID_SIZE);
    status_msg.content.status = status;
    if (result_file != NULL) {
        strncpy(status_msg.content.result_file, result_file, 256);
    } else {
        status_msg.content.result_file[0] = '\0';
    }
    
    if (msgsnd(status_queue_id, &status_msg, 
               sizeof(status_msg.content), 0) == -1) {
        perror("msgsnd status");
    }
}

// Функция обработки запроса
void* process_request(void* arg) {
    struct RequestMsg* request = (struct RequestMsg*)arg;
    
    // Обновляем статус на "обрабатывается"
    send_status_update(request->content.guid, 1, NULL);
    
    // Выполняем валидацию
    ValidationResult result;
    result.error_count = 0;
    memset(result.errors, 0, sizeof(result.errors));
    
    execute_validation_chain(validation_chain, request->content.data, &result);
    
    // Сохраняем результат
    char path[256];
    snprintf(path, sizeof(path), "results/%s.txt", request->content.guid);
    
    mkdir("results", 0700);
    
    FILE* f = fopen(path, "w");
    if (f) {
        if (result.error_count == 0) {
            // Извлекаем имя и возраст
            char* sep = strchr(request->content.data, ';');
            if (sep) {
                *sep = '\0';
                fprintf(f, "name: %s\nage: %s\n", 
                        request->content.data, sep + 1);
                *sep = ';'; // восстанавливаем строку
            }
            fprintf(f, "validation: passed\n");
        } else {
            fprintf(f, "validation errors:\n");
            for (int i = 0; i < result.error_count; i++) {
                fprintf(f, "%s\n", result.errors[i]);
            }
            fprintf(f, "total errors: %d\n", result.error_count);
        }
        fclose(f);
    }
    
    // Обновляем статус на "завершено"
    send_status_update(request->content.guid, 2, path);
    
    printf("Processed request: GUID=%s, Priority=%d\n", 
           request->content.guid, request->content.priority);
    
    free(request);
    return NULL;
}

// Поток для обработки запросов статуса
void* status_monitor_thread(void* arg) {
    while (1) {
        struct StatusMsg status_request;
        
        // Получаем запросы статуса (mtype = 1 - запрос статуса)
        ssize_t r = msgrcv(status_queue_id, &status_request, 
                          sizeof(status_request.content), 1, IPC_NOWAIT);
        
        if (r > 0) {
            // Проверяем наличие файла результата
            char path[256];
            snprintf(path, sizeof(path), "results/%s.txt", 
                    status_request.content.guid);
            
            int file_status = 0;
            if (access(path, F_OK) == 0) {
                file_status = 2; // завершено
            } else {
                // Можно добавить проверку очереди на обработку
                file_status = 0; // в очереди
            }
            
            // Отправляем ответ
            struct StatusMsg response;
            response.mtype = 3; // Ответ на запрос статуса
            strcpy(response.content.guid, status_request.content.guid);
            response.content.status = file_status;
            if (file_status == 2) {
                strcpy(response.content.result_file, path);
            } else {
                response.content.result_file[0] = '\0';
            }
            
            msgsnd(status_queue_id, &response, 
                   sizeof(response.content), 0);
        }
        
        usleep(100000); // Проверяем каждые 100ms
    }
    
    return NULL;
}

int main() {
    srand(time(NULL));
    
    // Создаем директорию для результатов
    if (mkdir("results", 0700) == -1 && errno != EEXIST) {
        perror("mkdir");
        return 1;
    }
    
    // Создаем цепочку валидации
    validation_chain = create_validation_chain();
    add_validator(validation_chain, validate_not_empty);
    add_validator(validation_chain, validate_format);
    add_validator(validation_chain, validate_age);
    
    // Создаем очереди
    request_queue_id = create_queue(1234);
    status_queue_id = create_queue(STATUS_QUEUE_ID);
    
    printf("Server started\n");
    printf("Request queue ID: %d\n", request_queue_id);
    printf("Status queue ID: %d\n", status_queue_id);
    printf("Results directory: results/\n\n");
    
    // Запускаем поток для мониторинга статусов
    pthread_t status_thread;
    pthread_create(&status_thread, NULL, status_monitor_thread, NULL);
    
    // Основной цикл обработки запросов
    while (1) {
        struct RequestMsg* request = malloc(sizeof(struct RequestMsg));
        
        // Получаем запрос
        ssize_t r = msgrcv(request_queue_id, request, 
                          sizeof(request->content), 1, 0);
        if (r == -1) {
            perror("msgrcv request");
            free(request);
            continue;
        }
        
        // Генерируем GUID для запроса
        generate_guid_v1(request->content.guid);
        
        // Обновляем статус на "в очереди"
        send_status_update(request->content.guid, 0, NULL);
        
        printf("Received request, assigned GUID: %s\n", request->content.guid);
        
        // Обрабатываем запрос в отдельном потоке
        pthread_t worker_thread;
        pthread_create(&worker_thread, NULL, process_request, request);
        pthread_detach(worker_thread);
    }
    
    // Очистка (никогда не выполнится в бесконечном цикле)
    pthread_mutex_destroy(&queue_mutex);
    msgctl(request_queue_id, IPC_RMID, NULL);
    msgctl(status_queue_id, IPC_RMID, NULL);
    
    return 0;
}