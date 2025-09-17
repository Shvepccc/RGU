//make run TASK=4 ARGS="0 3 ../"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dirent.h"
#include "sys/stat.h"
#include "limits.h"

void process_directory(char *path, int current_depth, int recmin, int recmax);

int main(int argc, char *argv[])
{
    int recmin, recmax, i;
    
    if (argc < 4)
    {
        printf("Usage: %s recmin recmax dir1 [dir2 ...]\n", argv[0]);
        return 1;
    }
    
    recmin = atoi(argv[1]);
    recmax = atoi(argv[2]);
    
    if (recmin < 0 || recmax < 0 || recmin > recmax)
    {
        fprintf(stderr, "Invalid recmin and(or) recmax values\n");
        return 1;
    }
    
    for (i = 3; i < argc; i++)
    {
        process_directory(argv[i], 0, recmin, recmax);
    }
    
    return 0;
}

void process_directory(char *path, int current_depth, int recmin, int recmax)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char full_path[PATH_MAX];
    char *extension;
    ino_t inode;
    char *dot_pos;
    
    if (current_depth > recmax)
    {
        return;
    }
    
    dir = opendir(path);
    if (dir == NULL)
    {
        return;
    }
    printf("\n ----- Dir %s is open\n", path);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        
        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);
        
        if (lstat(full_path, &statbuf) == -1)
        {
            continue;
        }

        if (S_ISREG(statbuf.st_mode))
        {
            if (current_depth >= recmin && current_depth <= recmax)
            {
                dot_pos = strrchr(entry->d_name, '.');
                if (dot_pos != NULL)
                {
                    printf("%s\t\t%s\t\t%lu\n", entry->d_name, dot_pos, (unsigned long)statbuf.st_ino);
                }
                else
                {
                    printf("%s\t\t\t\t%lu\n", entry->d_name, (unsigned long)statbuf.st_ino);
                }
            }
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            if (current_depth < recmax)
            {
                process_directory(full_path, current_depth + 1, recmin, recmax);
            }
        }
    }
    
    closedir(dir);
}