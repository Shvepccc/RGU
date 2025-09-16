//make run TASK=1 ARGS="files/task_1"
#include <stdio.h>
#include <string.h>

void print_file_struct(FILE *file);

int main(int argc, char *argv[])
{
  unsigned char buff = 0;
  unsigned char buff_2[4];
  FILE* file;
  int i = 0;

  if (argc < 2)
  {
    printf("Not enoght arguments\n");
    return 1;
  }

  file = fopen(argv[1], "rb");
  if (file == NULL)
  {
    printf("Ivalid file name\n");
    return 1;
  }

  while (fread(&buff, 1, 1, file) == 1)
  {
    printf("byte: %u\n", buff);
    print_file_struct(file);
  }

  fclose(file);

  file = NULL;
  file = fopen(argv[1], "rb");
  if (file == NULL)
  {
    printf("Ivalid file\n");
    return 1;
  }

  fseek(file, 3, SEEK_SET);

  fread(buff_2, 4, 4, file);
  for (i = 0; i < 4; i++)
  {
    printf(" %u", buff_2[i]);
  }
  printf("\n");

  fclose(file);
  file = NULL;

  return 0;
}

void print_file_struct(FILE* file)
{
  printf("\nFILE structure at %p:\n", (void*)file);
  printf("─────────────────────────────────\n");
  
  printf("_file:     %d\n", file->_file);
  printf("_flags:    0x%x\n", file->_flags);
  printf("_p:        %p\n", (void*)file->_p);
  printf("_r:        %d\n", file->_r);
  printf("_w:        %d\n", file->_w);
  printf("_offset:   %lld\n", file->_offset);
  
  printf("_bf:       base=%p, size=%d\n", 
          (void*)file->_bf._base, file->_bf._size);
  printf("_ub:       base=%p, size=%d\n", 
          (void*)file->_ub._base, file->_ub._size);
  printf("_lb:       base=%p, size=%d\n", 
          (void*)file->_lb._base, file->_lb._size);
  
  printf("_blksize:  %d\n", file->_blksize);
  printf("_lbfsize:  %d\n", file->_lbfsize);
  printf("_ur:       %d\n", file->_ur);
  printf("_nbuf:     %s\n", file->_nbuf);
  
  printf("_read:     %p\n", (void*)file->_read);
  printf("_write:    %p\n", (void*)file->_write);
  printf("_seek:     %p\n", (void*)file->_seek);
  printf("_close:    %p\n", (void*)file->_close);
  
  printf("_cookie:   %p\n", (void*)file->_cookie);
  printf("_extra:    %p\n", (void*)file->_extra);
  
  printf("_ubuf:     [0x%x, 0x%x, 0x%x]\n", 
          file->_ubuf[0], file->_ubuf[1], file->_ubuf[2]);
  
  printf("─────────────────────────────────\n");
}