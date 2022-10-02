#include "main.h"

int get_input(char *buffer);

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

    char *input_buffer = malloc(sizeof(char) * 256);

    /*
        * Main shell loop
    */
    while (1)
    {
        printf("(~) ");

        // accept user input
        int char_count = 0;
        int word_count = 1;
        get_input(input_buffer);

        // loop through input buffer if it has characters
        if (input_buffer[0] != '\n')
        {
            for (int i = 0; input_buffer[i]; i++)
            {
                printf("%i, ", input_buffer[i]);
                char_count++;
                if (input_buffer[i] == ' ')
                {
                    word_count++;
                }
            }
            printf("Done\n");
        } 
        // nothing was entered
        else 
        {
            word_count = 0;
        }

        // single word commands
        if (word_count == 1)
        {
            // "exit" and only "exit" was entered, so quit
            if (strcmp(input_buffer, "exit\n") == 0)
            {
                printf("\n");
                printf("goodbye <3\n");
                printf("\n");

                free(input_buffer);
                return 0;
            }
            else 
            {
                printf("%s", input_buffer);
            }
        }
        printf("%i chars\n", char_count);
        printf("%i words\n", word_count);
        printf("%s", input_buffer);
    }
}                            
            
int get_input(char *buffer)
{
    fgets(buffer, MAX_INPUT, stdin);
    return 0;
}