#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define WEIRD 1

#define RED "\033[31m"
#define COLOR_RESET "\x1b[0m"
#define CYAN "\x1B[36m"
#define BLUE "\x1B[34m"

#define error(...) fprintf(stderr, RED "[%s] ", SHELL_NAME); fprintf(stderr, __VA_ARGS__); fprintf(stderr, COLOR_RESET "\n")
#define info(...) printf(CYAN "[%s] ", SHELL_NAME); printf(COLOR_RESET __VA_ARGS__); printf("\n")

char *get_path();

#endif
