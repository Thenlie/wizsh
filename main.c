#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_INPUT = 256;

int main (void)
{
    /*
        * Print opening screen and ASCII art
        * [0;33m] = yellow
        * [0;31m] = red
    */
    printf("\033[0;33m");
    printf("                        Welcome to WIZSH!\n");
    printf("                   A simple shell written in C\n");
    printf("                     Created by Thenlie 2022\n");
    printf("\n");
    printf("\033[0;31m");
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

    printf("(~) ");

    // accept user input
    char *input_buffer = malloc(sizeof(char) * 256);
    int char_count = 0;
    // int input = scanf("%[^\n]%256c", input_buffer);

    fgets(input_buffer, MAX_INPUT, stdin);

    if (input_buffer[0] != '\n')
    {
        for (int i = 0; input_buffer[i]; i++)
        {
            printf("%i, ", input_buffer[i]);
            // printf("hit\n");
            char_count++;
        }
        printf("Done\n");
    }
    printf("%i", char_count);
    printf("\n");
    printf("goodbye <3\n");
    printf("\n");

    free(input_buffer);
    return 0;
}                            
            