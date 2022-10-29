#include "../printutility.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int create_file(char **input, int word_count) {
    if (word_count == 2) {
        FILE *f;
        char *file_name = input[1];

        // create file if it does not exist
        f = fopen(file_name, "a");
        if (f == NULL) {
            perror("Unable to create file!");
            return 1;
        }

        fclose(f);
        return 0; 
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int remove_file(char **input, int word_count) {
    if (word_count == 2) {
        char *file_name = input[1];

        if (remove(file_name) == -1) {
            perror("Unable to remove file!");
            return 1;
        }

        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int write_to_file(char **input, int word_count) {
    if (word_count > 2) {
        FILE *f;
        int i = 2;

        // open in append mode
        if (strcmp(input[2], "-a") == 0 || strcmp(input[2], "--append") == 0) {
            f = fopen(input[1], "a");
            i++;
        // open in write mode
        } else if (strcmp(input[2], "-o") == 0 || strcmp(input[2], "--overwrite") == 0) {
            f = fopen(input[1], "w");
            i++;
        // DEFAULT: open in write mode
        } else {
            f = fopen(input[1], "w");
        }

        if (f == NULL) {
            perror("Unable to open file for writing!");
            return 1;
        } 

        for (int j = i; j < word_count; j++) {
            fprintf(f, "%s", input[j]);
            fprintf(f, " ");
        }

        fclose(f);
        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int read_file(char **input, int word_count) {
    if (word_count == 2) {
        FILE *f;

        f = fopen(input[1], "r");
        if (f == NULL) {
            perror("Unable to read file!");
            return 1;
        }

        while(1) {
            int c = fgetc(f);
            if (feof(f)) {
                printf("\n");
                break;
            } else if (ferror(f)) {
                perror("Error reading file!");
                break;
            }
            printf("%c", c);
        }

        // make sure to close file!
        fclose(f);
        return 0; 
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int move_file(char **input, int word_count) {
    if (word_count == 3) {
        if (rename(input[1], input[2]) == -1) {
            perror("Error renaming file!");
            return 1;
        } else {
            return 0;
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int copy_file(char **input, int word_count) {
    if (word_count == 3 || word_count == 4) {
        FILE *rf;
        FILE *wf;

        rf = fopen(input[1], "r");
        if (rf == NULL) {
            perror("Unable to open read file to copy!");
            return 1;
        }
        // check for append flag
        if (strcmp(input[3], "-a") == 0 || strcmp(input[3], "--append") == 0) {
            wf = fopen(input[2], "a");
        // DEFAULT overwrite
        } else {
            wf = fopen(input[2], "w");
        }
        
        if (wf == NULL) {
            perror("Unable to open write file to copy!");
            fclose(rf);
            return 1;
        }

        // read from rf
        // fprintf to wf
        while(1) {
            int c = fgetc(rf);
            if (feof(rf)) {
                break;
            } else if (ferror(rf)) {
                perror("Error reading file!");
                break;
            }
            fprintf(wf, "%c", c);
        }

        fclose(wf);
        fclose(rf);
        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int open_file_with_vim(char **input, int word_count) {
    if (word_count == 2) {
        // create a fork of the current process
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error forking process!");
            return 1;
        } else if (pid == 0) {
            // child process
            FILE *f;
            f = fopen(input[1], "a");
            // open vim
            if (f == NULL) {
                char *exec_args[] = {"vim", NULL};
                int e = execv("/usr/bin/vim", exec_args);
                if (e == -1) {
                    perror("Error opening Vim!");
                }
            }
            else {
                char *exec_args[] = {"vim", input[1], NULL};
                int e = execv("/usr/bin/vim", exec_args);
                if (e == -1) {
                    perror("Error opening Vim!");
                }
            }
        } else {
            // parent process
            wait(NULL);
        }
        // https://www.section.io/engineering-education/fork-in-c-programming-language/
        return 0;
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}