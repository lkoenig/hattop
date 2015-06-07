#include "uri.h"
#include "hattest.h"

#include <string.h>

enum hattest_status test_split_after_token(const char * uri, const char * query, char * token){
    char * query_ret = split_after_token(uri, strlen(uri), "?");
    if (query != NULL){
        HATTEST_CHECK_EQ(strcmp(query, query_ret), 0);
    }
    else if(query == NULL){
        HATTEST_CHECK_EQ((int)query_ret, (int)NULL);
    }
    return HATTEST_PASS;
}

enum hattest_status test_parse_uri(const char * uristr){
    struct hattop_uri * uri = parse_uri(uristr);

    return HATTEST_PASS;
}

int main()
{
    HATTEST_RUN_TEST(test_split_after_token, "example/uri", NULL, "?");
    HATTEST_RUN_TEST(test_split_after_token, "example/uri?with=param", "with=param", "?");
    HATTEST_RUN_TEST(test_parse_uri, "example/uri?with=param&et=cetera&the=end");
    return 0;
}
