#set project name
project(ss2dist)

#define cmake commands/policies
cmake_minimum_required(VERSION 3.1)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} LD_LIBRARY_PATH)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include_directories(${CMAKE_SOURCE_DIR})

include_directories(${.})


set (CMAKE_CXX_STANDARD 11)

# Sources:
set(base_srcs
    SurfaceSource.cpp
    histogram.cpp
    helpers.cpp
    InputFile.cpp
)

# actual target:
add_executable(ss2dist ${base_srcs} ss2dist.cpp)
add_executable(ss2more ${base_srcs} ss2more.cpp)
add_executable(turtle2rssa ${base_srcs} turtle2rssa.cpp)
add_executable(writetest ${base_srcs} writetest.cpp)

target_link_libraries(ss2more flann)
