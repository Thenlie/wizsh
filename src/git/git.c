#include "git.h"
#include "../handlers/printutility.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <git2.h>

bool is_git_dir(char* dir_path) {
    struct dirent *dir_entry; // Pointer for directory entry
    DIR *directory = opendir(dir_path); // opendir() returns a pointer of DIR type. 

    if (directory == NULL) {
        perror("Unable to open current directory" );
        closedir(directory);
        return false;
    }

    while ((dir_entry = readdir(directory)) != NULL) {
        // check if entry is a .git
        if (strcmp(dir_entry->d_name, ".git")) {
            closedir(directory);
            return true;
        }
    }
    closedir(directory);    
    return false;
}

int print_git_log(char **input, int word_count) {

    return 0; 
}

int clone_git_repo(char **input, int word_count) {
    if (word_count == 3) {
        git_repository *repo = NULL;
        const char *url = input[2];
        const char *path = ".";
        int e = git_clone(&repo, url, path, NULL);
        if (e != 0) {
            perror(git_error_last()->message);
        }
        return 0;
    } else {
        print_invalid_use_cmd("git clone");
        return 1;
    }
}