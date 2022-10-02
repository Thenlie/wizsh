#include "main.h"

int get_input(char *buffer);

int main (void)
{
    print_logo();

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
            // lowercase the entire input
            for (int i = 0; input_buffer[i]; i++)
            {
                input_buffer[i] = tolower(input_buffer[i]);
            }
            if (strcmp(input_buffer, "exit\n") == 0)
            {
                printf("\ngoodbye <3\n\n");

                free(input_buffer);
                return 0;
            }
            else if (strcmp(input_buffer, "info\n") == 0)
            {
                printf("\033[1;34m");
                printf("                         --  Info --\n");
                printf("\033[1;33m"); 
                printf("            wizsh! Created by Leithen Crider in 2022\n");
                printf("\033[0m");
                printf("Written in C, this was a final project for the Harvard CS50 course.\n");
                printf("  This project was built to gain a better understanding of the C\n");
                printf("programming language and how shells like bash and zsh might work.\n");
            }
            else 
            {
                printf("%s", input_buffer);
            }
        }
        // printf("%i chars\n", char_count);
        // printf("%i words\n", word_count);
        // printf("%s", input_buffer);
    }
}                            
            
int get_input(char *buffer)
{
    fgets(buffer, MAX_INPUT, stdin);
    return 0;
}