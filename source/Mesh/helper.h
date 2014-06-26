#ifndef CCSUBDIV_HELPER_H
#define CCSUBDIV_HELPER_H

#include <vector>
#include "datatype.h"

namespace ccsubdiv {


class SubdivHelper {
public:
  static void create_face(const std::vector<vertex_handle>&, mesh_handle&);
  static hedge_handle previous_edge(const hedge_handle&);
  static hedge_handle last_edge_without_pair(const hedge_handle&);
  static vertex_handle centerpoint(const face_handle&);
  static void split_face_by_edge(const face_handle&, hedge_handle, mesh_handle);
  static size_t average_of_adjacent_edgepoints(const vertex_handle& vert, vertex_handle avg);
  static void average_of_adjacent_facepoints(const vertex_handle&, vertex_handle);
};

}


#endif
