/*
 * filhandler.c
 *
 * This file contains all the filehandling functionality of TAV
 *
 * Author: Abhishek Shrivastava <abhishek.shrivastava.ts@gmail.com>
 *
 */
#include "helper.h"
/*
 * This method is used to read a given file and store each line as a sequence
 *
 * @param filename: The filename that was passed
 * @return : 0 if fail, else 1
 */
int readFile(char* filename)
{
  FILE* fp = NULL;

  fp = fopen(filename, "r");

  if (fp == NULL)
    return 0;

  char* line = malloc(1024 * sizeof(char));

  while (fgets(line, 1024, fp))
  {
    int line_len = strlen(line);
    sequence* new_seq = malloc(sizeof(sequence));

    new_seq -> prev    = NULL;
    new_seq -> next    = NULL;
    new_seq -> seq_row = 0;
    new_seq -> len     = line_len;
    new_seq -> max_len = line_len + LINE_SIZE;
    new_seq -> data    = malloc((line_len + LINE_SIZE) * sizeof(char));

    memcpy(new_seq -> data, line, line_len);

    new_seq -> data[line_len-1] = '\0'; // just to be sure and ignore the \n

    if (g_tavProps.act_rows == 1)
    {
      new_seq -> seq_row = 1;
      g_tavProps.first_seq = new_seq;
      g_tavProps.current_seq = g_tavProps.first_seq;
      g_tavProps.act_rows++;
    }
    else
    {
      new_seq -> seq_row = g_tavProps.current_seq -> seq_row + 1;
      g_tavProps.current_seq -> next = (struct sequence*) new_seq;
      new_seq -> prev = (struct sequence*) g_tavProps.current_seq;
      g_tavProps.current_seq = new_seq;
      g_tavProps.act_rows++;
    }
  }
  g_tavProps.current_seq = g_tavProps.first_seq; // IMP
  fclose(fp);
  // free because getline allocated the required memory to store it
  if (line)
      free(line);
  return 1;
}
