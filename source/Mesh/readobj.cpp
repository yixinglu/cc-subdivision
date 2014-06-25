
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

#include "readobj.h"

namespace ccsubdiv {

void Reader::read_vertex(const std::string& line) {
  std::stringstream sstr(line);
  char v;
  double x = 0.0, y = 0.0, z = 0.0, w = 1.0;
  sstr >> v >> x >> y >> z;
  if (!sstr.eof()) {
    sstr >> w;
    assert(w > 1.0e-08 || w < -1.0e-08);
    x /= w; y /= w; z /= w;
  }
  vertex_handle vert(new Vertex);
  vert->coord = vec3d(x, y, z);
  mesh->vertices.push_back(vert);
}


void Reader::read_normal(const std::string& line) {
  std::stringstream sstr(line);
  char v;
  double x = 0.0, y = 0.0, z = 0.0;
  sstr >> v >> v >> x >> y >> z; // erase vn
  normals.push_back(vec3d(x, y, z));
}

void Reader::create_face(std::vector<size_t>& vid){
  face_handle face(new Face);
  for (size_t i = 0; i < vid.size() - 1; ++i) {
    hedge_handle hedge(new HEdge);
    auto vert = mesh->vertices[vid[i] - 1];
    hedge->vert = vert;
    mesh->edges.push_back(hedge);
    if (vert->edge == nullptr) {
      vert->edge = mesh->edges.back();
    }
    // todo
  }
}


void Reader::read_face(const std::string& line) {
  std::stringstream sstr(line);
  char f; sstr >> f;
  std::vector<size_t> vid;
  std::string vert;
  while (sstr >> vert) {
    size_t id = 0;
    char slash;
    std::stringstream ss(vert);
    auto pos = vert.find_first_of("/");
    if (pos == std::string::npos) {
      // f v1 v2 v3 ...
      ss >> id;
      vid.push_back(id);
    }
    else {
      auto pos1 = vert.find_first_of("/", pos + 1);
      if (pos1 == std::string::npos) {
        // f v1/vt1 v2/vt2 v3/vt3 ...
        ss >> id;
        vid.push_back(id);
        ss >> slash >> id;
      }
      else {
        if (pos1 == pos + 1) {
          // f v1//vn1 v2//vn2 v3//vn3 ...
          ss >> id;
          vid.push_back(id);
          ss >> slash >> slash >> id;
          assert(vid.back() - 1 < mesh->vertices.size());
          mesh->vertices[vid.back() - 1]->norm = normals[id - 1];
        }
        else {
          // f v1/vt1/vn1 v2/vt2/vn2 ...
          ss >> id;
          vid.push_back(id);
          ss >> slash >> id >> slash >> id;
          assert(vid.back() - 1 < mesh->vertices.size());
          mesh->vertices[vid.back() - 1]->norm = normals[id - 1];
        }
      }
    }
  }
  assert(vid.size() >= 3);
  create_face(vid);
}

Reader::LineType Reader::get_line_type(const std::string& str){
  auto pos = str.find_first_not_of(" \t");
  if (pos != std::string::npos) {
    switch (str[pos]) {
    case 'f': return FACE;
    case 'v': {
      if (pos + 1 == std::string::npos) break;
      switch (str[pos + 1]) {
      case 'p': return PARAM_SPACE_VERTEX;
      case 'n': return NORMAL;
      case 't': return TEXTURE_COORD;
      default:  return VERTEX;
      }
    }
    }
  }
  return COMMENT;
}

mesh_handle Reader::load_obj_file() {
  if (!ifs.is_open()) return nullptr;
  std::string line;
  while (std::getline(ifs, line)) {
    switch (get_line_type(line))
    {
    case VERTEX: read_vertex(line); break;
    case NORMAL: read_normal(line); break;
    case FACE: read_face(line); break;
    default: break;
    }
  }
  return mesh;
}

}


