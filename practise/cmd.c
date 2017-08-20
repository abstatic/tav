#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define gotopos(row,col)    printf("\033[%d;%dH", (row), (col)) // gotopos will always be off by one (counts from 0)

int main(int argc, char** argv)
{
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
    char* a[2];
    a[1] = "-l";
    int n = execlp("/bin/sh", "/bin/sh", "-c", "ls -l", NULL);
  }

  return 0;
}
