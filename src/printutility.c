#include "printutility.h"
#include <stdio.h>
#include <string.h>

void print_logo(void)
{
    /*
        * Print opening screen and ASCII art
        * [0;33m] = yellow
        * [0;35m] = purple
        * [0;34m] = blue
    */
    printf("\033[0;34m");
    printf("                        Welcome to WIZSH!\n");
    printf("                   A simple shell written in C\n");
    printf("                     Created by Thenlie 2022\n");
    printf("\n");
    printf("\033[0;35m");
    printf("                     88                      88           \n");
    printf("                     \"\"                      88           \n");
    printf("                                             88           \n");
    printf("  8b      db      d8 88 888888888  ,adPPYba, 88,dPPYba,   \n");
    printf("  `8b    d88b    d8' 88      a8P\" |8[    \"\"  88P'    \"8a  \n");
    printf("   `8b  d8'`8b  d8'  88   ,d8P'     `\"Y8ba,  88       88  \n");
    printf("    `8bd8'  `8bd8'   88 ,d8\"       aa    ]8I 88       88  \n");
    printf("      YP      YP     88 888888888  `\"YbbdP\"' 88       88  \n");
    printf("\n");
    printf("\033[0m"); 
    // https://ascii.co.uk/
}

void print_info(char** input, int word_count) {
    if (word_count == 1) {
        printf("\033[1;34m");
        printf("                         --  Info --\n");
        printf("\033[1;33m"); 
        printf("            wizsh! Created by Leithen Crider in 2022\n");
        printf("\033[0m");
        printf("Written in C, this was a final project for the Harvard CS50 course.\n");
        printf("  This project was built to gain a better understanding of the C\n");
        printf("programming language and how shells like bash and zsh might work.\n");
    } else if (word_count == 2 && (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0)) {
        // printf("\033[1;34m");
        printf("\033[1;34m                                    -- %s --\n\n", input[0]); // <- first command
        printf("\033[0m The info command is used to display info about this shell and its contributors.\n");
    } else {
        print_invalid_use_cmd("info");
    }
}

void print_invalid_cmd(char *cmd) {
    printf("\033[0;31m");
    printf("Invalid command! ");
    printf("\033[4;37m");
    printf("%s", cmd);
    printf("\033[0m");
}

void print_invalid_use_cmd(char *cmd) {
    printf("\033[0;31m");
    printf("Invalid use of command! ");
    printf("\033[4;37m");
    printf("%s\n", cmd);
    printf("\033[0m");
    printf("Use %s --help for help using this command.\n", cmd);
}