/**
 * helper.c
 *
 * This file contains various helper functions related to TAV text editor
 *
 * Author: Abhishek Shrivastava <abhishek.shrivastava.ts@gmail.com>
 *
 */
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

  // the input flags are needed to handle the CR LF situation.
  // Its harder to detect \r\n , \r is easier to catch
  // New lines are completely handled by the program. Not terminal
  n_term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  n_term.c_lflag &= ~(ICANON | ECHO);

  n_term.c_cc[VTIME] = 0;
  n_term.c_cc[VMIN] = 1;
  // TCSANOW means apply the changes immediately
  tcsetattr(fd, TCSANOW, &n_term);
}

/*
 * This method is a signal handler
 * It will be called everytime the window resize happens
 *
 */
void handle_winresize(int sig)
{
  signal(SIGWINCH, SIG_IGN);
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  // store the windows sizei nto the props struct
  g_tavProps.w_row = w.ws_row;
  g_tavProps.w_col = w.ws_col;
  drawWindow();
}

/*
 * Initialize the screen and the editor config
 *
 */
void initscr(void)
{
  // get the window size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  g_tavProps.w_row       = w.ws_row;
  g_tavProps.w_col       = w.ws_col;
  g_tavProps.cursor_row  = 0;
  g_tavProps.cursor_col  = 0;
  g_tavProps.is_mod      = 0;
  g_tavProps.mode        = DEFAULT_MODE;
  g_tavProps.filename    = DEFAULT_FILE_NAME;
  g_tavProps.act_rows    = 1;
  g_tavProps.first_seq   = malloc(sizeof(sequence));

  g_tavProps.first_seq -> prev    = NULL;
  g_tavProps.first_seq -> seq_row = 1;
  g_tavProps.first_seq -> len     = 0;
  g_tavProps.first_seq -> max_len = LINE_SIZE;
  g_tavProps.first_seq -> data    = malloc(LINE_SIZE * sizeof(char));
  g_tavProps.first_seq -> next    = NULL;

  g_tavProps.current_seq = g_tavProps.first_seq;
}

/*
 * This method is used to refresh all the screen contents
 *
 */
void drawWindow(void)
{
  clrscr;
  int rows        = g_tavProps.w_row;
  int cols        = g_tavProps.w_col;
  int act_rows    = g_tavProps.act_rows;
  int current_row = g_tavProps.cursor_row;
  int current_col = g_tavProps.cursor_col;

  // write down all the sequences to the stdout
  sequence* start = g_tavProps.first_seq;

  while (start != NULL)
  {
    printf("%s\n", start -> data);
    start = (sequence*) start -> next;
  }

  gotopos(act_rows+1,0);
  for (int i = 1; i < rows-act_rows; i++)
    printf("~\n");

  setStatusLine();
  gotopos(current_row+1, current_col + 1); // i have no idea why we need +1 , but its required
}

/*
 * This method is used to set the status line in TAV
 *
 * If the file is dirty, set a * too
 */
void setStatusLine(void)
{
  int bottom       = g_tavProps.w_row; // last row left for commands and messages
  int right_offset = g_tavProps.w_col - 11;
  char* mode       = g_tavProps.mode;
  char* filename   = g_tavProps.filename;

  // go to the right postion of status line
  gotopos(bottom, 0);
  printf(" %6s |", mode);
  if (g_tavProps.is_mod)
    printf(" %s* ", filename);
  else
    printf(" %s ", filename);

  gotopos(bottom, right_offset);
  printf("LN %4d:%-3d", g_tavProps.cursor_row + 1, g_tavProps.cursor_col + 1);
}
