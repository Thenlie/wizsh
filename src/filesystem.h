#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

void print_current_dir_path(void);
void print_current_dir(void);
void list_current_dir(void);
void change_dir(char *input_buffer, int char_count);
int create_file(char *input_buffer, int char_count);

#endif