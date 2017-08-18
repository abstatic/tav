#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  char* string = (char*) malloc(sizeof(char)*80);

  sprintf(string, "hello world");
  int len = strlen(string);
  printf("Length: %d\n", len);
  printf("%s\n", string);

  string[len] = 'c';
  string[len+1] = 'd';
  for (int i = 0; i < 80; i++)
  {
    printf("I:%d %c \n",i, string[i]);
  }
  printf("\n");
  return 0;
}
