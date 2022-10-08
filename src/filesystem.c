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
    // check for help flag
    } else if (word_count == 2 && (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0)) {
        printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
        printf("\033[0mThe \033[1;33m%s\033[0m command is used to display the path to the current directory.\n\n", input[0]);
        printf("\033[1;35m                                   Usage\n\n"); 
        printf("\033[0m ~> \033[1;33m%s        \033[0m| The main usage of the command\n", input[0]);
        printf("\033[0m ~> \033[1;33m%s -h     \033[0m| Help with the command\n", input[0]);
        printf("\033[0m ~> \033[1;33m%s --help \033[0m| Help with the command\n\n", input[0]);
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

    // check for help flag
    } else if (word_count == 2 && (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0)) {
        printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
        printf("\033[0mThe \033[1;33m%s\033[0m command is used to display contents of the current directory.\nFolders will be blue and file will be white.\n\n", input[0]);
        printf("\033[1;35m                                   Usage\n\n");
        printf("\033[0m ~> \033[1;33m%s        \033[0m| The main usage of the command\n", input[0]);
        printf("\033[0m ~> \033[1;33m%s -h     \033[0m| Help with the command\n", input[0]);
        printf("\033[0m ~> \033[1;33m%s --help \033[0m| Help with the command\n\n", input[0]);
    } else {
        print_invalid_use_cmd("ls");
        return;
    }
}

void change_dir(char **input, int word_count) {
    if (word_count == 2) {
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to change the current directory. When provided with a file path as a second argument, the command will move the shell to that location.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n");
            printf("\033[0m ~> \033[1;33m%s <file_path> \033[0m| The main usage of the command. Navigate to \033[1;33mfile_path\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s ..          \033[0m| Navigate \'up\' one directory (to the parent).\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h          \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help      \033[0m| Help with the command\n\n", input[0]);
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
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to create a file. When provided with a file name as a second argument, the command will create a file in the current directory. If a file with that name already exists in the current directory, nothing will happen.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <file_name> \033[0m| The main usage of the command. Create a directory called \033[1;33<file_name>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h          \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help      \033[0m| Help with the command\n\n", input[0]);
            return 0;
        } else {
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
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int create_dir(char **input, int word_count) {
    if (word_count == 2) {
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to create a directory. When provided with a directory name as a second argument, the command will create a directory in the current directory. If a directory with that name already exists in the current directory, nothing will happen.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <directory_name> \033[0m| The main usage of the command. Create a directory called \033[1;33<directory_name>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h               \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help           \033[0m| Help with the command\n\n", input[0]);
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
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to remove a file. When provided with a file name as a second argument, the command will remove the file in the current directory with the same name.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <file_name> \033[0m| The main usage of the command. Remove a file called \033[1;33<file_name>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h          \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help      \033[0m| Help with the command\n\n", input[0]);
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
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to remove an empty directory. When provided with a directory name as a second argument, the command will remove the directory in the current directory with the same name as long as it is empty.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <directory_name> \033[0m| The main usage of the command. Remove a directory called \033[1;33<directory_name>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h               \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help           \033[0m| Help with the command\n\n", input[0]);
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
    } else if (word_count == 2) {
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to write to a file. When provided a file name as a second argument and a string of text, the string of text will be written to the file. If the file does not already exist, the file will be created.\nBy default, the command is set to \033[1;33moverwrite\033[0m mode. You can change this with the \033[1;33m--append\033[0m or \033[1;33m-a\033[0m flag.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <file_name> <input>              \033[0m| The main usage of the command. Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s <file_name> -a <input>           \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in append mode.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s <file_name> --append <input>     \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in append mode.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s <file_name> -o <input>           \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in overwrite mode.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s <file_name> -- overwrite <input> \033[0m| Write \033[1;33m<input>\033[0m to \033[1;33m<file_name>\033[0m in overwrite mode.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h                               \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help                           \033[0m| Help with the command\n\n", input[0]);
            return 0;
        } else {
            print_invalid_use_cmd(input[0]);
            return 1;
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int read_file(char **input, int word_count) {
    if (word_count == 2) {
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to read a file. When provided with a file name as a second argument, the command will print the file to the standard output.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <file_name> \033[0m| The main usage of the command. Read a file called \033[1;33<file_name>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h          \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help      \033[0m| Help with the command\n\n", input[0]);
            return 0;
        } else {
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

            fclose(f);
            return 0; 
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}

int move_file(char **input, int word_count) {
    if (word_count == 2) {
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to move a file. When provided with a file path as a second argument and a new file path as a third argument, the command will move the file to the new file path. This does not remove the contents of the file.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <old_path> <new_path> \033[0m| The main usage of the command. move a file called \033[1;33<old_path>\033[0m to \033[1;33<new_path>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h                    \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help                \033[0m| Help with the command\n\n", input[0]);
            return 0;
        } else {
            print_invalid_use_cmd(input[0]);
            return 1;
        }
    } else if (word_count == 3) {
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
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to open a file in Vim, the ubiquitous text editor. When provided with a file path as a second argument, that file will be opened with vim in a child process of the shell.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <file_path> \033[0m| The main usage of the command. Open \033[1;33m<file_name>\033[0m with Vim.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h          \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help      \033[0m| Help with the command\n\n", input[0]);
            return 0;
        } else {
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
        } 
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}
