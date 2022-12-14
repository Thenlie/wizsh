#include "requests.h"
#include "printutility.h"

#include <string.h>
#include <stdio.h>
#include <curl/curl.h>

int fetch(char **input, int word_count) {
    if (word_count == 2) {
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
    } else {
        print_invalid_use_cmd(input[0]);
        return 1;
    }
}