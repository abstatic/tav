#include "helper.h"

// global variables
config g_tavProps;

int main(void)
{
  // enable the non canonical mode with all the required flags
  getControl(STDIN_FILENO);

  // first flush all the contents of stdout
  fflush(stdout);

  initscr();

  while (1)
  {
    readKey();
    signal(SIGWINCH, handle_winresize);
  }
  return 0;
}
