#include "git.print.h"
#include "../filesystem/fs.print.h"
#include "../printutility.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <git2.h>

int clone_git_repo(char **input, int word_count) {
    if (word_count == 3) {
        git_repository *repo = NULL;
        const char *url = input[2];
        const char *path = ".";
        int error;

        // clone repository to path at given url
        error = git_clone(&repo, url, path, NULL);
        if (error != 0) {
            perror(git_error_last()->message);
        }

        git_repository_free(repo);
        return 0;
    } else {
        print_invalid_use_cmd("git clone");
        return 1;
    }
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

int git_add_cleanup(git_repository *repo, git_index *index) {
    // free memory from 'git add' command
    git_repository_free(repo);
    git_index_free(index);

    return 0;
}

int git_add_to_index(char **input, int word_count) {
    if (word_count == 3) {
        git_index *index;
        git_repository *repo;
        int error;

        error = git_repository_open(&repo, ".");
        // check for error opening repo
        if (error != 0) {
            perror(git_error_last()->message);
            return 1;
        }

        // get the current index    
        error = git_repository_index(&index, repo);
        if (error != 0) {
            perror(git_error_last()->message);
            git_add_cleanup(repo, NULL);
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
            git_add_cleanup(repo, index);
            return 1;
        }  

        // write index to disk
        error = git_index_write(index);
        if (error != 0) {
            perror(git_error_last()->message);
            git_add_cleanup(repo, index);
            return 1;
        }  

        git_add_cleanup(repo, index);
        return 0;
    } else {
        print_invalid_use_cmd("git add");
        return 1;
    }
}

int commit_cleanup(git_repository *repo, git_index *index, git_signature *sig, git_tree *tree, git_object *obj, git_reference *ref) {
    // free memory from 'git commit -m' command
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

int git_remove_cleanup(git_repository *repo, git_index *index) {
    // free memory from 'git restore' command
    git_repository_free(repo);
    git_index_free(index);

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
            git_remove_cleanup(repo, NULL);
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
            git_remove_cleanup(repo, index);
            return 1;
        }

        // write index to disk
        error = git_index_write(index); 
        if (error != 0) {
            perror(git_error_last()->message);
            git_remove_cleanup(repo, index);
            return 1;
        }       

        git_remove_cleanup(repo, index);
    } else {
        print_invalid_use_cmd("git restore");
        return 1;
    }
    return 0;
}
