#ifndef FS_FILE_H_ 
#define FS_FILE_H_

// create a new file
int create_file(char **input, int word_count);
// delete a specified file
int remove_file(char **input, int word_count);
// write content to a specified file
int write_to_file(char **input, int word_count);
// read the contents of a specified file, print the output
int read_file(char **input, int word_count);
// move a file from one location to another
int move_file(char **input, int word_count);
// copy a file from one location to another
int copy_file(char **input, int word_count);
// open a file for editing with Vim
int open_file_with_vim(char **input, int word_count);

#endif