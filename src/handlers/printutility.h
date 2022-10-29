#ifndef PRINTUTILITY_H_
#define PRINTUTILITY_H_

// print ASCII 'wizsh'. used on program start
void print_logo(void);
// print synopsis of the wizsh project
int print_info(char **input, int word_count);
// print information at the start of each line in the shell
void print_start_of_line(void);
// Let the user know the input was invalid. No help suggestions are offered
void print_invalid_cmd(char *cmd);
// Let the user know the input was a valid command, but used improperly. Help suggestions are offered
void print_invalid_use_cmd(char *cmd);
// Print a full list of commands
int print_commands(char **input, int word_count);
// Handler for commands called it '-h, --help'
void print_command_help(char *name, char *description, char **usage_list);
// ASCII art
int print_wizard(char **input, int word_count);

#endif