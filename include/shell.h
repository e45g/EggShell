#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>

#define MAX_LINE_BUFSIZE 1024
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"
#define SHELL_NAME "EggShell"

char *egg_readline(char *prompt);
char **split(char *line);
int launch(char **args);
int execute(char **args);
void loop();
int egg_cd(char **args);
int egg_help(char **args);
int egg_exit(char **args);

int size_of_builtin_func();

extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

#endif
