/**
 * keyhandler.c
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

  int cur_row = g_tavProps.cursor_row;
  int cur_col = g_tavProps.cursor_col;
  int cur_seq_len = g_tavProps.current_seq -> len;

  if (c == ESCAPE)
    handleEscapeSequence();
  else if (c == '\r')
  {
    /* Enter also has three cases
     * 1. Enter at line ending
     * 2. Enter at line start
     * 3. Enter at line middle
     */
    // below condition means that we are at the row end
    if (cur_col == cur_seq_len)
    {
      sequence* new_seq = malloc(sizeof(sequence));

      new_seq -> prev    = (struct sequence *) g_tavProps.current_seq;
      new_seq -> seq_row = g_tavProps.current_seq -> seq_row + 1;
      new_seq -> len     = 0;
      new_seq -> max_len = LINE_SIZE;
      new_seq -> data    = malloc(LINE_SIZE * sizeof(char));
      new_seq -> next    = g_tavProps.current_seq -> next;

      g_tavProps.current_seq -> next = (struct sequence *) new_seq;
      g_tavProps.current_seq = new_seq;
      g_tavProps.act_rows++; // increase the number of active rows by 1

      modify_cur_pos(LF, CR, 1);
    }
  }
  else if (c == BACKSPACE)
  {
    // handle backspace here, similar to enter at line end
    if (cur_col == cur_seq_len)
    {
      g_tavProps.current_seq -> data[cur_seq_len-1] = '\0';
      modify_seq_len(-1);
      modify_cur_pos(0, -1, 1);
    }
    else
    {
      insert(g_tavProps.current_seq, cur_col-1, 1, '\0');
      modify_seq_len(-1);
      modify_cur_pos(0, -1, 1);
    }
  }
  else
  {
    // append the read character to the end of currrent sequence or the cursor
    // pos?
    // below condition means that we are inserting at the row end
    if (cur_col == cur_seq_len)
    {
      g_tavProps.current_seq -> data[cur_seq_len] = c;
      g_tavProps.current_seq -> data[cur_seq_len + 1] = '\0';
      modify_seq_len(1);
      modify_cur_pos(0, 1, 1);
    }
    else
    {
      insert(g_tavProps.current_seq, cur_col, 1, c);
      modify_seq_len(1);
      modify_cur_pos(0, 1, 1);
    }
  }
}

/*
 * This method is used to insert the text in between a sequence
 *
 */
void insert(sequence* s, int make_room_at, int room_to_make, char toInsert)
{
  char* base = s -> data;

  // max length of the sequence
  int len = s -> max_len;
  if (toInsert == '\0')
  {
    memmove (
        base + make_room_at,
        base + make_room_at + 1,
        len - (make_room_at + room_to_make)
    );
  }
  else
  {
    memmove (
        base + make_room_at + room_to_make,
        base + make_room_at,
        len - (make_room_at + room_to_make)
    );
    base[make_room_at] = toInsert;
  }
}

/*
 * This method is used to modify the value of cursor positions
 *
 * It also handles the cases related to invalid positions
 * Basically the cursor should not be able to move to invalid locations using
 * arrow keys.
 *
 * @param row: value by which to modify the row +- 1
 * @param col: value by which to modify the col +- 1
 * @param KorA: 1- means called by keypress, 0- means called by arrow 
 */
void modify_cur_pos(int row, int col, int KorA)
{
  int col_limit = g_tavProps.current_seq -> len;
  int row_limit = g_tavProps.act_rows;

  // if we are doing a arrow movement. make sure that its a valid movement
  // else we return
  if (KorA == 0)
  {
    if (g_tavProps.cursor_row + row >= row_limit || g_tavProps.cursor_col + col > col_limit)
      return;
  }

  // if enter was pressed as a keypress
  if (row == LF)
  {
    g_tavProps.cursor_row += 1;
    if (col == CR)
      g_tavProps.cursor_col = 0;
  }
  else
  {
    // below statements execute when there has been a keypress or a valid
    // arrow movement was detected
    g_tavProps.cursor_row += row;
    g_tavProps.cursor_col += col;
    // code for updating the current_seq as well
    if (row != 0)
    {
      if (row == -1)
      {
        if (g_tavProps.current_seq -> prev != NULL)
        {
          g_tavProps.current_seq = (sequence *) g_tavProps.current_seq -> prev;
        }
      }
      else
      {
        if (g_tavProps.current_seq -> next != NULL)
          g_tavProps.current_seq = (sequence *) g_tavProps.current_seq -> next;
      }
      col_limit = g_tavProps.current_seq -> len;
    }

  }

  if (g_tavProps.cursor_row < 0)
    g_tavProps.cursor_row = 0;

  if (g_tavProps.cursor_col < 0)
    g_tavProps.cursor_col = 0;

  if (g_tavProps.cursor_col > col_limit)
    g_tavProps.cursor_col = col_limit;
}

/*
 * This method is used to either increment or decrement the current seq length
 *
 * @param val: Value by which to change the sequence length
 */
void modify_seq_len(int val)
{
  g_tavProps.current_seq -> len += val;
  if (g_tavProps.current_seq -> len < 0)
    g_tavProps.current_seq -> len = 0;

  // if the array is almost about to exceed the size, reallocate
  int m_len = g_tavProps.current_seq -> max_len;
  if (g_tavProps.current_seq -> len > m_len - 10)
  {
    g_tavProps.current_seq -> data = (char *) realloc(g_tavProps.current_seq -> data, m_len * 2 * sizeof(char));
    g_tavProps.current_seq -> max_len = m_len * 2;
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
        key = RIGHT;
        break;
      case 'D':
        key = LEFT;
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
      switch (key)
      {
        case UP:
          modify_cur_pos(-1, 0, 0);
          break;
        case DOWN:
          modify_cur_pos(1, 0, 0);
          break;
        case LEFT:
          modify_cur_pos(0, -1, 0);
          break;
        case RIGHT:
          modify_cur_pos(0, 1, 0);
          break;
        default:
          printf("Unknown");
      }
    }
  }
}
