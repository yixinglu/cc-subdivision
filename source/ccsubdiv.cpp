#include "datatype.h"
#include "helper.h"
#include <math.h>

namespace ccsubdiv {



void MeshMgr::calc_facepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& face : meshes[current_mesh]->faces) {
    meshes[next_mesh]->vertices.push_back(SubdivHelper::centerpoint(face));
    face->facepoint = meshes[next_mesh]->vertices.back();
  }
}


void MeshMgr::calc_edgepoints() {
  auto next_mesh = current_mesh + 1;
  for (auto& edge : meshes[current_mesh]->edges) {
    vertex_ptr vert = std::make_shared<Vertex>();
    auto next_edge = edge->next;
    assert(next_edge);
    vert->coord = next_edge->vert->coord + edge->vert->coord;
    vert->coord += edge->face->facepoint->coord;
    vert->coord += edge->pair->face->facepoint->coord;
    vert->coord /= 4;
    meshes[next_mesh]->vertices.push_back(vert);
    edge->edgepoint = edge->pair->edgepoint
                    = meshes[next_mesh]->vertices.back();
  }
}


void MeshMgr::calc_newpoints() {
  auto next_mesh = current_mesh + 1;
  for (auto vert : meshes[current_mesh]->vertices) {
    vertex_ptr new_vert(new Vertex);
    SubdivHelper::average_of_adjacent_facepoints(vert, new_vert);
    size_t sz = SubdivHelper::average_of_adjacent_edgepoints(vert, new_vert);
    new_vert->coord += vert->coord * (sz - 3);
    new_vert->coord /= sz;
    meshes[next_mesh]->vertices.push_back(new_vert);
    vert->newpoint = meshes[next_mesh]->vertices.back();
  }
}


void MeshMgr::connect_edges() {
  auto next_mesh = current_mesh + 1;
  for (auto& face : meshes[current_mesh]->faces) {
    hedge_ptr pe = face->edge;
    do {
      std::vector<vertex_ptr> vertices;
      vertices.push_back(face->facepoint);
      vertices.push_back(pe->edgepoint);
      pe = pe->next;
      vertices.push_back(pe->vert->newpoint);
      vertices.push_back(pe->edgepoint);
      SubdivHelper::create_face(vertices, meshes[next_mesh]);
    } while (pe != face->edge);
  }
}


mesh_ptr MeshMgr::ccsubdiv(size_t n) {
  size_t sz = n + meshes.size();
  meshes.reserve(sz);
  for (; current_mesh < sz-1; ++current_mesh) {
    meshes.push_back(std::make_shared<Mesh>());
    calc_facepoints();
    calc_edgepoints();
    calc_newpoints();
    connect_edges();
  }
  return meshes[current_mesh];
}




} // namespace ccsubdiv



