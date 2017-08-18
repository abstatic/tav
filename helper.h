/***
 * helper.h
 *
 * This header file contains all the related fucntion declarations, imports
 * and macros for handing various escape sequences
 *
 * Author: Abhishek Shrivastava <abhishek.shrivastava.ts@gmail.com>
 */

// Header guard  we don't want to deal with multiple import of same type
#ifndef HELPER
#define HELPER

// system header files required go here
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>

// macros for escape sequences go here
#define cursorforward(x)    printf("\033[%dD", (x))
#define cursorbackward(x)   printf("\033[%dC", (x))
#define cursorupward(x)     printf("\033[%dA", (x))
#define cursordownward(x)   printf("\033[%dB", (x))
#define clrscr              printf("\033c")  //VT100 clear screen without scroll
#define gotopos(row,col)    printf("\033[%d;%dH", (row), (col))

// escape characters for keys
#define ESCAPE              0x001b
#define ENTER               0x000a
#define UP                  0x0105
#define DOWN                0x0106
#define LEFT                0x0107
#define RIGHT               0x0108
#define BACKSPACE           0x7f
#define DELETE              126

// application defaults
#define NORMAL_MODE         "Normal"
#define INSERT_MODE         "Insert"
#define DEFAULT_FILE_NAME   "[NoName]"
#define DEFAULT_MODE        "Normal"
#define LINE_SIZE           80


// function declaration 
void getControl(int fd);
void readKey(void);
void handleEscapeSequence(void);
void handle_winresize(int sig);
void initscr(void);
void drawWindow(void);
void setStatusLine(void);

/*
 * A sequence will contain the metadata about the particular row
 *  Reference - https://www.cs.unm.edu/~crowley/papers/sds.pdf
 *
 * Linked line approach -
 *  The document is represented as linked list of different sequences.
 *  This is better than using a 2D array for document because line insertions
 *  are pretty easy and termination also, just have to use \0
 *
 * prev         - the previous line
 * sequence_row - the row number at which the sequence is supposed to come
 * len          - the length of sequence
 * data         - a pointer to the stored string;
 * next         - the next line
 */
typedef struct
{
  struct sequence* prev;
  int seq_row;
  int len;
  char* data;
  struct sequence* next;
} sequence;

/*
 * config struct holds all the information related to the current state of
 * editor
 *
 * w_row    - number of rows supported by window, will change with window resize
 * w_col    - number of cols supported by window, will change with window resize
 * cursor_x - the current x coordinate of cursor
 * curosr_y - the current y coordinate of cursor
 * is_mod   - boolean to check whether the data in editor was modified or not
 * mode     - the mode the editor is opetating in
 * filename - the name of file being currently edited
 * act_row  - the number of active rows
 */
typedef struct
{
  int w_row;
  int w_col;
  int cursor_x;
  int cursor_y;
  int is_mod;
  char* mode; // 0 is for NORMAL, 1 is for EDIT
  char* filename;
  int act_rows; // number of active rows must match the number of sequecnes
  sequence* first_seq;
  sequence* current_seq;
} config;


/* extern variables go here
 * global so we use suffix g_
 * This only tells the other files that this variable exists
 */
extern config g_tavProps;

#endif
