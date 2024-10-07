#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>

#define RED "\033[31m"
#define COLOR_RESET "\x1b[0m"
#define CYAN "\x1B[36m"
#define BLUE "\x1B[34m"

#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"
#define SHELL_NAME "EggShell"


#define error(...) fprintf(stderr, RED "[%s] ", SHELL_NAME); \
                   fprintf(stderr, __VA_ARGS__); \
                   fprintf(stderr, COLOR_RESET "\n")

#define info(...) printf(CYAN "[%s] ", SHELL_NAME); \
                  printf(COLOR_RESET __VA_ARGS__); \
                  printf("\n")

char *egg_readline(char *prompt);
char **split(char *line);
int launch(char **args);
int execute(char **args);
void loop();

int egg_cd(char **args);
int egg_help(char **args);
int egg_exit(char **args);
int egg_export(char **args);

int size_of_builtin_func();

extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

#endif
