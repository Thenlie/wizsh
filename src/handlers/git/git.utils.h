#include <git2.h>
#ifndef GIT_UTILS_H_
#define GIT_UTILS_H_ 

#include <stdbool.h>

bool is_git_dir(char* dir_path);
int credentials_cb(git_credential **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload);

#endif