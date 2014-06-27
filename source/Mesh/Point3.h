#ifndef POINT3_H_INCLUDE
#define POINT3_H_INCLUDE

#include "Define.h"

namespace Subdiv {

template<typename Real>
struct Point3 {
    Real x;
    Real y;
    Real z;
};

typedef Point3<REAL32> Point3f;
typedef Point3<REAL64> Point3d;

}
#endif
