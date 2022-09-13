cmake_minimum_required(VERSION 3.24)
set(CMAKE_BUILD_TYPE RELEASE)
project(test)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CGAL)
add_executable(test test.cpp)
target_link_libraries(test CGAL::CGAL)
