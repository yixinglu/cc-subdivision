#include "datatype.h"
#include "helper.h"


namespace ccsubdiv {


void MeshMgr::calc_facepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& face : meshes[current_mesh]->faces) {
    face->facepoint = SubdivHelper::centerpoint(face);
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
    edge->edgepoint = std::make_shared<Vertex>();
    auto next_edge = edge->next;
    assert(next_edge);

    edge->edgepoint->coord = next_edge->vert->coord + edge->vert->coord;
    edge->edgepoint->norm = next_edge->vert->norm + edge->vert->norm;

    if (edge->pair) {
      edge->pair->edgepoint = edge->edgepoint;

      edge->edgepoint->coord += edge->face->facepoint->coord;
      edge->edgepoint->coord += edge->pair->face->facepoint->coord;
      edge->edgepoint->coord /= 4.0;

      edge->edgepoint->norm += edge->face->facepoint->norm;
      edge->edgepoint->norm += edge->pair->face->facepoint->norm;
      edge->edgepoint->norm /= 4.0;
    }
    else { // boundary edge
      edge->edgepoint->coord /= 2.0;
      edge->edgepoint->norm /= 2.0;
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
    vert->newpoint = std::make_shared<Vertex>();
    auto avg_adj_fp = std::make_shared<Vertex>();
    auto avg_adj_ep = std::make_shared<Vertex>();

    auto sz = SubdivHelper::average_facepoints(vert, &avg_adj_fp);
    auto sz2 = SubdivHelper::average_mid_edges(vert, &avg_adj_ep);

    if (sz == sz2) {
      avg_adj_fp->coord /= sz;
      avg_adj_fp->norm /= sz;

      avg_adj_ep->coord *= 2.0 / sz;
      avg_adj_ep->norm *= 2.0 / sz;

      vert->newpoint->coord = vert->coord * (1.0 - 3.0/sz)
        + avg_adj_ep->coord + avg_adj_fp->coord;
      vert->newpoint->norm = vert->norm * (1.0 - 3.0/sz)
        + avg_adj_ep->norm + avg_adj_fp->norm;
    }
    else { // boundary vertex
      auto edge = vert->edge;
      size_t num = 0;
      do {
        vert->newpoint->coord += edge->next->vert->coord;
        vert->newpoint->norm += edge->next->vert->norm;
        num++;
      } while (edge && edge != vert->edge);
      vert->newpoint->coord += vert->coord * (2.0 + num);
      vert->newpoint->coord /= 2.0*(num + 1.0);
      vert->newpoint->norm += vert->norm * (2.0 + num);
      vert->newpoint->norm /= 2.0*(num + 1.0);
    }

    meshes[next_mesh]->vertices.push_back(vert->newpoint);
    vert->newpoint = meshes[next_mesh]->vertices.back();
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



