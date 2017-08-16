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

  /* raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); */
  n_term.c_lflag &= ~(ICANON | ECHO);
  n_term.c_cc[VMIN] = 0;
  n_term.c_cc[VTIME] = 1;

  // TCSANOW means apply the changes immediately
  tcsetattr(fd, TCSANOW, &n_term);
}

/*
 * This function is used to handle all the keypresses.
 *
 * Depending on the mode and keypress this function will call respective 
 * routines
 */
void readkey(void)
{
  int c;

  c = getchar();

  if (c == ESCAPE)
    handleEscapeSequence();
  else
    putchar(c);
}

/*
 * This function will handle the escape sequence of keys
 */
void handleEscapeSequence(void)
{
  printf("Handle Escape seq\n");
}
