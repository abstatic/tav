#include "helper.h"

/*
 * This function is used to enable the non canonoical mode for the terminal
 * All the required structs are declared inside
 *
 * fd is the FILE DESCRIPTOR of the stream on which we want to modify
 */
void getControl(int fd)
{
  struct termios o_term, n_term;
  tcgetattr(fd, &o_term);

  n_term = o_term;

  /* n_term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); */
  n_term.c_lflag &= ~(ICANON | ECHO);

  // TCSANOW means apply the changes immediately
  tcsetattr(fd, TCSANOW, &n_term);
}

/*
 * This function is used to handle all the keypresses.
 *
 * Depending on the mode and keypress this function will call respective 
 * routines
 */
void readKey(void)
{
  int c;

  c = getchar();

  if (c == ESCAPE)
    handleEscapeSequence();
  else
    printf("%c", c);
}

/*
 * This function will handle the escape sequence of keys
 */
void handleEscapeSequence(void)
{
  /*
   * Since every escape sequence is basically three characters
   * First has already been read
   */
  int c;
  c = getchar();
  if (c == '[')
  {
    int key = getchar();

    switch(key)
    {
      case 'A':
        key = UP;
        break;
      case 'B':
        key = DOWN;
        break;
      case 'C':
        key = LEFT;
        break;
      case 'D':
        key = RIGHT;
        break;
      default:
        key = 0;
        break;
    }

    if (key != 0)
    {
      if (key == UP)
        cursorupward(1);
      else if (key == DOWN)
        cursordownward(1);
      else if (key == LEFT)
        cursorbackward(1);
      else if (key == RIGHT)
        cursorforward(1);
    }
  }
}
