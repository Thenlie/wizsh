#ifndef GIT_PRINT_H_
#define GIT_PRINT_H_

// determines which git print command to run
int git_print_command_handler(char **input, int word_count);
// print the name of the currently checked out git branch
void print_current_branch(void);

#endif