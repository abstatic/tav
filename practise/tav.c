#include "helper.h"

// global variables
//hello rajat agrawal world�U
config g_tavProps;

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    printf("Usage: ./tav [filename]\nAdditional parameters not accepted");
    exit(0);
  }

  initscr();

  if (argc == 2)
  {
    g_tavProps.filename = argv[1];
  }
  readFile(g_tavProps.filename);

  // enable the non canonical mode with all the required flags
  getControl(STDIN_FILENO);

  // first flush all the contents of stdout
  fflush(stdout);

  signal(SIGWINCH, handle_winresize);
  while (1)
  {
    drawWindow();
    readKey(STDIN_FILENO);
    signal(SIGWINCH, handle_winresize);
  }
  return 0;
}
