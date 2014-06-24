#ifndef CCSUBDIV_MATH_UTIL_H_
#define CCSUBDIV_MATH_UTIL_H_

#include <assert.h>
#include <math.h>

namespace ccsubdiv {

template<typename T> class Vec3 {
public:
  Vec3(const T x = 0, const T y = 0, const T z = 0) {
    xyz[0] = x; xyz[1] = y; xyz[2] = z;
  }

  Vec3(const T coord[3]) {
    assign(coord);
  }

  Vec3(const Vec3& v) {
    if (this != &v) {
      assign(v.xyz);
    }
  }

  Vec3& operator = (const Vec3& v) {
    if (this != &v) {
      assign(v.xyz);
    }
    return *this;
  }

  Vec3& operator += (const Vec3& v) {
    xyz[0] += v.xyz[0];
    xyz[1] += v.xyz[1];
    xyz[2] += v.xyz[2];
    return *this;
  }

  template<typename Ty>
  Vec3& operator *= (const Ty d) {
    xyz[0] *= d;
    xyz[1] *= d;
    xyz[2] *= d;
    return *this;
  }

  template<typename Ty>
  Vec3& operator /= (const Ty d) {
    assert(abs((long double)d) > 1.0e-08);
    xyz[0] /= d;
    xyz[1] /= d;
    xyz[2] /= d;
    return *this;
  }

private:
  void assign(const T coord[3]) {
    xyz[0] = coord[0];
    xyz[1] = coord[1];
    xyz[2] = coord[2];
  }

  T xyz[3];
};

template<typename T>
Vec3<T> operator + (const Vec3<T>& v1, const Vec3<T>& v2) {
  Vec3<T> v; v += v1; v += v2;
  return v;
}

template<typename T, typename Ty>
Vec3<T> operator * (const Vec3<T>& v1, const Ty d) {
  Vec3<T> v; v *= d;
  return v;
}


typedef Vec3<double> vec3d;

}


#endif