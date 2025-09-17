#include "stdio.h"
#include "unistd.h"

int main()
{
  pid_t pid, ppid, pgrp;
  uid_t ruid, euid;
  gid_t rgid, egid;

  pid = getpid();
  ppid = getppid();
  pgrp = getpgrp();
  ruid = getuid();
  euid = geteuid();
  rgid = getgid();
  egid = getegid();

  printf("PID of the current process: %d\n", (int)pid);
  printf("PID of the parent process: %d\n", (int)ppid);
  printf("Process group ID: %d\n", (int)pgrp);
  printf("Real user ID: %d\n", (int)ruid);
  printf("Effective user ID: %d\n", (int)euid);
  printf("Real group ID: %d\n", (int)rgid);
  printf("Effective group ID: %d\n", (int)egid);

  return 0;
}