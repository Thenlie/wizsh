#ifndef PRINTUTILITY_H_
#define PRINTUTILITY_H_

void print_logo(void);
int print_info(char **input, int word_count);
void print_invalid_cmd(char *cmd);
void print_invalid_use_cmd(char *cmd);
int clear_term(char** input, int word_count);
int print_commands(char** input, int word_count);
int print_wizard(char **input, int word_count);

#endif