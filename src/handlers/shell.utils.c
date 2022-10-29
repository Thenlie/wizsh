#include "shell.utils.h"
#include "printutility.h"
#include <stdio.h>

int clear_term(char** input, int word_count) {
    if (word_count == 1) {
        printf("\e[1;1H\e[2J");
    } else {
        print_invalid_use_cmd("clear");
    }
    return 0;
}

int dummy_exit(char **input, int word_count) {
    return -1;
}