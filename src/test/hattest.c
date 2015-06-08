#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hattest.h"

static int test_id = 0;

void _hattest_start_test(const char * test_name, const char * test_args){
    test_id++;
    printf("%i: %s(%s)\n", test_id, test_name, test_args);
}

void _hattest_eval_test(enum hattest_status result){
    if (result != HATTEST_PASS){
        printf("i:\tFAIL\n", test_id);
        exit(test_id);
    }
    else{
        printf("%i:\tPASS\n", test_id);
    }
}

void _hattest_check(int lh, int rh, const char * lhs, const char *rhs){
    if (lh != rh){
        printf("%i:\tFAIL (%s [%i] != %s [%i])\n", test_id, lhs, lh, rhs, rh);
        exit(test_id);
    }
}
