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
 * File Descriptor closing by exit method
 *
 * If the filename cannot be found then we open the file descriptor in
 * write mode.
 *
 * @param filename: The filename that was passed
 * @return : 0 if fail, else 1
 */
int readFile(char* filename)
{
  g_tavProps.openFile = fopen(filename, "r");

  // if the file can't be opened in "r" mode then perhaps it doesn't exist
  // We initialize the editor and start getting user input.
  // We anyway, create a new file while writing the file
  if (g_tavProps.openFile == NULL)
    return 1;

  char* line   = (char*)calloc(1024 , sizeof(char));
  char* w_line = (char*)calloc(1024 , sizeof(char));

  while (fgets(line, 1024, g_tavProps.openFile))
  {
    // this code is to handle the tabs. just interpret tabs as sequence of
    // spaces
    int i = 0;
    int j = 0;
    while (line[i])
    {
      if (line[i] == '\t')
        for (int i = 0; i < TAB_LENGTH; i++)
          w_line[j++] = ' ';
      else
        w_line[j++] = line[i];
      i++;
    }
    int line_len = strlen(w_line);
    sequence* new_seq = malloc(sizeof(sequence));

    new_seq -> prev    = NULL;
    new_seq -> next    = NULL;
    new_seq -> seq_row = 0;
    new_seq -> len     = line_len - 1;
    new_seq -> max_len = line_len + LINE_SIZE;
    new_seq -> data    = calloc((line_len + LINE_SIZE) , sizeof(char));

    /* memcpy(new_seq -> data, line, line_len); */
    memcpy(new_seq -> data, w_line, line_len);

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
  // free because getline allocated the required memory to store it
  if (line)
    free(line);
  if (w_line)
    free(w_line);
  // needed because while loop runs one extra time than intended 
  // while reading the file 
  g_tavProps.act_rows--;

  //  after reading the file close the file
  fclose(g_tavProps.openFile);
  return 1;
}

/**
 * This method is used to write to the file
 *
 * Also we need to rewind the file desecriptor. As the saving methhod is to
 * save most of the file by rewriting approach
 */
void writeFile()
{
  // recreate the file from scratch
  g_tavProps.openFile = fopen(g_tavProps.filename, "w");
  sequence* start = g_tavProps.first_seq;
  rewind(g_tavProps.openFile);

  while (start != NULL)
  {
    fprintf(g_tavProps.openFile, "%s\n", start -> data);
    start = start -> next;
  }
  g_tavProps.is_mod = 0;
}
