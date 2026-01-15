// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ipc.h>
// #include <sys/msg.h>
// #include <unistd.h>
// #include <sys/time.h>
// #include <time.h>
// #include <errno.h>
// #include <sys/stat.h>

// #define MAX 256

// struct msgbuf
// {
//     long mtype;
//     char mtext[MAX];
// };

// void guid(char *out, size_t n)
// {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     unsigned int seed = tv.tv_sec ^ tv.tv_usec ^ getpid() ^ (rand() << 16);
//     srand(seed);
    
//     // Генерируем более длинный GUID
//     unsigned int part1 = rand();
//     unsigned int part2 = rand();
//     unsigned int part3 = rand();
//     snprintf(out, n, "%08x%08x%08x", part1, part2, part3);
// }

// void validate_and_save(const char *guid, const char *data)
// {
//     char path[128];
//     snprintf(path, sizeof path, "results/%s.txt", guid);
    
//     mkdir("results", 0700);
    
//     FILE *f = fopen(path, "w");
//     if(!f) 
//     {
//         perror("fopen");
//         return;
//     }

//     char msg[MAX];
//     strncpy(msg, data, sizeof msg);
//     msg[sizeof msg - 1] = 0;

//     char *sep = strchr(msg, ';');
//     int errs = 0;

//     if(strlen(msg) == 0)
//     {
//         fprintf(f, "error: empty string\n");
//         errs++;
//     }
//     if(!sep)
//     {
//         fprintf(f, "error: expected ';'\n");
//         errs++;
//     }

//     if(sep)
//     {
//         *sep = '\0';
//         char *name = msg;
//         char *age_str = sep + 1;
        
//         int age = atoi(age_str);
//         if(age == 0 && strcmp(age_str, "0") != 0)
//         {
//             fprintf(f, "error: age is not a number\n");
//             errs++;
//         }
//         else if(age < 0 || age > 120)
//         {
//             fprintf(f, "error: age out of range\n");
//             errs++;
//         }
        
//         if(errs == 0)
//         {
//             fprintf(f, "name: %s\nage: %d\n", name, age);
//         }
//     }

//     if(errs > 0)
//     {
//         fprintf(f, "total errors: %d\n", errs);
//     }

//     fclose(f);
// }

// int main()
// {
//     srand(time(NULL));
    
//     if(mkdir("results", 0700) == -1 && errno != EEXIST)
//     {
//         perror("mkdir");
//         return 1;
//     }

//     key_t key = 1234; 
//     int qid = msgget(key, IPC_CREAT | 0666);
//     if(qid == -1)
//     {
//         perror("msgget");
//         return 1;
//     }

//     printf("Server started. Queue ID: %d\n", qid);
//     printf("Waiting for messages...\n");

//     while(1)
//     {
//         struct msgbuf m;
//         ssize_t r = msgrcv(qid, &m, sizeof m.mtext, 1, 0);
//         if(r == -1)
//         {
//             perror("msgrcv");
//             break;
//         }

//         m.mtext[r] = '\0';
        
//         int pr;
//         char payload[MAX];
//         if(sscanf(m.mtext, "%d|%255[^\n]", &pr, payload) != 2)
//         {
//             fprintf(stderr, "Bad message format: %s\n", m.mtext);
//             continue;
//         }

//         char id[64];
//         guid(id, sizeof id);

//         validate_and_save(id, payload);

//         printf("Processed (priority %d), GUID=%s, file: results/%s.txt\n", pr, id, id);
//         usleep(100000);
//     }

//     msgctl(qid, IPC_RMID, NULL);
//     printf("Server stopped.\n");
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <time.h>

// Структура для сообщения
typedef struct {
    long mtype;            // приоритет обработки
    int priority;          // дублируем приоритет в данных
    char name[32];
    int age;
    char request_id[37];   // GUID для отслеживания
} Record;

// Структура для сообщений в очереди
typedef struct {
    long mtype;            // тип = приоритету
    Record record;
} Message;

// Структура для результата
typedef struct {
    int status;            // 0 - в очереди, 1 - в обработке, 2 - обработан
    char errors[256];      // ошибки валидации
    char name[32];
    int age;
    char request_id[37];
} ValidationResult;

// Структура для результата в очереди
typedef struct {
    long mtype;            // тип = 1
    ValidationResult result;
} ResultMessage;

// Базовый класс обработчика (Chain of Responsibility)
typedef struct Handler Handler;
struct Handler {
    Handler* next;
    char* (*validate)(Handler* self, Record* record);
};

