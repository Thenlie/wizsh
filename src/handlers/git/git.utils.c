#include "git.utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <git2.h>

bool is_git_dir(char* dir_path) {
    struct dirent *dir_entry; // Pointer for directory entry
    DIR *directory = opendir(dir_path); // opendir() returns a pointer of DIR type. 

    if (directory == NULL) {
        perror("Unable to open current directory");
        closedir(directory);
        return false;
    }

    // loop through all directories in the given path
    while ((dir_entry = readdir(directory)) != NULL) {
        // check if entry is named '.git'
        if (strcmp(dir_entry->d_name, ".git") == 0) {
            closedir(directory);
            return true;
        }
    }

    closedir(directory);    
    return false;
}

