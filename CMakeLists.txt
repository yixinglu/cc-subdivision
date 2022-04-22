cmake_minimum_required(VERSION 3.1)

project("CCSubdiv" C CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ccsubdiv)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_compile_options(-Wno-deprecated)
endif()

add_subdirectory(ccsubdiv)
