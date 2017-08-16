#ifndef HELPER
#define HELPER
// system header files required
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))
#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))
#define clearscr printf("\033[2J") // erases the screen and moves cursor to 0,0
#define gotopos(x,y) printf("\033[%d;%dH", (x), (y))

// escape characters for keys
#define ESCAPE    0x001b
#define ENTER     0x000a
#define UP        0x0105
#define DOWN      0x0106
#define LEFT      0x0107
#define RIGHT     0x0108
#define BACKSPACE 0x7f

// function declaration 
void getControl(int fd);
void readKey(void);
void handleEscapeSequence(void);

// struct declarations
#endif
