#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

void print_current_dir_path(void) {
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

void print_current_dir(void) {
    char cwd[256];
    char cur_dir[256];
    int count = 0;
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        for (int i = 0; cwd[i]; i++) {
            // printf("%c", cwd[i]);
            if (cwd[i] == 47) {
                count = 0;
            } else {
                if (count == 0) {
                    for (int j = 0; j < i; j++) {
                        cur_dir[j] = 0;
                    }
                }
                cur_dir[count] = cwd[i];
                count++;
            }
        }
        printf(" %s ", cur_dir);
        return;
    } else {
        perror("Error while getting dir! 256 char limit.");
    }
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

void change_dir(char *input_buffer, int char_count) {
    char *path;
    path = malloc(256);
    int path_count = 0;
    for (int i = 3; i < char_count - 1; i++) {
        path[path_count] = input_buffer[i];
        path_count++;
    }
    // path[path_count] = '\0';
    if (chdir(path) == -1) {
        perror("Error while changing directories!");
    } else {
        char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("Error while getting dir! 256 char limit.");
        }
    }
    free(path);
    return;
}

int create_file(char *input_buffer, int char_count) {
    FILE *f;
    char *file_name = malloc(char_count - 4);
    int file_count = 0;

    printf("%i\n", char_count);
    printf("%s\n", input_buffer);

    for (int i = 3; i < char_count - 1; i++) {
        file_name[file_count] = input_buffer[i];
        printf("%i\n", file_name[file_count]);
        file_count++;
    }

    printf("%s", file_name);

    f = fopen(file_name, "w");
    if (f == NULL) {
        perror("Unable to create file!");
        fclose(f);
        free(file_name);
        return 1;
    }
    fclose(f);
    free(file_name);
    return 0; 
}