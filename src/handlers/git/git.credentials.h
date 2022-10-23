#ifndef GIT_CREDENTIALS_H_
#define GIT_CREDENTIALS_H_
#include <git2.h>

int credentials_cb(git_credential **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload);

#endif