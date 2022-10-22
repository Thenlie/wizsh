#include "git.remote.h"
#include "../printutility.h"
#include <git2.h>
#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

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
        } else if (strcmp(input[1], "push") == 0 && word_count == 4) {
            push_git_remote(input, word_count);
        } else if (strcmp(input[1], "push") == 0) {
            print_invalid_use_cmd("git push");
        } else if (strcmp(input[1], "fetch") == 0 && word_count == 4) {
            fetch_git_remote(input, word_count);
        } else if (strcmp(input[1], "fetch") == 0) {
            print_invalid_use_cmd("git push");
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

// TODO: Check for invalid password, exit upon 2 invalid attempts
int credentials_cb(git_credential **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload) {
    
    // printf("%s\n", git_error_last()->message);
    
    const char* SSH_PUB_PATH = "/.ssh/id_ed25519.pub";
    const char* SSH_PRIVATE_PATH = "/.ssh/id_ed25519";

    const char* user = "git";
    char tmp_pass[256], pbk[256], prk[256];

    // get users home directory
    const char *homeDir = getenv("HOME");
    if (!homeDir) {
        struct passwd* pwd = getpwuid(getuid());
        if (pwd)
           homeDir = pwd->pw_dir;
    }

    // create public key path
    strcpy(pbk, homeDir);
    strcat(pbk, SSH_PUB_PATH);

    // create private key path
    strcpy(prk, homeDir);
    strcat(prk, SSH_PRIVATE_PATH);

    const char *public_key = pbk;
    const char *private_key = prk;

    printf("Enter password: ");
    if (fgets(tmp_pass, 256, stdin) == NULL) {
        return GIT_EUSER;
    }
    size_t len = strlen(tmp_pass);
    if (len > 0 && tmp_pass[len - 1] == '\n') {
        tmp_pass[--len] = '\0';
    }

    const char *pass = tmp_pass;

    return git_credential_ssh_key_new(out, user, public_key, private_key, pass);
}

void push_remotes_cleanup(git_repository *repo, git_remote *remote) {
    // free memory associated with 'git push' command
    git_remote_free(remote);
    git_repository_free(repo);
    return;
}

int push_git_remote(char **input, int word_count) {
    git_remote *remote = NULL;
    git_repository *repo = NULL;
    git_push_options opts;
    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;    
    int error;

    char tmp[128] = {"refs/heads/"};
    strcat(tmp, input[3]);
    char *refspec = tmp;
    const git_strarray refspecs = { &refspec, 1 };

    callbacks.credentials = credentials_cb;
    // https://libgit2.org/docs/guides/authentication/

    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // find remote
    error = git_remote_lookup(&remote, repo, input[2]);
    if (error != 0) {
        perror(git_error_last()->message);
        push_remotes_cleanup(repo, NULL);
        return 1;
    }

    // connect to remote for push
    error = git_remote_connect(remote, GIT_DIRECTION_PUSH, &callbacks, NULL, NULL);
    if (error != 0) {
        perror(git_error_last()->message);
        push_remotes_cleanup(repo, NULL);
        return 1;
    }

    // initialize default push options
    error = git_push_options_init(&opts, GIT_PUSH_OPTIONS_VERSION);
    if (error != 0) {
        perror(git_error_last()->message);
        push_remotes_cleanup(repo, remote);
        return 1;
    }

    // push!
    error = git_remote_push(remote, &refspecs, &opts);
    if (error != 0) {
        perror(git_error_last()->message);
        push_remotes_cleanup(repo, remote);
        return 1;
    }

    push_remotes_cleanup(repo, remote);
    return 0;
    // https://libgit2.org/libgit2/ex/HEAD/push.html#git_remote_push-3
}

void fetch_remotes_cleanup(git_repository *repo, git_remote *remote) {
    // free memory associated with 'git fetch' command
    git_remote_free(remote);
    git_repository_free(repo);
    return;
}

int fetch_git_remote(char **input, int word_count) {
    git_remote *remote;
    git_repository *repo;
    git_fetch_options f_opts = GIT_FETCH_OPTIONS_INIT;
    const git_indexer_progress *stats;
    int error;

    char tmp[128] = {"refs/heads/"};
    strcat(tmp, input[3]);
    char *refspec = tmp;
    const git_strarray refspecs = { &refspec, 1 };

    f_opts.callbacks.credentials = credentials_cb;

    error = git_repository_open(&repo, ".");
    // check for error opening repo
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // find remote
    error = git_remote_lookup(&remote, repo, input[2]);
    if (error != 0) {
        perror(git_error_last()->message);
        fetch_remotes_cleanup(repo, NULL);
        return 1;
    }

    // fetch!
    error = git_remote_fetch(remote, &refspecs, &f_opts, NULL); 
    if (error != 0) {
        perror(git_error_last()->message);
        fetch_remotes_cleanup(repo, remote);
        return 1;
    }

    // print content of fetch

    stats = git_remote_stats(remote);
    if (stats->local_objects > 0) {
        printf("\rReceived %u/%u objects in %lu bytes (used %u local objects)\n",
           stats->indexed_objects, stats->total_objects, stats->received_bytes, stats->local_objects);
    } else{
        printf("\rReceived %u/%u objects in %lu bytes\n",
            stats->indexed_objects, stats->total_objects, stats->received_bytes);
    }

    // begin merging of fetch
    git_oid head_oid;
    git_oid fetchhead_oid;
    git_annotated_commit *commit;
    git_merge_options m_opts = GIT_MERGE_OPTIONS_INIT;
    git_checkout_options c_opts = GIT_CHECKOUT_OPTIONS_INIT;

    // get oid for HEAD and FETCH_HEAD
    git_reference_name_to_id(&head_oid, repo, "HEAD");
    git_reference_name_to_id(&fetchhead_oid, repo, "FETCH_HEAD");
    
    // check to see if they are equal
    int equal = git_oid_equal(&head_oid, &fetchhead_oid);
    printf("Merge Analysis: %i\n", equal);
    
    // if equal, no need to merge
    if (equal) {
        fetch_remotes_cleanup(repo, remote);
        return 0;
    }

    // get commit from fetch head for merge
    error = git_annotated_commit_lookup(&commit, repo, &fetchhead_oid);
    if (error != 0) {
        perror(git_error_last()->message);
        fetch_remotes_cleanup(repo, remote);
        return 1;
    }

    // merge!
    error = git_merge(repo, (const git_annotated_commit **)&commit, 1, &m_opts, &c_opts);
    if (error != 0) {
        perror(git_error_last()->message);
        fetch_remotes_cleanup(repo, remote);
        return 1;
    }

    fetch_remotes_cleanup(repo, remote);

    return 0;
    // https://stackoverflow.com/questions/57791388/how-to-write-a-proper-git-pull-with-libgit2
}
