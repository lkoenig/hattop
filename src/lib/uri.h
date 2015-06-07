#ifndef URI_H
#define URI_H

struct hattop_query_parameters {
    int num;
    char ** keys;
    char ** values;
};

struct hattop_uri {
    char * path;
    struct hattop_query_parameters query_parameters;
};

char * split_after_token(const char * uri, int uri_len, const char * token);

struct hattop_uri * parse_uri(const char * uri);

#endif
