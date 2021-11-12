cmake_minimum_required(VERSION 3.15)
project(Catch)

add_library(catch
    catch.hpp
    catch.cc
    )
target_include_directories(catch PUBLIC .)
