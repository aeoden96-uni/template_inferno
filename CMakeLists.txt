cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
project(meta)
find_package(fmt)

add_executable(lecture_1 01_introduction.cpp)
target_link_libraries(lecture_1 fmt::fmt-header-only)

add_executable(lecture_2 02_implementations.cpp)
target_link_libraries(lecture_2 fmt::fmt-header-only)

add_executable(lecture_3 03_recursion_is_prime.cpp)
target_link_libraries(lecture_3 fmt::fmt-header-only)