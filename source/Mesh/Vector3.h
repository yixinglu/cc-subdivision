#ifndef VECTOR3_H_INCLUDE
#define VECTOR3_H_INCLUDE

#include "Point3.h"

namespace Subdiv {

template<typename Real>
class Vector3 {
public:
    Vector3();
    Vector3(Real, Real, Real);
    Vector3(const Vector3 &);
    ~Vector3() {}

    Vector3 & operator = (const Vector3 &);

    inline void setCoordinate(Real, Real, Real);
    inline Real getX() const;
    inline Real getY() const;
    inline Real getZ() const;

    inline void normalize();
    inline Real length() const;

private:
    Point3<Real> coord;
};

template<typename Real>
Vector3<Real>::Vector3() {
    setCoordinate((Real)0, (Real)0, (Real)0);
}

template<typename Real>
Vector3<Real>::Vector3(Real x, Real y, Real z) {
    setCoordinate(x, y, z);
}

template<typename Real>
Vector3<Real>::Vector3(const Vector3 &v) {
    setCoordinate(v.coord.x, v.coord.y, v.coord.z);
}

template<typename Real>
Vector3<Real> & Vector3<Real>::operator = (const Vector3 &v) {
    if (this != &v) {
        setCoordinate(v.coord.x, v.coord.y, v.coord.z);
    }
    return *this;
}

template<typename Real>
inline void Vector3<Real>::setCoordinate(Real x, Real y, Real z) {
    coord.x = x;
    coord.y = y;
    coord.z = z;
}

template<typename Real>
inline Real Vector3<Real>::getX() const {
    return coord.x;
}

template<typename Real>
inline Real Vector3<Real>::getY() const {
    return coord.y;
}

template<typename Real>
inline Real Vector3<Real>::getZ() const {
    return coord.z;
}

template<typename Real>
inline void Vector3<Real>::normalize() {
    Real len = length();
    if (len < TOLERANCE_EPSILON) {
        assert(!"length is zero");
        return;
    }
    len = (Real)1 / len;
    setCoordinate(getX()*len, getY()*len, getZ()*len);
}

template<typename Real>
inline Real Vector3<Real>::length() const {
    return sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
}

typedef Vector3<REAL32> Vector3f;
typedef Vector3<REAL64> Vector3d;

template<typename Real>
Real dotProduct(const Vector3<Real> &v1, const Vector3<Real> &v2) {
    return v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ();
}

template<typename Real>
Vector3<Real> crossProduct(const Vector3<Real> &v1, const Vector3<Real> &v2) {
    return Vector3<Real>(v1.getY() * v2.getZ() - v1.getZ() * v2.getY(),
                         v1.getZ() * v2.getX() - v1.getX() * v2.getZ(),
                         v1.getX() * v2.getY() - v1.getY() * v2.getX());
}

}

#endif
