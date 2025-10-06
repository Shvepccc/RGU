#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int pid_arr[2];
    pid_t pid;
    char buffer[100];
    char message[] = "Hello world!";
    int status;
    ssize_t err;

    if (pipe(pid_arr) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(pid_arr[0]);

        err = write(pid_arr[1], message, strlen(message) + 1);
        if (err == -1)
        {
            perror("write");
            return 1;
        }
    }
    else
    {
        close(pid_arr[1]);

        err = read(pid_arr[0], buffer, sizeof(buffer));
        if (err == -1)
        {
            perror("read");
            return 1;
        }

        printf("%s\n", buffer);
    }

    return 0;
}