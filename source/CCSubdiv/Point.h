#ifndef POINT_H
#define POINT_H

namespace CCSubdiv {

template <Real>
class Point {
    public:
        explicit Point(Real x = (Real)0,
                       Real y = (Real)0,
                       Real z = (Real)0);
        Point(const Point&);
        ~Point();

        Point& operator = (const Point&);

        inline void setX(Real);
        inline Real getX() const;
        inline void setY(Real);
        inline Real getY() const;
        inline void setZ(Real);
        inline Real getZ() const;

    private:
        Real _x, _y, _z;
};

template<Real>
Point::Point(Real x, Real y, Real z)
    : _x(x)
    , _y(y)
    , _z(z)
    {}

template<Real>
Point::Point(const Point& p)
    : _x(p._x)
    , _y(p._y)
    , _z(p._z)
    {}

template<Real>
Point::~Point() {}

template<Real>
Point::operator = (const Point& p) {
    if (this != &p) {
        _x = p._x;
        _y = p._y;
        _z = p._z;
    }
    return *this;
}

template<Real>
inline void Point::setX(Real x) {
    _x = x;
}

template<Real>
inline Real Point::getX() const {
    return _x;
}


template<Real>
inline void Point::setY(Real y) {
    _y = y;
}

template<Real>
inline Real Point::getY() const {
    return _y;
}

template<Real>
inline void Point::setZ(Real z) {
    _z = z;
}

template<Real>
inline Real Point::getZ() const {
    return _z;
}

typedef Point<double> Point3d;
typedef Point<float>  Point3f;

}
#endif
