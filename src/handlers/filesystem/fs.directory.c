#include "../printutility.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int change_dir(char **input, int word_count) {
    if (word_count == 2) {
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

        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 0;
    }
}

int create_dir(char **input, int word_count) {
    if (word_count == 2) {
        char *dir_name = input[1];
        
        if (mkdir(dir_name, 0777) == -1) {
            perror("Unable to create directory!");
            return 1;
        }

        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int remove_dir(char **input, int word_count) {
    if (word_count == 2) {
        char *file_name = input[1];

        if (rmdir(file_name) == -1) {
            perror("Unable to remove directory!");
            return 1;
        }

        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}