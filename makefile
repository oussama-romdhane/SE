CC=gcc
CFLAGS=-I.

shell: shell.o readline.o splitline.o execute.o create_shell.o removeWhitespace.o
	$(CC) -o shell shell.o readline.o splitline.o execute.o create_shell.o removeWhitespace.o