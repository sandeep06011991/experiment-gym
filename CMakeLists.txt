cmake_minimum_required(VERSION 3.0)
project(v3)
option(test "Build all tests." OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-o1 -mavx2 -fPIC -std=c++0x -pedantic  -Wall \
        -Wno-unused-function -Wextra -march=native -mtune=native \
        -DNUM_THREADS_IN=${NUM_THREADS} -fdump-tree-profile_estimate")
set(CMAKE_BUILD_TYPE Debug)

file(COPY data DESTINATION .)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(src)

#enable_testing ()
add_subdirectory (test)
