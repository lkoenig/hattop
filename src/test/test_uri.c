#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uri.h"

enum hattest_status {
    HATTEST_PASS = 0,
    HATTEST_FAIL = -1
};

static int test_id = 0;
#define RUN_TEST(foo, ...) test_id++; printf( "%i: " #foo "("#__VA_ARGS__") ", test_id); if(foo(#__VA_ARGS__) != 0){printf("FAIL\n"); exit(test_id);}else{printf("PASS\n");}

enum hattest_status bar(){
    return HATTEST_PASS;
}

int main()
{
    RUN_TEST(bar);
    return 0;
}
