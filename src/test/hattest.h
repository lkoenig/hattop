#ifndef HATTEST_H
#define HATTEST_H

enum hattest_status {
    HATTEST_PASS = 0,
    HATTEST_FAIL = -1
};

void hattest_evaluate(enum hattest_status result, const char * test_name, const char * test_args);

#define HATTEST_RUN_TEST(foo, ...) hattest_evaluate(foo(#__VA_ARGS__), ""#foo, ""#__VA_ARGS__)

#endif