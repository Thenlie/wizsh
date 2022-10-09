#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "printutility.h"
#include "filesystem.h"
#include "requests.h"
#include "commands.h"

int get_input(char *buffer);
void parse_input(char *input, char** input_array, int char_count);

int main (void) {

    print_logo();
    /*
        * Main shell loop
    */
    while (1) {
        char *input_buffer = malloc(256);
        char *clean_input = malloc(256);
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
            parse_input(clean_input, input_array, char_count);

            // printf("Words: %i\n", word_count);
            // printf("Chars: %i\n", char_count);

            // for (int i = 0; i < word_count; i++) {
            //     printf("Parse: %s\n", input_array[i]);
            // }

            

            if (word_count > 0) {
                // lowercase the first cmd 
                for (int i = 0; input_array[0][i]; i++) {
                    input_array[0][i] = tolower(input_array[0][i]);
                }

                // COMMAND REFACTOR CHECK
                int c = command_handler(input_array, word_count, input_buffer);
                
                // -- EXIT --
                if (c == -1) {
                    printf("\ngoodbye <3\n\n");
                    free(input_buffer);
                    free(clean_input);
                    // free input array
                    for (int i = 0; i < word_count; i++) {
                        free(input_array[i]);
                    }
                    return 0;
                }
            } else {
                print_invalid_cmd(input_buffer);
            }
            // free input array
            for (int i = 0; i < word_count; i++) {
                free(input_array[i]);
            }
        }
        free(clean_input);
        free(input_buffer);
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

void parse_input(char *input, char** input_array, int char_count) {
    int str_count = 0;
    int arr_count = 0;
    int chr_count = 0;
    char* string_buffer = malloc(128);
    for (int i = 0; input[i]; i++) {
        chr_count++;
        if (chr_count > char_count) {
            break;
        }
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
        } else if (input[i] == '\n') {
            break;
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