#ifndef INPUT_H_
#define INPUT_H_

// get input from the user, read as a single string
int get_input(char *buffer);
// split user input by spaces into array of words
void parse_input(char *input, char** input_array, int char_count);
// remove unecessary whitespace from user input
void trim_input(char *input_buffer, char *clean_input, int *word_count, int *char_count);

#endif