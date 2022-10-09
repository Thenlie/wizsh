#ifndef PRINTUTILITY_H_
#define PRINTUTILITY_H_

void print_logo(void);
void print_info(char **input, int word_count);
void print_invalid_cmd(char *cmd);
void print_invalid_use_cmd(char *cmd);
void clear_term(char** input, int word_count);
int print_commands(char** input, int word_count);
void print_wizard(void);

#endif