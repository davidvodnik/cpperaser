cmake_minimum_required(VERSION 3.15)

project(DuckTypeErasureGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_subdirectory(thirdparty/catch)
add_subdirectory(thirdparty/fmt)
add_subdirectory(src)
