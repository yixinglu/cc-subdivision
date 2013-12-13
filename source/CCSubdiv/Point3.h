#ifndef POINT3_H_INCLUDE
#define POINT3_H_INCLUDE

namespace Subdiv{

template <typename Real>
class Point3 {
    public:
        explicit Point3(Real x = (Real)0,
                        Real y = (Real)0,
                        Real z = (Real)0);
        Point3(const Point3&);
        ~Point3();

        Point3& operator = (const Point3&);

        inline void setX(Real);
        inline Real getX() const;
        inline void setY(Real);
        inline Real getY() const;
        inline void setZ(Real);
        inline Real getZ() const;

    private:
        Real _x, _y, _z;
};

template<typename Real>
Point3<Real>::Point3(Real x, Real y, Real z)
    : _x(x)
    , _y(y)
    , _z(z)
    {}

template<typename Real>
Point3<Real>::Point3(const Point3& p)
    : _x(p._x)
    , _y(p._y)
    , _z(p._z)
    {}

template<typename Real>
Point3<Real>::~Point3() {}

template<typename Real>
Point3<Real>& Point3<Real>::operator = (const Point3& p) {
    if (this != &p) {
        _x = p._x;
        _y = p._y;
        _z = p._z;
    }
    return *this;
}

template<typename Real>
inline void Point3<Real>::setX(Real x) {
    _x = x;
}

template<typename Real>
inline Real Point3<Real>::getX() const {
    return _x;
}


template<typename Real>
inline void Point3<Real>::setY(Real y) {
    _y = y;
}

template<typename Real>
inline Real Point3<Real>::getY() const {
    return _y;
}

template<typename Real>
inline void Point3<Real>::setZ(Real z) {
    _z = z;
}

template<typename Real>
inline Real Point3<Real>::getZ() const {
    return _z;
}

typedef Point3<double> Point3d;
typedef Point3<float>  Point3f;
typedef Point3f        Vector3f;
typedef Point3d        Vector3d;

template<typename Real>
Real dotProduct(const Point3<Real>& p1, const Point3<Real>& p2) {
    return p1.getX()*p2.getX() + p1.getY()*p2.getY() + p1.getZ()*p2.getZ();
}

template<typename Real>
Point3<Real> crossProduct(const Point3<Real>& p1, const Point3<Real>& p2) {
    return Point3<Real>(p1.getY()*p2.getZ() - p1.getZ()*p2.getY(),
                  p1.getZ()*p2.getX() - p1.getX()*p2.getZ(),
                  p1.getX()*p2.getY() - p1.getY()*p2.getX());
}


}
#endif
