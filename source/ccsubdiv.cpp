#include "datatype.h"
#include "helper.h"


namespace ccsubdiv {


void MeshMgr::calc_facepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& face : meshes[current_mesh]->faces) {
    face->facepoint = Helper::face_centerpoint(face);
    Helper::add_vertex_to_mesh(face->facepoint, meshes[next_mesh]);
  }
}


void MeshMgr::calc_edgepoints() {
  auto next_mesh = current_mesh + 1;
  auto vert = std::make_shared<Vertex>();
  for (auto& edge : meshes[current_mesh]->edges) {
    if (edge->edgepoint) continue;
    if (edge->pair) { // not boundary edge
      auto& fp1 = edge->face->facepoint;
      auto& fp2 = edge->pair->face->facepoint;
      Helper::reset_vertex(vert);
      Helper::append_avg_vertex2(fp1, fp2, &vert);
      auto midep = Helper::edge_midpoint(edge);
      edge->edgepoint = std::make_shared<Vertex>();
      Helper::append_avg_vertex2(vert, midep, &edge->edgepoint);
      edge->pair->edgepoint = edge->edgepoint;
    }
    else {
      edge->edgepoint = Helper::edge_midpoint(edge);
    }

    Helper::add_vertex_to_mesh(edge->edgepoint, meshes[next_mesh]);
  }
}


void MeshMgr::calc_newpoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& vert : meshes[current_mesh]->vertices) {
    vertex_ptr avg_adj_fp, avg_adj_ep;
    auto sz = Helper::average_facepoints(vert, &avg_adj_fp);
    auto sz2 = Helper::average_mid_edges(vert, &avg_adj_ep);

    if (sz == sz2) {
      double m1 = 1.0 / sz; double m2 = m1 + m1;
      Helper::vertex_prod_num(avg_adj_fp, m1);
      Helper::vertex_prod_num(avg_adj_ep, m2);

      double m3 = 1.0 - m1 - m2;
      vert->newpoint = std::make_shared<Vertex>();
      Helper::append_vertex(vert, &vert->newpoint);
      Helper::vertex_prod_num(vert->newpoint, m3);
      Helper::append_vertex(avg_adj_ep, &vert->newpoint);
      Helper::append_vertex(avg_adj_fp, &vert->newpoint);
    }
    else { // boundary vertex
      vert->newpoint = Helper::average_border_edge_midpoints(vert);
    }

    Helper::add_vertex_to_mesh(vert->newpoint, meshes[next_mesh]);
  }
}


void MeshMgr::connect_edges() {
  for (auto& face : meshes[current_mesh]->faces) {
    hedge_ptr pe = face->edge;
    do {
      std::vector<vertex_ptr> vertices;
      vertices.push_back(face->facepoint);
      vertices.push_back(pe->edgepoint);
      pe = pe->next;
      vertices.push_back(pe->vert->newpoint);
      vertices.push_back(pe->edgepoint);
      Helper::create_face(vertices, meshes[current_mesh + 1]);
    } while (pe != face->edge);
  }
}

mesh_ptr MeshMgr::ccsubdiv(size_t n) {
  if (current_mesh + n < meshes.size()) {
    current_mesh += n;
    return meshes[current_mesh];
  }

  size_t sz = current_mesh + n + 1;
  meshes.reserve(sz);
  for (current_mesh = meshes.size()-1;
       current_mesh < sz-1; ++current_mesh) {
    meshes.push_back(std::make_shared<Mesh>());
    calc_facepoints();
    calc_edgepoints();
    calc_newpoints();
    connect_edges();
  }
  return meshes[current_mesh];
}




} // namespace ccsubdiv



