cmake_minimum_required(VERSION 3.1)

project("CCSubdiv" C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ccsubdiv)

add_subdirectory(ccsubdiv)
