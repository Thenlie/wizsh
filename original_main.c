#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

    // accept user input
    char input_buffer[256];
    // int cmd_count = 0;
    // int input = scanf("%255[^\n]", input_buffer);
    int input;

    // if (input > 0)
    // {
    //     // lowercase the entire input
    //     for (int i = 0; input_buffer[i]; i++)
    //     {
    //         input_buffer[i] = (input_buffer[i]);
    //         printf("%c", input_buffer[i]);
    //         cmd_count++;
    //     }
    //     printf("\n");
    // }
    // else {
    //     input = scanf("%255[^\n]", input_buffer);
    // }

    // printf("%i", input);

    // int input = scanf("%[^\n]%256c", input_buffer);


    while (1)
    {
        printf("--> ");
        input = get_input(input_buffer);
        if (input > 0)
        {
            // lowercase the entire input
            for (int i = 0; input_buffer[i]; i++)
            {
                input_buffer[i] = tolower(input_buffer[i]);
                printf("%c", input_buffer[i]);
                // cmd_count++;
            }
            printf("\n");
            input = 0;
        }
    }


    // do {
    //     printf("--> ");
    //     scanf("%s", input_buffer);

    //     // lowercase the entire input
    //     for (int i = 0; input_buffer[i]; i++)
    //     {
    //         input_buffer[i] = tolower(input_buffer[i]);
    //         // printf("%c", input_buffer[i]);
    //         cmd_count++;
    //     }

    //     if (strcmp(&input_buffer[0], "exit") != 0 && cmd_count == 1)
    //     {
    //         // print out input
    //         printf("%s\n", input_buffer);
    //     }
    // }
    // while (strcmp(&input_buffer[0], "exit") != 0);

    printf("goodbye <3\n");
    printf("\n");
    return 0;
}                            
                                                       
int get_input(char *buffer)
{
    int input = scanf("%255[^\n]", buffer);
    return input;
}
                                                       