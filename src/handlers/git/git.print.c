#include "git.utils.h"
#include "git.print.h"
#include "../filesystem/fs.print.h"
#include "../printutility.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <git2.h>

int print_git_log(void);
int print_git_status(void);

int git_print_command_handler(char **input, int word_count) {
    // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    }

    if (word_count == 2) {
        if (strcmp(input[1], "log") == 0) {
            print_git_log();
        } else if (strcmp(input[1], "status") == 0) {
            print_git_status();
        }
    }
    
    return 0;
}

void print_current_branch(void) {
    FILE *f;
    f = fopen("./.git/HEAD", "r");
    if (f == NULL) {
        perror("Unable to read git file!");
        fclose(f);
        return;
    }

    // remove '/refs/heads' from path and print branch name
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

int print_git_log(void) {
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

struct status_opts {
    // git status options
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

int print_git_status(void) {
    // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    } else {
        git_status_list *status;
        git_repository *repo = NULL;
        int error;

        // initialize status options
        struct status_opts o = { GIT_STATUS_OPTIONS_INIT, "." };
        // set basic status options
        o.statusopt.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX | GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
        
        // open repo in current directory
        error = git_repository_open(&repo, ".");
        // check for error opening repo
        if (error != 0) {
            perror(git_error_last()->message);
            return 1;
        }

        // ensure you are not in a bare repo (one that cannot accept commits)
        if (git_repository_is_bare(repo)) {
            printf("Cannot report status on bare repository! %s", git_repository_path(repo));
            git_repository_free(repo);
            return 1;
        }

        // initialize a new status list with the address of status list buffer, the repo and status options
        error = git_status_list_new(&status, repo, &o.statusopt);
        if (error != 0) {
            perror(git_error_last()->message);
            git_repository_free(repo);
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
        printf("\n");

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
                printf("Changes staged for commit:\n");
                printf("  (use \"git commit -m\" to commit these files)\n");
                printf("  (use \"git restore <file>\" to un-stage)\n\033[0;32m");
                first_entry = false;
            }

            // check for flag on index to determine file paths to print
            switch (istatus) {
                case 'R':
                    printf("      Renamed:     %s %s %s\n", a, b, extra);
                    break;
                case 'A':
                    printf("      New:         %s %s\n", a, extra);
                    break;
                case 'M':
                    printf("      Modified:    %s %s\n", a, extra);
                    break;
                case 'D': 
                    printf("      Deleted:     %s %s\n", a, extra);
                    break;
                case 'T':
                    printf("      Typechanged: %s %s\n", a, extra);
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

            // check for flag on index to determine file paths to print
            switch (wstatus) {
                case 'R':
                    printf("      Renamed:     %s %s %s\n", a, c, extra);
                    break;
                case 'M':
                    printf("      Modified:    %s %s\n", a, extra);
                    break;
                case 'D': 
                    printf("      Deleted:     %s %s\n", a, extra);
                    break;
                case 'T':
                    printf("      Typechanged: %s %s\n", a, extra);
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
        git_repository_free(repo);
        git_status_list_free(status);
        return 0;
        // https://libgit2.org/libgit2/ex/HEAD/status.html#git_status_foreach-6
    }
}

