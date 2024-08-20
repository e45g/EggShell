#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RED "\033[31m"
#define WHITE "\x1B[37m"
#define CYAN "\x1B[36m"

#define MAX_LINE_BUFSIZE 1024
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"
#define SHELL_NAME "EggShell"

#define error(...) fprintf(stderr, RED "[%s] ", SHELL_NAME); fprintf(stderr, __VA_ARGS__); fprintf(stderr, WHITE "\n")
#define info(...) printf(CYAN "[%s] ", SHELL_NAME); printf(WHITE __VA_ARGS__); printf("\n")

int egg_cd(char **args);
int egg_help(char **args);
int egg_exit(char **args);

char *builtin_str[] = {"cd", "help", "exit"};

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
        chdir("$HOME_DIR");
    }
    else if(chdir(args[1]) != 0){
        error("Ehhhh?");
    }
    return 1;
}

int egg_help(char **args){
    info("to remove from:"); 
    info("/usr/bin");
    info("/etc/shells");
    return 1;
}

int egg_exit(char **args){
    info("And for my next trick, I'll disappear!\n");
    exit(1);
}

char *readline(){
    int buffer_size = MAX_LINE_BUFSIZE;
    char *buffer = malloc(sizeof(char) * buffer_size);
    int position = 0;
    int c;

    if(!buffer){
        error("Memory allocation erorr.");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();
        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        }else{
            buffer[position] = c;
        }
        position++;

        if(position >= buffer_size){
            buffer_size += MAX_LINE_BUFSIZE;
            buffer = realloc(buffer, buffer_size);
            if(!buffer){
                error("Memory reallocation error.");
                exit(EXIT_FAILURE);
            }
        }
    }
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
                /*error("This is your captain speaking: We're experiencing some turbulence. Oh wait, that's just the program crashing. Bye!");*/
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int launch(char **args){
    pid_t cpid, wpid;
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
            wpid = waitpid(cpid, &status, WUNTRACED);
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

void loop(){
    char *line;
    char **args;
    int status = 1;

    do {
        printf(CYAN SHELL_NAME " > " WHITE);
        line = readline();     
        args = split(line);
        status = execute(args);
        free(line);
        free(args);
        printf("\n");
    } while(status);
}

int main(){
    loop();
    return EXIT_SUCCESS;
}
