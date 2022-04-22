// half edge structure defination

#ifndef CCSUBDIV_DATATYPE_H_
#define CCSUBDIV_DATATYPE_H_

#include <memory>
#include <vector>

#include "memchk.h"
#include "vec3.h"

namespace ccsubdiv {

typedef std::weak_ptr<struct HEdge> hedge_wptr;
typedef std::shared_ptr<struct HEdge> hedge_ptr;
typedef std::shared_ptr<struct Vertex> vertex_ptr;
typedef std::shared_ptr<struct Face> face_ptr;
typedef std::shared_ptr<struct Mesh> mesh_ptr;

struct Vertex {
  vec3d coord;
  vec3d norm;
  hedge_wptr edge;
  vertex_ptr newpoint;

  vertex_ptr avg_border_edge_midpts() const;
  size_t avg_adj_edge_midpts(vertex_ptr*) const;
  size_t avg_adj_facepts(vertex_ptr*) const;
};

struct Face {
  hedge_wptr edge;
  vertex_ptr facepoint;

  vertex_ptr centerpoint() const;
};

struct HEdge : std::enable_shared_from_this<HEdge> {
  vertex_ptr edgepoint;
  vertex_ptr vert;  // start vertex of edge
  hedge_wptr pair;
  hedge_wptr next;
  face_ptr face;

  bool is_pair_edge(const hedge_ptr&) const;
  hedge_ptr previous_edge();
  hedge_ptr backward_edge_without_pair();
  hedge_ptr forward_edge_without_pair();
  vertex_ptr midpoint() const;
};

struct Mesh {
  std::vector<vertex_ptr> vertices;
  std::vector<hedge_ptr> edges;
  std::vector<face_ptr> faces;
  // bounding box low and high coordinate
  vec3d bbox[2];

  void create_face(std::vector<vertex_ptr>&);
  void add_vertex_to_mesh(const vertex_ptr&);
  static void update_bbox(const vec3d& in, vec3d* max, vec3d* min);
};

Vertex operator*(const Vertex& v, double d);
Vertex operator+(const Vertex& v1, const Vertex& v2);

}  // namespace ccsubdiv
#endif
