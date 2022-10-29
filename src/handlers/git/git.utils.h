#ifndef GIT_UTILS_H_
#define GIT_UTILS_H_ 

#include <stdbool.h>
#include <git2.h>

// determine if provided directory path is git enabled
bool is_git_dir(char* dir_path);
// prompts user for a password when using SSH
int credentials_cb(git_credential **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload);

#endif