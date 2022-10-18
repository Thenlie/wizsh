#include "git.remote.h"
#include "git2.h"
#include "../handlers/printutility.h"
#include <string.h>
#include <stdio.h>

int git_remote_command_handler(char**input, int word_count) {
    if (word_count == 2) {
        print_git_remotes(input, 0);
    } else if (word_count == 3 && (strcmp(input[2], "-v") == 0 || strcmp(input[2], "--verbose") == 0)) {
        print_git_remotes(input, 1);
    } else if (word_count >= 4) {
        if (strcmp(input[2], "add") == 0 && word_count == 5) {
            add_git_remote(input, word_count);
        } else if (strcmp(input[2], "remove") == 0 && word_count == 4) {
            remove_git_remote(input, word_count);
        } else if (strcmp(input[2], "rename") == 0 && word_count == 5) {
            rename_git_remote(input, word_count);
        } 
    } else {
        print_invalid_use_cmd("git remote");
        return 1;
    }
    return 0;  
}

int print_remotes_cleanup(git_repository *repo, git_remote *remote, git_strarray *remotes) {
    // free memory from 'git remote' command
    git_repository_free(repo);
    git_remote_free(remote);
    git_strarray_dispose(remotes);

    return 0; 
}

int print_git_remotes(char **input, int verbose) {
    git_strarray remotes = {0};
    git_remote *remote = NULL;
    git_repository *repo = NULL;    
    int error;

    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    error = git_remote_list(&remotes, repo);
    if (error != 0) {
        perror(git_error_last()->message);
        print_remotes_cleanup(repo, NULL, NULL);
        return 1;
    }

    // loop through remotes and print them
    for (int i = 0; i < (int)remotes.count; i++) {
        char *name = remotes.strings[i];
        if (!verbose) {
            printf("%s\n", name);
        } else {
            error = git_remote_lookup(&remote, repo, name);
            if (error != 0) {
                perror(git_error_last()->message);
                print_remotes_cleanup(repo, NULL, &remotes);
                return 1;
            }
            const char *fetch = git_remote_url(remote);
            if (fetch) {
                printf("%s %s (fetch)\n", name, fetch);
            }
            const char *push = git_remote_pushurl(remote);
            // set push to fetch url if not push is explicitly set
            push = push ? push : fetch;
            if (push) {
                printf("%s %s (push)\n", name, push);
            }
        }
    } 

    print_remotes_cleanup(repo, remote, &remotes);
    return 0;
}

int add_git_remote(char **input, int word_count) {
    git_remote *remote = NULL;
    git_repository *repo;
    int error;

    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // create a new remote for the current repo
    error = git_remote_create(&remote, repo, input[3], input[4]);
    if (error != 0) {
        perror(git_error_last()->message);
        git_repository_free(repo);
        return 1;
    }

    git_repository_free(repo);
    git_remote_free(remote);
    return 0;
}

int remove_git_remote(char **input, int word_count) {
    git_repository *repo = NULL;
    int error;

    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // delete remote from current repo
    error = git_remote_delete(repo ,input[3]);
    if (error != 0) {
        perror(git_error_last()->message);
        git_repository_free(repo);
        return 1;
    }

    git_repository_free(repo);
    return 0;    
}

int rename_git_remote(char **input, int word_count) {
    git_repository *repo;
    git_strarray problems = {0};
    int error;

    char *old = input[3];
    char *new = input[4];
    
    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }
    
    error = git_remote_rename(&problems, repo, old, new);
    // check for problem with rename, print if they exist
    if (error) {
        for (int i = 0; i < (int)problems.count; i++) {
            printf("%s\n", problems.strings[i]);
        }

        git_strarray_dispose(&problems);
        git_repository_free(repo);
        return 1;
    }

    git_strarray_dispose(&problems);
    git_repository_free(repo);
    return 0;
}