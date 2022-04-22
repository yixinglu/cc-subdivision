#include "meshmgr.h"

namespace ccsubdiv {

void MeshMgr::calc_facepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& face : meshes[current_mesh]->faces) {
    face->facepoint = face->centerpoint();
    meshes[next_mesh]->add_vertex_to_mesh(face->facepoint);
  }
}

void MeshMgr::calc_edgepoints() {
  auto next_mesh = current_mesh + 1;
  auto vert = std::make_shared<Vertex>();
  for (auto& edge : meshes[current_mesh]->edges) {
    if (edge->edgepoint) continue;
    edge->edgepoint = edge->midpoint();
    if (!edge->pair.expired()) {  // not boundary edge
      auto& fp1 = edge->face->facepoint;
      auto& fp2 = edge->pair.lock()->face->facepoint;
      *vert = (*fp1 + *fp2) * 0.5;
      *edge->edgepoint = (*edge->edgepoint + *vert) * 0.5;
      edge->pair.lock()->edgepoint = edge->edgepoint;
    }

    meshes[next_mesh]->add_vertex_to_mesh(edge->edgepoint);
  }
}

void MeshMgr::calc_newpoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& vert : meshes[current_mesh]->vertices) {
    vertex_ptr avg_adj_fp, avg_adj_ep;
    auto sz = vert->avg_adj_facepts(&avg_adj_fp);
    auto sz2 = vert->avg_adj_edge_midpts(&avg_adj_ep);

    if (sz == sz2) {
      double m1 = 1.0 / sz;
      double m2 = m1 + m1;
      double m3 = 1.0 - m1 - m2;
      vert->newpoint = std::make_shared<Vertex>();
      *vert->newpoint = (*vert * m3) + (*avg_adj_ep * m2) + (*avg_adj_fp * m1);
    } else {  // boundary vertex
      vert->newpoint = vert->avg_border_edge_midpts();
    }

    meshes[next_mesh]->add_vertex_to_mesh(vert->newpoint);
  }
}

void MeshMgr::connect_edges() {
  for (auto& face : meshes[current_mesh]->faces) {
    hedge_ptr pe = face->edge.lock();
    do {
      std::vector<vertex_ptr> vertices;
      vertices.push_back(face->facepoint);
      vertices.push_back(pe->edgepoint);
      pe = pe->next.lock();
      vertices.push_back(pe->vert->newpoint);
      vertices.push_back(pe->edgepoint);
      meshes[current_mesh + 1]->create_face(vertices);
    } while (pe != face->edge.lock());
  }
}

mesh_ptr MeshMgr::ccsubdiv(size_t n) {
  if (!has_mesh()) return nullptr;

  if (current_mesh + n < meshes.size()) {
    current_mesh += n;
    return meshes[current_mesh];
  }

  size_t sz = current_mesh + n + 1;
  meshes.reserve(sz);
  for (current_mesh = meshes.size() - 1; current_mesh < sz - 1;
       ++current_mesh) {
    meshes.push_back(std::make_shared<Mesh>());
    calc_facepoints();
    calc_edgepoints();
    calc_newpoints();
    connect_edges();
  }
  return meshes[current_mesh];
}

}  // namespace ccsubdiv
