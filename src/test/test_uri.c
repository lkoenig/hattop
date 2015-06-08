#include "uri.h"
#include "hattest.h"

#include <string.h>

enum hattest_status test_split_after_token(const char * uri, const char * query, char token){
    char * query_ret = split_after_token(uri, strlen(uri), token);
    if (query != NULL){
        HATTEST_CHECK_EQ(query_ret == NULL, 0);
        HATTEST_CHECK_EQ(strcmp(query, query_ret), 0);
    }
    else if(query == NULL){
        HATTEST_CHECK_EQ((int)query_ret, (int)NULL);
    }
    return HATTEST_PASS;
}

enum hattest_status test_hattop_uri_create(const char * uristr, struct hattop_uri * expected){
    struct hattop_uri * uri = hattop_uri_create(uristr);

    if (expected == NULL){
        HATTEST_CHECK_EQ(uri, NULL);
    }
    else{
        int i;

        HATTEST_CHECK_EQ(uri->path == NULL, 0);
        HATTEST_CHECK_EQ(strcmp(uri->path, expected->path), 0);

        HATTEST_CHECK_EQ(uri->query_parameters.num, expected->query_parameters.num);
        for (i = 0; i < expected->query_parameters.num; i++){
            if (expected->query_parameters.keys[i] == NULL){
                HATTEST_CHECK_EQ(uri->query_parameters.keys[i], NULL);
            }
            else{
                HATTEST_CHECK_EQ(uri->query_parameters.keys[i] == NULL, 0);
                HATTEST_CHECK_EQ(strcmp(uri->query_parameters.keys[i], expected->query_parameters.keys[i]), 0);
            }
            if (expected->query_parameters.values[i] == NULL){
                HATTEST_CHECK_EQ(uri->query_parameters.values[i], NULL);
            }
            else{
                HATTEST_CHECK_EQ(uri->query_parameters.values[i] == NULL, 0);
                HATTEST_CHECK_EQ(strcmp(uri->query_parameters.values[i], expected->query_parameters.values[i]), 0);
            }
        }
    }

    hattop_uri_destroy(uri);
    return HATTEST_PASS;
}

int main()
{
    HATTEST_RUN_TEST(test_split_after_token, "/example/uri", NULL, '?');
    HATTEST_RUN_TEST(test_split_after_token, "/example/uri?", "", '?');
    HATTEST_RUN_TEST(test_split_after_token, "/example/uri?with=param", "with=param", '?');

    HATTEST_RUN_TEST(test_hattop_uri_create, "example/uri", NULL);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?", NULL);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri??", NULL);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?hello==there&bla", NULL);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?hello==there", NULL);

    {
        struct hattop_uri exp;
        exp.path = "/example/uri"; exp.query_parameters.num = 0;
        HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri", &exp);
    }

    {
        struct hattop_uri exp; char * keys[1]; char * values[1];
        exp.path = "/example/uri"; exp.query_parameters.num = 1;
        exp.query_parameters.keys = keys; exp.query_parameters.values = values;
        exp.query_parameters.keys[0] = "with"; exp.query_parameters.values[0] = "param";
        HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param", &exp);
    }

    {
        struct hattop_uri exp; char * keys[4]; char * values[4];
        exp.path = "/example/uri"; exp.query_parameters.num = 4;
        exp.query_parameters.keys = keys; exp.query_parameters.values = values;
        exp.query_parameters.keys[0] = "with"; exp.query_parameters.values[0] = "param";
        exp.query_parameters.keys[1] = "et"; exp.query_parameters.values[1] = "cetera";
        exp.query_parameters.keys[2] = "no_value"; exp.query_parameters.values[2] = NULL;
        exp.query_parameters.keys[3] = "the"; exp.query_parameters.values[3] = "end";
        HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&et=cetera&no_value&the=end", &exp);
    }

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&&et=cetera", NULL);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param=yeah&et=cetera", NULL);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&=cetera", NULL);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&et=cetera&", NULL);

    return 0;
}
