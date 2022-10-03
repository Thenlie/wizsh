#include "filesystem.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

void print_current_dir_long(void) {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\033[1;34m"); // blue
        printf("Current dir: ");
        printf("\033[0m");
        printf("%s\n", cwd);
    } else {
        perror("Error while getting dir! 256 char limit.");
    }
    return;
}

void list_current_dir(void) {
    struct dirent *dir_entry; // Pointer for directory entry
    DIR *directory = opendir("."); // opendir() returns a pointer of DIR type. 

    if (directory == NULL) 
    {
        printf("\033[1;31m"); // red
        printf("Could not open current directory" );
        printf("\033[0m");
        return;
    }

    while ((dir_entry = readdir(directory)) != NULL) {
        // check if entry is a folder
        if (dir_entry->d_type == 4) {
            printf("\033[0;34m"); // blue
        }
        printf("%s\n", dir_entry->d_name);
        printf("\033[0m");
    }

    closedir(directory);    
    return;
    // https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
}