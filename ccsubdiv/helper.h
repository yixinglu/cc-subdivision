#ifndef CCSUBDIV_HELPER_H
#define CCSUBDIV_HELPER_H

#include <vector>

#include "datatype.h"

namespace ccsubdiv {

class VertHelper {
 public:
  static vertex_ptr avg_border_edge_midpts(const vertex_ptr&);
  static size_t avg_adj_edge_midpts(const vertex_ptr&, vertex_ptr*);
  static size_t avg_adj_facepts(const vertex_ptr&, vertex_ptr*);
};

class EdgeHelper {
 public:
  static bool is_pair_edge(const hedge_ptr&, const hedge_ptr&);
  static hedge_ptr previous_edge(const hedge_ptr&);
  static hedge_ptr backward_edge_without_pair(const hedge_ptr&);
  static hedge_ptr forward_edge_without_pair(const hedge_ptr&);
  static vertex_ptr midpoint(const hedge_ptr&);
};

class FaceHelper {
 public:
  static vertex_ptr centerpoint(const face_ptr&);
};

class MeshHelper {
 public:
  static void create_face(std::vector<vertex_ptr>&, mesh_ptr&);
  static void update_bbox(const vec3d& in, vec3d* max, vec3d* min);
  static void add_vertex_to_mesh(const vertex_ptr&, mesh_ptr&);
};

Vertex operator*(const Vertex& v, double d);
Vertex operator+(const Vertex& v1, const Vertex& v2);

}  // namespace ccsubdiv

#endif
