#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/shell.h"
#include "../include/utils.h"



char *egg_readline(char *prompt){
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

    if(!tokens){
        error("Memory allocation error.");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIM); 
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= tokens_size){
            tokens_size += MAX_TOK_BUFSIZE;
            tokens = realloc(tokens, tokens_size);
            if(!tokens){
                error("Memory reallocation error.");
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

    if(cpid <= -1){
        error("Error forking");
        exit(EXIT_FAILURE);
    }
    else if(cpid == 0){
        if (execvp(args[0], args) < 0) {
            error("Command not found: %s", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else{
        do {
            waitpid(cpid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int execute(char **args){
    if(args[0] == NULL) { return 1; }
    for(int i = 0; i < size_of_builtin_func(); i++) {
        if(strcmp(builtin_str[i], args[0]) == 0){
            return (*builtin_func[i])(args);
        }
    } 
    return launch(args);
}


