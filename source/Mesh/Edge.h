#ifndef EDGE_H_INCLUDE
#define EDGE_H_INCLUDE

#include <vector>
#include "GlobalDef.h"

namespace Subdiv {

class Edge {
    public:
        Edge();
        Edge(const Edge &);
        ~Edge() {}

        Edge & operator = (const Edge &);

        inline void setVertex(int, int);
        inline void getVertex(int &, int &) const;

        inline void addAdjacentFace(int);
        inline void getAdjacentFace(std::vector<int> &) const;

    private:
        int     startVertexIndex;
        int     endVertexIndex;

        std::vector<int>    vecAdjacentFaceIndex;
};

inline void Edge::setVertex(int start, int end) {
    startVertexIndex = start;
    endVertexIndex   = end;
}

inline void Edge::getVertex(int &start, int &end) const {
    start = startVertexIndex;
    end   = endVertexIndex;
}

inline void Edge::addAdjacentFace(int face) {
    vecAdjacentFaceIndex.push_back(face);
}

inline void Edge::getAdjacentFace(std::vector<int> &faceIdx) const {
    faceIdx = vecAdjacentFaceIndex;
}

} // namespace
#endif
