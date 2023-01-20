#include "execute.h"
#include "readline.h"
#include "splitline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void create_shell(void) {
  char *cmd;
  FILE *history;

  do {
    history = fopen("history.txt", "a+");

    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
    } else {
      printf("error getting the current system path\n");
      return;
    }

    printf("%s %% ", cwd);

    cmd = readline();

    if (strcmp(cmd, "quit") == 0) {
      exit(EXIT_SUCCESS);
    }

    else if (strcspn(cmd, "cd") == 0) {
      char **args = splitline(cmd, " ");
      chdir(args[1]);
    } else if (strcmp(cmd, "help") == 0) {
      printf("     //////////     \n");
      printf("\nOussema Romdhane and Anaghim Ben Souissi's Shell\n");
      printf("Type program names and arguments, and hit enter.\n");
      printf("The following commands are built in:\n");
      printf("cd: navigate to different directory\n");
      printf("help: display built in command names\n");
      printf("history: display previous commands history\n");
      printf("quit: close the shell: \n");
      printf("\n     //////////     \n");
    } else if (strcmp(cmd, "history") == 0) {
      char *args[3];
      int pid;
      args[0] = "cat";
      args[1] = "history.txt";
      args[2] = NULL;

      pid = fork();
      if (pid == 0) {

        if (execvp(args[0], args) == -1) {
          printf("Error executing the command\n");
          exit(9);
        }
        exit(0);
      } else {
        wait(NULL);
        printf("%% ");
      }
    } else if (strstr(cmd, ".sh") != NULL) {
      char filename[20];

      FILE *fp;
      char *line = NULL;
      size_t len = 0;
      ssize_t read;

      strcpy(filename, cmd);
      fp = fopen(filename, "r");

      if (fp == NULL)
        printf("This file doesn't exist\n");

      else {
        while ((read = getline(&line, &len, fp)) != EOF) {
          line[strcspn(line, "\n")] = 0;
          printf("Line : %s\n", line);
          execute_multiple(line);
          free(line);
          line = NULL;
        }
      }
    } else {
      fprintf(history, "%s\n", cmd);
      execute_multiple(cmd);
      fclose(history);
    }
    free(cmd);
  } while (1);
}