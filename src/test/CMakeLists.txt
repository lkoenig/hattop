add_executable(test_uri test_uri.c)
target_link_libraries(test_uri hattop)
add_test(NAME test_uri COMMAND test_uri)
