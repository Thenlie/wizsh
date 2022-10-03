// #include "../main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "printutility.h"

#define MAX_INPUT 256

#include "filesystem.h"

int get_input(char *buffer);

int main (void) {

    print_logo();
    char *input_buffer = malloc(sizeof(char) * 256);

    /*
        * Main shell loop
    */
    while (1) {
        // print current directory only
        char cwd[MAX_INPUT];
        char cur_dir[MAX_INPUT];
        int count = 0;
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            for (int i = 0; cwd[i]; i++) {
                // printf("%c", cwd[i]);
                if (cwd[i] == 47) {
                    count = 0;
                } else {
                    if (count == 0) {
                        for (int j = 0; j < i; j++) {
                            cur_dir[j] = 0;
                        }
                    }
                    cur_dir[count] = cwd[i];
                    count++;
                }
            }
            printf("\033[1;105m");
            printf(" %s ~> ", cur_dir);
            printf("\033[0m ");
        } else {
            perror("Error while getting dir! 256 char limit.");
        }

        // accept user input
        int char_count = 0;
        int word_count = 1;
        if (get_input(input_buffer) == 1) {
            continue;
        } else {
            // loop through input buffer if it has characters
            if (input_buffer[0] != '\n') {
                for (int i = 0; input_buffer[i]; i++) {
                    // printf("%i, ", input_buffer[i]);
                    char_count++;
                    if (input_buffer[i] == ' ') {
                        word_count++;
                    }
                }
            } else {
            // nothing was entered
                word_count = 0;
                continue;
            }

            // single word commands
            if (word_count == 1) {
                // lowercase the entire input
                for (int i = 0; input_buffer[i]; i++) {
                    input_buffer[i] = tolower(input_buffer[i]);
                }
                // exit
                if (strcmp(input_buffer, "exit\n") == 0) {
                    printf("\ngoodbye <3\n\n");

                    free(input_buffer);
                    return 0;
                // list project info
                } else if (strcmp(input_buffer, "info\n") == 0) {
                    print_info();
                // print current directory
                } else if (strcmp(input_buffer, "dir\n") == 0) {
                    print_current_dir_long();
                // list files in current directory
                } else if (strcmp(input_buffer, "ls\n") == 0) {
                    list_current_dir();
                } else {
                    printf("%s", input_buffer);
                }
            // multi-word commands
            } else if (word_count == 2) {
                // change directories
                if (tolower(input_buffer[0]) == 'c' && tolower(input_buffer[1]) == 'd') {
                    char *path;
                    path = malloc(256);
                    int path_count = 0;
                    for (int i = 3; i < char_count - 1; i++) {
                        path[path_count] = input_buffer[i];
                        path_count++;
                    }
                    // path[path_count] = '\0';
                    if (chdir(path) == -1) {
                        perror("Error while changing directories!");
                    } else {
                        char cwd[MAX_INPUT];
                        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                            printf("%s\n", cwd);
                        } else {
                            perror("Error while getting dir! 256 char limit.");
                        }
                    }
                    free(path);
                }
            }
            // printf("%i chars\n", char_count);
            // printf("%i words\n", word_count);
            // printf("%s", input_buffer);
        }
    }
}                            
            
int get_input(char *buffer) {
    if (fgets(buffer, MAX_INPUT, stdin) != NULL)
    {
        return 0;
    } else {
        perror("Error while reading input! 256 char limit.");
        return 1;
    }
}