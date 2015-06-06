#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hattest.h"

static int test_id = 0;

void hattest_evaluate(enum hattest_status result, const char * test_name, const char * test_args){
    test_id++;
    printf("%i: %s(%s) ", test_id, test_name, test_args);
    if (result != HATTEST_PASS){
        printf("FAIL\n");
        exit(test_id);
    }
    else{
        printf("PASS\n");
    }
}
