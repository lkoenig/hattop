#include "uri.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char * split_after_token(const char * uri, int uri_len, char token){
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

struct hattop_uri * hattop_uri_create(const char * uristr){
    struct hattop_uri * uri = NULL;

    int uristrlen = strlen(uristr);
    char * querystr = split_after_token(uristr, uristrlen, '?');

    uri = malloc(sizeof(struct hattop_uri));
    uri->query_parameters.num = 0;
    uri->path = NULL;

    /* Parse query string */
    if (querystr){
        int i = 0;
        char * key = querystr;
        char * last_key = NULL;

        if (querystr == uristr + uristrlen){
            hattop_uri_destroy(uri);
            return NULL; // BAD URI, has '?' but nothing after
        }

        if (split_after_token(querystr, uristrlen - (querystr - uristr), '?')){
            hattop_uri_destroy(uri);
            return NULL; // BAD URI, has at least two '?'
        }

        uri->path = calloc(querystr - uristr, sizeof(char));
        memcpy(uri->path, uristr, querystr - uristr - 1);

        /* how many parameters */
        do{
            key = split_after_token(key, uristrlen - (key - uristr), '&');
            i++;
            if (key == last_key + 1){
                hattop_uri_destroy(uri);
                return NULL; // BAD URI, has at least two consecutive '&'
            }
            last_key = key;
        } while (key);

        uri->query_parameters.num = i;
        uri->query_parameters.keys = calloc(uri->query_parameters.num, sizeof(char *));
        uri->query_parameters.values = calloc(uri->query_parameters.num, sizeof(char *));

        /* find all keys, and values if present */
        key = querystr;
        for (i = 0; i < uri->query_parameters.num; i++){

            char * value = NULL;
            int key_len = 0;
            int val_len = 0;

            char * next_key = split_after_token(key, uristrlen - (key - uristr), '&');
            if (next_key == NULL){
                assert(i == uri->query_parameters.num-1);
                next_key = uristr + uristrlen + 1;
            }

            value = split_after_token(key, next_key - key, '=');

            if (value){
                char * next_value = split_after_token(value, uristrlen - (value - uristr), '=');
                if (next_value != NULL && next_value < next_key){
                    hattop_uri_destroy(uri);
                    return NULL; // BAD URI, parameter has at least two '='
                }

                key_len = value - key - 1;
                val_len = next_key - value - 1;
            }
            else{
                key_len = next_key - key - 1;
            }

            if (key_len){
                uri->query_parameters.keys[i] = calloc(key_len + 1, sizeof(char));
                memcpy(uri->query_parameters.keys[i], key, key_len);
                if (val_len){
                    uri->query_parameters.values[i] = calloc(val_len + 1, sizeof(char));
                    memcpy(uri->query_parameters.values[i], value, val_len);
                }
            }
            else{
                hattop_uri_destroy(uri);
                return NULL; // BAD URI, parameter has a key of length 0
            }

            key = next_key;

        };
    }
    else{
        // no query string, but path is ok
        uri->path = calloc(uristrlen + 1, sizeof(char));
        strcpy(uri->path, uristr);
    }

    return uri;
}
