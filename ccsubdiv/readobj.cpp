
#include "readobj.h"

#include <assert.h>

#include <iostream>
#include <sstream>
#include <string>

#include "helper.h"

namespace ccsubdiv {

void Reader::read_vertex(const std::string& line) {
  std::stringstream sstr(line);
  char v;
  double x = 0.0, y = 0.0, z = 0.0, w = 1.0;
  sstr >> v >> x >> y >> z;
  if (!sstr.eof()) {
    sstr >> w;
    CCASSERT(w);
    x /= w;
    y /= w;
    z /= w;
  }
  vertex_ptr vert = std::make_shared<Vertex>();
  vert->coord = vec3d(x, y, z);
  MeshHelper::add_vertex_to_mesh(vert, mesh);
}

void Reader::read_normal(const std::string& line) {
  std::stringstream sstr(line);
  char v;
  sstr >> v >> v;  // erase vn
  double x = 0.0, y = 0.0, z = 0.0;
  sstr >> x >> y >> z;
  normals.push_back(vec3d(x, y, z));
}

void Reader::read_face(const std::string& line) {
  std::stringstream sstr(line);
  char f;
  sstr >> f;
  std::vector<vertex_ptr> vertices;
  std::vector<size_t> vids;
  std::string vert;
  while (sstr >> vert) {
    size_t id = 0, id2 = 0;
    char slash;
    std::stringstream ss(vert);
    auto pos = vert.find_first_of("/");
    if (pos == std::string::npos) {
      // f v1 v2 v3 ...
      ss >> id;
      vertices.push_back(mesh->vertices[id - 1]);
      vids.push_back(id);
    } else {
      auto pos1 = vert.find_first_of("/", pos + 1);
      if (pos1 == std::string::npos) {
        // f v1/vt1 v2/vt2 v3/vt3 ...
        ss >> id;
        vertices.push_back(mesh->vertices[id - 1]);
        vids.push_back(id);
        ss >> slash >> id;
      } else {
        if (pos1 == pos + 1) {
          // f v1//vn1 v2//vn2 v3//vn3 ...
          ss >> id;
          assert(id - 1 < mesh->vertices.size());
          vertices.push_back(mesh->vertices[id - 1]);
          vids.push_back(id);
          ss >> slash >> slash >> id2;
          mesh->vertices[id - 1]->norm = normals[id2 - 1];
        } else {
          // f v1/vt1/vn1 v2/vt2/vn2 ...
          ss >> id;
          assert(id - 1 < mesh->vertices.size());
          vertices.push_back(mesh->vertices[id - 1]);
          vids.push_back(id);
          ss >> slash >> id2 >> slash >> id2;
          mesh->vertices[id - 1]->norm = normals[id2 - 1];
        }
      }
    }
  }
  assert(vertices.size() >= 3);
  MeshHelper::create_face(vertices, mesh);
}

Reader::LineType Reader::get_line_type(const std::string& str) {
  auto pos = str.find_first_not_of(" \t");
  if (pos != std::string::npos) {
    switch (str[pos]) {
      case 'f':
        return FACE;
      case 'v': {
        if (pos + 1 == std::string::npos) break;
        switch (str[pos + 1]) {
          case 'p':
            return PARAM_SPACE_VERTEX;
          case 'n':
            return NORMAL;
          case 't':
            return TEXTURE_COORD;
          default:
            return VERTEX;
        }
      }
    }
  }
  return COMMENT;
}

mesh_ptr Reader::load_obj_file() {
  if (!ifs.is_open()) return nullptr;
  std::string line;
  while (std::getline(ifs, line)) {
    switch (get_line_type(line)) {
      case VERTEX:
        read_vertex(line);
        break;
      case NORMAL:
        read_normal(line);
        break;
      case FACE:
        read_face(line);
        break;
      default:
        break;
    }
  }
  return mesh;
}

}  // namespace ccsubdiv
