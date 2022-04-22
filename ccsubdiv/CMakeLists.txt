
add_library(
  ccsubdiv
  SHARED
  ccsubdiv.cpp
  datatype.cpp
  readobj.cpp
)

add_executable(
  ccsubd
  main.cpp
)

target_link_libraries(
  ccsubd
  ccsubdiv
  GL
  GLU
  ${GLUT_LIBRARIES}
)
