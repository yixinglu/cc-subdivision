cmake_minimum_required(VERSION 3.1)

project("CCSubdiv" C CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_libraries(${GLUT_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ccsubdiv)

add_subdirectory(ccsubdiv)
