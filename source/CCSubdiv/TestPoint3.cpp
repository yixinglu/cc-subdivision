#include <iostream>
#include "Point3.h"

using namespace std;
using namespace Subdiv;

int main(int argc, char** argv) {
    Point3<double> p1(1,2,3);
    cout << p1.getX() << endl;
    cout << p1.getY() << endl;
    p1.setX(1.23);
    cout << p1.getX() << endl;

    Point3f pf;
    cout << pf.getX() << endl;
    pf.setZ(1.0f);
    cout << pf.getZ() << endl;
    return 0;

}
