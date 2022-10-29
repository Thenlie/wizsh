#ifndef COMMANDS_H_
#define COMMANDS_H_

// main command handler, determines which command function to run
// does not handle git commands
int command_handler(char **input, int word_count, char *input_buffer);

#endif