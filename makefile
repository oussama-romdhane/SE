CC=gcc
CFLAGS=-I.

shell: shell.o removeWhitespace.o
	$(CC) -o shell shell.o removeWhitespace.o