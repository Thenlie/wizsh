#ifndef GIT_H_
#define GIT_H_

#include <stdbool.h>

int print_git_log(char **input, int word_count);
bool is_git_dir(char* dir_path);
void print_current_branch(void);
int clone_git_repo(char **input, int word_count);

#endif