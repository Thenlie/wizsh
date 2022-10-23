#include "git.utils.h"
#include "git.branch.h"
#include "../filesystem/fs.print.h"
#include "../printutility.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <git2.h>

int print_git_branches(git_repository *repo);
int create_git_branch(char *branch_name, git_repository *repo);
int delete_git_branch(char *branch_name, git_repository *repo);
int checkout_git_branch(char *branch_name, git_repository *repo);

int git_branch_command_handler(char** input, int word_count) {
    // ensure current directory is git enabled
    bool is_git = is_git_dir(".");
    if (!is_git) {
        printf("You are not in a git enabled directory!\n");
        return 1;
    }

    // open repository in current directory
    git_repository *repo = NULL;
    int error = git_repository_open(&repo, ".");
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // handle 'branch' command
    if (strcmp(input[1], "branch") == 0) {
        if (word_count == 2) {
            print_git_branches(repo);
        } else if (word_count == 4) {
            if ((strcmp(input[2], "-n") == 0) || strcmp(input[2], "--new") == 0) {
                create_git_branch(input[3], repo);
            } else if ((strcmp(input[2], "-d") == 0) || strcmp(input[2], "--delete") == 0) {
                delete_git_branch(input[3], repo);
            } else {
                print_invalid_use_cmd("git branch");
            } 
        } else {
            print_invalid_use_cmd("git branch");
        } 
    // handle 'checkout' command
    } else if (strcmp(input[1], "checkout") == 0) {
        if (word_count == 3) {
            checkout_git_branch(input[2], repo);
        } else if (word_count == 4) {
            if ((strcmp(input[2], "-b") == 0) || strcmp(input[2], "--branch") == 0) {
                create_git_branch(input[3], repo);
                checkout_git_branch(input[3], repo);
            } else {
                print_invalid_use_cmd("git checkout");
            }  
        } else {
            print_invalid_use_cmd("git checkout");
        }   
    } 

    git_repository_free(repo);
    return 0;
}

int print_git_branches(git_repository *repo) {
    git_branch_iterator *iter;

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
    return 0;
} 

void create_branch_cleanup(git_reference *ref, git_commit *commit) {
    // free memory from 'git branch --new' command
    git_reference_free(ref);
    git_commit_free(commit);
    return;
}

int create_git_branch(char *branch_name, git_repository *repo) {
    git_reference *ref;       
    git_commit *commit;
    FILE *f;
    git_oid oid;
    int error;

    // get most recent commit oid from the git logs
    f = fopen("./.git/logs/HEAD", "r");
    if (f == NULL) {
        perror("Unable to read git file!");
        fclose(f);
        return 1;
    }

    // parse log file for last line, second SHA
    char tmp[1024];
    while (!feof(f)) {
        fgets(tmp, 1024, f);
    }
    char recent_oid[42];
    memcpy(recent_oid, &tmp[41], 40);
    recent_oid[41] = '\0';

    git_oid_fromstr(&oid, recent_oid);

    // get commit from oid
    error = git_commit_lookup(&commit, repo, &oid);
    if (error != 0) {
        perror(git_error_last()->message);
        fclose(f);
        return 1;
    }
    fclose(f);
    
    // create new branch
    error = git_branch_create(&ref, repo, branch_name, commit, 0);
    if (error != 0) {
        perror(git_error_last()->message);
        create_branch_cleanup(NULL, commit);
        return 1;
    }

    create_branch_cleanup(ref, commit);
    return 0;
}

void delete_branch_cleanup(git_reference *ref) {
    // free memory from 'git branch --delete' command
    git_reference_free(ref);
    return;
}

int delete_git_branch(char *branch_name, git_repository *repo) {
    git_reference *ref;
    int error;

    // find local branch based on command input
    error = git_branch_lookup(&ref, repo, branch_name, GIT_BRANCH_LOCAL);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    } 

    // delete local branch
    error = git_branch_delete(ref);
    if (error != 0) {
        perror(git_error_last()->message);
        delete_branch_cleanup(ref);
        return 1;
    } 

    delete_branch_cleanup(ref);
    return 0;
}

void checkout_cleanup(git_reference *ref, git_object *obj) {
    // cleanup memory from 'git checkout' command
    git_reference_free(ref);
    git_object_free(obj);
    return;
}

int checkout_git_branch(char *branch_name, git_repository *repo) {
    git_reference *ref;       
    git_object *treeish = NULL;
    git_checkout_options opts= GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    int error;

    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // get branch name and validate
    error = git_branch_lookup(&ref, repo, branch_name, GIT_BRANCH_LOCAL);
    if (error != 0) {
        printf("Invalid branch name provided!\n");
        return 1; 
    } 
    
    // set treeish to an object specified by the branch name
    error = git_revparse_single(&treeish, repo, branch_name);
    if (error != 0) {
        perror(git_error_last()->message);
        checkout_cleanup(ref, treeish);
        return 1;
    } 
    
    // checkout to the branch, DOES NOT MOVE HEAD
    error = git_checkout_tree(repo, treeish, &opts);
    if (error != 0) {
        perror(git_error_last()->message);
        checkout_cleanup(ref, treeish);
        return 1;
    } 

    char path_buf[128] = "refs/heads/"; 
    strcat(path_buf, branch_name);
    // set HEAD to newly checked out branch
    error = git_repository_set_head(repo, path_buf);
    if (error != 0) {
        perror(git_error_last()->message);
        checkout_cleanup(ref, treeish);
        return 1;
    } 

    checkout_cleanup(ref, treeish);
    return 0;
    // https://stackoverflow.com/questions/46757991/checkout-branch-with-libgit2
}