#ifndef GIT_LOCAL_H_
#define GIT_LOCAL_H_

int clone_git_repo(char **input, int word_count);
int init_git_repo(char **input, int word_count);
int git_local_command_handler(char **intput, int word_count);

#endif