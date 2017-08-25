#TAV - This Ain't Vi

Welcome to TAV! Tav is a minimalistic vi like editor with the following features-

**1. Editing Modes**- Insert/Edit, Normal Mode, Command Mode  

**2. Save** - Use :w to save. Tav won't let you quit with unsave data.  

**3. Navigation** - Navigation using arrow and h,j,k,l characters to navigate around or scroll  

**4. Single Character Replace**  

**5. Go to first line, go to last line**- Using gg and G  

**6. Deletion** - Deletion using Backspace key.  


##Design-
Data structure and text rendering are at the heart of any text editor, we will look at how TAV handles them.

###**Data Structure**

TAV uses a simple linked line approach to storing, reading and rendering text. Basically a document is represented as a sequence of linked lines. Each sequence is aware about its row number, position in document and about the previous sequence and the next sequence.

A sequence is basically a struct with-
````
typedef struct sequence
{
  struct sequence* prev;
  int seq_row;
  int len;
  int max_len;
  char* data;
  struct sequence* next;
} sequence;
````

The meaty part of the sequence is the array pointer to data. `data` is the pointer to the contents of the line. `memcpy` , `reallloc` and `calloc` are used to manipulate the contents of data. 

For inserting in middle `memcpy` is used to make the space. In case the `data` pointer is about to run out of memory, realloc is used to reallocate more memory. 

For initialization `calloc` is preffered over `malloc` because having `\0` in arrays really helps in figuring out the line ends.

###**Text Rendering**

Text rendering is very naive. The editor knows about the document state, the screen state and window size. Basically it redraws all the screen contents as soon as anything changes.

The editor knows about which sequence number to start from and at which sequence number it should end. From there on its just a simple matter of traversing each sequence and printing out the data part of sequence.


##TODO-
Some things that I would like to implement are-

**1. Syntax highlighting**- We can make a trie out of keyworrds, and then for each existing keyword add color. There is existing code which renders color. Can be reused/looked at.

**2. Autosave** 
