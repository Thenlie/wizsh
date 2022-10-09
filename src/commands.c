#include "filesystem.h"
#include "printutility.h"

#include <stdio.h>
#include <string.h>

// Define command structure
typedef struct Command {
    char *name_verbose;
    char *name_short;
    char *description;
    char *usage_list[10]; 
    int (*run)(char **input, int word_count);
} Command;

// Create a global array of commands
Command cmd_arr[] = {
    // -- HELP --
    {
        "help",
        "h",
        "This command prints a complete list of commands available on wizsh.",
        { "\0" },
        print_commands
    },
    // -- READ --
    {
        "read",
        "r",
        "The \033[1;33mread\033[0m command is used to read a file. When provided with a file name as a second argument, the command will print the file to the standard output.",
        {
            "\033[0m ~> \033[1;33mread <file_name> \033[0m| The main usage of the command. Read a file called \033[1;33m<file_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mread -h          \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mread --help      \033[0m| Help with the command\n\n",
        },
        read_file
    },
    // -- CHANGE DIR --
    {
        "cd",
        "\0",
        "The \033[1;33mcd\033[0m command is used to change the current directory. When provided with a file path as a second argument, the command will move the shell to that location.",
        {
                "\033[0m ~> \033[1;33mcd <file_path> \033[0m| The main usage of the command. Navigate to \033[1;33mfile_path\033[0m.\n",
                "\033[0m ~> \033[1;33mcd ..          \033[0m| Navigate \'up\' one directory (to the parent).\n", 
                "\033[0m ~> \033[1;33mcd -h          \033[0m| Help with the command\n", 
                "\033[0m ~> \033[1;33mcd --help      \033[0m| Help with the command\n\n", 
        },
        change_dir
    },
};

int command_handler(char **input, int word_count) {
    // loop through global array of commands
    for (int i = 0; cmd_arr[i].description; i++) {
        // check if command matches first string of user input
        if (strcmp(cmd_arr[i].name_verbose, input[0]) == 0 || strcmp(cmd_arr[i].name_short, input[0]) == 0) {
            printf("Found!\n");
            if (word_count > 1) {
                // check for HELP flag
                if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
                    // print help usage strings
                    for (int j = 0; cmd_arr[i].usage_list[j]; j++) {
                        printf("%s", cmd_arr[i].usage_list[j]);
                    }
                    return 0;
                }
            }
            cmd_arr[i].run(input, word_count);
            return 0;
        }
    }
    // if not returned, input is not a valid command
    printf("Not found!\n");
    return 1;
}