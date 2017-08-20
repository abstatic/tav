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
  tcgetattr(fd, &g_tavProps.o_term);

  g_tavProps.n_term = g_tavProps.o_term;

  // the input flags are needed to handle the CR LF situation.
  // Its harder to detect \r\n , \r is easier to catch
  // New lines are completely handled by the program. Not terminal
  g_tavProps.n_term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  g_tavProps.n_term.c_lflag &= ~(ICANON | ECHO);

  g_tavProps.n_term.c_cc[VTIME] = 0;
  g_tavProps.n_term.c_cc[VMIN] = 1;
  // TCSANOW means apply the changes immediately
  tcsetattr(fd, TCSANOW, &g_tavProps.n_term);
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
  g_tavProps.cmd_buf     = calloc(CMD_LEN, sizeof(char));
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
  if (strlen(g_tavProps.cmd_buf) == 0)
  {
    printf(" %6s |", mode);
    if (g_tavProps.is_mod)
      printf(" %s* ", filename);
    else
      printf(" %s ", filename);
  }
  else
  {
    printf("%s", g_tavProps.cmd_buf);
    printf(" %ld", strlen(g_tavProps.cmd_buf));
  }

  gotopos(bottom, right_offset);
  printf("LN %4d:%-3d", g_tavProps.cursor_row + 1, g_tavProps.cursor_col + 1);
}

/*
 * Replace the character right under
 */
void replace(void)
{
  g_tavProps.mode = REPLACE_MODE;
  drawWindow(); // because i want my replace mode written
  char c;
  c = getc(stdin);

  int col = g_tavProps.cursor_col;

  if (isalnum(c))
    g_tavProps.current_seq -> data[col] = c;
  g_tavProps.mode = NORMAL_MODE;
}


/*
 * This function helps in interpreting and processing the command
 * at the last line
 *
 * Idea- Just change the contents at he memory location of cmd_buf
 */
void interpretCommand(void)
{
  char c;
  g_tavProps.cmd_buf[0] = ':';
  drawWindow();
  int i = 1;
  while (1)
  {
    c = getc(stdin);
    if (c == ESCAPE)
    {
      memset(g_tavProps.cmd_buf, '\0', CMD_LEN);
      return;
    }
    else if (c == '\r')
      break;
    else if (c == BACKSPACE)
    {
      g_tavProps.cmd_buf[i-1] = '\0';
      i--;
    }
    else
    {
      g_tavProps.cmd_buf[i] = c;
      i++;
    }
    if (i < 0)
      i = 0;
    drawWindow();
  }

  char inp = g_tavProps.cmd_buf[1];

  switch (inp)
  {
    case 'w':
      //write the damned file
      writeFile();
      break;
    case 'q':
      // check if not dirty then save else wait and show error message
      // also check for force quit
      if (g_tavProps.cmd_buf[2] == '!' || g_tavProps.is_mod == 0)
        exit_safely();
      else
      {

      }
      break;
    case '!':
      // try to execute the bash command
      executeBash();
      memset(g_tavProps.cmd_buf, '\0', CMD_LEN);
      break;
    default:
      printf("h");
      // default case
  }
  memset(g_tavProps.cmd_buf, '\0', CMD_LEN);
}

/**
 * This method is used to safely exit from tav after setting back the original
 * parameters of the terminal
 */
void exit_safely(void)
{
  int fd = STDOUT_FILENO;
  fclose(g_tavProps.openFile);
  tcsetattr(fd, TCSANOW, &g_tavProps.o_term);
  clrscr;
  exit(0);
}
