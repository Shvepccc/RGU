#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <errno.h>

int main()
{
    int fd;
    char buffer[101];
    int length, i, bytes_written;
    char *fifo_name = "sp_lab_3_3_fifo";
    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    fd = open("sp_lab_3_3_fifo", O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    while (1)
    {
        length = rand() % 100 + 1;
        
        for (i = 0; i < length; i++)
        {
            buffer[i] = 'A' + rand() % 26;
        }
        buffer[length] = '\0';

        printf("Client: sent '%s'\n", buffer);

        bytes_written = write(fd, buffer, length + 1);
        if (bytes_written < 0)
        {
            if (errno == EPIPE) {
                printf("Client: server closed FIFO.\n");
                break;
            }
            else
            {
                perror("write in client");
                break;
            }
        }
        usleep(100000);
    }
    close(fd);
    return 0;
}