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
        printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
        printf("\033[0mThe \033[1;33minfo\033[0m command is used to display info about this shell and its contributors.\n\n");
        printf("\033[1;35m                                   Usage\n\n"); 
        printf("\033[0m ~> \033[1;33minfo        \033[0m| The main usage of the command\n");
        printf("\033[0m ~> \033[1;33minfo -h     \033[0m| Help with the command\n");
        printf("\033[0m ~> \033[1;33minfo --help \033[0m| Help with the command\n\n");
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
    printf("Use %s --help or -h for help using this command.\n", cmd);
}

void clear_term(char** input, int word_count) {
    if (word_count == 1) {
        printf("\e[1;1H\e[2J");
    } else if (word_count == 2 && (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0)) {
        printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
        printf("\033[0mThe \033[1;33mclear\033[0m command is used to clear the terminal of all input.\n\n");
        printf("\033[1;35m                                   Usage\n\n"); 
        printf("\033[0m ~> \033[1;33mclear        \033[0m| The main usage of the command\n");
        printf("\033[0m ~> \033[1;33mclear -h     \033[0m| Help with the command\n");
        printf("\033[0m ~> \033[1;33mclear --help \033[0m| Help with the command\n\n");
    } else {
        print_invalid_use_cmd("clear");
    }
}

void print_commands(void) {
    printf("\033[1;34m                - Commands --\n\n");
    printf("\033[0m ~> \033[1;33mhelp  \033[0m| The main usage of the command\n");
    printf("\033[0m ~> \033[1;33minfo  \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mclear \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mdir   \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mls    \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mcd    \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mmkdir \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mmk    \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mread  \033[0m| Help with the command\n");
    printf("\033[0m ~> \033[1;33mwrite \033[0m| Help with the command\n\n");
}

