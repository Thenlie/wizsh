#include "handlers/filesystem.h"
#include "handlers/printutility.h"
#include "handlers/requests.h"
#include "git/git-commands.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
        "The \033[1;3mexit\033[0m command quits wizsh.",
        {
            "exit        \033[0m| Exit wizsh.\n",
            "exit -h     \033[0m| Help with the command\n",
            "exit --help \033[0m| Help with the command\n\n",
        },
        dummy_exit
    },
    // -- HELP --
    {
        "help",
        "h",
        "The \033[1;33mhelp\033[0m command prints a complete list of commands available on wizsh.\n",
        { 
            "help        \033[0m| Print a list of all commands.\n",
            "help -h     \033[0m| Help with the command\n",
            "help --help \033[0m| Help with the command\n\n" ,
        },
        print_commands
    },
    // -- INFO -- 
    {
        "info",
        "i",
        "The \033[1;33minfo\033[0m command is used to display info about this shell and its contributors.\n",
        {
            "info        \033[0m| Print info about wizsh.\n",
            "info -h     \033[0m| Help with the command\n",
            "info --help \033[0m| Help with the command\n\n",
        },
        print_info
    },
    // -- CHANGE DIR --
    {
        "cd",
        "\0",
        "The \033[1;33mcd\033[0m command is used to change the current directory. When provided with a file path as a second argument, the command will move the shell to that location.\n",
        {
                "cd <file_path> \033[0m| Navigate to \033[1;33mfile_path\033[0m.\n",
                "cd ..          \033[0m| Navigate \'up\' one directory (to the parent).\n", 
                "cd -h          \033[0m| Help with the command\n", 
                "cd --help      \033[0m| Help with the command\n\n", 
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
            "clear        \033[0m| Clear the terminal window.",
            "clear -h     \033[0m| Help with the command\n",
            "clear --help \033[0m| Help with the command\n\n",
        },
        clear_term
    },
    // -- FETCH --
    {
        "fetch",
        "f",
        "The \033[1;33m%s\033[0m command is used to rename a file. When provided with a file name as a second argument and a new file name as a third argument, the command will rename the file to the new file name. This does not remove the contents of the file.\n",
        {
            "fetch <old_name> <new_name> \033[0m| Rename a file called \033[1;33<old_name>\033[0m to \033[1;33<new_name>\033[0m.\n",
            "fetch -h                    \033[0m| Help with the command\n",
            "fetch --help                \033[0m| Help with the command\n\n",
        },
        fetch
    },
    // -- PRINT CURRENT DIR --
    {
        "dir",
        "\0",
        "The \033[1;33mdir\033[0m command is used to display the path of the current directory.\n",
        {
            "dir        \033[0m| Display the current working directory path.",
            "dir -h     \033[0m| Help with the command\n",
            "dir --help \033[0m| Help with the command\n\n",
        },
        print_current_dir_path
    },
    // -- LIST FILES IN CURRENT DIR --
    {
        "ls",
        "\0",
        "The \033[1;33mls\033[0m command is used to display contents of the current directory.\nFolders will be blue and file will be white.\n",
        {
            "ls        \033[0m| List the contents of the current working directory",
            "ls -h     \033[0m| Help with the command\n",
            "ls --help \033[0m| Help with the command\n\n",
        },
        list_current_dir

    },
    // -- MAKE DIR --
    {
        "mkdir",
        "\0",
        "The \033[1;33mmkdir\033[0m command is used to create a directory. When provided with a directory name as a second argument, the command will create a directory in the current directory. If a directory with that name already exists in the current directory, nothing will happen.\n",
        {
            "mkdir <directory_name> \033[0m| Create a directory called \033[1;33<directory_name>\033[0m.\n",
            "mkdir -h               \033[0m| Help with the command\n",
            "mkdir --help           \033[0m| Help with the command\n\n",
        },
        create_dir
    },

    // -- MAKE FILE --
    {
        "mk",
        "\0",
        "The \033[1;33mmk\033[0m command is used to create a file. When provided with a file name as a second argument, the command will create a file in the current directory. If a file with that name already exists in the current directory, nothing will happen.\n",
        {
            "mk <file_name> \033[0m| Create a file called \033[1;33<file_name>\033[0m.\n",
            "mk -h          \033[0m| Help with the command\n",
            "mk --help      \033[0m| Help with the command\n\n",
        },
        create_file
    },
    // -- MOVE FILE --
    {
        "move",
        "mv",
        "The \033[1;33mmove\033[0m command is used to move a file. When provided with a file path as a second argument and a new file path as a third argument, the command will move the file to the new file path. This does not remove the contents of the file.\n",
        {
            "move <old_path> <new_path> \033[0m| Move a file called \033[1;33<old_path>\033[0m to \033[1;33<new_path>\033[0m.\n",
            "move -h                    \033[0m| Help with the command\n",
            "move --help                \033[0m| Help with the command\n\n",
        },
        move_file
    },
    // -- REMOVE FILE --
    {
        "rmf",
        "\0",
        "The \033[1;33mrmf\033[0m command is used to remove a file. When provided with a file name as a second argument, the command will remove the file in the current directory with the same name.\n",
        {
            "rmf <file_name> \033[0m| Remove a file called \033[1;33<file_name>\033[0m.\n",
            "rmf -h          \033[0m| Help with the command\n",
            "rmf --help      \033[0m| Help with the command\n\n",
        },
        remove_file
    },
    // -- REMOVE DIR --
    {
        "rmdir",
        "\0",
        "The \033[1;33mrmdir\033[0m command is used to remove an empty directory. When provided with a directory name as a second argument, the command will remove the directory in the current directory with the same name as long as it is empty.\n",
        {
            "rmdir <directory_name> \033[0m| Remove a directory called \033[1;33<directory_name>\033[0m.\n",
            "rmdir -h               \033[0m| Help with the command\n",
            "rmdir --help           \033[0m| Help with the command\n\n",
        },
        remove_dir
    },
    // -- WRITE TO FILE --
    {
        "write",
        "w",
        "The \033[1;33mwrite\033[0m command is used to write to a file. When provided a file name as a second argument and a string of text, the string of text will be written to the file. If the file does not already exist, the file will be created.\nBy default, the command is set to \033[1;33moverwrite\033[0m mode. You can change this with the \033[1;33m--append\033[0m or \033[1;33m-a\033[0m flag.\n",
        {
            "write <file_name> <input>             \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m\n",
            "write <file_name> -a <input>          \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in append mode.\n",
            "write <file_name> --append <input>    \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in append mode.\n",
            "write <file_name> -o <input>          \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in overwrite mode.\n",
            "write <file_name> --overwrite <input> \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in overwrite mode.\n",
            "write -h                              \033[0m| Help with the command\n",
            "write --help                          \033[0m| Help with the command\n\n",
        },
        write_to_file
    },
    // -- READ FILE --
    {
        "read",
        "r",
        "The \033[1;33mread\033[0m command is used to read a file. When provided with a file name as a second argument, the command will print the file to the standard output.\n",
        {
            "read <file_name> \033[0m| Read a file called \033[1;33m<file_name>\033[0m.\n",
            "read -h          \033[0m| Help with the command\n",
            "read --help      \033[0m| Help with the command\n\n",
        },
        read_file
    },
    // -- OPEN WITH VIM --
    {
        "vim",
        "v",
        "The \033[1;33mvim\033[0m command is used to open a file in Vim, the ubiquitous text editor. When provided with a file path as a second argument, that file will be opened with vim in a child process of the shell.\n",
        {
            "vim <file_path> \033[0m| Open \033[1;33m<file_name>\033[0m with Vim.\n",
            "vim -h          \033[0m| Help with the command\n",
            "vim --help      \033[0m| Help with the command\n\n",
        },
        open_file_with_vim
    },
    // -- COPY FILE
    {
        "copy",
        "cp",
        "The \033[1;33mcopy\033[0m command is used to copy a file from one location to another. When provided with a file path as a second argument and another file path as a third argument, the file from the first argument will be copied to the second. If the second file already exist, by default it will be overwritten.\n",
        {
            "copy <file_path> <copy_path>             \033[0m| Copy \033[1;33m<file_path>\033[0m to \033[1;33m<copy_path>\033[0m.\n",
            "copy <file_path> <copy_path> -a          \033[0m| Copy \033[1;33m<file_path>\033[0m to \033[1;33m<copy_path>\033[0m in append mode.\n",
            "copy <file_path> <copy_path> --append    \033[0m| Copy \033[1;33m<file_path>\033[0m to \033[1;33m<copy_path>\033[0m in append mode.\n",
            "copy <file_path> <copy_path> -o         \033[0m| Copy \033[1;33m<file_path>\033[0m to \033[1;33m<copy_path>\033[0m in overwrite mode.\n",
            "copy <file_path> <copy_path> --overwrite \033[0m| Copy \033[1;33m<file_path>\033[0m to \033[1;33m<copy_path>\033[0m in overwrite mode.\n",
            "copy -h                                  \033[0m| Help with the command\n",
            "copy --help                              \033[0m| Help with the command\n\n",
        },
        copy_file
    },
    {
        "tree",
        "tr",
        "The \033[1;33mtree\033[0m command is used to copy a file from one location to another. When provided with a file path as a second argument and another file path as a third argument, the file from the first argument will be copied to the second. If the second file already exist, by default it will be overwritten.\n",
        {
            "tree        \033[0m| Print a directory tree of the current directory\n",
            "tree -f     \033[0m| Print a directory tree of the current directory in flat view\n",
            "tree --flat \033[0m| Print a directory tree of the current directory in flat view\n",
            "tree -h     \033[0m| Help with the command\n",
            "tree --help \033[0m| Help with the command\n\n",
        },
        print_dir_tree
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
                    print_command_help(cmd_arr[i].name_verbose, cmd_arr[i].description, cmd_arr[i].usage_list);
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