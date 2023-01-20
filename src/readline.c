#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *readline() {
  char *line = NULL;
  size_t len = 0;
  int position = 0;
  if (getline(&line, &len, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);
    } else {
      printf("Error reading input line\n");
      exit(EXIT_FAILURE);
    }
  }

  while (line[position] != '\0') {
    position++;
  }
  line[position - 1] = '\0';

  return line;
}