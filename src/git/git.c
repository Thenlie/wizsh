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
        perror("Unable to open current directory");
        closedir(directory);
        return false;
    }

    while ((dir_entry = readdir(directory)) != NULL) {
        // check if entry is a .git
        if (strcmp(dir_entry->d_name, ".git") == 0) {
            closedir(directory);
            return true;
        }
    }
    closedir(directory);    
    return false;
}

void print_current_branch(void) {
    FILE *f;
    f = fopen("./.git/HEAD", "r");
    if (f == NULL) {
        perror("Unable to read git file!");
        fclose(f);
        return;
    }

    // parse git logs
    int count = 0;
    while(1) {
        int c = fgetc(f);
        if (feof(f)) {
            break;
        } else if (ferror(f)) {
            perror("Error reading file!");
            break;
        }
        count++;
        if (count > 16 && c != 10) {
            printf("%c", c);
        }
    }
    return;
}

int print_git_log(char **input, int word_count) {
    // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    } else {
        FILE *f;

        f = fopen("./.git/logs/HEAD", "r");
        if (f == NULL) {
            perror("Unable to read git file!");
            fclose(f);
            return 1;
        }

        // parse git logs
        int count = 0;
        while(1) {
            int c = fgetc(f);
            if (feof(f)) {
                printf("\n");
                break;
            } else if (ferror(f)) {
                perror("Error reading file!");
                break;
            }
            if (count < 82){
                count++;
                continue;
            } else if (c == '\n') {
                count = 0;
                printf("\033[0m%c", c);
            } else {
                if (c == '<' || c == '>'){
                    count++;
                }
                // start of email
                if (count == 83) {
                    printf("\033[1;34m");
                }  
                printf("%c", c);
                // end of email
                if (count == 84) {
                    printf("\033[0m");
                }  
                if (count >= 84) {
                    count++;
                    // start of log type
                    if (count == 103) {
                        printf("\033[1;33m");
                    }
                    // end of log type, start of description
                    if (c == ':') {
                        printf("\033[1;32m");
                    }
                } 
            }
        }
        // make sure to close file!
        fclose(f);
        return 0; 
    }
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