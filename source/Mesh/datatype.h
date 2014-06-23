// half edge structure defination

#ifndef CCSUBDIV_DATATYPE_H_
#define CCSUBDIV_DATATYPE_H_

#include <vector>
#include <memory>

namespace ccsubdiv {

struct HEdge;

struct Vertex {
  double coord[3];
  double norm[3];

  HEdge* edge;
  Vertex* newpoint; 
};

struct Face {
  HEdge* edge;
  Vertex* facepoint;
};

struct HEdge {
  Vertex* vert; // start vertex(not end) of edge
  Vertex* edgepoint;
  HEdge* pair;
  Face* face;
  HEdge* next;
};

struct Mesh {
  std::vector< std::shared_ptr<Vertex> > vertices;
  std::vector< std::shared_ptr<HEdge> > edges;
  std::vector< std::shared_ptr<Face> > faces;
};

void ccsubdivision(Mesh&, const size_t, Mesh*);

}
#endif
