#include "git.credentials.h"
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

// TODO: Check for invalid password, exit upon 2 invalid attempts
int credentials_cb(git_credential **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload) {
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
    
    // accept input from user
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