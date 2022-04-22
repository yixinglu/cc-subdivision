#include "datatype.h"

namespace ccsubdiv {

Vertex operator+(const Vertex& v1, const Vertex& v2) {
  Vertex ret;
  ret.coord = v1.coord + v2.coord;
  ret.norm = v1.norm + v2.norm;
  return ret;
}

Vertex operator*(const Vertex& v, double d) {
  Vertex ret(v);
  ret.coord *= d;
  ret.norm *= d;
  return ret;
}

bool HEdge::is_pair_edge(const hedge_ptr& e1) const {
  return (e1->vert->coord == next.lock()->vert->coord &&
          e1->next.lock()->vert->coord == vert->coord);
}

void Mesh::create_face(std::vector<vertex_ptr>& vertices) {
  if (vertices.empty()) return;

  face_ptr face = std::make_shared<Face>();
  const size_t sz = vertices.size();
  std::vector<hedge_ptr> edges(sz);

  edges.reserve(edges.size() + sz);
  for (size_t i = 0; i < sz; ++i) {
    edges[i] = std::make_shared<HEdge>();
    edges[i]->vert = vertices[i];
    edges[i]->face = face;
    edges.push_back(edges[i]);
  }

  for (size_t i = 0; i < sz; ++i) {
    auto ni = (i + 1) % sz;
    edges[i]->next = edges[ni];
    if (!vertices[i]->edge.expired()) {
      if (!vertices[ni]->edge.expired()) {
        auto pe = vertices[i]->edge.lock()->backward_edge_without_pair();
        if (pe && pe->is_pair_edge(edges[i])) {
          edges[i]->pair = pe;
          pe->pair = edges[i];
        }
      }
    } else {
      vertices[i]->edge = edges[i];
    }
  }
  face->edge = edges[0];
  faces.push_back(face);
}

hedge_ptr HEdge::previous_edge() {
  auto pe = shared_from_this();
  while (pe && pe->next.lock().get() != this) {
    pe = pe->next.lock();
  }
  return pe;
}

hedge_ptr HEdge::backward_edge_without_pair() {
  auto pe = previous_edge();
  while (pe && !pe->pair.expired()) {
    pe = pe->pair.lock()->previous_edge();
  }
  return pe;
}

hedge_ptr HEdge::forward_edge_without_pair() {
  auto pe = shared_from_this();
  while (pe && !pe->pair.expired()) {
    pe = pe->pair.lock()->next.lock();
  }
  return pe;
}

vertex_ptr Face::centerpoint() const {
  auto beg = edge.lock();
  size_t sz = 0;
  vertex_ptr cp = std::make_shared<Vertex>();
  do {
    *cp = *cp + *beg->vert;
    beg = beg->next.lock();
    ++sz;
  } while (beg != edge.lock());
  *cp = *cp * (1.0 / sz);
  return cp;
}

vertex_ptr HEdge::midpoint() const {
  if (next.expired()) return nullptr;
  auto midpnt = std::make_shared<Vertex>();
  *midpnt = (*vert + *next.lock()->vert) * 0.5;
  return midpnt;
}

vertex_ptr Vertex::avg_border_edge_midpts() const {
  if (this->edge.expired()) return nullptr;
  auto edge = this->edge.lock()->forward_edge_without_pair();
  auto mp1 = edge->midpoint();
  edge = this->edge.lock()->backward_edge_without_pair();
  auto mp2 = edge->midpoint();
  assert(mp2 && mp1);
  auto avg = std::make_shared<Vertex>();
  *avg = (*mp1 + *mp2 + *this) * (1.0 / 3.0);
  return avg;
}

size_t Vertex::avg_adj_facepts(vertex_ptr* avg) const {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = this->edge.lock();
  do {
    **avg = **avg + *beg->face->facepoint;
    ++sz;
    if (beg->pair.expired()) {
      for (auto pre = this->edge.lock()->previous_edge();
           pre && !pre->pair.expired();
           pre = pre->pair.lock()->previous_edge()) {
        **avg = **avg + *pre->pair.lock()->face->facepoint;
        ++sz;
      }
      break;
    }
    beg = beg->pair.lock()->next.lock();
  } while (beg != this->edge.lock());

  **avg = **avg * (1.0 / sz);
  return sz;
}

size_t Vertex::avg_adj_edge_midpts(vertex_ptr* avg) const {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = this->edge.lock();
  do {
    auto& v1 = beg->vert;
    assert(!beg->next.expired());
    auto& v2 = beg->next.lock()->vert;
    **avg = **avg + ((*v1 + *v2) * 0.5);
    ++sz;
    if (beg->pair.expired()) {
      for (auto pre = this->edge.lock()->previous_edge(); pre;
           pre = pre->pair.lock()->previous_edge()) {
        auto& v1 = pre->vert;
        assert(!pre->next.expired());
        auto& v2 = pre->next.lock()->vert;
        **avg = **avg + ((*v1 + *v2) * 0.5);
        ++sz;
      }
      break;
    }
    beg = beg->pair.lock()->next.lock();
  } while (beg != this->edge.lock());
  **avg = **avg * (1.0 / sz);
  return sz;
}

void Mesh::update_bbox(const vec3d& in, vec3d* min, vec3d* max) {
  for (size_t i = 0; i < 3; ++i) {
    if ((*max)[i] < in[i]) {
      (*max)[i] = in[i];
    }
    if ((*min)[i] > in[i]) {
      (*min)[i] = in[i];
    }
  }
}

void Mesh::add_vertex_to_mesh(const vertex_ptr& vert) {
  vertices.push_back(vert);
  update_bbox(vert->coord, &bbox[0], &bbox[1]);
}

}  // namespace ccsubdiv
