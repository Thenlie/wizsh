#include "git.h"
#include "../handlers/printutility.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <git2.h>
#include <git2/common.h>


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
    fclose(f);
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
        git_repository_free(repo);
        return 0;
    } else {
        print_invalid_use_cmd("git clone");
        return 1;
    }
}

// git status options
struct status_opts {
  git_status_options statusopt;
  char *repodir;
  char *pathspec[8];
  int npaths;
  int format;
  int zterm;
  int showbranch;
  int showsubmod;
  int repeat;
};

int print_git_status(char **input, int word_count) {
    // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    } else {
        git_status_list *status;
        git_repository *repo = NULL;
        // initialize status options
        struct status_opts o = { GIT_STATUS_OPTIONS_INIT, "." };
        // set basic status options
        o.statusopt.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
            GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
            GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
        // open repo in current directory
        int x = git_repository_open(&repo, ".");
        // check for error opening repo
        if (x != 0) {
            perror(git_error_last()->message);
            return 1;
        }
        // ensure you are not in a bare repo (one that cannot accept commits)
        if (git_repository_is_bare(repo)) {
            printf("Cannot report status on bare repository! %s", git_repository_path(repo));
            git_repository_free(repo);
            return 1;
        }
        // initialize a new status list with the address of our status list buffer, the repo and status options
        int y = git_status_list_new(&status, repo, &o.statusopt);
        if (y != 0) {
            perror(git_error_last()->message);
            return 1;
        }
        // create iterator variables the size of the newly created status list
        size_t i, maxi = git_status_list_entrycount(status);
        // create a pointer a single status entry
        const git_status_entry *s;
        // create status flags for the index and working dir
        char istatus, wstatus;
        // create additional flags for submodules and file paths
        const char *extra, *a, *b, *c;
        bool first_entry = true;

        printf("On branch ");
        print_current_branch();

        // loop through status list and print staged files
        for (i = 0; i < maxi; ++i) {
            s = git_status_byindex(status, i);
            if (s->status == GIT_STATUS_CURRENT) {
                continue;
            }
            a = b = c = NULL;
            istatus = ' ';
            extra = "";
            /** 
             * THE `GIT_STATUS_INDEX` SET OF FLAGS REPRESENTS 
             * THE STATUS OF FILE IN THE INDEX RELATIVE TO THE HEAD, AND THE
             * `GIT_STATUS_WT` SET OF FLAGS REPRESENT THE STATUS OF THE FILE IN THE
             * WORKING DIRECTORY RELATIVE TO THE INDEX.
            */
            if (s->status & GIT_STATUS_INDEX_NEW) {
                istatus = 'A';
            } if (s->status & GIT_STATUS_INDEX_MODIFIED) {
                istatus = 'M';
            } if (s->status & GIT_STATUS_INDEX_DELETED) {
                istatus = 'D';
            } if (s->status & GIT_STATUS_INDEX_RENAMED) {
                istatus = 'R';
            } if (s->status & GIT_STATUS_INDEX_TYPECHANGE) {
                istatus = 'T';
            }
            // if no index status is set, go to next status entry
            // if file is being ignored, do not print and go to next status entry
            if (istatus == ' ' || s->status & GIT_STATUS_IGNORED) {
                continue;
            }

            // check submodules for changes
            if (s->index_to_workdir && s->index_to_workdir->new_file.mode == GIT_FILEMODE_COMMIT) {
                unsigned int smstatus = 0;

                if (!git_submodule_status(&smstatus, repo, s->index_to_workdir->new_file.path,GIT_SUBMODULE_IGNORE_UNSPECIFIED)) {
                    if (smstatus & GIT_SUBMODULE_STATUS_WD_MODIFIED) {
                        extra = " (new commits)";
                    } else if (smstatus & GIT_SUBMODULE_STATUS_WD_INDEX_MODIFIED) {
                        extra = " (modified content)";
                    } else if (smstatus & GIT_SUBMODULE_STATUS_WD_WD_MODIFIED) {
                        extra = " (modified content)";
                    } else if (smstatus & GIT_SUBMODULE_STATUS_WD_UNTRACKED) {
                        extra = " (untracked content)";
                    }
                }
            }

            // check for renamed files, get path names
            if (s->head_to_index) {
                a = s->head_to_index->old_file.path;
                b = s->head_to_index->new_file.path;
            }
            if (s->index_to_workdir) {
                if (!a) {
                    a = s->index_to_workdir->old_file.path;
                }
                if (!b) {
                    b = s->index_to_workdir->old_file.path;
                    c = s->index_to_workdir->new_file.path;
                }
            }

            if (first_entry) {
                printf("\nChanges staged for commit:\n");
                printf("  (use \"git restore --staged <file>\" to unstage)\n\033[0;32m");
                first_entry = false;
            }

            // check for rename flag on index to determine file paths to print
            switch (istatus) {
                case 'R':
                    printf("    Renamed:     %s %s %s\n", a, b, extra);
                    break;
                case 'A':
                    printf("    New:         %s %s\n", a, extra);
                    break;
                case 'M':
                    printf("    Modified:    %s %s\n", a, extra);
                    break;
                case 'D': 
                    printf("    Deleted:     %s %s\n", a, extra);
                    break;
                case 'T':
                    printf("    Typechanged: %s %s\n", a, extra);
                    break;
            }
        }
        if (first_entry) {
            printf("No changes currently added to next commit\n");
        }
        printf("\033[0m");
        first_entry = true;

        // loop through status list again and print un-staged files
        for (i = 0; i < maxi; ++i) {
            s = git_status_byindex(status, i);
            if (s->status == GIT_STATUS_CURRENT) {
                continue;
            }
            a = b = c = NULL;
            wstatus = ' ';
            extra = "";

            if (s->status & GIT_STATUS_WT_NEW) {
                continue;
            } if (s->status & GIT_STATUS_WT_MODIFIED) {
                wstatus = 'M';
            } if (s->status & GIT_STATUS_WT_DELETED) {
                wstatus = 'D';
            } if (s->status & GIT_STATUS_WT_RENAMED) {
                wstatus = 'R';
            } if (s->status & GIT_STATUS_WT_TYPECHANGE) {
                wstatus = 'T';
            }
            // if no working dir status is set, go to next status entry
            // if file is being ignored, go to next status entry
            if (wstatus == ' ' || s->status & GIT_STATUS_IGNORED) {
                continue;
            }

            // check submodules for changes
            if (s->index_to_workdir && s->index_to_workdir->new_file.mode == GIT_FILEMODE_COMMIT) {
                unsigned int smstatus = 0;

                if (!git_submodule_status(&smstatus, repo, s->index_to_workdir->new_file.path,GIT_SUBMODULE_IGNORE_UNSPECIFIED)) {
                    if (smstatus & GIT_SUBMODULE_STATUS_WD_MODIFIED) {
                        extra = " (new commits)";
                    } else if (smstatus & GIT_SUBMODULE_STATUS_WD_INDEX_MODIFIED) {
                        extra = " (modified content)";
                    } else if (smstatus & GIT_SUBMODULE_STATUS_WD_WD_MODIFIED) {
                        extra = " (modified content)";
                    } else if (smstatus & GIT_SUBMODULE_STATUS_WD_UNTRACKED) {
                        extra = " (untracked content)";
                    }
                }
            }

            // check for renamed files, get path names
            if (s->head_to_index) {
                a = s->head_to_index->old_file.path;
                b = s->head_to_index->new_file.path;
            }
            if (s->index_to_workdir) {
                if (!a) {
                    a = s->index_to_workdir->old_file.path;
                }
                if (!b) {
                    b = s->index_to_workdir->old_file.path;
                    c = s->index_to_workdir->new_file.path;
                }
            }

            if (first_entry) {
                printf("Changes not staged for commit:\n");
                printf("  (use \"git add <file>\" to include changes on your next commit)\n\033[0;31m");
                first_entry = false;
            }

            // check for rename flag on index to determine file paths to print
            switch (wstatus) {
                case 'R':
                    printf("    Renamed:     %s %s %s\n", a, c, extra);
                    break;
                case 'M':
                    printf("    Modified:    %s %s\n", a, extra);
                    break;
                case 'D': 
                    printf("    Deleted:     %s %s\n", a, extra);
                    break;
                case 'T':
                    printf("    Typechanged: %s %s\n", a, extra);
                    break;
            }

        }
        first_entry = true;
        printf("\033[0m");

        // loop through status list again and print untracked files 
        for (i = 0; i < maxi; ++i) {
            s = git_status_byindex(status, i);
            if (s->status == GIT_STATUS_WT_NEW) {
                if (first_entry) {
                    printf("Untracked files:\n");
                    printf("  (use \"git add <file>\" to include files on your next commit)\n\033[0;31m");
                    first_entry = false;
                }
                printf("    %s\n", s->index_to_workdir->old_file.path);
            }
        }
        printf("\033[0m\n");

        // https://libgit2.org/libgit2/ex/HEAD/status.html#git_status_foreach-6
        git_repository_free(repo);
        git_status_list_free(status);
        return 0;
    }
}

