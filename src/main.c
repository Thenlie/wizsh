#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "printutility.h"
#include "filesystem.h"

int get_input(char *buffer);
void parse_input(char *input, char** input_array);

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
            /*
                * loop through input buffer if it has characters
                * ignore trailing and leading whitespace
                * ignore consecutive spaces
            */
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


            char* input_array[word_count];
            for (int i = 0; i < word_count; i++) {
                input_array[i] = malloc(64);
            }
            parse_input(clean_input, input_array);

            // printf("Words: %i\n", word_count);

            // for (int i = 0; i < word_count; i++) {
            //     printf("Parse: %s\n", input_array[i]);
            // }

            if (word_count > 0) {
                // lowercase the first cmd 
                for (int i = 0; input_array[0][i]; i++) {
                    input_array[0][i] = tolower(input_array[0][i]);
                }

                // exit
                if (strcmp(input_array[0], "exit") == 0 && word_count == 1) {
                    printf("\ngoodbye <3\n\n");
                    free(input_buffer);
                    free(clean_input);
                    // free input array
                    for (int i = 0; i < word_count; i++) {
                        free(input_array[i]);
                    }
                    return 0;
                } 
                // list project info
                else if (strcmp(input_array[0], "info") == 0 && word_count <= 2) {
                    print_info(input_array, word_count);
                } 
                // clear terminal
                else if (strcmp(input_array[0], "clear") == 0 && word_count <= 2) {
                    clear_term(input_array, word_count);
                } 
                // print current directory
                else if (strcmp(input_array[0], "dir") == 0 && word_count <= 2) {
                    print_current_dir_path(input_array, word_count);
                } 
                // list files in current directory
                else if (strcmp(input_array[0], "ls") == 0 && word_count <= 2) {
                    list_current_dir(input_array, word_count);
                } 
                // change directories
                else if (strcmp(input_array[0], "cd") == 0 && word_count <= 2) {
                    change_dir(input_array, word_count);
                } 
                // create file
                else if (strcmp(input_array[0], "mkdir") == 0 && word_count <= 2) {
                    create_dir(input_array, word_count);
                } 
                // create folder
                else if (strcmp(input_array[0], "mk") == 0 && word_count <= 2) {
                    create_file(input_array, word_count);
                } 
                // remove file
                else if (strcmp(input_array[0], "rmf")== 0 && word_count <= 2) {
                    remove_file(input_array, word_count);
                } 
                // remove folder
                else if (strcmp(input_array[0], "rmdir") == 0 && word_count <= 2) {
                    remove_dir(input_array, word_count);
                } 
                // write to file
                else if (strcmp(input_array[0], "write") == 0) {
                    write_to_file(input_array, word_count);
                } 
                // read file
                else if (strcmp(input_array[0], "read") == 0 && word_count <= 2) {
                    read_file(input_array, word_count);
                }
                else {
                    print_invalid_cmd(input_buffer);
                }

            } else {
                print_invalid_cmd(input_buffer);
            }
            // free input array
            for (int i = 0; i < word_count; i++) {
                free(input_array[i]);
            }
        }

        // clear out clean input in prep for next input
        for (int i = 0; input_buffer[i]; i++) {
            clean_input[i] = 0;
        }
    }
}                            
            
int get_input(char *buffer) {
    if (fgets(buffer, 256, stdin) != NULL)
    {
        return 0;
    } else {
        perror("Error while reading input! 256 char limit.");
        return 1;
    }
}

void parse_input(char *input, char** input_array) {
    int str_count = 0;
    int arr_count = 0;
    char* string_buffer = malloc(128);
    for (int i = 0; input[i]; i++) {
        if (input[i] == ' ') {
            // finish string, push to array
            string_buffer[str_count] = '\0';
            strcpy(input_array[arr_count], string_buffer);
            // reset string buffer
            for (int j = 0; j < str_count+1; j++) {
                string_buffer[j] = 0;
            }
            str_count = 0;
            arr_count++;
        } else {
            // push to string buffer, increase count
            string_buffer[str_count] = input[i];
            str_count++;
        }
    }
    // finish string, push to array
    string_buffer[str_count] = '\0';
    strcpy(input_array[arr_count], string_buffer);
    free(string_buffer);
}