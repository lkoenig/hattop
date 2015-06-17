#ifndef HATTEST_H
#define HATTEST_H

enum hattest_status {
    HATTEST_PASS = 0,
    HATTEST_FAIL = -1
};

void _hattest_start_test(const char * test_name, const char * test_args);
int _hattest_should_test();
void _hattest_eval_test(enum hattest_status result);

void _hattest_check(int condition, const char * condition_string);

#define HATTEST_RUN_TEST(foo, ...) _hattest_start_test(""#foo, ""#__VA_ARGS__); if(_hattest_should_test()){_hattest_eval_test( foo(__VA_ARGS__) );}
#define HATTEST_CHECK( condition ) _hattest_check( ( condition ), ""#condition)

void hattest_start(int argc, char *argv[]);
void hattest_exit();

#endif
