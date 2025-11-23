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

void writer(int msgid)
{
    struct msgbuf m;
    m.mtype = 1;
    strncpy(m.mtext, "type_1 message: pipipu", sizeof m.mtext);
    if(msgsnd(msgid, &m, strlen(m.mtext) + 1, 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
    m.mtype = 2;
    strncpy(m.mtext, "type_2: abracadabra", sizeof m.mtext);
    if(msgsnd(msgid, &m, strlen(m.mtext) + 1, 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
    m.mtype = 3;
    strncpy(m.mtext, "end_type", sizeof m.mtext);
    if(msgsnd(msgid, &m, strlen(m.mtext) + 1, 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
}

void reader(int msgid)
{
    struct msgbuf r;
    while(1)
    {
        ssize_t n = msgrcv(msgid, &r, sizeof r.mtext, 0, 0);
        if(n == -1)
        {
            perror("msgrcv");
            exit(1);
        }
        if(r.mtype == 1)
        {
            printf("handler 1 (type_1): %s\n", r.mtext);
        }
        else if(r.mtype == 2)
        {
            printf("handler 2 (type_2): %s\n", r.mtext);
        }
        else if (r.mtype == 3)
        {
            break;
        }
        else
        {
            printf("unknown type: %s\n", r.mtext);
        }
    }
}

int main()
{
    int msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if(msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    pid_t pid = fork();
    if(pid > 0)
    {
        writer(msgid);
        wait(NULL);
        if(msgctl(msgid, IPC_RMID, NULL) == -1)
        {
            perror("msgctl");
            return 1;
        }
    }
    else if(pid == 0)
    {
        reader(msgid);
        exit(0);
    }
    else
    {
        perror("fork");
        return 1;
    }
    return 0;
}
