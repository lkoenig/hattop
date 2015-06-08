#include "uri.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char * move_to_after_token(const char * uri, int uri_len, char token){
    char * start = strchr(uri, token);
    if (start != NULL){
        if (start < uri + uri_len){
            return start + 1;
        }
    }
    return NULL;
}

void hattop_uri_destroy(struct hattop_uri * uri){
    int i = 0;

    if (uri == NULL){
        return;
    }

    for (i = 0; i < uri->query_parameters.num; i++){
        if (uri->query_parameters.keys[i]){
            free(uri->query_parameters.keys[i]);
        }
        if (uri->query_parameters.values[i]){
            free(uri->query_parameters.values[i]);
        }
    }

    if (uri->path){
        free(uri->path);
    }

    free(uri);
}

char * copy_and_sanitize_string(const char * string, int string_length){
    char * dst = calloc(string_length + 1, sizeof(char));
    // do the allowed cleanup of strings in here
    memcpy(dst, string, string_length);
    return dst;
}

struct hattop_uri * hattop_uri_create(const char * uristr){
    struct hattop_uri * uri = NULL;

    int uristrlen = strlen(uristr);
    char * querystr = move_to_after_token(uristr, uristrlen, '?');

    if (*uristr != '/'){
        goto bad_uri;
    }

    uri = malloc(sizeof(struct hattop_uri));
    uri->query_parameters.num = 0;
    uri->path = NULL;

    /* Parse query string */
    if (querystr){
        int i = 0;
        char * key = querystr;
        char * last_key = NULL;

        if (querystr == uristr + uristrlen){
            goto bad_uri; //'?' has nothing after
        }

        if (move_to_after_token(querystr, uristrlen - (querystr - uristr), '?')){
            goto bad_uri; // at least two '?'
        }

        uri->path = copy_and_sanitize_string(uristr, querystr - uristr - 1);

        /* how many parameters */
        do{
            key = move_to_after_token(key, uristrlen - (key - uristr), '&');
            i++;
            if (key == last_key + 1){
                goto bad_uri; // at least two consecutive '&'
            }
            last_key = key;
        } while (key);

        if (last_key == uristr + uristrlen){
            goto bad_uri; // last '&' has nothing after
        }

        uri->query_parameters.num = i;
        uri->query_parameters.keys = calloc(uri->query_parameters.num, sizeof(char *));
        uri->query_parameters.values = calloc(uri->query_parameters.num, sizeof(char *));

        /* find all keys, and values if present */
        key = querystr;
        for (i = 0; i < uri->query_parameters.num; i++){

            char * value = NULL;
            int key_len = 0;
            int val_len = 0;

            char * next_key = move_to_after_token(key, uristrlen - (key - uristr), '&');
            if (next_key == NULL){
                assert(i == uri->query_parameters.num-1);
                next_key = (char *)uristr + uristrlen + 1; // a bit dangerous?
            }

            value = move_to_after_token(key, next_key - key, '=');

            if (value){
                char * next_value = move_to_after_token(value, uristrlen - (value - uristr), '=');
                if (next_value != NULL && next_value < next_key){
                    goto bad_uri; // parameter has at least two '='
                }

                key_len = value - key - 1;
                val_len = next_key - value - 1;
            }
            else{
                key_len = next_key - key - 1;
            }

            if (key_len){
                uri->query_parameters.keys[i] = copy_and_sanitize_string(key, key_len);
                if (val_len){
                    uri->query_parameters.values[i] = copy_and_sanitize_string(value, val_len);
                }
            }
            else{
                goto bad_uri; // parameter has a key of length 0
            }

            key = next_key;

        };
    }
    else{
        // no query string, but path is ok
        uri->path = copy_and_sanitize_string(uristr, uristrlen);
    }

    return uri;

bad_uri:
    hattop_uri_destroy(uri);
    return NULL;

}
