#include "uri.h"
#include <string.h>

char * split_after_token(const char * uri, int uri_len, const char * token){
    char * start = strstr(uri, token);
    if (start != NULL){
        if (start < uri + uri_len){
            return start + 1;
        }
    }
    return NULL;
};

struct hattop_uri * parse_uri(const char * uristr){
    struct hattop_uri * uri = NULL;

    char * query = split_after_token(uristr, strlen(uristr), "?");

    if (query){
        int param = 0;
        int i;
        char * key = query;
        char * value = NULL;

        if (split_after_token(query, strlen(query), "?")){
            return NULL; // BAD URI, has at least two '?'
        }

        // iterate through each parameter, split on =, sanity check, malloc etc

    }

    uri = malloc(sizeof(struct hattop_uri));

    return uri;
}
