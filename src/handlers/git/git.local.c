#include "git.print.h"
#include "git.utils.h"
#include "../filesystem/fs.print.h"
#include "../printutility.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <git2.h>

int git_add_to_index(char **input, git_repository *repo);
int create_git_commit(char **input, int word_count, git_repository *repo);
int git_remove_from_index(char **input, git_repository *repo);

int git_local_command_handler(char **input, int word_count) {
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
    if (strcmp(input[1], "add") == 0) {
        word_count == 3 ? git_add_to_index(input, repo) : print_invalid_use_cmd("git add");
    } else if (strcmp(input[1], "commit") == 0) {
        word_count >= 4 ? create_git_commit(input, word_count, repo) : print_invalid_use_cmd("git commit");
    } else if (strcmp(input[1], "remove") == 0) {
        word_count == 3 ? git_remove_from_index(input, repo) : print_invalid_use_cmd("git restore");
    }

    return 0;
}

int clone_git_repo(char **input, int word_count) {
    git_repository *repo = NULL;
    const char *url = input[2];
    int error;

    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;

    opts.fetch_opts.callbacks.credentials = credentials_cb;


    // clone repo to current dir or specified dir
    if (word_count == 3) {
        const char *path = ".";
        error = git_clone(&repo, url, path, &opts);
    } else if (word_count == 4) {
        const char *path = input[3];
        error = git_clone(&repo, url, path, &opts);
    } else {
        print_invalid_use_cmd("git clone");
        return 1;
    }

    // check for error in clone
    if (error != 0) {
        perror(git_error_last()->message);
    }

    git_repository_free(repo);
    return 0;
}

int init_git_repo(char **input, int word_count) {
    git_repository *repo = NULL;
    int error;

    // initialize repo in current dir
    if (word_count == 2) {
        error = git_repository_init(&repo, ".", 0);
    // initialize repo in specified dir
    } else if (word_count == 3) {
        error = git_repository_init(&repo, input[2], 0);
    } else {
        print_invalid_use_cmd("git init");
        return 1;
    }

    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    if (word_count == 2) {
        printf("Initialized empty Git repository in the current directory.\n");
    } else {
        printf("Initialized empty Git repository in %s.\n", input[2]);
    }       

    git_repository_free(repo);
    return 0;
}

void cleanup_git_add(git_index *index) {
    // free memory from 'git add' command
    git_index_free(index);
    return;
}

int git_add_to_index(char **input, git_repository *repo) {
    git_index *index;
    int error;

    // get the current index    
    error = git_repository_index(&index, repo);
    if (error != 0) {
        perror(git_error_last()->message);
        return 1;
    }

    // add all files to index
    if ((strcmp(input[2], "-a") == 0) || strcmp(input[2], "--all") == 0) {
        git_index_add_option_t opt = GIT_INDEX_ADD_CHECK_PATHSPEC;
        error = git_index_add_all(index, NULL, opt, NULL, NULL);
    // add a specified file to the index
    } else {
        error = git_index_add_bypath(index, input[2]);
    }
    if (error != 0) {
        perror(git_error_last()->message);
        cleanup_git_add(index);
        return 1;
    }  

    // write index to disk
    error = git_index_write(index);
    if (error != 0) {
        perror(git_error_last()->message);
        cleanup_git_add(index);
        return 1;
    }  

    cleanup_git_add(index);
    return 0;
}

void cleanup_git_commit(git_index *index, git_signature *sig, git_tree *tree, git_object *obj, git_reference *ref) {
    // free memory from 'git commit -m' command
    git_index_free(index);
    git_signature_free(sig);
    git_tree_free(tree);
    git_object_free(obj);
    git_reference_free(ref);
    return;
}

int create_git_commit(char **input, int word_count, git_repository *repo) {
    if (strcmp(input[2], "-m") == 0 || strcmp(input[2], "--message") == 0) {
        git_oid commit_oid, tree_oid;
        git_tree *tree;
        git_index *index;
        git_object *parent = NULL;
        git_reference *ref = NULL;
        git_signature *signature;
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

        // find object pointed to by HEAD and set it to 'parent'
        error = git_revparse_ext(&parent, &ref, repo, "HEAD"); 
        if (error == GIT_ENOTFOUND) {
            printf("HEAD not found. Creating first commit...\n");
            error = 0;
        } else if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(NULL, NULL, NULL, parent, ref);
            return 1;
        }       

        // find index (staged files)
        error = git_repository_index(&index, repo);
        if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(index, NULL, NULL, parent, ref);
            return 1;
        }       

        // create a tree with the index and get oid
        error = git_index_write_tree(&tree_oid, index);
        if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(index, NULL, NULL, parent, ref);
            return 1;
        }       

        // write index to disk
        error = git_index_write(index);
        if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(index, NULL, NULL, parent, ref);
            return 1;
        }       

        // get new tree object using oid
        error = git_tree_lookup(&tree, repo, &tree_oid);
        if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(index, NULL, tree, parent, ref);
            return 1;
        }       

        // create a signature with the default user
        error = git_signature_default(&signature, repo);
        if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(index, signature, tree, parent, ref);
            return 1;
        }       

        error = git_commit_create_v(&commit_oid, repo, "HEAD", signature, signature, NULL, commit_msg, tree, parent ? 1 : 0, parent); 
        if (error != 0) {
            perror(git_error_last()->message);
            cleanup_git_commit(index, signature, tree, parent, ref);
            return 1;
        }       

        cleanup_git_commit(index, signature, tree, parent, ref);
        return 0;
    } else {
        print_invalid_use_cmd("git commit");
        return 1;
    }
    // https://libgit2.org/libgit2/ex/HEAD/commit.html#git_commit_create_v-1
}

void cleanup_git_remove_from_index(git_index *index) {
    // free memory from 'git restore' command
    git_index_free(index);
    return;
}

int git_remove_from_index(char **input, git_repository *repo) {
    git_index *index;
    int error;
    
    // get repo index
    error = git_repository_index(&index, repo);
    if (error != 0) {
        perror(git_error_last()->message);
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
        cleanup_git_remove_from_index(index);
        return 1;
    }

    // write index to disk
    error = git_index_write(index); 
    if (error != 0) {
        perror(git_error_last()->message);
        cleanup_git_remove_from_index(index);
        return 1;
    }       

    cleanup_git_remove_from_index(index);
    return 0;
}
