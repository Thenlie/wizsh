#include "main.h"

int get_input(char *buffer);

int main (void) {

    print_logo();
    char *input_buffer = malloc(sizeof(char) * 256);

    /*
        * Main shell loop
    */
    while (1) {
        // print current directory only
        char cwd[MAX_INPUT];
        char cur_dir[MAX_INPUT];
        int count = 0;
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            for (int i = 0; cwd[i]; i++) {
                // printf("%c", cwd[i]);
                if (cwd[i] == 47) {
                    count = 0;
                } else {
                    if (count == 0) {
                        for (int j = 0; j < i; j++) {
                            cur_dir[j] = 0;
                        }
                    }
                    cur_dir[count] = cwd[i];
                    count++;
                }
            }
            printf("\033[1;105m");
            printf(" %s ~> ", cur_dir);
            printf("\033[0m ");
        } else {
            perror("Error while getting dir! 256 char limit.");
        }

        // accept user input
        int char_count = 0;
        int word_count = 1;
        if (get_input(input_buffer) == 1) {
            continue;
        } else {
            // loop through input buffer if it has characters
            if (input_buffer[0] != '\n') {
                for (int i = 0; input_buffer[i]; i++) {
                    // printf("%i, ", input_buffer[i]);
                    char_count++;
                    if (input_buffer[i] == ' ') {
                        word_count++;
                    }
                }
            } else {
            // nothing was entered
                word_count = 0;
                continue;
            }

            // single word commands
            if (word_count == 1) {
                // lowercase the entire input
                for (int i = 0; input_buffer[i]; i++) {
                    input_buffer[i] = tolower(input_buffer[i]);
                }
                // exit
                if (strcmp(input_buffer, "exit\n") == 0) {
                    printf("\ngoodbye <3\n\n");

                    free(input_buffer);
                    return 0;
                // list project info
                } else if (strcmp(input_buffer, "info\n") == 0) {
                    printf("\033[1;34m");
                    printf("                         --  Info --\n");
                    printf("\033[1;33m"); 
                    printf("            wizsh! Created by Leithen Crider in 2022\n");
                    printf("\033[0m");
                    printf("Written in C, this was a final project for the Harvard CS50 course.\n");
                    printf("  This project was built to gain a better understanding of the C\n");
                    printf("programming language and how shells like bash and zsh might work.\n");
                // print current directory
                } else if (strcmp(input_buffer, "dir\n") == 0) {
                    char cwd[MAX_INPUT];
                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                        printf("\033[1;34m");
                        printf("Current dir: ");
                        printf("\033[0m");
                        printf("%s\n", cwd);
                    } else {
                        perror("Error while getting dir! 256 char limit.");
                    }
                } else {
                    printf("%s", input_buffer);
                }
            // multi-word commands
            } else if (word_count == 2) {
                // change directories
                if (tolower(input_buffer[0]) == 'c' && tolower(input_buffer[1]) == 'd') {
                    char *path;
                    path = malloc(256);
                    int path_count = 0;
                    for (int i = 3; i < char_count - 1; i++) {
                        path[path_count] = input_buffer[i];
                        path_count++;
                    }
                    // path[path_count] = '\0';
                    if (chdir(path) == -1) {
                        perror("Error while changing directories!");
                    } else {
                        char cwd[MAX_INPUT];
                        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                            printf("%s\n", cwd);
                        } else {
                            perror("Error while getting dir! 256 char limit.");
                        }
                    }
                    free(path);
                }
            }
            // printf("%i chars\n", char_count);
            // printf("%i words\n", word_count);
            // printf("%s", input_buffer);
        }
    }
}                            
            
int get_input(char *buffer) {
    if (fgets(buffer, MAX_INPUT, stdin) != NULL)
    {
        return 0;
    } else {
        perror("Error while reading input! 256 char limit.");
        return 1;
    }
}