cmake_minimum_required(VERSION 3.24)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(convex_hull)

# gnuplot-iostream header
include_directories(/usr/local/include/)

find_package(CGAL)
find_package(Boost)

add_executable(${PROJECT_NAME} convex_hull.cpp)
target_link_libraries(${PROJECT_NAME}
    CGAL::CGAL
    boost_iostreams
    boost_system
    boost_filesystem)
