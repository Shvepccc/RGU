#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

struct msgbuf
{
    long mtype;
    char mtext[256];
};

int main()
{
    int msgid;
    pid_t pid;
    struct msgbuf msg;
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if(msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    pid = fork();
    if(pid > 0)
    {
        msg.mtype = 1;
        strncpy(msg.mtext, "Hello world! Hello everyone!\0", sizeof msg.mtext);
        if(msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) == -1)
        {
            perror("msgsnd");
            return 1;
        }
        wait(NULL);
    }
    else if(pid == 0)
    {
        ssize_t r;
        struct msgbuf rmsg;
        r = msgrcv(msgid, &rmsg, sizeof rmsg.mtext, 0, 0);
        if(r == -1)
        {
            perror("msgrcv");
            _exit(1);
        }
        printf("%s\n", rmsg.mtext);
        if(msgctl(msgid, IPC_RMID, NULL) == -1)
        {
            perror("msgctl");
            _exit(1);
        }
        _exit(0);
    }
    else
    {
        perror("fork");
        return 1;
    }
    return 0;
}
