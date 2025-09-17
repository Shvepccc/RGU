//make run TASK=2 ARGS="4"
#include "stdio.h"
#include "unistd.h"

int main(int argc, char* argv[])
{
    switch (*argv[1]) {
    case '1':
        printf("----- PART 1 -----\n");
        int pid;
        pid=fork();
        printf("%d\n", pid);
        return 0;
    break;

    case '2':
        printf("----- PART 2 -----\n");
        fork();
        printf("Hi\n");
        fork();
        printf("Hi\n");
        return 0;
    break;

    case '3':
        printf("----- PART 3 -----\n");
        if (fork() || fork()) fork();
        printf("forked! %d\n", fork());
        return 0;
    break;

    case '4':
        printf("----- PART 4 -----\n");
        if (fork() && (!fork()))
        {
            if (fork() || fork()) fork();
        }
        printf("52\n");
        return 0;
    break;

    default:
        printf("Invalid mode =(\n");
    break;
    }
    return 0;
}