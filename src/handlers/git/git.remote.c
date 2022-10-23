#include "git.remote.h"
#include "../printutility.h"
#include "git.utils.h"
#include <git2.h>
#include <string.h>
#include <stdio.h>

int print_git_remotes(char **input, int verbose, git_repository *repo);
int merge_git_commit_to_head(git_repository *repo, git_remote *remote, const git_oid *oid);
int add_git_remote(char **input, git_repository *repo);
int remove_git_remote(char **input, git_repository *repo);
int rename_git_remote(char **input, git_repository *repo);
int pull_git_remote(char **input, git_repository *repo);
int push_git_remote(char **input, git_repository *repo);

int git_remote_command_handler(char **input, int word_count) {
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

    // handle command
    if (word_count == 2) {
        print_git_remotes(input, 0, repo);
    } else if (word_count == 3 && (strcmp(input[2], "-v") == 0 || strcmp(input[2], "--verbose") == 0)) {
        print_git_remotes(input, 1, repo);
    } else if (word_count >= 4) {
        if (strcmp(input[2], "add") == 0 && word_count == 5) {
            add_git_remote(input, repo);
        } else if (strcmp(input[2], "remove") == 0 && word_count == 4) {
            remove_git_remote(input, repo);
        } else if (strcmp(input[2], "rename") == 0 && word_count == 5) {
            rename_git_remote(input, repo);
        } else if (strcmp(input[1], "push") == 0 && word_count == 4) {
            push_git_remote(input, repo);
        } else if (strcmp(input[1], "push") == 0) {
            print_invalid_use_cmd("git push");
        } else if (strcmp(input[1], "pull") == 0 && word_count == 4) {
            pull_git_remote(input, repo);
        } else if (strcmp(input[1], "pull") == 0) {
            print_invalid_use_cmd("git pull");
        }
    } else {
        print_invalid_use_cmd("git remote");
        git_repository_free(repo);
        return 1;
    }

    git_repository_free(repo);
    return 0;  
}

void print_remotes_cleanup(git_remote *remote, git_strarray *remotes) {
    // free memory from 'git remote' command
    git_remote_free(remote);
    git_strarray_dispose(remotes);
    return;
}

int print_git_remotes(char **input, int verbose, git_repository *repo) {
    git_strarray remotes = {0};
    git_remote *remote = NULL;

    int error = git_remote_list(&remotes, repo);
    if (error != 0) {
        perror(git_error_last()->message);
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
                print_remotes_cleanup(NULL, &remotes);
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

    print_remotes_cleanup(remote, &remotes);
    return 0;
}

int add_git_remote(char **input, git_repository *repo) {
    git_remote *remote = NULL;

    // create a new remote for the current repo
    int error = git_remote_create(&remote, repo, input[3], input[4]);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    git_remote_free(remote);
    return 0;
}

int remove_git_remote(char **input, git_repository *repo) {
    // delete remote from current repo
    int error = git_remote_delete(repo ,input[3]);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    return 0;    
}

int rename_git_remote(char **input, git_repository *repo) {
    git_strarray problems = {0};
    char *old = input[3];
    char *new = input[4];
    
    int error = git_remote_rename(&problems, repo, old, new);
    // check for problem with rename, print if they exist
    if (error) {
        for (int i = 0; i < (int)problems.count; i++) {
            printf("%s\n", problems.strings[i]);
        }

        git_strarray_dispose(&problems);
        return 1;
    }

    git_strarray_dispose(&problems);
    return 0;
}

int push_git_remote(char **input, git_repository *repo) {
    git_remote *remote = NULL;
    git_push_options opts;
    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;    
    int error;

    char tmp[128] = {"refs/heads/"};
    strcat(tmp, input[3]);
    char *refspec = tmp;
    const git_strarray refspecs = { &refspec, 1 };

    callbacks.credentials = credentials_cb;
    // https://libgit2.org/docs/guides/authentication/

    // find remote
    error = git_remote_lookup(&remote, repo, input[2]);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // connect to remote for push
    error = git_remote_connect(remote, GIT_DIRECTION_PUSH, &callbacks, NULL, NULL);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // initialize default push options
    error = git_push_options_init(&opts, GIT_PUSH_OPTIONS_VERSION);
    if (error != 0) {
        perror(git_error_last()->message);
        git_remote_free(remote);
        return 1;
    }

    // push!
    error = git_remote_push(remote, &refspecs, &opts);
    if (error != 0) {
        perror(git_error_last()->message);
        git_remote_free(remote);
        return 1;
    }

    git_remote_free(remote);
    return 0;
    // https://libgit2.org/libgit2/ex/HEAD/push.html#git_remote_push-3
}

int pull_git_remote(char **input, git_repository *repo) {
    git_remote *remote;
    git_oid head_oid, fetchhead_oid;
    git_fetch_options f_opts = GIT_FETCH_OPTIONS_INIT;
    const git_indexer_progress *stats;
    int error;

    char tmp[128] = {"refs/heads/"};
    strcat(tmp, input[3]);
    char *refspec = tmp;
    const git_strarray refspecs = { &refspec, 1 };

    f_opts.callbacks.credentials = credentials_cb;

    // find remote
    error = git_remote_lookup(&remote, repo, input[2]);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // fetch!
    error = git_remote_fetch(remote, &refspecs, &f_opts, NULL); 
    if (error != 0) {
        perror(git_error_last()->message);
        git_remote_free(remote);
        return 1;
    }

    // print byte info of fetch
    stats = git_remote_stats(remote);
    if (stats->local_objects > 0) {
        printf("\rReceived %u/%u objects in %lu bytes (used %u local objects)\n",
           stats->indexed_objects, stats->total_objects, stats->received_bytes, stats->local_objects);
    } else{
        printf("\rReceived %u/%u objects in %lu bytes\n",
            stats->indexed_objects, stats->total_objects, stats->received_bytes);
    }
    
    // get oid for HEAD and FETCH_HEAD
    git_reference_name_to_id(&head_oid, repo, "HEAD");
    git_reference_name_to_id(&fetchhead_oid, repo, "FETCH_HEAD");
    
    // check to see if they are equal
    int equal = git_oid_equal(&head_oid, &fetchhead_oid);
    printf("Merge Analysis: %i\n", equal);
    
    // if equal, no need to merge
    if (!equal) {
        merge_git_commit_to_head(repo, remote, &fetchhead_oid);
    }

    git_remote_free(remote);
    return 0;
    // https://stackoverflow.com/questions/57791388/how-to-write-a-proper-git-pull-with-libgit2
}

int merge_git_commit_to_head(git_repository *repo, git_remote *remote, const git_oid *oid) {
    git_annotated_commit *commit;
    git_merge_options m_opts = GIT_MERGE_OPTIONS_INIT;
    git_checkout_options c_opts = GIT_CHECKOUT_OPTIONS_INIT;
    int error;

    // get annotated commit from commit oid
    error = git_annotated_commit_lookup(&commit, repo, oid);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // merge!
    error = git_merge(repo, (const git_annotated_commit **)&commit, 1, &m_opts, &c_opts);
    if (error != 0) {
        perror(git_error_last()->message);
        git_annotated_commit_free(commit);
        return 1;
    }

    git_annotated_commit_free(commit);
    git_repository_state_cleanup(repo);
    return 0;
}