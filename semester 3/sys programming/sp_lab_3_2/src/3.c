//make run TASK=3 ARGS="files/task_3.txt home"
#include <stdio.h>
#include <string.h>
#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "string.h"

#define OPEN_FILLE_ERROR 1
#define INVALID_INPUT 2

void fork_bomb(int n)
{
    printf("HEHEHE IT'S FORK BOMB %d\n", n);
    pid_t pid;
    if (n == 0)
        return;
    pid = fork();
    if (pid == 0)
    {
        fork_bomb(n - 1);
        exit(0);
    }
    wait(NULL);
}

int count_occurrences(const char *filename, const char *s)
{
    FILE *f;
    char buf[4096];
    int count, len;
    char *p;

    f = fopen(filename, "r");
    if (!f)
        return OPEN_FILLE_ERROR;

    count = 0;
    len = strlen(s);
    while (fgets(buf, sizeof(buf), f))
    {
        p = buf;
        while ((p = strstr(p, s)) != NULL)
        {
            count++;
            p += len;
        }
    }
    fclose(f);
    return count;
}

int main(int argc, char *argv[])
{
    FILE* file;
    char buffer[BUFSIZ];
    int c, flag = 0;
    pid_t pid = 0;
    int file_count = 0, status, i;

    if (argc < 3)
    {
        printf("Not enough arguments\n");
        return INVALID_INPUT;
    }

    file = fopen(argv[1], "r");
    if (!file)
    {
        printf("ERROR: Invalid argument");
        return OPEN_FILLE_ERROR;
    }

    while(fgets(buffer, sizeof(buffer), file) != NULL)
    {
        buffer[strcspn(buffer, "\r\n")] = 0;
        pid = fork();
        if (pid == 0)
        {
            c = count_occurrences(buffer, argv[2]);
            if (c > 0)
                printf("file: %s (%d)\n", buffer, c);
            exit(c > 0 ? 1 : 0);
        }
        file_count++;
    }
    for (i = 0; i < file_count; i++)
    {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
        {
            flag = 1;
        }
    }

    fclose(file);
    if (flag == 0)
    {
        printf("String '%s' not found in any file\n", argv[2]);
        fork_bomb(strlen(argv[2]));
    }
    return 0;
}