
#include "removeWhitespace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  char str[20];

  char command[50];

  int i = 0;

  while (i < 10) {
    int status;

    printf("veuillez introduire une commande: \n");
    gets(str);

    if (strcmp(str, "quit") == 0) {
      exit(0);
    }

    if (strstr(str, ".sh") != NULL) {
      char filename[20];
      FILE *fp;
      char *line = NULL;
      size_t len = 0;
      ssize_t read;

      strcpy(filename, str);
      fp = fopen(filename, "r");

      if (fp == NULL)
        printf("Ce fichier n'existe pas\n");

      else {
        while ((read = getline(&line, &len, fp)) != -1) {
          char *cleanLine = NULL;
          char *cmd = NULL;
          cleanLine = removeWhitespace(line);
          printf("commande: %s\n", cleanLine);

          cmd = strtok(cleanLine, "&&");

          while (cmd != NULL) {
            printf("split commands: %s\n", cmd);
            system(cmd);

            cmd = strtok(NULL, "&&");
          }
        }
        free(line);
      }

    } else {
      printf("vous avez choisi la commande: %s \n", str);

      strcpy(command, str);

      int c = fork();

      if (c == 0) {
        system(command);
        exit(0);
      }
    }

    wait(&status);

    i++;
  }
  return 0;
}