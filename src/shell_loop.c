#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/utils.h"
#include "../include/shell.h"

void loop(){
    char *line;
    char **args;
    int status = 1;

    do {
        char *prompt = malloc(sizeof(char)*PATH_MAX);
        char *path = get_path();
        if(path){
            snprintf(prompt, PATH_MAX, BLUE "%s\n" CYAN "❯ " COLOR_RESET, path); 
            free(path);
        }
        line = egg_readline(prompt);     
        
        if(WEIRD){
            /* >:-) */
            printf("\033[A");
            printf("\r\033[K");
            printf("\033[A");
            printf("\r\033[K");
            printf("\033[A");
            printf("\r\033[K");
            printf("%s❯%s %s\n", CYAN, COLOR_RESET, line);
            /* >:-) */
        }
        args = split(line);
        status = execute(args);
        free(line);
        free(prompt);
        free(args);
        printf("\n");
    } while(status);
}
