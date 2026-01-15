#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "/opt/homebrew/include/gmp.h"

#define SHM_NAME "primeorial_primes_shm"
#define NUM_COUNT 52
#define FILENAME "primeorial_primes.txt"

int main()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        perror("Failed to open primeorial primes file");
        exit(EXIT_FAILURE);
    }

    mpz_t numbers[NUM_COUNT];
    for (int i = 0; i < NUM_COUNT; i++)
    {
        mpz_init(numbers[i]);
    }

    char buffer[4096];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL && count < NUM_COUNT)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        if (mpz_set_str(numbers[count], buffer, 10) == -1)
        {
            fprintf(stderr, "Failed to parse number at line %d\n", count + 1);
            fclose(file);
            for (int i = 0; i < NUM_COUNT; i++)
            {
                mpz_clear(numbers[i]);
            }
            exit(EXIT_FAILURE);
        }
        count++;
    }

    fclose(file);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open failed");
        for (int i = 0; i < NUM_COUNT; i++)
        {
            mpz_clear(numbers[i]);
        }
        exit(EXIT_FAILURE);
    }

    size_t total_size = 0;
    for (int i = 0; i < NUM_COUNT; i++)
    {
        total_size += mpz_sizeinbase(numbers[i], 10) + 1;
    }

    if (ftruncate(shm_fd, total_size) == -1)
    {
        perror("ftruncate failed");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        for (int i = 0; i < NUM_COUNT; i++)
        {
            mpz_clear(numbers[i]);
        }
        exit(EXIT_FAILURE);
    }

    char *shared_data = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("mmap failed");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        for (int i = 0; i < NUM_COUNT; i++)
        {
            mpz_clear(numbers[i]);
        }
        exit(EXIT_FAILURE);
    }

    char *current_pos = shared_data;
    for (int i = 0; i < NUM_COUNT; i++)
    {
        mpz_get_str(current_pos, 10, numbers[i]);
        current_pos += strlen(current_pos) + 1;
    }

    printf("Primeorial primes loaded from file and written to shared memory\n");
    printf("Total numbers: %d\n", NUM_COUNT);
    printf("Total shared memory size: %zu bytes\n", total_size);

    for (int i = 0; i < NUM_COUNT; i++)
    {
        mpz_clear(numbers[i]);
    }

    if (munmap(shared_data, total_size) == -1)
    {
        perror("munmap failed");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }

    close(shm_fd);

    return 0;
}