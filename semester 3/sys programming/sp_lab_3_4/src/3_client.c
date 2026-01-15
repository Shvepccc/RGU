// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/msg.h>
// #include <time.h>
// #include <unistd.h>

// struct msgbuf
// {
//     long mtype;
//     char mtext[256];
// };

// void generate_random_data(char *buffer, size_t size)
// {
//     const char *names[] = {"Alice", "Bob", "Charlie", "David", "Eve", 
//                           "Frank", "Grace", "Henry", "Ivy", "Jack"};
//     const char *errors[] = {"", ";abc", "NameOnly", "Name;", ";", ""};
    
//     static int call_count = 0;
    
//     if (call_count % 3 == 0) {
//         int name_idx = rand() % 10;
//         int age = 20 + rand() % 60;
//         snprintf(buffer, size, "%s;%d", names[name_idx], age);
//     } else if (call_count % 3 == 1) {
//         int name_idx = rand() % 10;
//         int error_type = rand() % 3;
//         if (error_type == 0) {
//             snprintf(buffer, size, "%s;abc", names[name_idx]);
//         } else if (error_type == 1) {
//             snprintf(buffer, size, "%s;150", names[name_idx]);
//         } else {
//             snprintf(buffer, size, "%s;-5", names[name_idx]);
//         }
//     } else {
//         int error_idx = rand() % 6;
//         snprintf(buffer, size, "%s", errors[error_idx]);
//     }
    
//     call_count++;
// }

// int main(int argc, char *argv[])
// {
//     srand(time(NULL));
    
//     key_t key = 1234;
//     int qid = msgget(key, 0666);
//     if(qid == -1)
//     {
//         perror("msgget client");
//         printf("Make sure server is running first!\n");
//         return 1;
//     }

//     struct msgbuf m;
//     m.mtype = 1;

//     printf("Client connected to queue ID: %d\n", qid);
    
//     int auto_mode = 0;
//     int message_count = 1;
    
//     if (argc > 1) {
//         if (strcmp(argv[1], "-auto") == 0) {
//             auto_mode = 1;
//             if (argc > 2) {
//                 message_count = atoi(argv[2]);
//                 if (message_count <= 0) message_count = 10;
//             }
//         }
//     }
    
//     if (auto_mode) {
//         printf("Sending %d messages automatically...\n", message_count);
        
//         for (int i = 0; i < message_count; i++) {
//             char data[256];
//             generate_random_data(data, sizeof(data));
            
//             int priority = 1 + rand() % 5;
            
//             snprintf(m.mtext, sizeof(m.mtext), "%d|%s", priority, data);
            
//             printf("Sending [%d/%d]: priority = %d, data = '%s'\n", 
//                    i+1, message_count, priority, data);
            
//             if(msgsnd(qid, &m, strlen(m.mtext) + 1, 0) == -1)
//             {
//                 perror("msgsnd");
//                 return 1;
//             }
//             usleep(100000);
//         }
        
//         printf("All messages sent.\n");
//     } else {
//         printf("Enter messages in format: PRIORITY|DATA\n");
//         printf("Example: 3|John;25\n");
//         printf("Priority: 1 (highest) to 5 (lowest)\n");
//         printf("Data format: NAME;AGE (age 0-120)\n");
//         printf("Empty line to exit.\n\n");
        
//         while(1)
//         {
//             char buf[256];
//             printf("> ");
//             if(!fgets(buf, sizeof buf, stdin)) break;
//             buf[strcspn(buf, "\n")] = 0;
//             if(buf[0] == 0) break;

//             strncpy(m.mtext, buf, sizeof m.mtext);
//             m.mtext[sizeof m.mtext - 1] = 0;

//             if(msgsnd(qid, &m, strlen(m.mtext) + 1, 0) == -1)
//             {
//                 perror("msgsnd");
//                 return 1;
//             }
            
//             printf("Message sent.\n");
//         }
//     }

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
#include <time.h>

// Структуры должны быть такие же как на сервере
typedef struct {
    long mtype;
    int priority;
    char name[32];
    int age;
    char request_id[37];
} Record;

typedef struct {
    long mtype;
    Record record;
} Message;

typedef struct {
    int status;
    char errors[256];
    char name[32];
    int age;
    char request_id[37];
} ValidationResult;

typedef struct {
    long mtype;
    ValidationResult result;
} ResultMessage;

// Функция для генерации простого UUID (упрощенная)
void generate_simple_uuid(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(buffer, "%04d%02d%02d-%02d%02d%02d-%06d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec, rand() % 1000000);
}

int main() {
    key_t key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    
    srand(time(NULL));
    
    // Автоматическая генерация 5 сообщений
    char* names[] = {"Боб", "Алиса", "Иван", "Мария", "Петр"};
    
    for (int i = 0; i < 5; i++) {
        Message msg;
        msg.mtype = rand() % 5 + 1;  // приоритет 1-5
        msg.record.priority = msg.mtype;
        strcpy(msg.record.name, names[rand() % 5]);
        msg.record.age = rand() % 100 + 1;
        generate_simple_uuid(msg.record.request_id);
        
        printf("Отправка: приоритет=%d, имя=%s, возраст=%d, ID=%s\n",
               msg.record.priority, msg.record.name, msg.record.age, msg.record.request_id);
        
        // Иногда отправляем некорректные данные для тестирования
        if (i == 2) {
            strcpy(msg.record.name, "");  // пустое имя
        }
        if (i == 3) {
            msg.record.age = -5;  // отрицательный возраст
        }
        
        if (msgsnd(msgid, &msg, sizeof(msg.record), 0) == -1) {
            perror("msgsnd");
            continue;
        }
        
        sleep(1);  // небольшая задержка
        
        // Запрос результата
        ResultMessage req;
        req.mtype = 1;  // тип запроса результата
        strcpy(req.result.request_id, msg.record.request_id);
        
        if (msgsnd(msgid, &req, sizeof(req.result), 0) == -1) {
            perror("msgsnd запроса результата");
            continue;
        }
        
        // Получение результата
        ResultMessage result;
        if (msgrcv(msgid, &result, sizeof(result.result), 2, 0) == -1) {
            perror("msgrcv результата");
            continue;
        }
        
        printf("Результат для ID %s:\n", result.result.request_id);
        printf("  Статус: %d\n", result.result.status);
        printf("  Ошибки: %s\n", result.result.errors);
        printf("  Имя: %s\n", result.result.name);
        printf("  Возраст: %d\n\n", result.result.age);
        
        sleep(2);  // пауза между сообщениями
    }
    
    printf("Клиент завершил отправку сообщений\n");
    
    return 0;
}