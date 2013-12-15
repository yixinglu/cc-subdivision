#include <iostream>
#include "../Mesh/Vertex.h"

using namespace std;
using namespace Subdiv;

int main(int argc, char** argv) {
    Vertex v;
    v.addAdjacentEdge(0);
    cout << v.getNormal().getX() << endl;
    return 0;
}
