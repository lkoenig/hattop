#include "uri.h"
#include "hattest.h"

enum hattest_status bar(){
    return HATTEST_PASS;
}

int main()
{
    HATTEST_RUN_TEST(bar);
    return 0;
}
