
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
        auto pe = backward_edge_without_pair(vertices[i]->edge);
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

hedge_ptr SubdivHelper::backward_edge_without_pair(const hedge_ptr& edge) {
  auto pe = previous_edge(edge);
  while (pe && pe->pair) {
    pe = previous_edge(pe->pair);
  }
  return pe;
}

hedge_ptr SubdivHelper::forward_edge_without_pair(const hedge_ptr& edge) {
  auto pe = edge;
  while (pe && pe->pair) {
    pe = pe->pair->next;
  }
  return pe;
}


vertex_ptr SubdivHelper::face_centerpoint(const face_ptr& face) {
  auto beg = face->edge;
  size_t sz = 0;
  vertex_ptr cp = std::make_shared<Vertex>();
  do {
    cp->coord += beg->vert->coord;
    cp->norm += beg->vert->norm;
    beg = beg->next;
    ++sz;
  } while (beg != face->edge);
  cp->coord /= sz;
  cp->norm /= sz;
  return cp;
}


vertex_ptr SubdivHelper::edge_midpoint(const hedge_ptr& edge) {
  if (!edge || !edge->next) return nullptr;
  auto midpnt = std::make_shared<Vertex>();
  auto& v1 = edge->vert;
  auto& v2 = edge->next->vert;
  midpnt->coord = (v1->coord + v2->coord)*0.5;
  midpnt->norm = (v1->norm + v2->norm)*0.5;
  return midpnt;
}


vertex_ptr SubdivHelper::average_border_edge_midpoints(
  const vertex_ptr& vert) {
  if (!vert->edge) return nullptr;
  auto edge = forward_edge_without_pair(vert->edge);
  auto mp1 = edge_midpoint(edge);
  edge = backward_edge_without_pair(vert->edge);
  auto mp2 = edge_midpoint(edge);
  assert(mp2 && mp1);
  auto avg = std::make_shared<Vertex>();
  avg->coord = mp1->coord + mp2->coord + vert->coord;
  avg->coord /= 3.0;
  avg->norm = mp1->norm + mp2->norm + vert->norm;
  avg->norm /= 3.0;
  return avg;
}


size_t SubdivHelper::average_facepoints(const vertex_ptr& vert,
                                        vertex_ptr* avg) {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = vert->edge;
  do {
    (*avg)->coord += beg->face->facepoint->coord;
    (*avg)->norm += beg->face->facepoint->norm;
    ++sz;
    if (!beg->pair) {
      for (auto pre = previous_edge(vert->edge);
        pre && pre->pair; pre = previous_edge(pre->pair)) {
        (*avg)->coord += pre->pair->face->facepoint->coord;
        (*avg)->norm += pre->pair->face->facepoint->norm;
        ++sz;
      }
      break;
    }
    beg = beg->pair->next;
  } while (beg != vert->edge);

  (*avg)->coord /= sz;
  (*avg)->norm /= sz;
  return sz;
}

size_t SubdivHelper::average_mid_edges(const vertex_ptr& vert,
                                       vertex_ptr* avg) {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = vert->edge;
  do {
    auto& v1 = beg->vert;
    assert(beg->next);
    auto& v2 = beg->next->vert;
    (*avg)->coord += (v1->coord + v2->coord) * 0.5;
    (*avg)->norm += (v1->norm + v2->norm) * 0.5;
    ++sz;
    if (!beg->pair) {
      for (auto pre = previous_edge(vert->edge);
        pre ; pre = previous_edge(pre->pair)) {
        auto& v1 = pre->vert;
        assert(pre->next);
        auto& v2 = pre->next->vert;
        (*avg)->coord += (v1->coord + v2->coord) * 0.5;
        (*avg)->norm += (v1->norm + v2->norm) * 0.5;
        ++sz;
      }
      break;
    }
    beg = beg->pair->next;
  } while (beg != vert->edge);
  (*avg)->coord /= sz;
  (*avg)->norm /= sz;
  return sz;
}

void SubdivHelper::boundingbox_xyz(const vec3d& in,
                                   vec3d* min, vec3d* max) {
  for (size_t i = 0; i < 3; ++i) {
    if ((*max)[i] < in[i]) {
      (*max)[i] = in[i];
    }
    if ((*min)[i] > in[i]) {
      (*min)[i] = in[i];
    }
  }
}


}

