#include "git.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

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