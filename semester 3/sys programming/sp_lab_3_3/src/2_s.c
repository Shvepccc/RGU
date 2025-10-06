#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    char *fifo_name = "sp_lab_3_3_fifo";
    int fd;
    char buffer[101];
    int length_counts[101];
    int i, bytes_read, done;

    for (i = 0; i < 101; i++)
    {
        length_counts[i] = 0;
    }
    
    mkfifo(fifo_name, 0666);

    fd = open(fifo_name, O_RDONLY);
    if (fd == -1)
    {
        perror("open fifo in server");
        return 1;
    }

    done = 0;
    while (!done)
    {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            length_counts[bytes_read]++;
            printf("Received: %s (length: %d)\n", buffer, bytes_read);
            
            if (length_counts[bytes_read] >= 5)
            {
                printf("Server: Length %d encountered 5 times.\n", bytes_read);
                break;
            }
        }
    }

    close(fd);
    unlink(fifo_name);

    return 0;
}