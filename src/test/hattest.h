#ifndef HATTEST_H
#define HATTEST_H

enum hattest_status {
    HATTEST_PASS = 0,
    HATTEST_FAIL = -1
};

void _hattest_start_test(const char * test_name, const char * test_args);
int _hattest_should_test();
void _hattest_eval_test(enum hattest_status result);

void _hattest_check(int lh, int rh, const char * lhs, const char *rhs);

#define HATTEST_RUN_TEST(foo, ...) _hattest_start_test(""#foo, ""#__VA_ARGS__); if(_hattest_should_test()){_hattest_eval_test( foo(__VA_ARGS__) );}
#define HATTEST_CHECK_EQ(lh, rh) _hattest_check(lh, rh, ""#lh, ""#rh)

void hattest_start(int argc, char *argv[]);
void hattest_exit();

#endif
