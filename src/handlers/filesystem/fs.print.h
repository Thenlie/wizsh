#ifndef FS_PRINT_H_ 
#define FS_PRINT_H_

// print the full path of the current working directory
int print_current_dir_path(char **input, int word_count);
// print the name of the current directory
void print_current_dir();
// list all the files and folders in the current directory
int list_current_dir(char **input, int word_count);
// print a tree of all files, folders, and their children of the current directory
int print_dir_tree(char **input, int word_count);

#endif