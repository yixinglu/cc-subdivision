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
};

struct Face {
  hedge_wptr edge;
  vertex_ptr facepoint;
};

struct HEdge {
  vertex_ptr edgepoint;
  vertex_ptr vert; // start vertex of edge
  hedge_wptr pair;
  hedge_wptr next;
  face_ptr face;
};


struct Mesh {
  std::vector< vertex_ptr > vertices;
  std::vector< hedge_ptr > edges;
  std::vector< face_ptr > faces;
  // bounding box low and high coordinate
  vec3d bbox[2];
};


class MeshMgr {
public:
  MeshMgr(const mesh_ptr& mesh) : current_mesh(0) {
    meshes.push_back(mesh);
  }

  mesh_ptr ccsubdiv(size_t = 1);
  mesh_ptr previous_mesh() {
    if (current_mesh > 0) {
      return meshes[--current_mesh];
    }
    return meshes[0];
  }
  mesh_ptr post_mesh() {
    if (current_mesh < meshes.size()-1) {
      return meshes[++current_mesh];
    }
    return ccsubdiv(1);
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
