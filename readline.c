#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *readline() {
  char *line = NULL;
  ssize_t len = 0; // have getline allocate a buffer for us
  int position = 0;
  if (getline(&line, &len, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS); // We recieved an EOF
    } else {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  while (line[position] != NULL) {
    position++;
  }
  line[position - 1] = '\0';

  return line;
}