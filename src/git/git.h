#ifndef GIT_H_
#define GIT_H_

#include <stdbool.h>

int print_git_log(char **input, int word_count);
bool is_git_dir(char* dir_path);
void print_current_branch(void);
int clone_git_repo(char **input, int word_count);
int print_git_status(char **input, int word_count);
int git_branch_command_handler(char** input, int word_count); 
int print_git_branches(char **input, int word_count);
int checkout_git_branch(char **input, int word_count);
int create_git_branch(char **input, int word_count);
int delete_git_branch(char **input, int word_count);
int git_add(char **input, int word_count);
int create_git_commit(char **input, int word_count); 
int init_git_repo(char **input, int word_count);
int git_remove_from_index(char **input, int word_count);

#endif