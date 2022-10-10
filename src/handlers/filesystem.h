#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

int print_current_dir_path(char **input, int word_count);
void print_current_dir();
int list_current_dir(char **input, int word_count);
int change_dir(char **input, int word_count);
int create_file(char **input, int word_count);
int create_dir(char **input, int word_count);
int remove_file(char **input, int word_count);
int remove_dir(char **input, int word_count);
int write_to_file(char **input, int word_count);
int read_file(char **input, int word_count);
int move_file(char **input, int word_count);
int open_file_with_vim(char **input, int word_count);
int dummy_exit(char **input, int word_count);
int copy_file(char **input, int word_count);
int print_dir_tree(char **input, int word_count);

#endif