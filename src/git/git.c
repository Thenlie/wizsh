#include "git.h"
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
    git_repository *repo = NULL;
    const char *url = "https://github.com/Thenlie/dynamics-NAV-automator.git";
    const char *path = "./tmp";
    int error = git_clone(&repo, url, path, NULL);
    printf("%i\n", error);
    return 0; 
}