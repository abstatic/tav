/**
 *
 * All key handling and reading functions go here
 * Part of the TAV text editor
 *
 * Author: Abhishek Shrivastav <abhishek.shrivastava.ts@gmail.com>
 */
#include "helper.h"
/*
 * This function is used to handle all the keypresses.
 *
 * Depending on the mode and keypress this function will call respective 
 * routines
 */
void readKey(void)
{
  char c;

  c = getc(stdin);

  if (c == ESCAPE)
    handleEscapeSequence();
  else
  {
    // append the read character to the end of currrent sequence or the cursor
    // pos?
    int len = g_tavProps.current_seq -> len;
    g_tavProps.current_seq -> data[len] = c;
    g_tavProps.current_seq -> data[len+1] = '\0';
    g_tavProps.current_seq -> len++;
  }
}

/*
 * This function will handle the escape sequence of keys
 */
void handleEscapeSequence(void)
{
  /*
   * Since every escape sequence is basically three characters
   * First has already been read
   */
  int c;
  c = getc(stdin);
  if (c == '[')
  {
    int key = getc(stdin);

    switch(key)
    {
      case 'A':
        key = UP;
        break;
      case 'B':
        key = DOWN;
        break;
      case 'C':
        key = LEFT;
        break;
      case 'D':
        key = RIGHT;
        break;
      case '~':
        key = DELETE;
        break;
      default:
        key = 0;
        break;
    }

    if (key != 0)
    {
      if (key == UP)
        cursorupward(1);
      else if (key == DOWN)
        cursordownward(1);
      else if (key == LEFT)
        cursorbackward(1);
      else if (key == RIGHT)
        cursorforward(1);
      else if (key == DELETE)
        printf("DEL");
    }
  }
}

