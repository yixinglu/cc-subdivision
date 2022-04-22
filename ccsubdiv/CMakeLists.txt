
add_library(
  ccsubdiv_lib
  SHARED
  ccsubdiv.cpp
  helper.cpp
  readobj.cpp
)

add_executable(
  ccsubdiv
  main.cpp
)

target_link_libraries(
  ccsubdiv
  ccsubdiv_lib
  GL
  GLU
  ${GLUT_LIBRARIES}
)
