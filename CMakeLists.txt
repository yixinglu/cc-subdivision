cmake_minimum_required(VERSION 3.5.0)

project(ccsubdiv)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
  include_directories("/usr/local/include")
endif()

add_subdirectory(ccsubdiv)
