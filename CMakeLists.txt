cmake_minimum_required(VERSION 3.15)

project(DuckTypeErasureGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_subdirectory(thirdparty/catch)
add_subdirectory(src)
