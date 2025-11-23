#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

struct msgbuf
{
    long mtype;
    char mtext[256];
};

int main()
{
    key_t key = 1234;
    int qid = msgget(key, 0600);
    if(qid == -1)
    {
        perror("msgget client");
        return 1;
    }

    struct msgbuf m;
    m.mtype = 1;

    while(1)
    {
        char buf[256];
        printf("Enter message (PRIORITY|DATA), empty to exit: ");
        if(!fgets(buf, sizeof buf, stdin)) break;
        buf[strcspn(buf, "\n")] = 0;
        if(buf[0] == 0) break;

        strncpy(m.mtext, buf, sizeof m.mtext);
        m.mtext[sizeof m.mtext - 1] = 0;

        if(msgsnd(qid, &m, strlen(m.mtext)+1, 0) == -1)
        {
            perror("msgsnd");
            return 1;
        }
    }

    return 0;
}
