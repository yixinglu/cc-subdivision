#ifndef VERTEX_H_INCLUDE
#define VERTEX_H_INCLUDE

#include <vector>
#include "Point3.h"
#include "Vector3.h"

namespace Subdiv {

class Vertex {
public:
    Vertex();
    Vertex(const Vertex &);
    ~Vertex() {}

    Vertex & operator = (const Vertex &);

    inline void addAdjacentEdge(int);
    inline void addAdjacentFace(int);
    void removeAdjacentEdge(int);
    void removeAdjacentFace(int);
    inline void getAdjacentEdge(const std::vector<int> &) const;
    inline void getAdjacentFace(const std::vector<int> &) const;

    inline const Point3f & getCoordinate() const;
    inline void setCoordinate(const Point3f &);
    inline const Vector3f & getNormal() const;
    inline void setNormal(const Vector3f &);

private:
    Point3f     coordinate;
    Vector3f    normal;

    std::vector<int>    vecEdgeIndex;
    std::vector<int>    vecFaceIndex;
};

void Vertex::addAdjacentEdge(int edgeIdx) {
    assert(edgeIdx >= 0);
    vecEdgeIndex.push_back(edgeidx);
}

void Vertex::addAdjacentFace(int faceIdx) {
    assert(faceIdx >= 0);
    vecFaceIndex.push_back(faceIdx);
}

inline const Point3f & Vertex::getCoordinate() const {
    return coordinate;
}

inline void Vertex::setCoordinate(const Point3f &p) {
    coordinate.x = p.x;
    coordinate.y = p.y;
    coordinate.z = p.z;
}

inline void setNormal(const Vector3f &norm) {
    normal = norm;
}

inline const Vector3f & getNormal() const {
    return normal;
}

inline void Vertex::getAdjacentEdge(const std::vector<int> &vecAdjEdges) const {
    vecAdjEdges = vecEdgeIndex;
}

inline void Vertex::getAdjacentFace(const std::vector<int> &vecAdjFaces) const;
    vecAdjFaces = vecFaceIndex;
}


#endif
