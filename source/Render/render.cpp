#include "../Mesh/datatype.h"
#include "../Mesh/readobj.h"

//#include <gl/GL.h>
//#include <gl/GLU.h>

void init() {
  //glClearColor(0.0, 0.0, 0.0, 0.0);
}




using namespace ccsubdiv;
int main() {
  Reader reader("E:\\cube.obj");
  mesh_handle mesh = reader.load_obj_file();
  //ccsubdivision(mesh, 1, mesh);

  return 0;
}