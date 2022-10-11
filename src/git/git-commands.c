#include "git.h"

// Define command structure
typedef struct Command {
    char *name;
    char *description;
    char *usage_list[10]; 
    int (*run)(char **input, int word_count);
} Command;

Command cmd_arr[] = {
    // -- LOG -- 
    {
        "log",
        "The \033[1;33mgit log\033[0m command prints out a log of the most recent commits to your current git directory.",
        {
            "git log        \033[0m| The main usage of the command. Print a log of recent git commits.\n",
            "git log -h     \033[0m| Help with the command\n",
            "git log --help \033[0m| Help with the command\n\n",
        },
        print_git_log
    },
};