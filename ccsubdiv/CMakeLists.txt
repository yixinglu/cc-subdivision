
add_library(
  ccsubdiv
  SHARED
  meshmgr.cpp
  datatype.cpp
  readobj.cpp
)

add_executable(
  ccsubd
  main.cpp
)

target_link_directories(
  ccsubd
  PUBLIC
  ${GLUT_INCLUDE_DIR}/../lib
)

target_link_libraries(
  ccsubd
  ccsubdiv
  ${GLUT_LIBRARIES}
  ${OPENGL_LIBRARIES}
)
