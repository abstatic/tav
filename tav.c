#include "helper.h"

int main(void)
{
  // enable the non canonical mode with all the required flags
  getControl(STDIN_FILENO);

  // first flush all the contents of stdout
  fflush(stdout);
  //clear the screen and goto 0,0
  clearscr;
  gotopos(0,0);
  while (1)
  {
    readKey();
  }
  return 0;
}
