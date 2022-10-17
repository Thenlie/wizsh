#include "git.h"
#include "../handlers/filesystem.h"
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
                printf("  (use \"git restore <file>\" to unstage)\n\033[0;32m");
                first_entry = false;
            }

            // check for rename flag on index to determine file paths to print
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

            // check for rename flag on index to determine file paths to print
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
    }  else if (word_count == 3) {
        create_git_branch(input, word_count);
        return 0;
    } else if (word_count == 4 && (strcmp(input[2], "-d") == 0 || strcmp(input[2], "--delete") == 0)) {
        delete_git_branch(input, word_count);
    } else if (word_count == 2) {
        git_repository *repo = NULL;
        git_branch_iterator *iter;
        
        int r = git_repository_open(&repo, ".");
        // check for error opening repo
        if (r != 0) {
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
    } else {
        print_invalid_use_cmd("git branch");
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
                git_reference_free(ref);
                git_repository_free(repo);
                return 1;
            } 
           
            // checkout to the branch, DOES NOT MOVE HEAD
            int y = git_checkout_tree(repo, treeish, &opts);
            if (y != 0) {
                perror(git_error_last()->message);
                git_object_free(treeish); 
                git_reference_free(ref);
                git_repository_free(repo);
                return 1;
            } 

            char path_buf[128] = "refs/heads/"; 
            strcat(path_buf, input[2]);
            // set HEAD to newly checked out branch
            int z = git_repository_set_head(repo, path_buf);
            if (z != 0) {
                perror(git_error_last()->message);
                git_object_free(treeish); 
                git_reference_free(ref);
                git_repository_free(repo);
                return 1;
            } 
            git_object_free(treeish);
            git_reference_free(ref);
            git_repository_free(repo);
        }
    }   
    // https://stackoverflow.com/questions/46757991/checkout-branch-with-libgit2
    return 0;
}

int create_git_branch(char **input, int word_count) {
    git_repository *repo = NULL;
    git_reference *ref;       
    git_commit *commit;

    int r = git_repository_open(&repo, ".");
    // check for error opening repo
    if (r != 0) {
        perror(git_error_last()->message);
        git_repository_free(repo);
        return 1;
    } 

    // get most recent commit oid
    FILE *f;
    git_oid oid;
    f = fopen("./.git/logs/HEAD", "r");
    if (f == NULL) {
        perror("Unable to read git file!");
        fclose(f);
        return 1;
    }

    char tmp[1024];
    while (!feof(f)) {
        fgets(tmp, 1024, f);
    }
    char recent_oid[42];
    memcpy(recent_oid, &tmp[41], 40);
    recent_oid[41] = '\0';

    git_oid_fromstr(&oid, recent_oid);

    // get commit from oid
    int l = git_commit_lookup(&commit, repo, &oid);
    if (l != 0) {
        perror(git_error_last()->message);
        fclose(f);
        git_commit_free(commit);
        git_repository_free(repo);
        return 1;
    }
    fclose(f);
    
    // create new branch
    int b = git_branch_create(&ref, repo, input[2], commit, 0);
    if (b!= 0) {
        perror(git_error_last()->message);
        git_commit_free(commit);
        git_repository_free(repo);
        return 1;
    }
    git_reference_free(ref);
    git_repository_free(repo);
    git_commit_free(commit);
    return 0;
}

