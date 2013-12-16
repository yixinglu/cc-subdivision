#include "Edge.h"

using namespace Subdiv;

Edge::Edge()
    : startVertexIndex(0)
    , endVertexIndex(0)
    , vecAdjacentFaceIndex()
    {}

Edge::Edge(const Edge &e)
    : startVertexIndex(e.startVertexIndex)
    , endVertexIndex(e.endVertexIndex)
    , vecAdjacentFaceIndex(e.vecAdjacentFaceIndex)
    {}

Edge & Edge::operator = (const Edge &e) {
    if (this != &e) {
        setVertex(e.startVertexIndex, e.endVertexIndex);
        vecAdjacentFaceIndex = e.vecAdjacentFaceIndex;
    }
    return *this;
}
