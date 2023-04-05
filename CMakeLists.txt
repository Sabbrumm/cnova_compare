cmake_minimum_required(VERSION 3.24)
project(cnova_compare)

set(CMAKE_CXX_STANDARD 17)

add_executable(cnova_compare main.cpp LCS_algo.h)
add_library(thepyway thepyway/thepyway.cpp)
target_link_libraries(cnova_compare thepyway)