int delete_git_branch(char **input, int word_count) {
    // lookup git branch with name of input[3]
    // use reference to delete branch
    git_reference *ref;
    git_repository *repo;

    int r = git_repository_open(&repo, ".");
    // check for error opening repo
    if (r != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    int l = git_branch_lookup(&ref, repo, input[3], GIT_BRANCH_LOCAL);
    if (l != 0) {
        perror(git_error_last()->message);
        return 1;
    } 

    int d = git_branch_delete(ref);
    if (d != 0) {
        perror(git_error_last()->message);
        return 1;
    } 

    git_reference_free(ref);
    git_repository_free(repo);
    return 0;
}

int git_add(char **input, int word_count) {
    if (word_count == 3) {
        git_index *index;
        git_repository *repo;

        int r = git_repository_open(&repo, ".");
        // check for error opening repo
        if (r != 0) {
            perror(git_error_last()->message);
            return 1;
        }
        // get the current index    
        int i = git_repository_index(&index, repo);
        if (i != 0) {
            perror(git_error_last()->message);
            return 1;
        }

        int a;
        // add all files to index
        if (strcmp(input[2], ".") == 0) {
            git_index_add_option_t opt = GIT_INDEX_ADD_CHECK_PATHSPEC;
            a = git_index_add_all(index, NULL, opt, NULL, NULL);
        // add a specified file to the index
        } else {
            a = git_index_add_bypath(index, input[2]);
        }
        if (a != 0) {
            perror(git_error_last()->message);
            return 1;
        }  

        // write index to disk
        int w = git_index_write(index);
        if (w != 0) {
            perror(git_error_last()->message);
            return 1;
        }  

        git_index_free(index);
        git_repository_free(repo);
    } else {
        print_invalid_use_cmd("git add");
        return 1;
    }
    return 0;
}

int commit_cleanup(git_repository *repo, git_index *index, git_signature *sig, git_tree *tree, git_object *obj, git_reference *ref) {

    git_repository_free(repo);
    git_index_free(index);
    git_signature_free(sig);
    git_tree_free(tree);
    git_object_free(obj);
    git_reference_free(ref);

    return 0;
}

int create_git_commit(char **input, int word_count) {
    if (word_count >= 4 && strcmp(input[2], "-m") == 0) {
        git_oid commit_oid, tree_oid;
        git_tree *tree;
        git_index *index;
        git_object *parent = NULL;
        git_reference *ref = NULL;
        git_signature *signature;
        git_repository *repo;
        char commit_msg[256];
        int error;

        // parse commit message
        int count = 0;
        for (int i = 3; i < word_count; i++) {
            size_t len = strlen(input[i]);
            for (int j = 0; j < len; j++) {
                commit_msg[count] = input[i][j];
                count++;
            }
            if (i + 1 == word_count) {
                commit_msg[count] = '\0';
            } else {
                commit_msg[count] = ' ';
                count++;
            }
        }

        error = git_repository_open(&repo, ".");
        // check for error opening repo
        if (error != 0) {
            commit_cleanup(repo, NULL, NULL, NULL, NULL, NULL);
            perror(git_error_last()->message);
            return 1;
        }       

        // find object pointed to by HEAD and set it to 'parent'
        error = git_revparse_ext(&parent, &ref, repo, "HEAD"); 
        if (error == GIT_ENOTFOUND) {
            printf("HEAD not found. Creating first commit...\n");
            error = 0;
        } else if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, NULL, NULL, NULL, parent, ref);
            return 1;
        }       

        // find index (staged files)
        error = git_repository_index(&index, repo);
        if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, index, NULL, NULL, parent, ref);
            return 1;
        }       

        // create a tree with the index and get oid
        error = git_index_write_tree(&tree_oid, index);
        if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, index, NULL, NULL, parent, ref);
            return 1;
        }       

        // write index to disk
        error = git_index_write(index);
        if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, index, NULL, NULL, parent, ref);
            return 1;
        }       

        // get new tree object using oid
        error = git_tree_lookup(&tree, repo, &tree_oid);
        if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, index, NULL, tree, parent, ref);
            return 1;
        }       

        // create a signature with the default user
        error = git_signature_default(&signature, repo);
        if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, index, signature, tree, parent, ref);
            return 1;
        }       

        error = git_commit_create_v(&commit_oid, repo, "HEAD", signature, signature, NULL, commit_msg, tree, parent ? 1 : 0, parent); 
        if (error != 0) {
            perror(git_error_last()->message);
            commit_cleanup(repo, index, signature, tree, parent, ref);
            return 1;
        }       

        commit_cleanup(repo, index, signature, tree, parent, ref);
        return 0;
    } else {
        print_invalid_use_cmd("git commit");
        return 1;
    }
    // https://libgit2.org/libgit2/ex/HEAD/commit.html#git_commit_create_v-1
}

int init_git_repo(char **input, int word_count) {
    git_repository *repo = NULL;
    int error;

    if (word_count == 2) {
        error = git_repository_init(&repo, ".", 0);
    } else if (word_count == 3) {
        error = git_repository_init(&repo, input[2], 0);
    } else {
        print_invalid_use_cmd("git init");
        return 1;
    }
    
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    } else {
        if (word_count == 2) {
            printf("Initialized empty Git repository in the current directory.\n");
        } else {
            printf("Initialized empty Git repository in %s.\n", input[2]);
        }
    }       

    git_repository_free(repo);
    return 0;
}

int git_remove_from_index(char **input, int word_count) {
    if (word_count == 3) {
        git_repository *repo;
        git_index *index;
        int error;
        
        // open repository
        error = git_repository_open(&repo, ".");
        if (error != 0) {
            commit_cleanup(repo, NULL, NULL, NULL, NULL, NULL);
            perror(git_error_last()->message);
            return 1;
        }       
        // get repo index
        error = git_repository_index(&index, repo);
        if (error != 0) {
            perror(git_error_last()->message);
            git_repository_free(repo);
            return 1;
        }       
        // remove all staged files
        if (strcmp(input[2], "-a") == 0 || strcmp(input[2], "--all") == 0) {
            error = git_index_remove_all(index, NULL, NULL, NULL);
        // remove file via specified path
        } else {
            error = git_index_remove_bypath(index, input[2]);
        }
        if (error != 0) {
            perror(git_error_last()->message);
            git_index_free(index);
            git_repository_free(repo);
            return 1;
        }
        // write index to disk
        error = git_index_write(index); 
        if (error != 0) {
            perror(git_error_last()->message);
            git_repository_free(repo);
            git_index_free(index);
            return 1;
        }       

        git_repository_free(repo);
        git_index_free(index);
    } else {
        print_invalid_use_cmd("git restore");
        return 1;
    }
    return 0;
}