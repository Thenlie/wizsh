#ifndef GIT_LOCAL_H_
#define GIT_LOCAL_H_

int clone_git_repo(char **input, int word_count);
int git_add_to_index(char **input, int word_count);
int create_git_commit(char **input, int word_count); 
int init_git_repo(char **input, int word_count);
int git_remove_from_index(char **input, int word_count);
 
#endif