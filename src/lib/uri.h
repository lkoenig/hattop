#ifndef URI_H
#define URI_H

typedef struct hattop_query_parameters {
    int num;
    char ** keys;
    char ** values;
} hattop_query_parameters;

typedef struct hattop_uri {
    char * path;
    hattop_query_parameters query_parameters;
} hattop_uri;

hattop_uri * hattop_uri_create(const char * uri);
void hattop_uri_destroy(hattop_uri * uri);

#endif
