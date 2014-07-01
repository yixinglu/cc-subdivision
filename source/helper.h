#ifndef CCSUBDIV_HELPER_H
#define CCSUBDIV_HELPER_H

#include <vector>
#include "datatype.h"

namespace ccsubdiv {


class SubdivHelper {
public:
  static bool is_pair_edge(const hedge_ptr&, const hedge_ptr&);
  static void create_face(std::vector<vertex_ptr>&, mesh_ptr&);
  static hedge_ptr previous_edge(const hedge_ptr&);
  static hedge_ptr backward_edge_without_pair(const hedge_ptr&);
  static hedge_ptr forward_edge_without_pair(const hedge_ptr&);
  static vertex_ptr face_centerpoint(const face_ptr&);
  static vertex_ptr edge_midpoint(const hedge_ptr&);
  static vertex_ptr average_border_edge_midpoints(const vertex_ptr&);
  static size_t average_mid_edges(const vertex_ptr& , vertex_ptr*);
  static size_t average_facepoints(const vertex_ptr&, vertex_ptr*);
  static void boundingbox_xyz(const vec3d& in, vec3d* max, vec3d* min);
};

}


#endif
