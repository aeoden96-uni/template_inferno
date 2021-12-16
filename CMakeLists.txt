cmake_minimum_required(VERSION 3.22)
find_package(fmt)
set(CMAKE_CXX_STANDARD 20)

project(lecture_1)
add_executable(lecture_1 lecture_1.cpp)
target_link_libraries(lecture_1 fmt::fmt-header-only)

project(lecture_2)
add_executable(lecture_2 lecture_2.cpp)
target_link_libraries(lecture_2 fmt::fmt-header-only)
