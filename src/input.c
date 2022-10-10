#include "input.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int str_count = 0, arr_count = 0, chr_count = 0;
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

void trim_input(char *input_buffer, char *clean_input, int *word_count, int *char_count) {
    // ignore trailing and leading whitespace, ignore consecutive spaces
    int clean_count = 0;
    char prev_char;
    for (int i = 0; input_buffer[i]; i++) {
        if (i == 0 && input_buffer[i] == ' ') {
            prev_char = ' ';
            continue;
        }
        if (input_buffer[i] == ' ' && prev_char != ' ') {
            (*word_count)++;
        }
        if (!(input_buffer[i] == ' ' && prev_char == ' ') && input_buffer[i] != '\n') {
            (*char_count)++;
            clean_input[clean_count] = input_buffer[i];
            clean_count++;
        }
        if (input_buffer[i] != '\n') {
            prev_char = input_buffer[i];
        }
    }
    if (prev_char == 32) {
        (*word_count)--;
        clean_input[clean_count-1] = 0;
    }
}   