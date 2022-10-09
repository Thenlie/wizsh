#include "filesystem.h"
#include "printutility.h"
#include "requests.h"

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
    // -- EXIT
    {
        "exit",
        "e",
        "The \033[1;33exit\033[0m command quits wizsh.",
        {
            "\033[0m ~> \033[1;33mexit        \033[0m| The main usage of the command. Exit wizsh.\n"
            "\033[0m ~> \033[1;33mexit -h     \033[0m| Help with the command\n"
            "\033[0m ~> \033[1;33mexit --help \033[0m| Help with the command\n\n"
        },
        dummy_exit
    },
    // -- HELP --
    {
        "help",
        "h",
        "The \033[1;33mhelp\033[0m command prints a complete list of commands available on wizsh.\n",
        { 
            "\033[0m ~> \033[1;33mhelp        \033[0m| The main usage of the command. Print a list of all commands.\n"
            "\033[0m ~> \033[1;33mhelp -h     \033[0m| Help with the command\n"
            "\033[0m ~> \033[1;33mhelp --help \033[0m| Help with the command\n\n" },
        print_commands
    },
    // -- INFO -- 
    {
        "info",
        "i",
        "The \033[1;33minfo\033[0m command is used to display info about this shell and its contributors.\n",
        {
            "\033[0m ~> \033[1;33minfo        \033[0m| The main usage of the command. Print info about wizsh.\n"
            "\033[0m ~> \033[1;33minfo -h     \033[0m| Help with the command\n"
            "\033[0m ~> \033[1;33minfo --help \033[0m| Help with the command\n\n"
        },
        print_info
    },
    // -- CHANGE DIR --
    {
        "cd",
        "\0",
        "The \033[1;33mcd\033[0m command is used to change the current directory. When provided with a file path as a second argument, the command will move the shell to that location.\n",
        {
                "\033[0m ~> \033[1;33mcd <file_path> \033[0m| The main usage of the command. Navigate to \033[1;33mfile_path\033[0m.\n",
                "\033[0m ~> \033[1;33mcd ..          \033[0m| Navigate \'up\' one directory (to the parent).\n", 
                "\033[0m ~> \033[1;33mcd -h          \033[0m| Help with the command\n", 
                "\033[0m ~> \033[1;33mcd --help      \033[0m| Help with the command\n\n", 
        },
        change_dir
    },

    // -- WIZARD --
    {
        "wizard",
        "\0",
        "Shh...",
        {
            "\0"
        },
        print_wizard
    },
    
    // -- CLEAR TERM --
    {
        "clear",
        "c",
        "The \033[1;33mclear\033[0m command is used to clear the terminal of all input.\n",
        {
            "\033[0m ~> \033[1;33mclear        \033[0m| The main usage of the command\n",
            "\033[0m ~> \033[1;33mclear -h     \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mclear --help \033[0m| Help with the command\n\n",
        },
        clear_term
    },
    // -- FETCH --
    {
        "fetch",
        "f",
        "The \033[1;33m%s\033[0m command is used to rename a file. When provided with a file name as a second argument and a new file name as a third argument, the command will rename the file to the new file name. This does not remove the contents of the file.\n",
        {
            "\033[0m ~> \033[1;33m%s <old_name> <new_name> \033[0m| The main usage of the command. Rename a file called \033[1;33<old_name>\033[0m to \033[1;33<new_name>\033[0m.\n",
            "\033[0m ~> \033[1;33m%s -h                    \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33m%s --help                \033[0m| Help with the command\n\n",
        },
        fetch
    },
    // -- PRINT CURRENT DIR --
    {
        "dir",
        "\0",
        "The \033[1;33mdir\033[0m command is used to display the path of the current directory.\n",
        {
            "\033[0m ~> \033[1;33mdir        \033[0m| The main usage of the command\n",
            "\033[0m ~> \033[1;33mdir -h     \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mdir --help \033[0m| Help with the command\n\n",
        },
        print_current_dir_path
    },
    // -- LIST FILES IN CURRENT DIR --
    {
        "ls",
        "\0",
        "The \033[1;33mls\033[0m command is used to display contents of the current directory.\nFolders will be blue and file will be white.\n",
        {
            "\033[0m ~> \033[1;33mls        \033[0m| The main usage of the command\n",
            "\033[0m ~> \033[1;33mls -h     \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mls --help \033[0m| Help with the command\n\n",
        },
        list_current_dir

    },
    // -- MAKE DIR --
    {
        "mkdir",
        "\0",
        "The \033[1;33mmkdir\033[0m command is used to create a directory. When provided with a directory name as a second argument, the command will create a directory in the current directory. If a directory with that name already exists in the current directory, nothing will happen.\n",
        {
            "\033[0m ~> \033[1;33mmkdir <directory_name> \033[0m| The main usage of the command. Create a directory called \033[1;33<directory_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mmkdir -h               \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mmkdir --help           \033[0m| Help with the command\n\n",
        },
        create_dir
    },

    // -- MAKE FILE --
    {
        "mk",
        "\0",
        "The \033[1;33mmk\033[0m command is used to create a file. When provided with a file name as a second argument, the command will create a file in the current directory. If a file with that name already exists in the current directory, nothing will happen.\n",
        {
            "\033[0m ~> \033[1;33mmk <file_name> \033[0m| The main usage of the command. Create a directory called \033[1;33<file_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mmk -h          \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mmk --help      \033[0m| Help with the command\n\n",
        },
        create_file
    },
    // -- MOVE FILE --
    {
        "move",
        "mv",
        "The \033[1;33mmove\033[0m command is used to move a file. When provided with a file path as a second argument and a new file path as a third argument, the command will move the file to the new file path. This does not remove the contents of the file.\n",
        {
            "\033[0m ~> \033[1;33mmove <old_path> <new_path> \033[0m| The main usage of the command. move a file called \033[1;33<old_path>\033[0m to \033[1;33<new_path>\033[0m.\n",
            "\033[0m ~> \033[1;33mmove -h                    \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mmove --help                \033[0m| Help with the command\n\n",
        },
        move_file
    },
    // -- REMOVE FILE --
    {
        "rmf",
        "\0",
        "The \033[1;33mrmf\033[0m command is used to remove a file. When provided with a file name as a second argument, the command will remove the file in the current directory with the same name.\n",
        {
            "\033[0m ~> \033[1;33mrmf <file_name> \033[0m| The main usage of the command. Remove a file called \033[1;33<file_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mrmf -h          \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mrmf --help      \033[0m| Help with the command\n\n",
        },
        remove_file
    },
    // -- REMOVE DIR --
    {
        "rmdir",
        "\0",
        "The \033[1;33mrmdir\033[0m command is used to remove an empty directory. When provided with a directory name as a second argument, the command will remove the directory in the current directory with the same name as long as it is empty.\n",
        {
            "\033[0m ~> \033[1;33mrmdir <directory_name> \033[0m| The main usage of the command. Remove a directory called \033[1;33<directory_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mrmdir -h               \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mrmdir --help           \033[0m| Help with the command\n\n",
        },
        remove_dir
    },
    // -- WRITE TO FILE --
    {
        "write",
        "w",
        "The \033[1;33mwrite\033[0m command is used to write to a file. When provided a file name as a second argument and a string of text, the string of text will be written to the file. If the file does not already exist, the file will be created.\nBy default, the command is set to \033[1;33moverwrite\033[0m mode. You can change this with the \033[1;33m--append\033[0m or \033[1;33m-a\033[0m flag.\n",
        {
            "\033[0m ~> \033[1;33mwrite <file_name> <input>              \033[0m| The main usage of the command. Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m\n",
            "\033[0m ~> \033[1;33mwrite <file_name> -a <input>           \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in append mode.\n",
            "\033[0m ~> \033[1;33mwrite <file_name> --append <input>     \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in append mode.\n",
            "\033[0m ~> \033[1;33mwrite <file_name> -o <input>           \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in overwrite mode.\n",
            "\033[0m ~> \033[1;33mwrite <file_name> -- overwrite <input> \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in overwrite mode.\n",
            "\033[0m ~> \033[1;33mwrite -h                               \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mwrite --help                           \033[0m| Help with the command\n\n",
        },
        write_to_file
    },
    // -- READ FILE --
    {
        "read",
        "r",
        "The \033[1;33mread\033[0m command is used to read a file. When provided with a file name as a second argument, the command will print the file to the standard output.\n",
        {
            "\033[0m ~> \033[1;33mread <file_name> \033[0m| The main usage of the command. Read a file called \033[1;33m<file_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mread -h          \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mread --help      \033[0m| Help with the command\n\n",
        },
        read_file
    },
    // -- OPEN WITH VIM --
    {
        "vim",
        "v",
        "The \033[1;33mvim\033[0m command is used to open a file in Vim, the ubiquitous text editor. When provided with a file path as a second argument, that file will be opened with vim in a child process of the shell.\n",
        {
            "\033[0m ~> \033[1;33mvim <file_path> \033[0m| The main usage of the command. Open \033[1;33m<file_name>\033[0m with Vim.\n",
            "\033[0m ~> \033[1;33mvim -h          \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mvim --help      \033[0m| Help with the command\n\n",
        },
        open_file_with_vim
    },

};

int command_handler(char **input, int word_count, char *input_buffer) {
    // loop through global array of commands
    for (int i = 0; cmd_arr[i].description; i++) {
        // check if command matches first string of user input
        if (strcmp(cmd_arr[i].name_verbose, input[0]) == 0 || strcmp(cmd_arr[i].name_short, input[0]) == 0) {
            if (word_count == 2) {
                // check for HELP flag
                if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
                    // print help usage strings
                    for (int j = 0; cmd_arr[i].usage_list[j]; j++) {
                        printf("%s", cmd_arr[i].usage_list[j]);
                    }
                    return 0;
                }
            }
            return cmd_arr[i].run(input, word_count);
        }
    }
    // if not returned, input is not a valid command
    print_invalid_cmd(input_buffer);
    return 1;
}