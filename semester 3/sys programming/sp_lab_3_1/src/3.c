//make run TASK=3 ARGS="files/src.txt files/res.txt"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* src, *dst;
    char buffer[4096];
    size_t bytes;
    if (argc != 3)
    {
        printf("Invalid arguments inpunt");
        return 1;
    }
    
    src = fopen(argv[1], "rb");
    if (!src)
    {
        printf("Cannot open source file\n");
        return 1;
    }

    dst = fopen(argv[2], "wb");
    if (!dst)
    {
        fclose(src);
        printf("Cannot open destination file\n");
        return 1;
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, dst);
    }

    fclose(src);
    fclose(dst);
    return 0;
}
