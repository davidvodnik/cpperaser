function(add_component name)
    message(STATUS "ARGN=${ARGN}")
    add_library(${name} INTERFACE)
    target_sources(${name} INTERFACE
        ${name}.h
        ${name}.cc
        )
    target_include_directories(${name} INTERFACE .)
    target_link_libraries(${name} INTERFACE ${ARGN})

    add_executable(test_${name}
        ${name}.t.cc
        )
    target_link_libraries(test_${name} PRIVATE ${ARGV0} catch)
    add_test(NAME test_${name} COMMAND test_${name} -o ${CMAKE_BINARY_DIR}/tokenizer.xml -r junit)
endfunction()

add_component(tokenizer)
add_component(parser tokenizer)

add_executable(cpperaser
    cpperaser.cc
    )
target_link_libraries(cpperaser PRIVATE parser fmt::fmt)
