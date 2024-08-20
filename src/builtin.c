#include <stdlib.h>
#include <unistd.h>

#include "../include/shell.h"
#include "../include/utils.h"

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &egg_cd,
    &egg_help,
    &egg_exit
};

int size_of_builtin_func(){
    return sizeof(builtin_str)/sizeof(char *);
}

int egg_cd(char **args){
    if(args[1] == NULL){
        chdir(getenv("HOME"));
    }
    else if(chdir(args[1]) != 0){
        error("Incorrect path you dumb bitch.");
    }
    return 1;
}

int egg_help(char **args __attribute__((unused))){
    info("to remove from:"); 
    info("/usr/bin");
    info("/etc/shells");
    return 1;
}

int egg_exit(char **args __attribute__((unused))){
    info("And for my next trick, I'll disappear!\n");
    exit(1);
}
