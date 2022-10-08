#include "requests.h"
#include "printutility.h"

#include <string.h>
#include <stdio.h>
#include <curl/curl.h>

int fetch(char **input, int word_count) {
    if (word_count == 2) {
        // check for help flag
        if (strcmp(input[1], "-h") == 0 || strcmp(input[1], "--help") == 0) {
            printf("\033[1;34m                                  -- %s --\n\n", input[0]); // <- first command
            printf("\033[0mThe \033[1;33m%s\033[0m command is used to rename a file. When provided with a file name as a second argument and a new file name as a third argument, the command will rename the file to the new file name. This does not remove the contents of the file.\n\n", input[0]);
            printf("\033[1;35m                                   Usage\n\n"); 
            printf("\033[0m ~> \033[1;33m%s <old_name> <new_name> \033[0m| The main usage of the command. Rename a file called \033[1;33<old_name>\033[0m to \033[1;33<new_name>\033[0m.\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s -h                    \033[0m| Help with the command\n", input[0]);
            printf("\033[0m ~> \033[1;33m%s --help                \033[0m| Help with the command\n\n", input[0]);
            return 0;
        } else {
            CURL *curl;
            CURLcode res;

            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, input[1]);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

                // make request
                res = curl_easy_perform(curl);

                // check for errors
                if (res != CURLE_OK) {
                    perror("Error performing fetch");
                } else {
                    printf("\n");
                }
                

                curl_easy_cleanup(curl);
            }

            return 0;
            // https://curl.se/libcurl/c/simple.html
        }
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}