#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

void print_current_dir_path(void);
void print_current_dir(void);
void list_current_dir(void);
void change_dir(char **input);
int create_file(char **input);
int create_dir(char **input);
int remove_file(char **input);
int remove_dir(char **input);

#endif