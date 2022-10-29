#ifndef FS_DIRECTORY_H_
#define FS_DIRECTORY_H_

// change the current working directory
int change_dir(char **input, int word_count);
// create a new, empty directory
int create_dir(char **input, int word_count);
// remove an empty directory
int remove_dir(char **input, int word_count);

#endif