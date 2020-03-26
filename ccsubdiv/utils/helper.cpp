
#include "ccsubdiv/utils/helper.h"

#include <vector>

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

bool EdgeHelper::is_pair_edge(const hedge_ptr& e1, const hedge_ptr& e2) {
  return (e1->vert->coord == e2->next.lock()->vert->coord &&
          e1->next.lock()->vert->coord == e2->vert->coord);
}

void MeshHelper::create_face(std::vector<vertex_ptr>& vertices,
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
    if (!vertices[i]->edge.expired()) {
      if (!vertices[ni]->edge.expired()) {
        auto pe =
            EdgeHelper::backward_edge_without_pair(vertices[i]->edge.lock());
        if (pe && EdgeHelper::is_pair_edge(pe, edges[i])) {
          edges[i]->pair = pe;
          pe->pair = edges[i];
        }
      }
    } else {
      vertices[i]->edge = edges[i];
    }
  }
  face->edge = edges[0];
  mesh->faces.push_back(face);
}

hedge_ptr EdgeHelper::previous_edge(const hedge_ptr& edge) {
  if (!edge) return nullptr;
  auto pe = edge;
  while (pe && pe->next.lock() != edge) {
    pe = pe->next.lock();
  }
  return pe;
}

hedge_ptr EdgeHelper::backward_edge_without_pair(const hedge_ptr& edge) {
  auto pe = previous_edge(edge);
  while (pe && !pe->pair.expired()) {
    pe = previous_edge(pe->pair.lock());
  }
  return pe;
}

hedge_ptr EdgeHelper::forward_edge_without_pair(const hedge_ptr& edge) {
  auto pe = edge;
  while (pe && !pe->pair.expired()) {
    pe = pe->pair.lock()->next.lock();
  }
  return pe;
}

vertex_ptr FaceHelper::centerpoint(const face_ptr& face) {
  auto beg = face->edge.lock();
  size_t sz = 0;
  vertex_ptr cp = std::make_shared<Vertex>();
  do {
    *cp = *cp + *beg->vert;
    beg = beg->next.lock();
    ++sz;
  } while (beg != face->edge.lock());
  *cp = *cp * (1.0 / sz);
  return cp;
}

vertex_ptr EdgeHelper::midpoint(const hedge_ptr& edge) {
  if (!edge || edge->next.expired()) return nullptr;
  auto midpnt = std::make_shared<Vertex>();
  *midpnt = (*edge->vert + *edge->next.lock()->vert) * 0.5;
  return midpnt;
}

vertex_ptr VertHelper::avg_border_edge_midpts(const vertex_ptr& vert) {
  if (vert->edge.expired()) return nullptr;
  auto edge = EdgeHelper::forward_edge_without_pair(vert->edge.lock());
  auto mp1 = EdgeHelper::midpoint(edge);
  edge = EdgeHelper::backward_edge_without_pair(vert->edge.lock());
  auto mp2 = EdgeHelper::midpoint(edge);
  assert(mp2 && mp1);
  auto avg = std::make_shared<Vertex>();
  *avg = (*mp1 + *mp2 + *vert) * (1.0 / 3.0);
  return avg;
}

size_t VertHelper::avg_adj_facepts(const vertex_ptr& vert, vertex_ptr* avg) {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = vert->edge.lock();
  do {
    **avg = **avg + *beg->face->facepoint;
    ++sz;
    if (beg->pair.expired()) {
      for (auto pre = EdgeHelper::previous_edge(vert->edge.lock());
           pre && !pre->pair.expired();
           pre = EdgeHelper::previous_edge(pre->pair.lock())) {
        **avg = **avg + *pre->pair.lock()->face->facepoint;
        ++sz;
      }
      break;
    }
    beg = beg->pair.lock()->next.lock();
  } while (beg != vert->edge.lock());

  **avg = **avg * (1.0 / sz);
  return sz;
}

size_t VertHelper::avg_adj_edge_midpts(const vertex_ptr& vert,
                                       vertex_ptr* avg) {
  *avg = std::make_shared<Vertex>();
  size_t sz = 0;

  auto beg = vert->edge.lock();
  do {
    auto& v1 = beg->vert;
    assert(!beg->next.expired());
    auto& v2 = beg->next.lock()->vert;
    **avg = **avg + ((*v1 + *v2) * 0.5);
    ++sz;
    if (beg->pair.expired()) {
      for (auto pre = EdgeHelper::previous_edge(vert->edge.lock()); pre;
           pre = EdgeHelper::previous_edge(pre->pair.lock())) {
        auto& v1 = pre->vert;
        assert(!pre->next.expired());
        auto& v2 = pre->next.lock()->vert;
        **avg = **avg + ((*v1 + *v2) * 0.5);
        ++sz;
      }
      break;
    }
    beg = beg->pair.lock()->next.lock();
  } while (beg != vert->edge.lock());
  **avg = **avg * (1.0 / sz);
  return sz;
}

void MeshHelper::update_bbox(const vec3d& in, vec3d* min, vec3d* max) {
  for (size_t i = 0; i < 3; ++i) {
    if ((*max)[i] < in[i]) {
      (*max)[i] = in[i];
    }
    if ((*min)[i] > in[i]) {
      (*min)[i] = in[i];
    }
  }
}

void MeshHelper::add_vertex_to_mesh(const vertex_ptr& vert, mesh_ptr& mesh) {
  mesh->vertices.push_back(vert);
  update_bbox(vert->coord, &mesh->bbox[0], &mesh->bbox[1]);
}

}  // namespace ccsubdiv
