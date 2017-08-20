/**
 * shell.c
 *
 * This file contains function related to fork-exec mechanism
 *
 * Author: Abhishek Shrivastava
 */
#include "helper.h"

void executeBash()
{
  char* command = g_tavProps.cmd_buf + 2;
  pid_t parent = getpid();
  pid_t pid = fork();
  if (pid == -1)
  {
      // error, failed to fork()
  }
  else if (pid > 0)
  {
    int status;
    waitpid(pid, &status, 0);
    int c = getc(stdin);
  }
  else 
  {
    printf("\033c");
    fflush(stdout);

    gotopos(0,0);
    int n = execlp("/bin/sh", "/bin/sh", "-c", command, NULL);
  }
}
