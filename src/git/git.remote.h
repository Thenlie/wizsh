#ifndef GIT_REMOTE_H_
#define GIT_REMOTE_H_

int git_remote_command_handler(char**input, int word_count);
int print_git_remotes(char **input, int word_count); 
int git_remote_add(char **input, int word_count);
int git_remote_remove(char **input, int word_count); 
int rename_git_remote(char **input, int word_count);

#endif