#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
  char* h = malloc(80*sizeof(char));
  sprintf(h, "hello world\0");
  printf("%s\n", h);

  char f = 'f';
  int len = strlen(h);
  int pos = 5;

  memmove (
      h + 5 + 1,
      h + 5,
      80 - (5 + 1)
  );

  h[5] = 'f';

  printf("%s\n", h);
  return 0;
}
