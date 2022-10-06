#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

void print_current_dir_path(int word_count);
void print_current_dir();
void list_current_dir(int word_count);
void change_dir(char **input, int word_count);
int create_file(char **input, int word_count);
int create_dir(char **input, int word_count);
int remove_file(char **input, int word_count);
int remove_dir(char **input, int word_count);

#endif