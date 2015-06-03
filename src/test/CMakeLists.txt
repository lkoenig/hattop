add_executable(test_uri test_uri.c)
target_link_libraries(test_uri hattop)
add_test(test_uri ${CMAKE_BINARY_DIR}/test_uri)
