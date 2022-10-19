#ifndef PRINTUTILITY_H_
#define PRINTUTILITY_H_

void print_logo(void);
int print_info(char **input, int word_count);
void print_start_of_line(void);
void print_invalid_cmd(char *cmd);
void print_invalid_use_cmd(char *cmd);
int print_commands(char** input, int word_count);
void print_command_help(char *name, char *description, char **usage_list);
int print_wizard(char **input, int word_count);

#endif