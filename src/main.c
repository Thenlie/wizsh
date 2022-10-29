#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "handlers/git/git.commands.h"
#include "handlers/printutility.h"
#include "commands.h"
#include "input.h"
#include <git2.h>

int main (void) {
    git_libgit2_init();
    print_logo();
    // -- Main shell loop --
    while (1) {
        char *input_buffer = malloc(256);
        char *clean_input = malloc(256);
        print_start_of_line();

        int char_count = 0, word_count = 1;
        // accept user input
        if (get_input(input_buffer) == 1) {
            continue;
        } else {
            
            // printf("Initial: %s\n", input_buffer);
            // for (int i = 0; input_buffer[i]; i++) {
            //     printf("Initial: %d\n", input_buffer[i]);
            // }

            if (input_buffer[0]) {
                // remove extra whitespace from input
                trim_input(input_buffer, clean_input, &word_count, &char_count);
            } else {
                // nothing was entered
                free(clean_input);
                free(input_buffer);
                continue;
            }

            // convert input to array of words
            char* input_array[word_count];
            if (word_count > 0) {
                for (int i = 0; i < word_count; i++) {
                    input_array[i] = malloc(64);
                }
                parse_input(clean_input, input_array, char_count);
            }

            
            // -- TESTING, REMOVE BEFORE FINAL --
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

                // COMMAND CHECK
                int c;
                if (strcmp(input_array[0], "git") == 0) {
                    c = git_command_handler(input_array, word_count, input_buffer);
                } else {
                    c = command_handler(input_array, word_count, input_buffer);
                }
                
                // -- EXIT --
                if (c == -1) {
                    printf("\ngoodbye <3\n\n");
                    free(input_buffer);
                    free(clean_input);
                    for (int i = 0; i < word_count; i++) {
                        free(input_array[i]);
                    }
                    return 0;
                }
            }
            for (int i = 0; i < word_count; i++) {
                free(input_array[i]);
            }
        }
        free(clean_input);
        free(input_buffer);
    }
    git_libgit2_shutdown();
}                            
