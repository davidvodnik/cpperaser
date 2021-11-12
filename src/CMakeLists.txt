add_executable(test_tokenizer
    tokenizer.h
    tokenizer.t.cc
    )
target_link_libraries(test_tokenizer PRIVATE catch)
add_test(NAME test_tokenizer COMMAND test_tokenizer -o ${CMAKE_BINARY_DIR}/tokenizer.xml -r junit)
