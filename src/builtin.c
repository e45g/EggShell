#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "export",
};

int (*builtin_func[]) (char **) = {
    &egg_cd,
    &egg_help,
    &egg_exit,
    &egg_export,
};

int size_of_builtin_func(){
    return sizeof(builtin_str)/sizeof(char *);
}


int egg_cd(char **args){
    if(args[1] == NULL){
        chdir(getenv("HOME"));
    }
    else if(chdir(args[1]) != 0){
        error("No such file or directory: %s", args[1]);
    }
    return EXIT_SUCCESS;
}

int egg_help(char **args __attribute__((unused))){
    info("to remove from:");
    info("/usr/bin");
    info("/etc/shells");
    return EXIT_SUCCESS;
}

int egg_exit(char **args __attribute__((unused))){
    info("And for my next trick, I'll disappear!\n");
    exit(EXIT_SUCCESS);
}

int egg_export(char **args __attribute__((unused))){
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: export VAR=value\n");
        return EXIT_FAILURE;
    }

    char *equals_sign = strchr(args[1], '=');
    if (equals_sign){
        *equals_sign = '\0';
        char *name = args[1];
        char *value = equals_sign + 1;
        if (setenv(name, value, 1) == -1) {
            error("Error setenv");
        } else{
            info("Exported %s=%s", name, value);
        }
    } else {
        fprintf(stderr, "Usage: export VAR=value\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
