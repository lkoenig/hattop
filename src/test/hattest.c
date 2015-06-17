#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hattest.h"
#include <assert.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum run_modes{
    run_all,
    break_on_fail,
    assert_on_fail,
    single_test,
    dry_run
};

static int run_mode = run_all;
static int single_test_id = 0;
static int test_id = 0;
static int has_failure = 0;

void _hattest_start_test(const char * test_name, const char * test_args){
    test_id++;
    if(run_mode != single_test || (run_mode == single_test && single_test_id == test_id)){
        printf("%s%i: %s%s%s(%s%s%s)%s\n", ANSI_COLOR_BLUE, test_id, ANSI_COLOR_CYAN, test_name, ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, test_args, ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    }
}

int _hattest_should_test(){
    return run_mode != dry_run && run_mode != single_test || (run_mode == single_test && single_test_id == test_id);
}

void _hattest_abort(){
    has_failure = test_id;
    switch(run_mode){
        case assert_on_fail:
            assert(0);
        case single_test:
        case break_on_fail:
            hattest_exit();
            break;
        case run_all:
        default:
            break;
    }
}

void _hattest_eval_test(enum hattest_status result){
    if (result != HATTEST_PASS){
        printf("%sFAIL%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        _hattest_abort();
    }
    else if(test_id == has_failure){
        //
    }
    else{
        printf("%sPASS%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    }
}

void _hattest_check(int condition, const char *condition_string){
    if ( ! condition){
        printf("%sFAIL%s (%s is false - %d )\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, condition_string, condition);
        _hattest_abort();
    }
}

void hattest_start(int argc, char *argv[]){
    int i=0;
    printf("Running hattest\n", test_id);
    for(i=0; i<argc-1; i++){
        char * key = argv[i];
        char * val = argv[i+1];

        if(strcmp(key, "--mode") == 0){
            if(strcmp(val, "all") == 0){
                run_mode = run_all;
            }
            else if(strcmp(val, "break") == 0){
                run_mode = break_on_fail;
            }
            else if(strcmp(val, "assert") == 0){
                run_mode = break_on_fail;
            }
            else if(strcmp(val, "list") == 0){
                run_mode = dry_run;
            }
            else{
                int test_id = (int)strtol(val, NULL, 10);
                if(test_id > 0){
                    run_mode = single_test;
                    single_test_id = test_id;
                }
            }
        }
    }
}

void hattest_exit(){
    if(run_mode == dry_run){
        printf("Found %i tests\n", test_id);
        exit(0);
    }
    else if(run_mode == single_test){
        if(test_id < single_test_id){
            printf("%sCould not find%s test %i, there exists %i tests.\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, single_test_id, test_id);
            exit(-1);
        }
        else if(has_failure){
            assert(single_test_id == test_id);
            printf("Ran test %i %sunsuccessfully%s\n", single_test_id, ANSI_COLOR_RED, ANSI_COLOR_RESET);
            exit(single_test_id);
        }
        else{
            printf("Ran test %i %ssuccessfully%s\n", single_test_id, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        }
    }
    else if(has_failure){
        printf("Ran %i tests %sunsuccessfully%s\n", test_id, ANSI_COLOR_RED, ANSI_COLOR_RESET);
        exit(run_mode==run_all?-1:test_id);
    }
    else{
        printf("Ran %i tests %ssuccessfully%s\n", test_id, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    }
    exit(0);
}
