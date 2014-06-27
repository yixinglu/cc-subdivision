
#include <vector>
#include "helper.h"


namespace ccsubdiv {


bool SubdivHelper::is_edge_endpt(const hedge_ptr& edge, const vertex_ptr& v1,
                                 const vertex_ptr& v2) {
  return (edge && edge->vert == v1 && edge->next && edge->next->vert == v2)
    || (edge && edge->vert == v2 && edge->next && edge->next->vert == v1);
}

void SubdivHelper::create_face(const std::vector<vertex_ptr>& vertices,
                               mesh_ptr& mesh) {
  if (vertices.empty()) return;

  face_ptr face = std::make_shared<Face>();
  std::vector<hedge_ptr> edges(vertices.size());
  for (auto& edge : edges) {
    edge = std::make_shared<HEdge>();
  }
  for (size_t i = 0, sz = vertices.size(); i < sz; ++i) {
    auto ni = (i + 1) % sz;
    if (vertices[i]->edge) {
      if (vertices[ni]->edge) {/*
        if (is_edge_endpt(vertices[i]->edge, vertices[i], vertices[ni])) {
          edges[i]->pair = vertices[i]->edge;
          vertices[i]->edge->pair = edges[i];
        }
        else {*/
          edges[i]->pair = last_edge_without_pair(vertices[i]->edge);
          if (edges[i]->pair != nullptr) {
            edges[i]->pair->pair = edges[i];
          //}
        }

      }
    }
    else {
      vertices[i]->edge = edges[i];
    }
    edges[i]->vert = vertices[i];
    edges[i]->face = face;
    edges[i]->next = edges[ni];
  }
  face->edge = edges[0];
  mesh->edges.insert(mesh->edges.end(), edges.begin(), edges.end());
  mesh->faces.push_back(face);
}


hedge_ptr SubdivHelper::previous_edge(const hedge_ptr& edge) {
  if (edge == nullptr) return nullptr;
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



void SubdivHelper::split_face_by_edge(const face_ptr& face, 
                                      hedge_ptr edge, mesh_ptr mesh) {
  face_ptr pf = std::make_shared<Face>();
  hedge_ptr pe1 = std::make_shared<HEdge>();
  pe1->vert = face->facepoint;
  assert(pe1->vert != nullptr);
  if (pe1->vert->edge) {
    // associate pair edges
    pe1->pair = last_edge_without_pair(pe1->vert->edge);
    pe1->pair->pair = pe1;
  }
  else {
    face->facepoint->edge = pe1;
  }
  pe1->face = pf;
  pf->edge = pe1;
  mesh->edges.push_back(pe1);

  hedge_ptr pe2 = std::make_shared<HEdge>();
  pe2->vert = edge->edgepoint;
  assert(pe2->vert);
  if (pe2->vert->edge) {
    pe2->pair = last_edge_without_pair(pe2->vert->edge);
    pe2->pair->pair = pe2;
  }
  else {
    edge->edgepoint->edge = pe2;
  }
  pe2->face = pf;
  pe1->next = pe2;
  mesh->edges.push_back(pe2);

  assert(edge->next);
  hedge_ptr pe3 = std::make_shared<HEdge>();
  pe3->vert = edge->next->vert;
  assert(edge->next->edgepoint);
  if (edge->next->edgepoint->edge) {
    // associate the pair half edge
    pe3->pair = edge->next->edgepoint->edge;
    pe3->pair->pair = pe3;
  }
  else {
    edge->next->edgepoint->edge = pe3;
  }
  pe3->face = pf;
  pe2->next = pe3;
  mesh->edges.push_back(pe3);

  hedge_ptr pe4 = std::make_shared<HEdge>();
  pe4->vert = edge->next->edgepoint;
  if (edge == edge->next) {
    pe4->pair = face->facepoint->edge;
    pe4->pair->pair = pe4;
  }
  pe4->face = pf;
  pe4->next = pe1;
  pe3->next = pe4;
  mesh->edges.push_back(pe4);

  mesh->faces.push_back(pf);
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

