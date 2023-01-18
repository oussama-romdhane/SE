#include "execute.h"
#include "readline.h"
#include "splitline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_shell(void) {
  char *cmd;
  int status;
  int position = 0;

  do {
    printf("%% ");

    cmd = readline();

    if (strcmp(cmd, "quit") == 0) {
      exit(EXIT_SUCCESS);
    }

    if (strstr(cmd, ".sh") != NULL) {
      char filename[20];

      FILE *fp;
      char *line = NULL;
      size_t len = 0;
      ssize_t read;

      strcpy(filename, cmd);
      fp = fopen(filename, "r");

      if (fp == NULL)
        printf("Ce fichier n'existe pas\n");

      else {
        while ((read = getline(&line, &len, fp)) != EOF) {
          line[strcspn(line, "\n")] = 0;
          printf("line : %s\n", line);
          execute_multiple(line);
          free(line);
          line = NULL;
        }
      }
    } else {
      execute_multiple(cmd);
    }

    free(cmd);
  } while (1);
}