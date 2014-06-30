#ifndef CCSUBDIV_MATH_UTIL_H_
#define CCSUBDIV_MATH_UTIL_H_

#include <assert.h>

namespace ccsubdiv {

#define EPSILON 1.0e-08
#define CCASSERT(d) assert(!is_zero(d))

template<typename T> bool is_zero(T t) {
  return -EPSILON < t && t < EPSILON;
}

template<typename T> class Vec3 {
public:
  Vec3(const T x = 0, const T y = 0, const T z = 0) {
    _xyz[0] = x; _xyz[1] = y; _xyz[2] = z;
  }

  Vec3(const T coord[3]) {
    assign(coord);
  }

  Vec3(const Vec3& v) {
    if (this != &v) {
      assign(v._xyz);
    }
  }

  const T* xyz() const {
    return _xyz;
  }

  Vec3& operator = (const Vec3& v) {
    if (this != &v) {
      assign(v._xyz);
    }
    return *this;
  }

  const T operator[](size_t dim) const {
    assert(dim >= 0 && dim < 3);
    return _xyz[dim];
  }

  T& operator[](size_t dim) {
    assert(dim >= 0 && dim < 3);
    return _xyz[dim];
  }

  Vec3& operator += (const Vec3& v) {
    _xyz[0] += v._xyz[0];
    _xyz[1] += v._xyz[1];
    _xyz[2] += v._xyz[2];
    return *this;
  }

  Vec3& operator *= (const T d) {
    _xyz[0] *= static_cast<T>(d);
    _xyz[1] *= static_cast<T>(d);
    _xyz[2] *= static_cast<T>(d);
    return *this;
  }

  Vec3& operator /= (const T d) {
    CCASSERT(d);
    return operator *=( static_cast<T>(1.0 / d) );
  }

  bool operator == (const Vec3& v) {
    return is_zero(_xyz[0] - v._xyz[0]) && is_zero(_xyz[1] - v._xyz[1])
      && is_zero(_xyz[2] - v._xyz[2]);
  }


private:
  void assign(const T coord[3]) {
    _xyz[0] = coord[0];
    _xyz[1] = coord[1];
    _xyz[2] = coord[2];
  }

  T _xyz[3];
};

template<typename T>
Vec3<T> operator + (const Vec3<T>& v1, const Vec3<T>& v2) {
  Vec3<T> v; v += v1; v += v2;
  return v;
}

template<typename T>
Vec3<T> operator - (const Vec3<T>& v1, const Vec3<T>& v2) {
  return v1 + (v2 * static_cast<T>(-1));
}

template<typename T>
Vec3<T> operator * (const Vec3<T>& v1, const T d) {
  Vec3<T> v(v1); v *= d;
  return v;
}

template<typename T>
double dot_prod (const Vec3<T>& v1, const Vec3<T>& v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

template<typename T>
Vec3<T> cross_prod(const Vec3<T>& v1, const Vec3<T>& v2) {
  return Vec3<T>(v1[1] * v2[2] - v1[2] * v2[1],
                 v1[2] * v2[0] - v1[0] * v2[2],
                 v1[0] * v2[1] - v1[1] * v2[0]);
}


typedef Vec3<double> vec3d;

}


#endif