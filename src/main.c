#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "printutility.h"
#include "filesystem.h"

#define MAX_INPUT 256


int get_input(char *buffer);

int main (void) {

    print_logo();
    char *input_buffer = malloc(sizeof(char) * 256);
    char *clean_input = malloc(sizeof(char) * 256);
    /*
        * Main shell loop
    */
    while (1) {
        // print start of line marking
        printf("\033[1;105m");
        print_current_dir();
        printf("~> ");
        printf("\033[0m ");

        // accept user input
        int char_count = 0;
        int word_count = 1;
        char prev_char;
        int clean_count = 0;
        if (get_input(input_buffer) == 1) {
            continue;
        } else {
            // loop through input buffer if it has characters
            if (input_buffer[0] != '\n') {
                for (int i = 0; input_buffer[i]; i++) {
                    if (i == 0 && input_buffer[i] == ' ') {
                        prev_char = ' ';
                        continue;
                    }
                    if (input_buffer[i] == ' ' && prev_char != ' ') {
                        word_count++;
                    }
                    if (!(input_buffer[i] == ' ' && prev_char == ' ') && input_buffer[i] != '\n') {
                        char_count++;
                        clean_input[clean_count] = input_buffer[i];
                        clean_count++;
                    }
                    if (input_buffer[i] != '\n') {
                        prev_char = input_buffer[i];
                    }
                }
                if (prev_char == ' ') {
                    word_count--;
                    clean_input[clean_count-1] = 0;
                }
            } else {
            // nothing was entered
                word_count = 0;
                continue;
            }

            // single word commands
            if (word_count == 1) {
                // lowercase the entire input
                for (int i = 0; clean_input[i]; i++) {
                    clean_input[i] = tolower(clean_input[i]);
                }
                // exit
                if (strcmp(clean_input, "exit") == 0) {
                    printf("\ngoodbye <3\n\n");
                    free(input_buffer);
                    free(clean_input);
                    return 0;
                // list project info
                } else if (strcmp(clean_input, "info") == 0) {
                    print_info();
                // print current directory
                } else if (strcmp(clean_input, "dir") == 0) {
                    print_current_dir_path();
                // list files in current directory
                } else if (strcmp(clean_input, "ls") == 0) {
                    list_current_dir();
                } else {
                    print_invalid_cmd(input_buffer);
                }
            // multi-word commands
            } else if (word_count == 2) {
                // change directories
                if (tolower(clean_input[0]) == 'c' && tolower(clean_input[1]) == 'd') {
                    change_dir(clean_input, char_count);
                } else if (tolower(clean_input[0]) == 'm' && tolower(clean_input[1]) == 'k' &&  tolower(clean_input[2]) == 'd' && tolower(clean_input[3]) == 'i' && tolower(clean_input[4]) == 'r') {
                    create_dir(clean_input, char_count);
                } else if (tolower(clean_input[0]) == 'm' && tolower(clean_input[1]) == 'k') {
                    create_file(clean_input, char_count);
                } else {
                    print_invalid_cmd(input_buffer);
                }
            } else {
                print_invalid_cmd(input_buffer);
            }
        }

        // clear out clean input in prep for next input
        for (int i = 0; input_buffer[i]; i++) {
            clean_input[i] = 0;
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