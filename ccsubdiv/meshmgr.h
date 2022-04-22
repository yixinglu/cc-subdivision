#ifndef CCSUBDIV_MESHMGR_H_
#define CCSUBDIV_MESHMGR_H_

#include "datatype.h"

namespace ccsubdiv {

class MeshMgr {
 public:
  static MeshMgr& instance() {
    static MeshMgr mgr;
    return mgr;
  }

  void set_mesh(const mesh_ptr& mesh) { meshes.push_back(mesh); }

  bool has_mesh() const { return !meshes.empty(); }

  mesh_ptr ccsubdiv(size_t = 1);
  mesh_ptr previous_mesh() {
    if (current_mesh > 0) {
      return meshes[--current_mesh];
    }
    return meshes[0];
  }
  mesh_ptr post_mesh() {
    if (current_mesh < meshes.size() - 1) {
      return meshes[++current_mesh];
    }
    return ccsubdiv(1);
  }

 private:
  MeshMgr() : current_mesh(0), meshes() {}

  MeshMgr(const MeshMgr&) = delete;
  MeshMgr& operator=(const MeshMgr&) = delete;

  void calc_facepoints();
  void calc_edgepoints();
  void calc_newpoints();
  void connect_edges();

  size_t current_mesh;
  std::vector<mesh_ptr> meshes;
};

}  // namespace ccsubdiv

#endif  // CCSUBDIV_MESHMGR_H_
