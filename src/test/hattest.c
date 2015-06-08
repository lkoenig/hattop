#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hattest.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static int test_id = 0;

void _hattest_start_test(const char * test_name, const char * test_args){
    test_id++;
    printf("%s%i: %s%s%s(%s%s%s)%s\n", ANSI_COLOR_BLUE, test_id, ANSI_COLOR_CYAN, test_name, ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, test_args, ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
}

void _hattest_abort(){
    exit(test_id);
}

void _hattest_eval_test(enum hattest_status result){
    if (result != HATTEST_PASS){
        printf("%sFAIL%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        _hattest_abort();
    }
    else{
        printf("%sPASS%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    }
}

void _hattest_check(int lh, int rh, const char * lhs, const char *rhs){
    if (lh != rh){
        printf("%sFAIL%s (%s [%i] != %s [%i])\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, test_id, lhs, lh, rhs, rh);
        _hattest_abort();
    }
}
