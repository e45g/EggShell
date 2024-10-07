#include <linux/limits.h>
#include <stdio.h>
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
    char path[PATH_MAX];
    if(strncmp(args[1], "~", 1) == 0){
        snprintf(path, PATH_MAX, "%s%s", getenv("HOME"), args[1]+1);
    } else{
        strcpy(path, args[1]);
    }
    if(chdir(path) == -1){
        error("cd: no such file or directory %s", path);
        return EXIT_FAILURE;
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
        error("Usage: export VAR=value");
        return EXIT_FAILURE;
    }

    char *equals_sign = strchr(args[1], '=');
    if (equals_sign){
        *equals_sign = '\0';
        char *name = args[1];
        char *value = equals_sign + 1;
        if (setenv(name, value, 1) == -1) {
            error("Error setenv");
            return EXIT_FAILURE;
        } else{
            info("Exported %s=%s", name, value);
        }
    } else {
        error("Usage: export VAR=value");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
