#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

char *read_line(char *prompt){
    char *line = readline(prompt);
    if(line && *line){
        add_history(line);
    }

    return line;
}

char **split(char *line){
    int tokens_size = MAX_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(sizeof(char*) * tokens_size);
    char *token;

    if(!token){
        error("Error allocating memory for *token.");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if (position >= tokens_size){
            tokens = realloc(tokens, tokens_size);

            if(!tokens){
                error("Error reallocating memory for **tokens.");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int launch(char **args){
    pid_t cpid;
    int status;

    cpid = fork();
    if(cpid == -1){
        error("Error forking process.");
        return EXIT_FAILURE;
    } else if(cpid == 0) {
        if(execvp(args[0], args) < 0){
            error("Command not found: %s", args[0]);
            return EXIT_FAILURE;
        }
    } else{
        do {
            waitpid(cpid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int execute(char **args){
    if (args[0] == NULL) return 1;
    for(int i = 0; i < size_of_builtin_func(); i++){
        if (strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }
    return launch(args);
}

char *get_path(){
    char *path = getcwd(NULL, PATH_MAX);
    char *home = getenv("HOME");

    if(strncmp(path, home, strlen(home)) == 0){
        char *new_path = malloc(sizeof(char) * PATH_MAX);
        snprintf(new_path, PATH_MAX, "~%s", path+strlen(home));
        free(path);
        return new_path;
    }

    return path;
}

void loop() {
    char *line;
    char **args;

    while(1) {
        char *path = get_path();
        printf(CYAN "\n[%s] " COLOR_RESET "%s", SHELL_NAME, path);
        line = read_line(" > ");
        args = split(line);

        execute(args);

        free(path);
        free(line);
        free(args);
    }
}

void handle_kill(int sig){
    info("\nsignal %d\n", sig);
}

int main(){
    signal(SIGINT, handle_kill);
    loop();
    return EXIT_SUCCESS;
}
