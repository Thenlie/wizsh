#include "filesystem.h"
#include "printutility.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void print_current_dir_path(char **input, int word_count) {
    if (word_count == 1) {
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
    } else if (word_count == 2 && (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0)) {
        printf("%s", input[0]); // <- first command
    } else {
        print_invalid_use_cmd("dir");
        return;
    }
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

void list_current_dir(char** input, int word_count) {
    if (word_count == 1) {
        struct dirent *dir_entry; // Pointer for directory entry
        DIR *directory = opendir("."); // opendir() returns a pointer of DIR type. 

        if (directory == NULL) 
        {
            perror("Unable to open current directory" );
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
    } else if (word_count == 2 && (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0)) {
        printf("%s", input[0]); // <- first command
    } else {
        print_invalid_use_cmd("ls");
        return;
    }
}

void change_dir(char **input, int word_count) {
    if (word_count == 2) {
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("%s", input[0]); // <- first command
        } else {
            char *path = input[1];

            // change dir if possible
            if (chdir(path) == -1) {
                perror("Unable to change directories!");
            } else {
                char cwd[256];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                } else {
                    perror("Error while getting dir! 256 char limit.");
                }
            }
            return;
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return;
    }
}

int create_file(char **input, int word_count) {
    if (word_count == 2) {
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("%s", input[0]); // <- first command
            return 0;
        } else {
            FILE *f;
            char *file_name = input[1];

            // create file if it does not exist
            f = fopen(file_name, "w");
            if (f == NULL) {
                perror("Unable to create file!");
                fclose(f);
                free(file_name);
                return 1;
            }
            fclose(f);
            return 0; 
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int create_dir(char **input, int word_count) {
    if (word_count == 2) {
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("%s", input[0]); // <- first command
            return 0;
        } else {
            char *dir_name = input[1];
            if (mkdir(dir_name, 0777) == -1) {
                perror("Unable to create directory!");
                return 1;
            }
            return 0;
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int remove_file(char **input, int word_count) {
    if (word_count == 2) {
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("%s", input[0]); // <- first command
            return 0;
        } else {
            char *file_name = input[1];
            if (remove(file_name) == -1) {
                perror("Unable to remove file!");
                return 1;
            }
            return 0;
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int remove_dir(char **input, int word_count) {
    if (word_count == 2) {
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("%s", input[0]); // <- first command
            return 0;
        } else {
            char *file_name = input[1];
            if (rmdir(file_name) == -1) {
                perror("Unable to remove directory!");
                return 1;
            }
            return 0;
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}