void print_wizard(void) {
    printf("\n");
    printf("\n");
    printf("                                            \033[48:5:166m&&&&BBBBBBB#&\033[m\n");
    printf("                                        \033[48:5:166m#GP5Y\033[38:2:160:10:10m5BGB5\033[38:2:200:200:200mJJJJJJJYP&\033[m\n");
    printf("                                      \033[48:5:166mBYJYPPP\033[38:2:160:10:10mGGPGGGG\033[38:2:200:200:200mPPYJJJJ5&\033[m\n");
    printf("                                     \033[48:5:166mBJJJ\033[38:2:160:10:10mYBBGGGGPPGGGG5\033[38:2:200:200:200mJJJJJP\033[m\n");
    printf("                                    \033[48:5:166mBJJJJJ\033[38:2:160:10:10mGGPPPPGBP5Y\033[38:2:200:200:200mJJJJJJJ5\033[m\n");
    printf("                                   \033[48:5:166m\033[38:2:0:0:0m&5\033[38:2:200:200:200m\033[48:5:166m?JJJ\033[38:2:160:10:10mPBGGGGPBP\033[38:2:200:200:200mJJJJJJJJJJP\033[m\n");
    printf("                                  \033[48:5:166m\033[38:2:0:0:0m&&B5\033[38:2:200:200:200m\033[48:5:166mYJJYYYY\033[38:2:160:10:10m5GGB\033[38:2:200:200:200mJJJJJJJJJJY#\033[m\n");
    printf("                                 \033[48:5:166m\033[38:2:0:0:0m&&&&&#5\033[38:2:200:200:200m\033[48:5:166mJJJJJJJJJJJJJJJJJJY#\033[m\n");
    printf("                                \033[48:5:166m\033[38:2:0:0:0m&&&&GY\033[38:2:200:200:200m\033[48:5:166mJJJJJJJJYYJJ?JJJJJJ5#\033[m\n");
    printf("                              \033[48:5:166m\033[38:2:0:0:0m&P5P#&Y\033[38:2:200:200:200m\033[48:5:166m?JJJJJJJJY5PP5YJJJYG&\033[m\n");
    printf("                            \033[48:5:166m&GJ???J\033[38:2:0:0:0m5J\033[38:2:200:200:200m?JJJJJJJ?J??J5&&#&\033[m\n");
    printf("                           \033[48:5:166mGY?????????????????J??\033[38:2:160:10:10mJ5#\033[m\n");
    printf("                         \033[48:5:166m#Y?????????????????????\033[38:2:160:10:10mJGBG#\033[m\n");
    printf("                        \033[48:5:166m#J?????JPJ????????\033[38:2:160:10:10m?PGPPPGGPPGB&\033[m\n");
    printf("                        \033[48:5:166m#JJJJJ5\033[38:2:0:0:0m#&\033[38:2:200:200:200mBY????????\033[38:2:160:10:10mJ5PPPPPPPPPPB&\033[m\n");
    printf("                       \033[48:5:166m&PG\033[38:2:0:0:0m####&&&&&#\033[38:2:200:200:200m\033[48:2:180:5:5mB5\033[48:5:166m???????\033[38:2:160:10:10mYGPPPPPPPPPBG\033[m\n");
    printf("                      \033[48:5:166m P?J5\033[38:2:0:0:0m#&&&&&&&\033[38:2:200:200:200m\033[48:2:180:5:5mBBGJ\033[48:5:166m??????\033[38:2:160:10:10mYGPPPPGGGPPGY5\033[m\n");
    printf("                      \033[48:5:166mB????5\033[38:2:0:0:0m&&&&&#\033[38:2:200:200:200m\033[48:2:180:5:5m5YYPB\033[48:5:166m??????\033[38:2:160:10:10m5GPGGPP\033[38:2:200:200:200mYY5\033[38:2:160:10:10mPGG?B\033[m                      \033[48:2:180:5:5m&#B#\033[m\n");
    printf("                     \033[48:5:166mGJ????\033[38:2:0:0:0mPBP\033[38:2:200:200:200mY\033[38:2:0:0:0mP#\033[38:2:200:200:200m\033[48:2:180:5:5m5Y5Y5#\033[48:5:166mJJJYYY\033[38:2:160:10:10m5BG5\033[38:2:200:200:200mYYYYYJJ5P?P\033[m                     \033[48:2:180:5:5m&PPPPB\033[m\n");
    printf("                \033[48:5:166m&&&#PYYYYJJJJYP\033[48:2:180:5:5mP5Y55YPBJ\033[48:5:166m?????????????JJYYJYG\033[m                     \033[48:2:180:5:5m&PPPPPB&\033[m\n");
    printf("             \033[48:5:166m&5JJJJ???JJJYPPP5\033[48:2:180:5:5m5YYYYYYGG7\033[48:5:166m????????????????????JP#\033[m                   \033[48:2:180:5:5mGPPPPPPB&\033[m\n");
    printf("             \033[48:5:166mB??????????5G5\033[48:2:180:5:5mYYYYYYYYYY5BP\033[48:5:166mYY5555PPPPPP5YJ????????5#\033[m                 \033[48:2:180:5:5mGPPPPPPPG&\033[m\n");
    printf("             \033[48:5:166m#J?????JJY\033[48:2:180:5:5mGGYYYYYYYYYYYYYG#\033[m\033[38:2:160:10:10mGPP5555555PPGBB#BPJ\033[m\033[48:5:166m??????B\033[m               \033[48:2:180:5:5mBPPPPPPPPP#\033[m\n");
    printf("              \033[48:5:166mBYJ5G##B\033[48:2:180:5:5m#GYYYYYYYYYYYYYYB\033[m\033[38:2:160:10:10mPYYYYYYYYYYYYYYY5P#&5\033[m\033[48:5:166m????7P\033[m             \033[48:2:180:5:5m&G5PPPPPPPPP#\033[m\n");
    printf("                \033[48:5:166m&   \033[48:2:180:5:5m&BBYYYYYYYYYYYYYYPG\033[m\033[38:2:160:10:10mYYYYYYYYYYYYYYYYYYYG#JJ\033[m\033[48:5:166m??5&\033[m           \033[48:2:180:5:5m&BP5PPPPPPPPPG\033[m\n");
    printf("                     \033[48:2:180:5:5m&PYYYYYYYYYYYY5P\033[m\033[38:2:160:10:10mBYYYYYYYYYYYYY55YYYYYYPPJ\033[m\033[48:5:166mYB\033[m           \033[48:2:180:5:5m#G555555PPPPPPG&\033[m\n");
    printf("                     \033[48:2:180:5:5mBYYYYYYYYYYYY5PP\033[m\033[38:2:160:10:10m#PYYYY55YYYYYY5BYYYYYYY5G#\033[m          \033[48:2:180:5:5m&P555555555PPPPB\033[m\n");
    printf("                     \033[48:2:180:5:5mBYYYYYYYYYYYY55Y5BG\033[m\033[38:2:160:10:10m5PPBGYYYYYYP&BP55YYYYY\033[m\033[48:2:180:5:5m55GBB##&&&B5555555555555P#\033[m\n");
    printf("                     \033[48:2:180:5:5m&PYYYYYYYYYYYYYYYYG&\033[m   \033[m\033[38:2:160:10:10m#5YYY5B&\033[m   \033[48:2:180:5:5m&&#BGP55YYYY5555555555555555555#\033[m\n");
    printf("                      \033[48:2:180:5:5m#5YYYYYYYYYYYYYYYYG&\033[m   \033[m\033[38:2:160:10:10m&&#&&\033[m            \033[48:2:180:5:5m&#GP5Y55555555555555555P&\033[m\n");
    printf("                       \033[48:2:180:5:5m&PYYYYYYYYYYYYYYYY#\033[m                        \033[48:2:180:5:5m#P55555555555555555P&\033[m\n");
    printf("                        \033[48:2:180:5:5m&GYYYYYYYYYYYYYY5#\033[m                          \033[48:2:180:5:5m#G5Y555555555555G#\033[m\n");
    printf("                          \033[48:2:180:5:5m#G5YYYYYYYYY5G&\033[m                             \033[48:2:180:5:5m&GP55555555PB&\033[m\n");
    printf("                            \033[48:2:180:5:5m&#GGGGGGB#&\033[m                                  \033[48:2:180:5:5m&##BBB#&\033[m\n");
    printf("\n");
}