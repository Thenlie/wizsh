#include "git.h"
#include "../handlers/printutility.h"

#include <stdio.h>
#include <string.h>

// Define command structure
typedef struct Command {
    char *name;
    char *description;
    char *usage_list[10]; 
    int (*run)(char **input, int word_count);
} Command;

Command git_cmd_arr[] = {
    // -- LOG -- 
    {
        "log",
        "The \033[1;33mgit log\033[0m command prints out a log of the most recent commits to your current git directory.\n",
        {
            "git log        \033[0m| The main usage of the command. Print a log of recent git commits.\n",
            "git log -h     \033[0m| Help with the command\n",
            "git log --help \033[0m| Help with the command\n\n",
        },
        print_git_log
    },
    {
        "clone",
        "The \033[1;33mgit clone\033[0m command clones a git repository to the current directory.\n",
        {
            "git clone <https_url> \033[0m| The main usage of the command. Clone a git repository at the location of \033[1;33m<https_url>\033[0m.\n",
            "git clone -h          \033[0m| Help with the command\n",
            "git clone --help      \033[0m| Help with the command\n\n",
        },
        clone_git_repo
    },
    {
        "status",
         "The \033[1;33mgit status\033[0m command lists the git status of the current directory.\n",
        {
            "git status        \033[0m| The main usage of the command. List the status of the current git directory.\n",
            "git status -h     \033[0m| Help with the command\n",
            "git status --help \033[0m| Help with the command\n\n",
        },
        print_git_status
    },
    // {
    //     "checkout",
    //     "The \033[1;33mgit checkout\033[0m command is used to checkout to a new git branch.\n",
    //     {
    //         "git checkout        \033[0m| The main usage of the command. Checkout to a new git branch.\n",
    //         "git checkout -h     \033[0m| Help with the command\n",
    //         "git checkout --help \033[0m| Help with the command\n\n",
    //     },
    //     // need command
    // }, 
    {
        "branch",
        "The \033[1;33mgit branch\033[0m command is used to list all branches in the current git repository.\n",
        {
            "git branch        \033[0m| The main usage of the command. View all branches of the current git repository.\n",
            "git branch -h     \033[0m| Help with the command\n",
            "git branch --help \033[0m| Help with the command\n\n",
        },
        print_git_branches
    },
};

int git_command_handler(char** input, int word_count, char* input_buffer) {
    if (word_count > 1) {
        // loop through global array of commands
        for (int i = 0; git_cmd_arr[i].description; i++) {
            // check if command matches first string of user input
            if (strcmp(git_cmd_arr[i].name, input[1]) == 0) {
                if (word_count >= 3) {
                    // check for HELP flag
                    if (strcmp(input[2], "-h") == 0 || strcmp(input[2], "--help") == 0) {
                        // print help usage strings
                        print_command_help(git_cmd_arr[i].name, git_cmd_arr[i].description, git_cmd_arr[i].usage_list);
                        return 0;
                    } 
                }
                return git_cmd_arr[i].run(input, word_count);
            }
        }
    }
    // if not returned, input is not a valid command
    print_invalid_cmd(input_buffer);
    return 1;
}
