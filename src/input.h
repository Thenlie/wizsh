#ifndef INPUT_H_
#define INPUT_H_

int get_input(char *buffer);
void parse_input(char *input, char** input_array, int char_count);
void trim_input(char *input_buffer, char *clean_input, int *word_count, int *char_count);

#endif