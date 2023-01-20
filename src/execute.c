#include "splitline.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int execute(char *cmd) {
  char **args = splitline(cmd, " ");

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      printf("Error executing the command\n");
      exit(9);
    }
    exit(0);
  } else if (pid < 0) {
    printf("Error creating subprocess\n");
  } else {
    do {
      wpid = waitpid(pid, &status, 0);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int execute_and(char **commands) {
  int position = 0;
  int isExecuted = 0;

  while (commands[position] != NULL && !isExecuted) {
    char **args = splitline(commands[position], " ");
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {

      if (execvp(args[0], args) == -1) {
        printf("Error executing the command\n");
        exit(9);
      }
      exit(0);
    } else if (pid < 0) {

      printf("Error creating subprocess\n");
    } else {

      do {
        wpid = waitpid(pid, &status, 0);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    isExecuted = WEXITSTATUS(status);

    position++;
  }
  return 1;
}

int execute_or(char **commands) {
  int position = 0;
  int isExecuted = 1;

  while (commands[position] != NULL && isExecuted) {
    char **args = splitline(commands[position], " ");

    pid_t pid, wpid;

    int status;

    pid = fork();
    if (pid == 0) {

      if (execvp(args[0], args) == -1) {
        printf("Error executing the command\n");
        exit(9);
      }
      exit(0);
    } else if (pid < 0) {
      printf("Error creating subprocess\n");
    } else {

      do {
        wpid = waitpid(pid, &status, 0);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    isExecuted = WEXITSTATUS(status);
    position++;
  }
  return 1;
}

int execute_wait(char **commands) {
  int position = 0;

  while (commands[position] != NULL) {
    char **args = splitline(commands[position], " ");

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {

      if (execvp(args[0], args) == -1) {
        printf("Error executing the command\n");
        exit(9);
      }
      exit(0);
    } else if (pid < 0) {

      printf("Error creating subprocess\n");
    } else {

      do {
        wpid = waitpid(pid, &status, 0);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    position++;
  }
  return 1;
}

int execute_pipe(char **commands) {
  int pipefd[2];
  pid_t p1, p2;

  char **parsed = splitline(commands[0], " ");
  char **parsedPipe = splitline(commands[1], " ");

  if (pipe(pipefd) < 0) {
    printf("Pipe could not be initialized\n");
    return 0;
  }

  p1 = fork();
  if (p1 < 0) {
    printf("Error creating subprocess\n");
    return 0;
  }

  if (p1 == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    if (execvp(parsed[0], parsed) < 0) {
      printf("Error executing the first command\n");
      exit(0);
    }
  } else {
    p2 = fork();

    if (p2 < 0) {
      printf("Error creating subprocess\n");
      return 0;
    }

    if (p2 == 0) {

      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);

      if (execvp(parsedPipe[0], parsedPipe) < 0) {
        printf("Error executing the second command\n");
        exit(0);
      }
    } else {
      close(pipefd[0]);
      close(pipefd[1]);

      wait(NULL);
      wait(NULL);
    }
  }

  return 1;
}

int execute_redirect(char **commands) {
  pid_t p1;

  char **parsed = splitline(commands[0], " ");
  char *parsedRedirect = commands[1];
  int file = open(parsedRedirect, O_WRONLY | O_CREAT, 0777);

  if (file == -1) {
    printf("Error creating file\n");
    return 0;
  }

  p1 = fork();
  if (p1 < 0) {
    printf("Error creating subprocess\n");
    return 0;
  }

  if (p1 == 0) {
    dup2(file, STDOUT_FILENO);
    close(file);
    if (execvp(parsed[0], parsed) < 0) {
      printf("Could not execute the first command\n");
      exit(0);
    }
  } else {
    close(file);

    wait(NULL);
  }
  return 1;
}

int execute_multiple(char *cmd) {
  char *delim = NULL;
  char **commands;

  if (strstr(cmd, "&&") != NULL) {
    delim = "&&";
  } else if (strstr(cmd, "||") != NULL) {
    delim = "||";
  } else if (strstr(cmd, ";") != NULL) {
    delim = ";";
  } else if (strstr(cmd, "|") != NULL) {
    delim = "|";
  } else if (strstr(cmd, ">") != NULL) {
    delim = ">";
  } else {
    execute(cmd);
    return 1;
  }

  if (strcmp(delim, "&&") == 0) {
    commands = splitline(cmd, delim);
    execute_and(commands);
  }

  else if (strcmp(delim, "||") == 0) {
    commands = splitline(cmd, delim);
    execute_or(commands);
  }

  else if (strcmp(delim, ";") == 0) {
    commands = splitline(cmd, delim);
    execute_wait(commands);
  } else if (strcmp(delim, "|") == 0) {

    commands = splitline(cmd, delim);
    execute_pipe(commands);
  } else if (strcmp(delim, ">") == 0) {

    commands = splitline(cmd, delim);
    execute_redirect(commands);
  }

  return 1;
}
