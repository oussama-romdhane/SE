int execute_multiple(char *cmd);
int execute_and(char **commands);
int execute_or(char **commands);
int execute_wait(char **commands);
int execute_pipe(char **commands);
int execute_redirect(char **commands);
int execute(char *cmd);
