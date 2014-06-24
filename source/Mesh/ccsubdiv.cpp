#include "datatype.h"
#include <math.h>

namespace ccsubdiv {

  
vertex_handle Face::centerpoint() const {
  auto beg = edge;
  size_t sz = 0;
  vertex_handle cp(new Vertex);
  do {
    cp->coord += beg->vert->coord;
    beg = beg->next;
    ++sz;
  } while (beg != edge);
  cp->coord /= sz;
  return cp;
}

static void calc_facepoints(Mesh& input_mesh, Mesh* result_mesh) {
  for (auto& face: input_mesh.faces) {
    result_mesh->vertices.push_back(face->centerpoint()); 
    face->facepoint = result_mesh->vertices.back();
  }
}

static void calc_edgepoints(Mesh& input_mesh, Mesh* result_mesh) {
  for (auto& edge: input_mesh.edges) {
    vertex_handle vert(new Vertex);
    auto next_edge = edge->next;
    vert->coord = next_edge->vert->coord + edge->vert->coord;
    vert->coord += edge->face->facepoint->coord;
    vert->coord += edge->pair->face->facepoint->coord;
    vert->coord /= 4;
    result_mesh->vertices.push_back(vert);
    edge->edgepoint = edge->pair->edgepoint
                    = result_mesh->vertices.back();
  }
}

void Vertex::average_of_adjacent_facepoints(vertex_handle avg) const {
  auto beg = edge;
  size_t sz = 0;
  do {
    avg->coord += beg->face->facepoint->coord;
    beg = beg->pair->next;
    ++sz;
  } while(beg != edge);
  avg->coord /= sz;
}

size_t Vertex::average_of_adjacent_edgepoints(vertex_handle avg) const {
  auto beg = edge;
  size_t sz = 0;
  do {
    avg->coord += beg->edgepoint->coord;
    beg = beg->pair->next;
    ++sz;
  } while( beg != edge );
  avg->coord /= sz * 0.5;
  return sz;
}

static void calc_new_vertices(Mesh& input_mesh, Mesh* result_mesh) {
  for (auto vert: input_mesh.vertices) {
    vertex_handle new_vert(new Vertex);
    vert->average_of_adjacent_facepoints(new_vert);
    size_t sz = vert->average_of_adjacent_edgepoints(new_vert);
    new_vert->coord += vert->coord * (sz - 3);
    new_vert->coord /= sz;
    result_mesh->vertices.push_back(new_vert);
    vert->newpoint = result_mesh->vertices.back();
  }
}

static void calc_vertices(Mesh& input_mesh, Mesh* result_mesh) {
  calc_facepoints(input_mesh, result_mesh);
  calc_edgepoints(input_mesh, result_mesh);
  calc_new_vertices(input_mesh, result_mesh);
}

hedge_handle HEdge::previous_edge() const {
  auto pnext = this->next;
  hedge_handle pe;
  while (pnext.get() != this) {
    pe = pnext;
    assert(pnext);
    pnext = pnext->next;
  }
  return pe;
}

hedge_handle HEdge::last_edge_without_pair() const {
  auto pe = previous_edge();
  while (pe) {
    pe = pe->pair->previous_edge();
  }
  return pe;
}


static void split_one_face(Face& face, HEdge& edge, Mesh* mesh) {
  face_handle pf(new Face);
  hedge_handle pe1(new HEdge);
  pe1->vert = face.facepoint;
  assert(pe1->vert != nullptr);
  if (pe1->vert->edge) {
    // associate pair edges
    pe1->pair = pe1->vert->edge->last_edge_without_pair();
    pe1->pair->pair = pe1;
  }
  else {
    face.facepoint->edge = pe1;
  }
  pe1->face = pf;
  pf->edge = pe1;
  mesh->edges.push_back(pe1);

  hedge_handle pe2(new HEdge);
  pe2->vert = edge.edgepoint;
  assert(pe2->vert);
  if (pe2->vert->edge) {
    pe2->pair = pe2->vert->edge->last_edge_without_pair();
    pe2->pair->pair = pe2;
  }
  else {
    edge.edgepoint->edge = pe2;
  }
  pe2->face = pf;
  pe1->next = pe2;
  mesh->edges.push_back(pe2);

  assert(edge.next);
  hedge_handle pe3(new HEdge);
  pe3->vert = edge.next->vert;
  assert(edge.next->edgepoint);
  if (edge.next->edgepoint->edge) {
    // associate the pair half edge
    pe3->pair = edge.next->edgepoint->edge;
    pe3->pair->pair = pe3;
  }
  else {
    edge.next->edgepoint->edge = pe3;
  }
  pe3->face = pf;
  pe2->next = pe3;
  mesh->edges.push_back(pe3);

  hedge_handle pe4(new HEdge);
  pe4->vert = edge.next->edgepoint;
  if (face.edge == edge.next) {
    pe4->pair = face.facepoint->edge;
    pe4->pair->pair = pe4;
  }
  pe4->face = pf;
  pe4->next = pe1;
  pe3->next = pe4;
  mesh->edges.push_back(pe4);

  mesh->faces.push_back(pf);
}

static void connect_edges(Mesh& input_mesh,
                          Mesh* result_mesh) {
  for (auto& face : input_mesh.faces) {
    auto pe = face->edge;
    split_one_face(*face, *pe, result_mesh);
  }
}

static void ccsubdivision_core(Mesh& input_mesh,
                               Mesh* result_mesh) {
  calc_vertices(input_mesh, result_mesh); 
  connect_edges(input_mesh, result_mesh);
}


void ccsubdivision(Mesh& input_mesh, const size_t n,
                   Mesh* result_mesh) {
  std::vector< std::shared_ptr<Mesh> > meshes;
  meshes.push_back( std::make_shared<Mesh>( input_mesh ) );
  for (size_t i = 0; i < n; ++i) {
    std::shared_ptr<Mesh> sp(new Mesh());
    ccsubdivision_core(*meshes.back(), sp.get());
    meshes.push_back(sp); 
  }
}



} // namespace ccsubdiv



