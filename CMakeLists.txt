#set project name
project(ss2dist)

#define cmake commands/policies
#need >= v2.8.10, see http://redmine.gromacs.org/issues/1051
cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} LD_LIBRARY_PATH)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake") 
include_directories(${CMAKE_SOURCE_DIR})

include_directories(${.})


# Sources:
set(base_srcs
    SurfaceSource.cpp
    histogram.cpp
    helpers.cpp
)

# actual target:
add_executable(ss2dist ${base_srcs} ss2dist.cpp)
