#include "../printutility.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

void print_tree(char *dir, int count, bool is_flat);

int print_current_dir_path(char **input, int word_count) {
    if (word_count == 1) {
        char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("\033[1;34mCurrent dir: \033[0m");
            printf("%s\n", cwd);
            return 0;
        } else {
            perror("Error while getting dir! 256 char limit.");
            return 1;
        }
    } else {
        print_invalid_use_cmd("dir");
        return 1;
    }
}

void print_current_dir(void) {
    char cwd[256];
    char cur_dir[256];
    int count = 0;
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        for (int i = 0; cwd[i]; i++) {
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
        printf("%s", cur_dir);
        return;
    } else {
        perror("Error while getting dir! 256 char limit.");
        return;
    }
}

int list_current_dir(char** input, int word_count) {
    if (word_count == 1) {
        struct dirent *dir_entry; // Pointer for directory entry
        DIR *directory = opendir("."); // opendir() returns a pointer of DIR type. 

        if (directory == NULL) {
            perror("Unable to open current directory" );
            closedir(directory);
            return 1;
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
        printf("\033[0m\n");
        return 0;
        // https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

    // check for help flag
    } else {
        print_invalid_use_cmd("ls");
        return 1;
    }
    return 0;
}

int print_dir_tree(char **input, int word_count) {
    if (word_count == 1) {
        printf(".\n");
        print_tree(".", 0, false);
        return 0;
    } else if (word_count == 2 && (strcmp(input[1], "-f") == 0 || strcmp(input[1], "--flat") == 0)) {
        printf(".\n");
        print_tree(".", 0, true);
        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

// tree helper function
void print_tree(char *dir, int count, bool is_flat) {
    struct dirent *dir_entry; // Pointer for directory entry
    DIR *directory = opendir(dir); // opendir() returns a pointer of DIR type. 

    if (directory == NULL) {
        perror("Unable to open current directory" );
        return;
    }

    while ((dir_entry = readdir(directory)) != NULL) {
        // entry is a folder
        if (dir_entry->d_type == 4 && (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0) && dir_entry->d_name[0] != '.') {
            if (!is_flat) {
                for (int i = 0; i < count; i++) {
                    printf("\u2502   ");
                }
            }
            printf("\u251C\u2500\u2500 \033[0;34m%s\033[0m\n", dir_entry->d_name);
            // create path to new sub-dir
            char *new_path;
            if (asprintf(&new_path, "%s/%s", dir, dir_entry->d_name) == 0) {
                perror("Unable to create directory path" );
            }
            // recursively call print tree for each sub-dir
            print_tree(new_path, count + 1, is_flat);
            free(new_path);
        // entry is a file, print
        } else if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0) {
            if (!is_flat) {
                for (int i = 0; i < count; i++) {
                    printf("\u2502   ");
                }
            }
            printf("\u251C\u2500\u2500 %s\n", dir_entry->d_name);
        }
    }
    closedir(directory);    
    return;
}