int print_git_branches(char **input, int word_count) {
    // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    }  else {
        git_repository *repo = NULL;
        git_branch_iterator *iter;
        
        int x = git_repository_open(&repo, ".");
        // check for error opening repo
        if (x != 0) {
            perror(git_error_last()->message);
            return 1;
        }
        // run a new iterator to print each branch name 
        if (!git_branch_iterator_new(&iter, repo, GIT_BRANCH_LOCAL)) {
            git_reference *ref;
            git_branch_t type;
            const char *branch_name = NULL;
            while (!git_branch_next(&ref, &type, iter)) {
                git_branch_name(&branch_name, ref);
                if (branch_name) {
                    if (git_branch_is_checked_out(ref)) {
                        printf("*\033[0;32m");
                    } else {
                        printf(" ");
                    }
                    printf(" %s\033[0m\n", branch_name);
                }
                git_reference_free(ref);
            }
        }
        git_branch_iterator_free(iter);
    }
    return 0;
} 

int checkout_git_branch(char **input, int word_count) {
   // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    }  else if (word_count != 3) {
        print_invalid_use_cmd("git checkout");
        return 1;
    } else {
        git_repository *repo = NULL;
        git_reference *ref;       

        int r = git_repository_open(&repo, ".");
        // check for error opening repo
        if (r != 0) {
            perror(git_error_last()->message);
            return 1;
        }

        // get branch name and validate
        int e = git_branch_lookup(&ref, repo, input[2], GIT_BRANCH_LOCAL);

        if (e != 0) {
            printf("Invalid branch name provided!\n");
            return 1; 
        } else {
            git_object *treeish = NULL;
            git_checkout_options opts= GIT_CHECKOUT_OPTIONS_INIT;
            opts.checkout_strategy = GIT_CHECKOUT_SAFE;
            
            // set treeish to an object specified by the branch name
            int x = git_revparse_single(&treeish, repo, input[2]);
            if (x != 0) {
                perror(git_error_last()->message);
                git_object_free(treeish);
                return 1;
            } 
           
            // checkout to the branch, DOES NOT MOVE HEAD
            int y = git_checkout_tree(repo, treeish, &opts);
            if (y != 0) {
                perror(git_error_last()->message);
                git_object_free(treeish); 
                return 1;
            } 

            char path_buf[128] = "refs/heads/"; 
            strcat(path_buf, input[2]);
 
            int z = git_repository_set_head(repo, path_buf);
            if (z != 0) {
                perror(git_error_last()->message);
                git_object_free(treeish); 
                return 1;
            } 
            git_object_free(treeish);
            
        }
    }   

    return 0;
}

// https://stackoverflow.com/questions/46757991/checkout-branch-with-libgit2
