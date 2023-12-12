function(add_component name)
    message(STATUS "ARGN=${ARGN}")
    add_library(${name} INTERFACE)
    target_sources(${name} INTERFACE ${name}.cc
        INTERFACE FILE_SET HEADERS
        BASE_DIRS .
        FILES ${name}.h
        )
    target_link_libraries(${name} INTERFACE ${ARGN})

    add_executable(test_${name}
        ${name}.t.cc
        )
    target_link_libraries(test_${name} PRIVATE ${ARGV0} catch)
    add_test(NAME test_${name} COMMAND test_${name})
endfunction()

add_component(tokenizer)
add_component(parser tokenizer)
add_component(generate_interface fmt::fmt)
add_component(print_error fmt::fmt)

add_executable(cpperaser
    cpperaser.cc
    )
target_link_libraries(cpperaser PRIVATE parser generate_interface print_error)
if(EMSCRIPTEN)
target_link_options(cpperaser PRIVATE "SHELL: -s EXPORT_ES6=1 -s MODULARIZE=1 --bind")
endif()

install(TARGETS cpperaser DESTINATION bin)
