#include "datatype.h"
#include <assert.h>
#include <math.h>

namespace ccsubdiv {

static void add_vec3(const double v1[3], const double v2[3], double ret[3])
{
  ret[0] = v1[0] + v2[0];
  ret[1] = v1[1] + v2[1];
  ret[2] = v1[2] + v2[2];
}

static void div_vec3(const double v[3], const double d, double ret[3]) {
  if (abs(d) > 1.0e-05) {
    ret[0] = v[0] / d;
    ret[1] = v[1] / d;
    ret[2] = v[2] / d;
  }
}
  
static void calc_face_centerpoint(const Face& face, Vertex* vertex) {
  HEdge* beg = face.edge;
  HEdge* end = beg;
  size_t sz = 0;
  do {
    add_vec3(vertex->coord, beg->vert->coord, vertex->coord);
    beg = beg->next;
    ++sz;
  } while (beg != end);
  div_vec3(vertex->coord, sz, vertex->coord);
}

static void calc_facepoints(Mesh& input_mesh, Mesh* result_mesh)
{
  for (auto& iter: input_mesh.faces) {
    std::shared_ptr<Vertex> vert(new Vertex);
    calc_face_centerpoint(*iter, vert.get());
    result_mesh->vertices.push_back(vert); 
    iter->facepoint = result_mesh->vertices.back().get();
  }
}

static void calc_edgepoints(Mesh& input_mesh, Mesh* result_mesh)
{
  for (auto& iter: input_mesh.edges) {
    std::shared_ptr<Vertex> vert(new Vertex);
    HEdge* another = iter->next;
    add_vec3(another->vert->coord, iter->vert->coord, vert->coord);
    add_vec3(vert->coord, iter->face->facepoint->coord, vert->coord);
    add_vec3(vert->coord, iter->pair->face->facepoint->coord, vert->coord);
    div_vec3(vert->coord, 4, vert->coord);
    result_mesh->vertices.push_back(vert);
    iter->edgepoint = iter->pair->edgepoint = result_mesh->vertices.back().get();
  }
}

static void reset_coord(Vertex& vert) {
  vert.coord[0] = vert.coord[1] = vert.coord[2];
}

static void average_of_adjacent_facepoints(const Vertex& vert,
                                           Vertex* result) {
  HEdge* beg = vert.edge;
  HEdge* end = beg;
  size_t sz = 0;
  do {
    add_vec3(result->coord, beg->face->facepoint->coord,result->coord);
    beg = beg->pair->next;
    ++sz;
  } while(beg != end);

  div_vec3(result->coord, sz, result->coord);
}

static void average_of_adjacent_edgepoints(const Vertex& vert,
                                           Vertex* result, size_t* sz)
{
  HEdge* beg = vert.edge;
  HEdge* end = beg;
  *sz = 0;
  do {
    add_vec3(result->coord, beg->edgepoint->coord, result->coord);
    beg = beg->pair->next;
    ++*sz;
  } while( beg != end );
  div_vec3(result->coord, *sz * 0.5, result->coord);
}

static void calc_new_vertices(Mesh& input_mesh, Mesh* result_mesh) {
  for (auto iter: input_mesh.vertices) {
    std::shared_ptr<Vertex> vert(new Vertex);
    size_t sz = 0;
    reset_coord(*vert); 
    average_of_adjacent_facepoints(*iter, vert.get());
    average_of_adjacent_edgepoints(*iter, vert.get(), &sz); 
    double temp[3];
    assert(sz != 3);
    div_vec3(iter->coord, 1.0/(sz-3), temp);
    add_vec3(vert->coord, temp, vert->coord);
    div_vec3(vert->coord, sz, vert->coord);
    result_mesh->vertices.push_back(vert);
    iter->newpoint = result_mesh->vertices.back().get(); 
  }
}

static void calc_vertices(Mesh& input_mesh,
                          Mesh* result_mesh) {
  calc_facepoints(input_mesh, result_mesh);
  calc_edgepoints(input_mesh, result_mesh);
  calc_new_vertices(input_mesh, result_mesh);
}

static const HEdge* get_previous_edge(const HEdge& edge) {
  const HEdge* pe = &edge;
  const HEdge* pnext = pe->next;
  while (pnext != &edge) {
    pe = pnext;
    assert(pnext != nullptr);
    pnext = pnext->next;
  }
  return pe;
}

static const HEdge* get_last_edge_without_pair(const HEdge& edge) {
  auto pe = get_previous_edge(edge);
  while (pe != nullptr) {
    pe = get_previous_edge( *pe->pair );
  }
  return pe;
}


static void split_one_face(Face& face, HEdge& edge, Mesh* mesh) {
  std::shared_ptr<Face> pf(new Face);
  std::shared_ptr<HEdge> pe1(new HEdge);
  pe1->vert = face.facepoint;
  assert(pe1->vert != nullptr);
  if (pe1->vert->edge) {
    // associate pair edges
    pe1->pair = const_cast<HEdge*>(
      get_last_edge_without_pair(*pe1->vert->edge));
    pe1->pair->pair = pe1.get();
  }
  else {
    face.facepoint->edge = pe1.get();
  }
  pe1->face = pf.get();
  pf->edge = pe1.get();
  mesh->edges.push_back(pe1);

  std::shared_ptr<HEdge> pe2(new HEdge);
  pe2->vert = edge.edgepoint;
  assert(pe2->vert != nullptr);
  if (pe2->vert->edge) {
    pe2->pair = const_cast<HEdge*>(
      get_last_edge_without_pair(*pe2->vert->edge));
    pe2->pair->pair = pe2.get();
  }
  else {
    edge.edgepoint->edge = pe2.get();
  }
  pe2->face = pf.get();
  pe1->next = pe2.get();
  mesh->edges.push_back(pe2);

  assert(edge.next != nullptr);
  std::shared_ptr<HEdge> pe3(new HEdge);
  pe3->vert = edge.next->vert;
  assert(edge.next->edgepoint != nullptr);
  if (edge.next->edgepoint->edge) {
    // associate the pair half edge
    pe3->pair = edge.next->edgepoint->edge;
    pe3->pair->pair = pe3.get();
  }
  else {
    edge.next->edgepoint->edge = pe3.get();
  }
  pe3->face = pf.get();
  pe2->next = pe3.get();
  mesh->edges.push_back(pe3);

  std::shared_ptr<HEdge> pe4(new HEdge);
  pe4->vert = edge.next->edgepoint;
  if (face.edge == edge.next) {
    pe4->pair = face.facepoint->edge;
    pe4->pair->pair = pe4.get();
  }
  pe4->face = pf.get();
  pe4->next = pe1.get();
  pe3->next = pe4.get();
  mesh->edges.push_back(pe4);

  mesh->faces.push_back(pf);
}

static void connect_edges(Mesh& input_mesh,
                          Mesh* result_mesh) {
  for (auto& face : input_mesh.faces) {
    HEdge* pe = face->edge;
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


using namespace ccsubdiv;
int main() {
  Mesh mesh;
  ccsubdivision(mesh, 1, &mesh); 

  return 0;
}
