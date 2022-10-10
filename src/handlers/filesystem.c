#include "filesystem.h"
#include "printutility.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int print_current_dir_path(char **input, int word_count) {
    if (word_count == 1) {
        char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("\033[1;34m"); // blue
            printf("Current dir: ");
            printf("\033[0m");
            printf("%s\n", cwd);
            return 0;
        } else {
            perror("Error while getting dir! 256 char limit.");
            return 1;
        }
    // check for help flag
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
        printf(" %s ", cur_dir);
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

        if (directory == NULL) 
        {
            perror("Unable to open current directory" );
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

int create_file(char **input, int word_count) {
    if (word_count == 2) {
        FILE *f;
        char *file_name = input[1];

        // create file if it does not exist
        f = fopen(file_name, "a");
        if (f == NULL) {
            perror("Unable to create file!");
            fclose(f);
            free(file_name);
            return 1;
        }
        fclose(f);
        return 0; 
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
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
            perror("Unable to create file!");
            fclose(f);
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
        int r = rename(input[1], input[2]);
        if (r == -1) {
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

int dummy_exit(char **input, int word_count) {
    return -1;
}

int copy_file(char **input, int word_count) {
    if (word_count >= 3) {
        FILE *rf;
        FILE *wf;

        rf = fopen(input[1], "r");
        if (rf == NULL) {
            perror("Unable to open read file to copy!");
            fclose(rf);
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
            fclose(wf);
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
