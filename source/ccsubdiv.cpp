#include "datatype.h"
#include "helper.h"


namespace ccsubdiv {


void MeshMgr::calc_facepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& face : meshes[current_mesh]->faces) {
    face->facepoint = SubdivHelper::face_centerpoint(face);
    meshes[next_mesh]->vertices.push_back(face->facepoint);
    SubdivHelper::boundingbox_xyz(face->facepoint->coord,
      &meshes[next_mesh]->boundingbox[0],
      &meshes[next_mesh]->boundingbox[1]);
  }
}


void MeshMgr::calc_edgepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& edge : meshes[current_mesh]->edges) {
    if (edge->edgepoint) continue;

    edge->edgepoint = SubdivHelper::edge_midpoint(edge);
    if (edge->pair) { // not boundary edge
      auto& fp1 = edge->face->facepoint;
      auto& fp2 = edge->pair->face->facepoint;
      auto& mp = edge->edgepoint;
      auto midfp = (fp1->coord + fp2->coord) * 0.5;
      edge->edgepoint->coord = (mp->coord + midfp) * 0.5;
      midfp = (fp1->norm + fp2->norm) * 0.5;
      edge->edgepoint->norm = (mp->norm + midfp) * 0.5;
      edge->pair->edgepoint = edge->edgepoint;
    }

    meshes[next_mesh]->vertices.push_back(edge->edgepoint);
    SubdivHelper::boundingbox_xyz(edge->edgepoint->coord,
      &meshes[next_mesh]->boundingbox[0],
      &meshes[next_mesh]->boundingbox[1]);
  }
}


void MeshMgr::calc_newpoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& vert : meshes[current_mesh]->vertices) {
    vertex_ptr avg_adj_fp, avg_adj_ep;
    auto sz = SubdivHelper::average_facepoints(vert, &avg_adj_fp);
    auto sz2 = SubdivHelper::average_mid_edges(vert, &avg_adj_ep);

    if (sz == sz2) {
      double m1 = 1.0 / sz; double m2 = m1 + m1;
      avg_adj_fp->coord *= m1; avg_adj_fp->norm *= m1;
      avg_adj_ep->coord *= m2; avg_adj_ep->norm *= m2;

      vert->newpoint = std::make_shared<Vertex>();
      double m3 = 1.0 - m1 - m2;
      vert->newpoint->coord = vert->coord * m3
        + avg_adj_ep->coord + avg_adj_fp->coord;
      vert->newpoint->norm = vert->norm * m3
        + avg_adj_ep->norm + avg_adj_fp->norm;
    }
    else { // boundary vertex
      vert->newpoint = SubdivHelper::average_border_edge_midpoints(vert);
    }

    meshes[next_mesh]->vertices.push_back(vert->newpoint);
    SubdivHelper::boundingbox_xyz(vert->newpoint->coord,
      &meshes[next_mesh]->boundingbox[0],
      &meshes[next_mesh]->boundingbox[1]);
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
      SubdivHelper::create_face(vertices, meshes[current_mesh + 1]);
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



