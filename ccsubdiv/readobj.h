#ifndef CCSUBDIV_READ_OBJ_FILE_H_
#define CCSUBDIV_READ_OBJ_FILE_H_

#include <fstream>

#include "datatype.h"

namespace ccsubdiv {

class Reader {
 public:
  explicit Reader(const char* filename) : ifs(filename) {
    mesh = std::make_shared<Mesh>();
  }
  ~Reader() { ifs.close(); }
  mesh_ptr load_obj_file();
  bool is_open() const { return ifs.is_open(); }

 private:
  Reader(const Reader&);
  Reader& operator=(const Reader&);

  void read_vertex(const std::string&);
  void read_normal(const std::string&);
  void read_face(const std::string&);
  // void read_param_space_vertex(const std::string&);
  // void read_texture_coord(const std::string&);

  enum LineType {
    COMMENT,
    VERTEX,
    TEXTURE_COORD,
    NORMAL,
    PARAM_SPACE_VERTEX,
    FACE,
    UNKNOWN
  };
  static LineType get_line_type(const std::string&);

  std::ifstream ifs;
  mesh_ptr mesh;
  std::vector<vec3d> normals;
};

}  // namespace ccsubdiv

#endif
