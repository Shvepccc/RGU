#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int is_prime(int n)
{
    int i;
    if (n < 2) return 0;
    for (i = 2; i * i <= n; i++)
    {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(void)
{
    int p_to_c[2];
    int c_to_p[2];
    pid_t pid;
    ssize_t err;
    int status, recived_n, res_count, i, count, n;

    if ((pipe(p_to_c) == -1) || (pipe(c_to_p) == -1))
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
        close(p_to_c[1]);
        close(c_to_p[0]);

        while(1)
        {
            err = read(p_to_c[0], &recived_n, sizeof(recived_n));
            if (err == -1)
            {
                perror("read");
                break;
            }
            if (err == 0) break;

            res_count = 0;
            for (i = 1; i < recived_n; i++)
            {
                if (gcd(i, recived_n) == 1)
                {
                    res_count++;
                }
            }

            err = write(c_to_p[1], &res_count, sizeof(res_count));
            if (err == -1)
            {
                perror("write");
                break;
            }
        }
    }
    else
    {
        close(p_to_c[0]);
        close(c_to_p[1]);

        count = 0;
        srand(getpid());

        while (count < 50)
        {
            n = rand() % 1000 + 1;
            err = write(p_to_c[1], &n, sizeof(n));
            if (err == -1)
            {
                perror("write");
                break;
            }

            err = read(c_to_p[0], &res_count, sizeof(res_count));
            if (err == -1)
            {
                perror("read");
                break;
            }

            if (res_count == n - 1)
            {
                count++;
                printf("Parent: received matching system for n=%d (count=%d)\n", n, count);
            }
        }
    }

    return 0;
}