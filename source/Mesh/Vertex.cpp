#include "Vertex.h"

using namespace Subdiv;

Vertex::Vertex()
    : normal()
    , vecEdgeIndex()
    , vecFaceIndex() {
    coordinate.x = coordinate.y = coordinate.z = 0.f;
}

Vertex::Vertex(const Vertex &v)
    : normal(v.normal)
    , vecEdgeIndex(v.vecEdgeIndex)
    , vecFaceIndex(v.vecFaceIndex) {
        setCoordinate(v.coordinate);
}

Vertex & Vertex::operator = (const Vertex &v) {
    if (this != &v) {
        setCoordinate(v.coordinate);
        normal = v.normal;
        vecEdgeIndex = v.vecEdgeIndex;
        vecFaceIndex = v.vecFaceIndex;
    }
    return *this;
}

