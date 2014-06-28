// half edge structure defination

#ifndef CCSUBDIV_DATATYPE_H_
#define CCSUBDIV_DATATYPE_H_

#include <memory>
#include <vector>

#include "vec3.h"

namespace ccsubdiv {

struct HEdge;
struct Vertex;
struct Face;
struct Mesh;

typedef std::shared_ptr<HEdge> hedge_ptr;
typedef std::shared_ptr<Vertex> vertex_ptr;
typedef std::shared_ptr<Face> face_ptr;
typedef std::shared_ptr<Mesh> mesh_ptr;


struct Vertex {
  vec3d coord;
  vec3d norm;
  hedge_ptr edge;
  vertex_ptr newpoint;
};

struct Face {
  hedge_ptr edge;
  vertex_ptr facepoint;
};

struct HEdge {
  vertex_ptr edgepoint;
  vertex_ptr vert; // start vertex of edge
  hedge_ptr pair;
  hedge_ptr next;
  face_ptr face;
};


struct Mesh {
  std::vector< vertex_ptr > vertices;
  std::vector< hedge_ptr > edges;
  std::vector< face_ptr > faces;
};


class MeshMgr {
public:
  MeshMgr(const mesh_ptr& mesh) : current_mesh(0) {
    meshes.push_back(mesh);
  }

  mesh_ptr ccsubdiv(size_t);
  mesh_ptr previous_mesh() {
    assert(current_mesh >= 1);
    return meshes[--current_mesh];
  }
  mesh_ptr post_mesh() {
    assert(current_mesh <= meshes.size() - 1);
    return meshes[++current_mesh];
  }

private:
  MeshMgr(const MeshMgr&) = delete;
  MeshMgr& operator = (const MeshMgr&) = delete;

  void calc_facepoints();
  void calc_edgepoints();
  void calc_newpoints();
  void connect_edges();

  size_t current_mesh;
  std::vector<mesh_ptr> meshes;
};


}
#endif
