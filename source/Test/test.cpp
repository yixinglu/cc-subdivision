#include <iostream>
#include "../Mesh/Vector3.h"
#include "../Mesh/Vertex.h"
#include "../Mesh/Edge.h"

using namespace std;
using namespace Subdiv;

void testVector3() {
    cout << "*** test Vector3 ***" << endl;
    Vector3f v(1,2,3), v2(2,1,4);
    cout << v.getX() << v.getY() << v.getZ() << endl;
    v.setCoordinate(3,2,1);
    cout << dotProduct(v, v2) << endl;
    cout << crossProduct(v, v2).getX() << endl;
    cout << v.getX() << v.getY() << v.getZ() << endl;
    cout << v.length() << endl;
    v.normalize();
    cout << v.length() << endl;
    cout << (__cplusplus == 201103L) << endl;
}

void testVertex() {
    cout << "*** test Vertex ***" << endl;
    Vertex v, v2;
    Point3f point;
    point.x = 1;
    point.y = 0.9;
    point.z = 3.14;
    v.setCoordinate(point);
    point.x = 0.02; point.y = 0.56; point.z =  800;
    v2.setCoordinate(point);
    cout << v.getCoordinate().z << endl;
    Vector3f norm(1,2,3);
    v2.setNormal(norm);
    Vertex v3 = v2;
    cout << v3.getNormal().getX() << endl;
    cout << v.getNormal().getZ() << endl;
    v3.addAdjacentEdge(0);
    v3.addAdjacentFace(1);

}

void testEdge() {
    cout << "*** test Edge ***" << endl;
    Edge edge;
    edge.setVertex(0, 1);
    int s, e;
    edge.getVertex(s, e);
    cout << s << " " << e << endl;
    edge.addAdjacentFace(0);
    vector<int> vec;
    edge.getAdjacentFace(vec);
}

int main (int argc, char** argv) {
    testVector3();
    testVertex();
    testEdge();
}
