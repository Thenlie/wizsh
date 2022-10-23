#include "../printutility.h"
#include "git.remote.h"
#include "git.branch.h"
#include "git.print.h"
#include "git.local.h"
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
            "git log        \033[0m| Print a log of recent git commits.\n",
            "git log -h     \033[0m| Help with the command\n",
            "git log --help \033[0m| Help with the command\n\n",
        },
        git_print_command_handler
    },
    {
        "clone",
        "The \033[1;33mgit clone\033[0m command clones a git repository to the current directory.\n",
        {
            "git clone <https_url>             \033[0m| Clone a git repository at the location of \033[1;33m<https_url>\033[0m.\n",
            "git clone <https_url> <directory> \033[0m| Clone a git repository at the location of \033[1;33m<https_url>\033[0m to the directory named \033[1;33m<direcotry>\033[0m.\n",
            "git clone -h                      \033[0m| Help with the command\n",
            "git clone --help                  \033[0m| Help with the command\n\n",
        },
        clone_git_repo
    },
    {
        "status",
         "The \033[1;33mgit status\033[0m command lists the git status of the current directory.\n",
        {
            "git status        \033[0m| List the status of the current git directory.\n",
            "git status -h     \033[0m| Help with the command\n",
            "git status --help \033[0m| Help with the command\n\n",
        },
        git_print_command_handler
    },
    {
        "checkout",
        "The \033[1;33mgit checkout\033[0m command is used to checkout to a different git branch.\n",
        {
            "git checkout <branch>    \033[0m| Checkout to an existing git branch named \033[1;33m<branch>\033[0m].\n",
            "git checkout -b <branch> \033[0m| Create a new branch named \033[1;33m<branch>\033[0m] and checkout to it.\n",
            "git checkout -h          \033[0m| Help with the command\n",
            "git checkout --help      \033[0m| Help with the command\n\n",
        },
        git_branch_command_handler
    }, 
    {
        "branch",
        "The \033[1;33mgit branch\033[0m command is used to list all branches in the current git repository. When provided with a branch name as a third argument, the command will attempt to create a new branch with that name based off the most recent commit.\n",
        {
            "git branch                        \033[0m| View all branches of the current git repository.\n",
            "git branch -n <branch_name>       \033[0m| Create a new branch called \033[1;33m<branch_name>\033[0m.\n",
            "git branch --new <branch_name>    \033[0m| Create a new branch called \033[1;33m<branch_name>\033[0m.\n",
            "git branch -d <branch_name>       \033[0m| Delete a git branch called \033[1;33m<branch_name>\033[0m.\n",
            "git branch --delete <branch_name> \033[0m| Delete a git branch called \033[1;33m<branch_name>\033[0m.\n",
            "git branch -h                     \033[0m| Help with the command\n",
            "git branch --help                 \033[0m| Help with the command\n\n",
        },
        git_branch_command_handler
    },
    {
        "add",
        "The \033[1;33mgit add\033[0m command is used to stage a file for committing. When provided with a file path as a third argument, the command will stage that file so it will be included on the next commit.\n",
        {
            "git add <file_name> \033[0m| Stage a file called \033[1;33m<file_name>\033[0m to be included on the next commit.\n",
            "git add .           \033[0m| Stage all files in the current directory to be included on the next commit.\n", 
            "git add -h          \033[0m| Help with the command\n",
            "git add --help      \033[0m| Help with the command\n\n", 
        },
        git_add_to_index
    },
    {
        "commit",
        "The \033[1;33mgit commit\033[0m command is used to create a new commit using the staged files. When provided with a commit message as a fourth parameter (not enclosed in quotes), a new commit will be created with that message.\n",
        {
            "git commit -m <message> \033[0m| Create a new commit with all staged files. The commit will be created with the message of \033[1;33m<message>\033[0m. This message is a required field.\n", 
            "git commit -h           \033[0m| Help with the command\n",
            "git commit --help       \033[0m| Help with the command\n\n", 
        },
        create_git_commit
    },
    {
        "init",
        "The \033[1;33mgit init\033[0m command is used initialize a new repository. When provided with a path as a third argument, an empty git repository will be initialized at the paths location. If that path does not exist, it will be created. When a path is omitted the repository will be created in the current directory.\n",
        {
            "git init                 \033[0m| Initialize an empty git repository in the current directory.\n", 
            "git init <direcory_path> \033[0m| Initialize an empty git repository in the \033[1;33m<directory_path>\033[0m directory.\n",
            "git init -h              \033[0m| Help with the command\n",
            "git init --help          \033[0m| Help with the command\n\n", 
        },
        init_git_repo
    },
    {
        "restore",
        "The \033[1;33mgit restore\033[0m command is used to remove files from the staging area so they will no longer be included in the next commit. When provided with a file path as a third argument, that file will be removed from the staging area.",
        {
            "git restore <file_path> \033[0m| Remove a file called \033[1;33m<file_name>\033[0m from the git staging area.\n", 
            "git restore -h          \033[0m| Help with the command\n",
            "git restore --help      \033[0m| Help with the command\n\n", 
        },
        git_remove_from_index
    },
    // {
    //     "merge",
    //     "The \033[1;33mgit merge\033[0m command is used to combine two branches into one. Given a branch name as a third argument, the command will merge that branch with the currently checked out branch.\n",
    //     {
    //         "git merge <branch> \033[0m| Merge a branch called \033[1;33m<branch>\033[0m into the currently checked out branch.\n", 
    //         "git merge -h       \033[0m| Help with the command\n",
    //         "git merge --help   \033[0m| Help with the command\n\n", 
    //     },
    //     merge_git_branches
    // },
    {
        "remote",
        "The \033[1;33mgit remote\033[0m commands are used to make changes to a remote repository.\n",
        {
            "git remote             \033[0m| View a list of remote repositories attached to the current local repository.\n", 
            "git remote -h          \033[0m| Help with the command\n",
            "git remote --help      \033[0m| Help with the command\n\n", 
        },
        git_remote_command_handler
    },
    {
        "push",
        "The \033[1;33mgit push\033[0m command is used to push the current commit history to a remote repository.\n",
        {
            "git push <remote> <branch> \033[0m| Push the branch named \033[1;33m<branch>\033[0m to the remote named \033[1;33m<remote>\033[0m.\n", 
            "git push -h                \033[0m| Help with the command\n",
            "git push --help            \033[0m| Help with the command\n\n", 
 
        },
        git_remote_command_handler
    },
    {
        "pull",
        "The \033[1;33mgit pull\033[0m command is used to pull the current commit history from a remote repository and merge it with main if there are differences.\n",
        {
            "git pull <remote> <branch> \033[0m| pull the branch named \033[1;33m<branch>\033[0m from the remote named \033[1;33m<remote>\033[0m.\n", 
            "git pull -h                \033[0m| Help with the command\n",
            "git pull --help            \033[0m| Help with the command\n\n", 
 
        },
        git_remote_command_handler
    }
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
