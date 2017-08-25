## Makefile for TAV (This Ain't Vi)
## Inspired by - http://sites.fas.harvard.edu/~lib113/reference/progtools/make/
## Author- Abhishek Shrivastava <abhishek.shrivastava.ts@gmail.com)

## Use EXE to represent the name of the program that we're building
EXE = tav

## SRC is the list of .c files we need to build our program
SRC = helper.c tav.c keyhandler.c filehandler.c shell.c

## Tell make to use gcc to compile
CC = gcc

## Specify some options to C compiler
CC_OPTIONS = -ggdb3 -Wall 
CFLAGS = $(CC_OPTIONS)

## target all only link after the object files have been created
## prerequisite is files that are used as input to create the target
all:
	$(CC) $(CC_OPTIONS) -o $(EXE) $(SRC)

run:
	$(CC) $(CC_OPTIONS) -o $(EXE) $(SRC)
	./$(EXE)

clean:
	rm tav
