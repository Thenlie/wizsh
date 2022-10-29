#ifndef GIT_LOCAL_H_
#define GIT_LOCAL_H_

// clone a git repository
int clone_git_repo(char **input, int word_count);
// initialize an empty git repository
int init_git_repo(char **input, int word_count);
// command handler for all git local commands other than those above
int git_local_command_handler(char **intput, int word_count);

#endif