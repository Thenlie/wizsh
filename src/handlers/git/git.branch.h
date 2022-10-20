#ifndef GIT_BRANCH_H_
#define GIT_BRANCH_H_

int git_branch_command_handler(char** input, int word_count); 
int print_git_branches(char **input, int word_count);
int checkout_git_branch(char **input, int word_count);
int create_git_branch(char **input, int word_count);
int delete_git_branch(char **input, int word_count);

#endif