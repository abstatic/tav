#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main(void)
{
  char c;
  static struct termios oldtio, newtio;
  tcgetattr(0, &oldtio);

  newtio = oldtio;
  newtio.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &newtio);

  printf("Give text:\n");
  fflush(stdout);

  while(1)
  {
    c = getchar();

    // is this an escape sequence?
    if (c == 27)
    {
      char c1, c2;
      c1 = getchar();
      c2 = getchar();

      if 
      continue;
    }


    // if backspace
    if (c == 0x7f)
    {
      // go one char left
      printf("\b");
      //overwrite the char with whitespcae
      printf(" ");
      // go back to "now removed char position
      printf("\b");
      continue;
    }

    if (c == 'q')
      break;

    printf("%c", c);
  }
  printf("\n");
  tcsetattr(0, TCSANOW, &oldtio);

  return 0;
}
