// half edge structure defination

#ifndef CCSUBDIV_DATATYPE_H_
#define CCSUBDIV_DATATYPE_H_

#include <memory>
#include <vector>

#include "mathutil.h"

namespace ccsubdiv {

class HEdge;
class Vertex;
class Face;
class Mesh;

typedef std::shared_ptr<HEdge> hedge_handle;
typedef std::shared_ptr<Vertex> vertex_handle;
typedef std::shared_ptr<Face> face_handle;


class Vertex {
public:
  Vertex() : coord(), norm(),
    edge(nullptr), newpoint(nullptr) {}

  void average_of_adjacent_facepoints(vertex_handle) const;
  size_t average_of_adjacent_edgepoints(vertex_handle) const;

  vec3d coord;
  vec3d norm;

  hedge_handle edge;
  vertex_handle newpoint;
};

class Face {
public:
  Face() : edge(nullptr), facepoint(nullptr) {}

  vertex_handle centerpoint() const;
  void split_by_edge(hedge_handle edge, Mesh* mesh);

  hedge_handle edge;
  vertex_handle facepoint;
};

class HEdge {
public:
  HEdge() : vert(nullptr), edgepoint(nullptr),
    pair(nullptr), next(nullptr), face(nullptr) {}

  hedge_handle previous_edge() const;
  hedge_handle last_edge_without_pair() const;

  vertex_handle vert; // start vertex(not end) of edge
  vertex_handle edgepoint;
  hedge_handle pair;
  hedge_handle next;
  face_handle face;
};

class Mesh {
public:
  std::vector< vertex_handle > vertices;
  std::vector< hedge_handle > edges;
  std::vector< face_handle > faces;
};

void ccsubdivision(Mesh&, const size_t, Mesh*);

}
#endif
