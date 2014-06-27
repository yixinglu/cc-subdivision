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
  vertex_ptr vert; // start vertex(not end) of edge
  vertex_ptr edgepoint;
  hedge_ptr pair;
  hedge_ptr next;
  face_ptr face;
};


struct Mesh {
  std::vector< vertex_ptr > vertices;
  std::vector< hedge_ptr > edges;
  std::vector< face_ptr > faces;
};

//class MeshMgr {
//public:
//
//};

void ccsubdivision(mesh_ptr, const size_t, mesh_ptr);

}
#endif
