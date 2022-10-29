#ifndef GIT_COMMANDS_H_
#define GIT_COMMANDS_H_

// main git command handler, handles all commands or passes to sub-handlers
int git_command_handler(char** input, int word_count, char* input_buffer);

#endif