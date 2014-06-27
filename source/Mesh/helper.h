#ifndef CCSUBDIV_HELPER_H
#define CCSUBDIV_HELPER_H

#include <vector>
#include "datatype.h"

namespace ccsubdiv {


class SubdivHelper {
public:
  static bool is_edge_endpt(const hedge_ptr&, const vertex_ptr&, const vertex_ptr&);
  static void create_face(const std::vector<vertex_ptr>&, mesh_ptr&);
  static hedge_ptr previous_edge(const hedge_ptr&);
  static hedge_ptr last_edge_without_pair(const hedge_ptr&);
  static vertex_ptr centerpoint(const face_ptr&);
  static void split_face_by_edge(const face_ptr&, hedge_ptr, mesh_ptr);
  static size_t average_of_adjacent_edgepoints(const vertex_ptr& vert, vertex_ptr avg);
  static void average_of_adjacent_facepoints(const vertex_ptr&, vertex_ptr);
};

}


#endif
