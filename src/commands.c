const char* COMMANDS = { 
    "help", 
    "h", 
    "info",
    "i",
    "clear",
    "c",
    "dir",
    "ls",
    "cd",
    "mkdir",
    "mk",
    "move",
    "mv",
    "read",
    "r",
    "write",
    "w",
    "fetch",
    "f",
    "vim",
    "v"
};

// Define command structure
typedef struct Command {
    char *name_verbose;
    char *name_short;
    char *description;
    char *usage_list[10]; 
    // TODO function pointer
    int (*run)(char **input, int word_count);
} Command;

// Create a global array of commands
Command cmd_arr = {
    // -- HELP --
    {
        "help",
        "h",
        "This command prints a complete list of commands available on wizsh.",
        { "\0" },
        
    },
    // -- READ --
    {
        "read",
        "r",
        "The \033[1;33mread\033[0m command is used to read a file. When provided with a file name as a second argument, the command will print the file to the standard output.",
        {
            "\033[0m ~> \033[1;33mread <file_name> \033[0m| The main usage of the command. Read a file called \033[1;33<file_name>\033[0m.\n",
            "\033[0m ~> \033[1;33mread -h          \033[0m| Help with the command\n",
            "\033[0m ~> \033[1;33mread --help      \033[0m| Help with the command\n\n",
        }
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
        }
    },
};