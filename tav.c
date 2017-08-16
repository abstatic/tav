#include "helper.h"

int main(void)
{
  // enable the non canonical mode with all the required flags
  getControl(STDIN_FILENO);

  int c;
  while (1)
  {
    readKey();
  }
  return 0;
}
