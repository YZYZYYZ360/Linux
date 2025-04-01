cmake_minimum_required(VERSION 3.1.0)
project(CGAL_Demo)

find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

add_executable(cgal_demo main.cpp)
target_link_libraries()((cgal_demo CGAL::CGAL))