
#include "helper.h"


namespace ccsubdiv {


Vertex operator + (const Vertex& v1, const Vertex& v2) {
  Vertex ret;
  ret.coord = v1.coord + v2.coord;
  ret.norm = v1.norm + v2.norm;
  return ret;
}

Vertex operator * (const Vertex& v, double d) {
  Vertex ret(v);
  ret.coord *= d;
  ret.norm *= d;
  return ret;
}



bool Helper::is_pair_edge(const hedge_ptr& e1,
                          const hedge_ptr& e2) {
  return (e1->vert->coord == e2->next->vert->coord
    && e1->next->vert->coord == e2->vert->coord);
}


void Helper::create_face(std::vector<vertex_ptr>& vertices,
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


hedge_ptr Helper::previous_edge(const hedge_ptr& edge) {
  if (!edge) return nullptr;
  auto pe = edge;
  while (pe && pe->next != edge) {
    pe = pe->next;
  }
  return pe;
}

hedge_ptr Helper::backward_edge_without_pair(const hedge_ptr& edge) {
  auto pe = previous_edge(edge);
  while (pe && pe->pair) {
    pe = previous_edge(pe->pair);
  }
  return pe;
}

hedge_ptr Helper::forward_edge_without_pair(const hedge_ptr& edge) {
  auto pe = edge;
  while (pe && pe->pair) {
    pe = pe->pair->next;
  }
  return pe;
}


vertex_ptr Helper::face_centerpoint(const face_ptr& face) {
  auto beg = face->edge;
  size_t sz = 0;
  vertex_ptr cp = std::make_shared<Vertex>();
  do {
    *cp = *cp + *beg->vert;
    beg = beg->next;
    ++sz;
  } while (beg != face->edge);
  *cp = *cp * (1.0 / sz);
  return cp;
}


vertex_ptr Helper::edge_midpoint(const hedge_ptr& edge) {
  if (!edge || !edge->next) return nullptr;
  auto midpnt = std::make_shared<Vertex>();
  *midpnt = (*edge->vert + *edge->next->vert) * 0.5;
  return midpnt;
}


vertex_ptr Helper::average_border_edge_midpoints(const vertex_ptr& vert) {
  if (!vert->edge) return nullptr;
  auto edge = forward_edge_without_pair(vert->edge);
  auto mp1 = edge_midpoint(edge);
  edge = backward_edge_without_pair(vert->edge);
  auto mp2 = edge_midpoint(edge);
  assert(mp2 && mp1);
  auto avg = std::make_shared<Vertex>();
  *avg = (*mp1 + *mp2 + *vert) * (1.0 / 3.0);
  return avg;
}


size_t Helper::average_facepoints(const vertex_ptr& vert,
                                  vertex_ptr* avg) {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = vert->edge;
  do {
    **avg = **avg + *beg->face->facepoint;
    ++sz;
    if (!beg->pair) {
      for (auto pre = previous_edge(vert->edge);
        pre && pre->pair; pre = previous_edge(pre->pair)) {
        **avg = **avg + *pre->pair->face->facepoint;
        ++sz;
      }
      break;
    }
    beg = beg->pair->next;
  } while (beg != vert->edge);

  **avg = **avg * (1.0 / sz);
  return sz;
}

size_t Helper::average_mid_edges(const vertex_ptr& vert,
                                 vertex_ptr* avg) {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = vert->edge;
  do {
    auto& v1 = beg->vert;
    assert(beg->next);
    auto& v2 = beg->next->vert;
    **avg = **avg + ((*v1 + *v2) * 0.5);
    ++sz;
    if (!beg->pair) {
      for (auto pre = previous_edge(vert->edge);
           pre ; pre = previous_edge(pre->pair)) {
        auto& v1 = pre->vert;
        assert(pre->next);
        auto& v2 = pre->next->vert;
        **avg =**avg + ((*v1 + *v2) * 0.5);
        ++sz;
      }
      break;
    }
    beg = beg->pair->next;
  } while (beg != vert->edge);
  **avg = **avg * (1.0 / sz);
  return sz;
}

void Helper::update_bbox(const vec3d& in, vec3d* min, vec3d* max) {
  for (size_t i = 0; i < 3; ++i) {
    if ((*max)[i] < in[i]) {
      (*max)[i] = in[i];
    }
    if ((*min)[i] > in[i]) {
      (*min)[i] = in[i];
    }
  }
}

void Helper::add_vertex_to_mesh(const vertex_ptr& vert,
                                mesh_ptr& mesh) {
  mesh->vertices.push_back(vert);
  update_bbox(vert->coord, &mesh->bbox[0], &mesh->bbox[1]);
}


}

