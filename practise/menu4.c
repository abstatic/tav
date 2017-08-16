#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

void handleInput(FILE*  in, FILE* out);

int main()
{
  int choice = 0;
  FILE* input;
  FILE* output;

  struct termios initial_settings, new_settings;

  if (!isatty(fileno(stdout)))
    fprintf(stderr, "You are not a terminal, OK.\n");

  input = fopen("/dev/tty", "r"); // input stream is the terminal
  output = fopen("/dev/tty", "w"); // output stream is also the terminal

  if (!input || !output)
  {
    fprintf(stderr, "Unable to open /dev/tty\n");
    exit(1);
  }

  tcgetattr(fileno(input), &initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON; // set non canonical mode
  new_settings.c_lflag &= ECHO; // set echo on
  new_settings.c_cc[VMIN] = 0;
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_lflag &= ~ISIG;

  if (tcsetattr(fileno(input), TCSANOW, &new_settings) != 0)
    fprintf(stderr, "could not set attributes\n");

  while (1)
    handleInput(input, output);

  // set the attributes back to initial
  tcsetattr(fileno(input), TCSANOW, &initial_settings);
  exit(0);
}

void handleInput(FILE* in, FILE* out)
{
  int input_char = 0;
  do
  {
    scanf("%c", &input_char);
    if (input_char == 0x7f)
    {
      // go one char left
      printf("\b");
      //overwrite the char with whitespcae
      printf(" ");
      // go back to "now removed char position
      printf("\b");
      continue;
    }
    input_char = 0;
  } while (input_char != 'q');
}
