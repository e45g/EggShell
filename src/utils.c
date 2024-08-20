#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/utils.h"



char *get_path(){
    char *path = malloc(sizeof(char) * PATH_MAX);
    char *home = getenv("HOME");
    getcwd(path, PATH_MAX);

    if (!path){
        free(path);
        return NULL;
    }

    if(home && strncmp(path, home, strlen(home))==0){
        char *new_path = malloc(sizeof(char) * PATH_MAX);
        snprintf(new_path, PATH_MAX, "~%s", path+strlen(home)); 
        free(path);
        return new_path;
    }
    return NULL;
}
