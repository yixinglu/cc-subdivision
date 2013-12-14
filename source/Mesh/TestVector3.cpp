#include <iostream>
#include <stdio.h>
#include "Vector3.h"

using namespace std;
using namespace Subdiv;

int main () {

    Vector3f v(1,2,3);
    cout << v.getX() << v.getY() << v.getZ() << endl;
    v.setCoordinate(3,2,1);
    cout << v.getX() << v.getY() << v.getZ() << endl;
    cout << v.length() << endl;
    v.normalize();
    cout << v.length() << endl;
    cout << (__cplusplus == 201103L) << endl;
    printf("%dL", __cplusplus);
}
