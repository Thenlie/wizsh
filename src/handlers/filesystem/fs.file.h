#ifndef FS_FILE_H_ 
#define FS_FILE_H_

int create_file(char **input, int word_count);
int remove_file(char **input, int word_count);
int write_to_file(char **input, int word_count);
int read_file(char **input, int word_count);
int move_file(char **input, int word_count);
int copy_file(char **input, int word_count);
int open_file_with_vim(char **input, int word_count);

#endif