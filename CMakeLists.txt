#set project name
project(ss2dist)

#define cmake commands/policies
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# paths
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} LD_LIBRARY_PATH)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(  src  )

# install support scripts
install (FILES "${PROJECT_SOURCE_DIR}/scripts/dist2plot.py" DESTINATION bin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
