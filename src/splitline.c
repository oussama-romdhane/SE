#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **splitline(char *line, char *delim) {
  int TOKEN_BUFFER_SIZE = 64;
  int bufferSize = TOKEN_BUFFER_SIZE;
  int position = 0;
  char **tokens = malloc(bufferSize * sizeof(char *));
  char *token;

  if (!tokens) {
    printf("Allocation error\n");
    exit(EXIT_SUCCESS);
  }

  token = strtok(line, delim);

  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufferSize) {
      bufferSize += TOKEN_BUFFER_SIZE;
      tokens = realloc(tokens, bufferSize * sizeof(char *));
      if (!tokens) {
        printf("Allocation error\n");
        exit(EXIT_SUCCESS);
      }
    }

    token = strtok(NULL, delim);
  }
  tokens[position] = NULL;
  return tokens;
}