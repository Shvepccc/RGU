//make run TASK=4 ARGS="20 3"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define MSG_TOKEN 1
#define MSG_ELIM 2
#define MSG_TERMINATE 3

typedef struct
{
    int type, count, target, target_next;
    pid_t pid;
} msg_t;

void create_fifos(int n)
{
    int i, res;
    char fname[64];
    for (i = 0; i < n; i++)
    {
        snprintf(fname, sizeof(fname), "fifo_%d", i);
        unlink(fname);
        res = mkfifo(fname, 0666);
        if (res < 0 && errno != EEXIST)
        {
            perror("mkfifo");
            exit(1);
        }
    }
}

void remove_fifos(int n)
{
    int i;
    char fname[64];
    for (i = 0; i < n; i++)
    {
        snprintf(fname, sizeof(fname), "fifo_%d", i);
        unlink(fname);
    }
    unlink("start_signal");
}

void wait_start_signal()
{
    while (access("start_signal", F_OK) != 0)
        usleep(10000);
}

void send_start_signal()
{
    int fd = open("start_signal", O_CREAT | O_WRONLY, 0666);
    if (fd >= 0)
        close(fd);
    else
    {
        perror("open start_signal");
        exit(1);
    }
}

void send_initial_token()
{
    int fd0;
    char fname[64];
    msg_t msg;
    snprintf(fname, sizeof(fname), "fifo_0");
    fd0 = open(fname, O_WRONLY);
    if (fd0 < 0)
    {
        perror("open fifo_0");
        exit(1);
    }
    msg.type = MSG_TOKEN;
    msg.count = 0;
    msg.target = -1;
    msg.target_next = -1;
    msg.pid = 0;
    write(fd0, &msg, sizeof(msg));
    close(fd0);
}

void child_process(int idx, int n, int k)
{
    int next, rd, wr;
    char myfifo[64], nextfifo[64];
    msg_t msg;
    int active = 1;

    next = (idx + 1) % n;
    snprintf(myfifo, sizeof(myfifo), "fifo_%d", idx);
    snprintf(nextfifo, sizeof(nextfifo), "fifo_%d", next);
    
    rd = open(myfifo, O_RDONLY);
    if (rd < 0)
        exit(1);

    wait_start_signal();
    wr = open(nextfifo, O_WRONLY);
    if (wr < 0)
    {
        close(rd);
        exit(1);
    }

    while (active)
    {
        ssize_t r = read(rd, &msg, sizeof(msg));
        if (r <= 0)
        {
            usleep(10000);
            continue;
        }

        if (msg.type == MSG_TOKEN)
        {
            if (msg.count + 1 == k)
            {
                msg_t em;
                em.type = MSG_ELIM;
                em.count = 0;
                em.target = idx;
                em.target_next = next;
                em.pid = getpid();
                write(wr, &em, sizeof(em));
            }
            else
            {
                msg.count++;
                write(wr, &msg, sizeof(msg));
            }
        }
        else if (msg.type == MSG_ELIM)
        {
            if (next == msg.target)
            {
                msg_t tm, newt;
                tm.type = MSG_TERMINATE;
                tm.count = 0;
                tm.target = msg.target;
                tm.target_next = msg.target_next;
                tm.pid = msg.pid;
                write(wr, &tm, sizeof(tm));
                close(wr);
                next = msg.target_next;
                snprintf(nextfifo, sizeof(nextfifo), "fifo_%d", next);
                wr = open(nextfifo, O_WRONLY);
                if (wr < 0)
                {
                    close(rd);
                    exit(1);
                }
                newt.type = MSG_TOKEN;
                newt.count = 0;
                newt.target = -1;
                newt.target_next = -1;
                newt.pid = 0;
                write(wr, &newt, sizeof(newt));
            }
            else
            {
                write(wr, &msg, sizeof(msg));
            }
        }
        else if (msg.type == MSG_TERMINATE)
        {
            if (msg.target == idx)
            {
                close(rd);
                close(wr);
                active = 0;
            }
            else
            {
                write(wr, &msg, sizeof(msg));
            }
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int n, k, i, remaining, status;
    pid_t pid, wpid;
    pid_t *order;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s n k\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    k = atoi(argv[2]);
    if (n <= 1 || k <= 1 || k >= n)
    {
        fprintf(stderr, "error: n>1, 1<k<n\n");
        return 1;
    }

    create_fifos(n);

    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            exit(1);
        if (pid == 0)
            child_process(i, n, k);
    }

    send_start_signal();
    send_initial_token();

    order = malloc(sizeof(pid_t) * n);
    remaining = n;
    i = 0;
    while (remaining > 0)
    {
        wpid = wait(&status);
        if (wpid > 0)
        {
            order[i++] = wpid;
            remaining--;
        }
    }

    printf("n=%d k=%d\n", n, k);
    for (i = 0; i < n; i++)
        printf("%d\n", (int)order[i]);

    free(order);
    remove_fifos(n);
    return 0;
}