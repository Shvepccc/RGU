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

#define MAX 256

struct msgbuf
{
    long mtype;
    char mtext[MAX];
};

unsigned long long uuid_time()
{
    const unsigned long long off = 0x01B21DD213814000ULL;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long long)tv.tv_sec * 10000000ULL + tv.tv_usec * 10ULL + off;
}

void guid(char *out, size_t n)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int seed = tv.tv_sec ^ tv.tv_usec ^ getpid();
    srand(seed);
    
    unsigned int value = (rand() << 16) | rand();
    snprintf(out, n, "%06x", value & 0xFFFFFF);
}

void validate_and_save(const char *guid, const char *data)
{
    char path[128];
    snprintf(path, sizeof path, "results/%s.txt", guid);
    
    mkdir("results", 0700);
    
    FILE *f = fopen(path, "w");
    if(!f) 
    {
        perror("fopen");
        return;
    }

    char msg[MAX];
    strncpy(msg, data, sizeof msg);
    msg[sizeof msg - 1] = 0;

    char *sep = strchr(msg, ';');
    int errs = 0;

    if(strlen(msg) == 0)
    {
        fprintf(f, "error: empty string\n");
        errs++;
    }
    if(!sep)
    {
        fprintf(f, "error: expected ';'\n");
        errs++;
    }

    if(sep)
    {
        int age = atoi(sep + 1);
        if(age == 0 && strcmp(sep + 1, "0") != 0)
        {
            fprintf(f, "error: age is not a number\n");
            errs++;
        }
        else if(age < 0 || age > 120)
        {
            fprintf(f, "error: age out of range\n");
            errs++;
        }
    }

    if(errs == 0)
        fprintf(f, "ok\n");

    fclose(f);
}

int main()
{
    srand(time(NULL));
    
    // Создаем директорию для результатов
    if(mkdir("results", 0700) == -1 && errno != EEXIST)
    {
        perror("mkdir");
        return 1;
    }

    key_t key = 1234; 
    int qid = msgget(key, IPC_CREAT | 0600);
    if(qid == -1)
    {
        perror("msgget");
        return 1;
    }

    printf("server queue id: %d\n", qid);

    while(1)
    {
        struct msgbuf m;
        ssize_t r = msgrcv(qid, &m, sizeof m.mtext, 0, 0);
        if(r == -1)
        {
            perror("msgrcv");
            break;
        }

        int pr;
        char payload[MAX];
        if(sscanf(m.mtext, "%d|%255[^\n]", &pr, payload) != 2)
        {
            fprintf(stderr, "bad message format\n");
            continue;
        }

        char id[64];
        guid(id, sizeof id);

        validate_and_save(id, payload);

        printf("processed (priority %d), guid=%s, file: results/%s.txt\n", pr, id, id);
    }

    msgctl(qid, IPC_RMID, NULL);
    return 0;
}