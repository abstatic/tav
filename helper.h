#ifndef HELPER
#define HELPER

// system header files required
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>

#define cursorforward(x) printf("\033[%dD", (x))
#define cursorbackward(x) printf("\033[%dC", (x))
#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))
#define clearscr printf("\033c")  // VT100 magic to clear screen without scroll
#define gotopos(x,y) printf("\033[%d;%dH", (x), (y))

// escape characters for keys
#define ESCAPE    0x001b
#define ENTER     0x000a
#define UP        0x0105
#define DOWN      0x0106
#define LEFT      0x0107
#define RIGHT     0x0108
#define BACKSPACE 0x7f
#define DELETE    126

// function declaration 
void getControl(int fd);
void readKey(void);
void handleEscapeSequence(void);
void handle_winresize(int sig);
void initscr(void);
void drawWindow(void);

// struct declarations
typedef struct
{
  int w_row;
  int w_col;
  int cursor_x;
  int cursor_y;
  int is_mod;
} config;

// extern variables go here
// global so we use suffix g_
// This only tells the other files that this variable exists
extern config g_tavProps;

#endif
