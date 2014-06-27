#ifndef DEFINE_H_INCLUDE
#define DEFINE_H_INCLUDE

#if __cplusplus < 201103L
    //#error "should use C++11 implementation"
#endif

typedef float  REAL32;
typedef double REAL64;

#include <math.h>
#include <assert.h>
#include <stdint.h>

#define TOLERANCE_EPSILON 1.0e-8

#endif
