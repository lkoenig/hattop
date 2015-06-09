set(SRC
    hattest.c
    hattest.h)

add_library(hattest ${SRC})

add_executable(test_uri test_uri.c)
target_link_libraries(test_uri hattop)
target_link_libraries(test_uri hattest)
add_test(NAME test_uri COMMAND test_uri ARGUMENTS --mode break)