// Конкретные обработчики валидации
typedef struct {
    Handler base;
    // специфичные поля
} NameValidator;

typedef struct {
    Handler base;
    // специфичные поля
} AgeValidator;

// Команда для обработки записи
typedef struct {
    Record* record;
    char errors[256];
    Handler* validator;
} ValidationCommand;

// Генерация UUID v1
void generate_uuid_v1(char* buffer) {
    uuid_t uuid;
    uuid_generate_time(uuid);
    uuid_unparse_lower(uuid, buffer);
}

// Реализация валидатора имени
char* validate_name(Handler* self, Record* record) {
    if (strlen(record->name) == 0) {
        return "Имя не может быть пустым";
    }
    if (strlen(record->name) > 30) {
        return "Имя слишком длинное";
    }
    return NULL;
}

// Реализация валидатора возраста
char* validate_age(Handler* self, Record* record) {
    if (record->age <= 0) {
        return "Возраст должен быть положительным";
    }
    if (record->age > 150) {
        return "Возраст слишком большой";
    }
    return NULL;
}

// Создание валидаторов
Handler* create_name_validator() {
    NameValidator* validator = malloc(sizeof(NameValidator));
    validator->base.next = NULL;
    validator->base.validate = validate_name;
    return (Handler*)validator;
}

Handler* create_age_validator() {
    AgeValidator* validator = malloc(sizeof(AgeValidator));
    validator->base.next = NULL;
    validator->base.validate = validate_age;
    return (Handler*)validator;
}

// Выполнение цепочки валидации
void execute_validation(ValidationCommand* cmd) {
    char* error;
    char* errors = cmd->errors;
    errors[0] = '\0';
    
    Handler* current = cmd->validator;
    int first_error = 1;
    
    while (current != NULL) {
        error = current->validate(current, cmd->record);
        if (error != NULL) {
            if (!first_error) {
                strcat(errors, "; ");
            }
            strcat(errors, error);
            first_error = 0;
        }
        current = current->next;
    }
    
    if (errors[0] == '\0') {
        strcpy(errors, "Валидация успешна");
    }
}

// Освобождение памяти валидаторов
void cleanup_validators(Handler* validator) {
    Handler* current = validator;
    while (current != NULL) {
        Handler* next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    key_t key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Сервер запущен. Ожидание сообщений...\n");
    
    // Создаем цепочку валидаторов
    Handler* name_validator = create_name_validator();
    Handler* age_validator = create_age_validator();
    name_validator->next = age_validator;
    
    // Хранилище результатов
    ValidationResult results[100];
    int result_count = 0;
    
    while (1) {
        Message msg;
        
        // Получаем сообщение (приоритет = тип сообщения)
        if (msgrcv(msgid, &msg, sizeof(msg.record), 0, 0) == -1) {
            perror("msgrcv");
            continue;
        }
        
        printf("Получена запись: приоритет=%d, имя=%s, возраст=%d\n",
               msg.record.priority, msg.record.name, msg.record.age);
        
        // Создаем команду валидации
        ValidationCommand cmd;
        cmd.record = &msg.record;
        cmd.validator = name_validator;
        
        // Выполняем валидацию
        execute_validation(&cmd);
        
        // Сохраняем результат
        if (result_count < 100) {
            results[result_count].status = 2; // обработан
            strcpy(results[result_count].errors, cmd.errors);
            strcpy(results[result_count].name, msg.record.name);
            results[result_count].age = msg.record.age;
            strcpy(results[result_count].request_id, msg.record.request_id);
            result_count++;
        }
        
        // Отправляем результат (если запрос на результат)
        ResultMessage result_msg;
        if (msgrcv(msgid, &result_msg, sizeof(result_msg.result), 1, IPC_NOWAIT) != -1) {
            // Ищем результат по request_id
            int found = 0;
            for (int i = 0; i < result_count; i++) {
                if (strcmp(results[i].request_id, result_msg.result.request_id) == 0) {
                    result_msg.result = results[i];
                    result_msg.mtype = 2;
                    msgsnd(msgid, &result_msg, sizeof(result_msg.result), 0);
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                // Если не найден - возможно в обработке
                strcpy(result_msg.result.errors, "Запрос в обработке или не найден");
                result_msg.result.status = 1;
                result_msg.mtype = 2;
                msgsnd(msgid, &result_msg, sizeof(result_msg.result), 0);
            }
        }
        
        printf("Результат валидации: %s\n", cmd.errors);
    }
    
    cleanup_validators(name_validator);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}