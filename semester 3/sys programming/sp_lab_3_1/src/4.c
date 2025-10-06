//make run TASK=4 ARGS="files/task_4.bin xor8"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_prime(unsigned char x)
{
    int i;
    if (x < 2)
        return 0;
    for (i = 2; i * i <= x; i++)
    {
        if (x % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    FILE *f;
    unsigned char buf[4];
    unsigned char b, has_prime, result8, result32;
    unsigned int val, mask, count;
    int i, r;

    if (argc < 3)
    {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "rb");
    if (!f)
    {
        printf("Invalid file.\n");
        return 1;
    }

    if (strcmp(argv[2], "xor8") == 0)
    {
        result8 = 0;
        while (fread(&b, 1, 1, f) == 1)
            result8 ^= b;
        printf("%X\n", result8);
    }
    else if (strcmp(argv[2], "xorodd") == 0)
    {
        result32 = 0;
        while ((r = fread(buf, 1, 4, f)) == 4)
        {
            has_prime = 0;
            for (i = 0; i < 4; i++)
            {
                if (is_prime(buf[i]))
                {
                    has_prime = 1;
                    break;
                }
            }
            if (has_prime)
            {
                val = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
                result32 ^= val;
            }
        }
        printf("%u\n", result32);
    }
    else if (strcmp(argv[2], "mask") == 0 && argc >= 4)
    {
        mask = strtoul(argv[3], NULL, 16);
        count = 0;
        while ((r = fread(buf, 1, 4, f)) == 4)
        {
            val = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
            if ((val & mask) == mask)
                count++;
        }
        printf("%u\n", count);
    }

    fclose(f);
    return 0;
}