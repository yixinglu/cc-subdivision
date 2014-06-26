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

typedef std::shared_ptr<HEdge> hedge_handle;
typedef std::shared_ptr<Vertex> vertex_handle;
typedef std::shared_ptr<Face> face_handle;
typedef std::shared_ptr<Mesh> mesh_handle;


struct Vertex {
  vec3d coord;
  vec3d norm;
  hedge_handle edge;
  vertex_handle newpoint;
};

struct Face {
  hedge_handle edge;
  vertex_handle facepoint;
};

struct HEdge {
  vertex_handle vert; // start vertex(not end) of edge
  vertex_handle edgepoint;
  hedge_handle pair;
  hedge_handle next;
  face_handle face;
};


struct Mesh {
  std::vector< vertex_handle > vertices;
  std::vector< hedge_handle > edges;
  std::vector< face_handle > faces;
};

//class MeshMgr {
//public:
//
//};

void ccsubdivision(mesh_handle, const size_t, mesh_handle);

}
#endif
