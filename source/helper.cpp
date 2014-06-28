
#include <vector>
#include "helper.h"


namespace ccsubdiv {

bool SubdivHelper::is_pair_edge(const hedge_ptr& e1,
                                const hedge_ptr& e2) {
  return (e1->vert->coord == e2->next->vert->coord
    && e1->next->vert->coord == e2->vert->coord);
    
}


void SubdivHelper::create_face(std::vector<vertex_ptr>& vertices,
                               mesh_ptr& mesh) {
  if (vertices.empty()) return;

  face_ptr face = std::make_shared<Face>();
  const size_t sz = vertices.size();
  std::vector<hedge_ptr> edges(sz);

  mesh->edges.reserve(mesh->edges.size() + sz);
  for (size_t i = 0; i < sz; ++i) {
    edges[i] = std::make_shared<HEdge>();
    edges[i]->vert = vertices[i];
    edges[i]->face = face;
    mesh->edges.push_back(edges[i]);
  }

  for (size_t i = 0; i < sz; ++i) {
    auto ni = (i + 1) % sz;
    edges[i]->next = edges[ni];
    if (vertices[i]->edge) {
      if (vertices[ni]->edge) {
        auto pe = last_edge_without_pair(vertices[i]->edge);
        if (pe && is_pair_edge(pe, edges[i])) {
          edges[i]->pair = pe;
          pe->pair = edges[i];
        }
      }
    }
    else {
      vertices[i]->edge = edges[i];
    }
  }
  face->edge = edges[0];
  mesh->faces.push_back(face);
}


hedge_ptr SubdivHelper::previous_edge(const hedge_ptr& edge) {
  if (!edge) return nullptr;
  auto pe = edge;
  while (pe && pe->next != edge) {
    pe = pe->next;
  }
  return pe;
}

hedge_ptr SubdivHelper::last_edge_without_pair(const hedge_ptr& edge) {
  auto pe = previous_edge(edge);
  while (pe && pe->pair) {
    pe = previous_edge(pe->pair);
  }
  return pe;
}


vertex_ptr SubdivHelper::centerpoint(const face_ptr& face) {
  auto beg = face->edge;
  size_t sz = 0;
  vertex_ptr cp = std::make_shared<Vertex>();
  do {
    cp->coord += beg->vert->coord;
    beg = beg->next;
    ++sz;
  } while (beg != face->edge);
  cp->coord /= sz;
  return cp;
}


void SubdivHelper::average_of_adjacent_facepoints(const vertex_ptr& vert, vertex_ptr avg) {
  auto beg = vert->edge;
  size_t sz = 0;
  do {
    avg->coord += beg->face->facepoint->coord;
    beg = beg->pair->next;
    ++sz;
  } while (beg != vert->edge);
  avg->coord /= sz;
}

size_t SubdivHelper::average_of_adjacent_edgepoints(const vertex_ptr& vert, vertex_ptr avg) {
  auto beg = vert->edge;
  size_t sz = 0;
  do {
    avg->coord += beg->edgepoint->coord;
    beg = beg->pair->next;
    ++sz;
  } while (beg != vert->edge);
  avg->coord /= sz * 0.5;
  return sz;
}


}

