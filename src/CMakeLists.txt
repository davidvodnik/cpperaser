add_executable(test_tokenizer
    tokenizer.h
    tokenizer.cc
    tokenizer.t.cc
    )
target_link_libraries(test_tokenizer PRIVATE catch)
add_test(NAME test_tokenizer COMMAND test_tokenizer -o ${CMAKE_BINARY_DIR}/tokenizer.xml -r junit)

add_executable(test_parser
    tokenizer.h
    tokenizer.cc
    parser.h
    parser.cc
    parser.t.cc
    )
target_link_libraries(test_parser PRIVATE catch)
add_test(NAME test_parser COMMAND test_parser -o ${CMAKE_BINARY_DIR}/parser.xml -r junit)
