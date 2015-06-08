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

void test_hattop_uri_create_eval_null(struct hattop_uri * uri){
    HATTEST_CHECK_EQ((int)uri, (int)NULL);
}

void test_hattop_uri_create_eval_1(struct hattop_uri * uri){
    HATTEST_CHECK_EQ((int)uri == (int)NULL, 0);
    HATTEST_CHECK_EQ(strcmp(uri->path, "/example/uri"), 0);
    HATTEST_CHECK_EQ(uri->query_parameters.num, 0);
}

void test_hattop_uri_create_eval_2(struct hattop_uri * uri){
    HATTEST_CHECK_EQ((int)uri == (int)NULL, 0);
    HATTEST_CHECK_EQ(strcmp(uri->path, "/example/uri"), 0);
    HATTEST_CHECK_EQ(uri->query_parameters.num, 1);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.keys[0], "with"), 0);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.values[0], "param"), 0);
}

void test_hattop_uri_create_eval_3(struct hattop_uri * uri){
    HATTEST_CHECK_EQ((int)uri == (int)NULL, 0);
    HATTEST_CHECK_EQ(strcmp(uri->path, "/example/uri"), 0);
    HATTEST_CHECK_EQ(uri->query_parameters.num, 4);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.keys[0], "with"), 0);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.values[0], "param"), 0);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.keys[1], "et"), 0);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.values[1], "cetera"), 0);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.keys[2], "no_value"), 0);
    HATTEST_CHECK_EQ((int)uri->query_parameters.values[2], (int)NULL);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.keys[3], "the"), 0);
    HATTEST_CHECK_EQ(strcmp(uri->query_parameters.values[3], "end"), 0);
}

enum hattest_status test_hattop_uri_create(const char * uristr, void(*eval)(struct hattop_uri *)){
    struct hattop_uri * uri = hattop_uri_create(uristr);

    eval(uri);

    hattop_uri_destroy(uri);
    return HATTEST_PASS;
}

int main()
{
    HATTEST_RUN_TEST(test_split_after_token, "/example/uri", NULL, '?');
    HATTEST_RUN_TEST(test_split_after_token, "/example/uri?", "", '?');
    HATTEST_RUN_TEST(test_split_after_token, "/example/uri?with=param", "with=param", '?');

    HATTEST_RUN_TEST(test_hattop_uri_create, "example/uri", &test_hattop_uri_create_eval_null);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?", &test_hattop_uri_create_eval_null);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri??", &test_hattop_uri_create_eval_null);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?hello==there&bla", &test_hattop_uri_create_eval_null);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?hello==there", &test_hattop_uri_create_eval_null);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri", &test_hattop_uri_create_eval_1);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param", &test_hattop_uri_create_eval_2);
    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&et=cetera&no_value&the=end", &test_hattop_uri_create_eval_3);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&&et=cetera", &test_hattop_uri_create_eval_null);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param=yeah&et=cetera", &test_hattop_uri_create_eval_null);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&=cetera", &test_hattop_uri_create_eval_null);

    HATTEST_RUN_TEST(test_hattop_uri_create, "/example/uri?with=param&et=cetera&", &test_hattop_uri_create_eval_null);

    return 0;
}
