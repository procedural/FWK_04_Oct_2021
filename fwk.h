/* game framework.
*  - rlyeh, public domain
*
* ## FWK License
*
* This software is available under 3 licenses. Choose whichever you prefer.
* ------------------------------------------------------------------------------
* ALTERNATIVE A - Public Domain (https://unlicense.org/)
* ------------------------------------------------------------------------------
* This is free and unencumbered software released into the public domain.
*
* Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
* software, either in source code form or as a compiled binary, for any purpose,
* commercial or non-commercial, and by any means.
*
* In jurisdictions that recognize copyright laws, the author or authors of this
* software dedicate any and all copyright interest in the software to the public
* domain. We make this dedication for the benefit of the public at large and to
* the detriment of our heirs and successors. We intend this dedication to be an
* overt act of relinquishment in perpetuity of all present and future rights to
* this software under copyright law.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* ------------------------------------------------------------------------------
* ALTERNATIVE B - MIT-0 (No Attribution clause)
* ------------------------------------------------------------------------------
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify,
* merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* ------------------------------------------------------------------------------
* ALTERNATIVE C - Zero BSD License (https://opensource.org/licenses/FPL-1.0.0)
* ------------------------------------------------------------------------------
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
* REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
* INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* ## License: Contributed Code ------------------------------------------------
*
* Dear Contributor,
*
* In order to ensure this project remains completely free and unencumbered by
* anyone's copyright monopoly, it is advisable that you dedicate your code-base
* contributions to the three licensing terms above. This removes any possible
* ambiguity as to what terms somebody might have thought they were contributing
* under, in case of a future dispute. These concerns are not unique to public
* domain software. Most large, established open-source projects have a
* Contributor License Agreement (CLA) process, of varying degrees of formality.
*
* Please add yourself to the list below before contributing.
* Thanks.
*
* --
*
* "I dedicate any and all copyright interest in this software to the three
* license terms listed above. I make this dedication for the benefit of the
* public at large and to the detriment of my heirs and successors. I intend
* this dedication to be an overt act of relinquishment in perpetuity of all
* present and future rights to this software under copyright law."
*
* Author (name)              I agree (YES/NO)    Files/Features (optional)
* ------------------------------------------------------------------------------
* @r-lyeh                    YES                 Initial codebase
* ------------------------------------------------------------------------------
*/

#ifndef FWK_H
#define FWK_H

//-----------------------------------------------------------------------------
// Headers

#line 1 "fwk_dll.h"
// dll utils
// - rlyeh, public domain

#ifdef _MSC_VER
#define IMPORT __declspec(dllimport)
#define EXPORT __declspec(dllexport)
#else
#define IMPORT
#define EXPORT
#endif

// msvc users may need to -DAPI=IMPORT/EXPORT as needed when loading/building FWK as DLL.
#ifndef API
#define API
#endif

API void* dll(const char *filename, const char *symbol);
#line 0
#line 1 "fwk_main.h"
// -----------------------------------------------------------------------------
// config directives
// debug /O0 /D3 > debugopt /O1 /D2 > release (+ndebug) /O2 /D1 > final (+ndebug+final) /O3 /D0

#ifdef FINAL
#define WITH_COOKER          0
#define WITH_FASTCALL_LUA    1
#define WITH_LEAK_DETECTOR   0
#define WITH_PROFILE         0
#define WITH_XREALLOC_POISON 0
#define WITH_VIDEO_YCBCR     1
#else
#define WITH_COOKER          1
#define WITH_FASTCALL_LUA    0
#define WITH_LEAK_DETECTOR   0
#define WITH_PROFILE         1
#define WITH_XREALLOC_POISON 1
#define WITH_VIDEO_YCBCR     1
#endif

//#define WITH_ASSIMP        0      // Only art/tools/ass2iqe.c would define this.
#define WITH_COMPRESSOR      ULZ|0  // Use COMPRESSOR|LEVEL[0..15], where compressor is one of:
                                    // LZMA,DEFL,LZ4X,ULZ,BCM,CRSH,BALZ,LZW3,LZSS,LZP1,PPP,RAW

// -----------------------------------------------------------------------------
// system headers

#ifndef _GNU_SOURCE
#define _GNU_SOURCE   // for linux
#endif

#ifdef _MSC_VER
#include <omp.h>      // compile with /openmp to speed up some computations
#endif
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// stl, forward includes

#define array(t) t*

#ifdef _MSC_VER
#define __thread         __declspec(thread)
#elif defined __TINYC__ && defined _WIN32
#define __thread         __declspec(thread) // compiles fine, but does not work apparently
#elif defined __TINYC__
#define __thread
#endif

// util for sanity checks (static_assert)

//#define STATIC_ASSERT_2(COND, LINE) typedef int static_assert_on_line_##LINE[ !!(COND) ]
#define STATIC_ASSERT_2(COND, LINE) typedef struct { unsigned static_assert_on_line_##LINE : !!(COND); } static_assert_on_line_##LINE
#define STATIC_ASSERT_1(COND, LINE) STATIC_ASSERT_2(COND, LINE)
#define STATIC_ASSERT(COND) STATIC_ASSERT_1(COND, __LINE__)

//#define STATIC_ASSERT(exp) typedef char UNIQUE_NAME(_static_assert_on_line)[(exp)?1:-1]

// pragma libs

#if defined _WIN32 && (defined _MSC_VER || defined __TINYC__)
#pragma comment(lib, "advapi32")
#pragma comment(lib, "dbghelp")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "shell32")
#pragma comment(lib, "user32")
#pragma comment(lib, "winmm")
#pragma comment(lib, "ws2_32")
#endif

#if defined __linux__ && defined __TINYC__
#pragma comment(lib, "dl")
#pragma comment(lib, "m")
#pragma comment(lib, "pthread")
#endif
#line 0
#line 1 "fwk_memory.h"
// -----------------------------------------------------------------------------
// memory framework
// - rlyeh, public domain

// memory leaks detector
#if WITH_LEAK_DETECTOR
#define WATCH(ptr,sz) watch((ptr), (sz))
#define FORGET(ptr)   forget(ptr)
#else
#define WATCH(ptr,sz) (ptr)
#define FORGET(ptr)   (ptr)
#endif

// default allocator (aborts on out-of-mem)
API void*  xrealloc(void* p, size_t sz);
API size_t xsize(void* p);

// stack based allocator (negative bytes does rewind stack, like when entering new frame)
API void*  stack(int bytes);

// memory leaks
API void*  watch( void *ptr, int sz );
API void*  forget( void *ptr );

// memory api
#define MSIZE(p)       xsize(p)
#define REALLOC(p,n)   (len1_ = (n), (len1_ ? WATCH(xrealloc((p),len1_),len1_) : xrealloc(FORGET(p),0)))
#define MALLOC(n)      REALLOC(0,(n))
#define FREE(p)        REALLOC(FORGET(p), 0)
#define CALLOC(m,n)    (len2_ = (m)*(n), memset(REALLOC(0,len2_),0,len2_))
#define STRDUP(s)      (len2_ = strlen(s)+1, ((char*)memcpy(REALLOC(0,len2_), (s), len2_)))
static __thread size_t len1_, len2_;

#if 0 // ifndef REALLOC
#define REALLOC            realloc
#define MALLOC(n)          REALLOC(0, n)
#define FREE(p)            REALLOC(p, 0)
#define CALLOC(c, n)       memset(MALLOC((c)*(n)), 0, (c)*(n))
#define STRDUP(s)          strcpy(MALLOC(strlen(s)+1), (s))
#endif
#line 0
#line 1 "fwk_math.h"
// -----------------------------------------------------------------------------
// math framework: rand, ease, vec2, vec3, vec4, quat, mat2, mat33, mat34, mat4
// - rlyeh, public domain
//
// Credits: @ands+@krig+@vurtun (PD), @datenwolf (WTFPL2), @evanw+@barerose (CC0), @sgorsten (Unlicense).

#include <math.h>
#include <float.h>
#include <stdint.h>
#include <stdbool.h>

#define C_EPSILON  (1e-6)
#define C_PI       (3.141592654f) // (3.14159265358979323846f)
#define TO_RAD     (C_PI/180.f)
#define TO_DEG     (180.f/C_PI)

#if defined(_MSC_VER) && !defined(__cplusplus)
#define m_inline __inline
#else
#define m_inline inline
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1700)
#define m_finite _finite
#else
#define m_finite isfinite
#endif

#ifdef __cplusplus
#define M_CAST(type, ...)  ( type { __VA_ARGS__ } )
#else
#define M_CAST(type, ...)  ((type){ __VA_ARGS__ } )
#endif

// ----------------------------------------------------------------------------

#define ptr(type)         0[&(type).x]
#define vec2(x, y      )  M_CAST(vec2, (float)(x), (float)(y)                        )
#define vec3(x, y, z   )  M_CAST(vec3, (float)(x), (float)(y), (float)(z),           )
#define vec4(x, y, z, w)  M_CAST(vec4, (float)(x), (float)(y), (float)(z), (float)(w))
#define quat(x, y, z, w)  M_CAST(quat, (float)(x), (float)(y), (float)(z), (float)(w))
#define axis(x, y, z)     M_CAST(axis, (float)(x), (float)(y), (float)(z))
#define mat33(...)        M_CAST(mat33, __VA_ARGS__ )
#define mat34(...)        M_CAST(mat34, __VA_ARGS__ )
#define mat44(...)        M_CAST(mat44, __VA_ARGS__ )
#define coord_system(...) M_CAST(coord_system, __VA_ARGS__)

typedef union vec2 { struct { float x,y; }; struct { float r,g; }; struct { float w,h; }; struct { float min,max; }; float v[1]; } vec2;
typedef union vec3 { struct { float x,y,z; }; struct { float r,g,b; }; struct { float w,h,d; }; vec2 xy; vec2 rg; vec2 wh; float v[1]; } vec3;
typedef union vec4 { struct { float x,y,z,w; }; struct { float r,g,b,a; }; vec2 xy; vec3 xyz; vec2 rg; vec3 rgb; vec2 wh; vec3 whd; float v[1]; } vec4;
typedef union quat { struct { float x,y,z,w; }; vec3 xyz; vec4 xyzw; float v[1]; } quat;
typedef float mat33[9];
typedef float mat34[12];
typedef float mat44[16];

// A value type representing an abstract direction vector in 3D space, independent of any coordinate system.
// A concrete 3D coordinate system with defined x, y, and z axes.
typedef enum { axis_front, axis_back, axis_left, axis_right, axis_up, axis_down } coord_axis;
typedef union coord_system { struct { coord_axis x,y,z; }; } coord_system;

// ----------------------------------------------------------------------------

API void     randset(uint64_t state);
API uint64_t rand64(void);
API double   randf(void); // [0, 1) interval
API int      randi(int mini, int maxi); // [mini, maxi) interval

// ----------------------------------------------------------------------------

API float simplex1( float x );
API float simplex2( vec2 xy );
API float simplex3( vec3 xyz );
API float simplex4( vec4 xyzw );

// ----------------------------------------------------------------------------

API float ease_linear(float t);

API float ease_out_sine(float t);
API float ease_out_quad(float t);
API float ease_out_cubic(float t);
API float ease_out_quart(float t);
API float ease_out_quint(float t);
API float ease_out_expo(float t);
API float ease_out_circ(float t);
API float ease_out_back(float t);
API float ease_out_elastic(float t);
API float ease_out_bounce(float t);

API float ease_in_sine(float t);
API float ease_in_quad(float t);
API float ease_in_cubic(float t);
API float ease_in_quart(float t);
API float ease_in_quint(float t);
API float ease_in_expo(float t);
API float ease_in_circ(float t);
API float ease_in_back(float t);
API float ease_in_elastic(float t);
API float ease_in_bounce(float t);

API float ease_inout_sine(float t);
API float ease_inout_quad(float t);
API float ease_inout_cubic(float t);
API float ease_inout_quart(float t);
API float ease_inout_quint(float t);
API float ease_inout_expo(float t);
API float ease_inout_circ(float t);
API float ease_inout_back(float t);
API float ease_inout_elastic(float t);
API float ease_inout_bounce(float t);

API float ease_perlin_inout(float t);

API float ease_ping_pong(float t, float(*fn1)(float), float(*fn2)(float));
API float ease_pong_ping(float t, float(*fn1)(float), float(*fn2)(float));

// ----------------------------------------------------------------------------

API float deg      (float radians)      ;
API float rad      (float degrees)      ;

API int   mini     (int    a, int    b);
API int   maxi     (int    a, int    b);
API int   absi     (int    a          );
API float minf     (float  a, float  b);
API float maxf     (float  a, float  b);
API float absf     (float  a          );
API float pmodf    (float  a, float  b);
API float signf    (float  a)           ;
API float clampf(float v,float a,float b);
API float mixf(float a,float b,float t);

// ----------------------------------------------------------------------------

API vec2  ptr2     (const float *a    );
//
API vec2  neg2     (vec2   a          );
API vec2  add2     (vec2   a, vec2   b);
API vec2  sub2     (vec2   a, vec2   b);
API vec2  mul2     (vec2   a, vec2   b);
API vec2  inc2     (vec2   a, float  b);
API vec2  dec2     (vec2   a, float  b);
API vec2  scale2   (vec2   a, float  b);
API vec2  div2     (vec2   a, float  b);
API vec2  pmod2    (vec2   a, float  b);
API vec2  min2     (vec2   a, vec2   b);
API vec2  max2     (vec2   a, vec2   b);
API vec2  abs2     (vec2   a          );
API vec2  floor2   (vec2   a          );
API vec2  fract2   (vec2   a          );
API vec2  ceil2    (vec2   a          );
API float dot2     (vec2   a, vec2   b);
API vec2  refl2    (vec2   a, vec2   b);
API float cross2   (vec2   a, vec2   b);
API float len2sq   (vec2   a          );
API float len2     (vec2   a          );
API vec2  norm2    (vec2   a          );
API int   finite2  (vec2   a          );
API vec2  mix2  (vec2 a,vec2 b,float t);
API vec2  clamp2(vec2 v,float a,float b);
// ----------------------------------------------------------------------------

API vec3  ptr3     (const float *a    );
API vec3  vec23    (vec2   a, float z );
//
API vec3  neg3     (vec3   a          );
API vec3  add3     (vec3   a, vec3   b);
API vec3  sub3     (vec3   a, vec3   b);
API vec3  mul3     (vec3   a, vec3   b);
API vec3  inc3     (vec3   a, float  b);
API vec3  dec3     (vec3   a, float  b);
API vec3  scale3   (vec3   a, float  b);
API vec3  div3     (vec3   a, float  b);
API vec3  pmod3    (vec3   a, float  b);
API vec3  min3     (vec3   a, vec3   b);
API vec3  max3     (vec3   a, vec3   b);
API vec3  abs3     (vec3   a          );
API vec3  floor3   (vec3   a          );
API vec3  fract3   (vec3   a          );
API vec3  ceil3    (vec3   a          );
API vec3  cross3   (vec3   a, vec3   b);
API float dot3     (vec3   a, vec3   b);
API vec3  refl3    (vec3   a, vec3   b);
API float len3sq   (vec3   a          );
API float len3     (vec3   a          );
API vec3  norm3    (vec3   a          );
API vec3  norm3sq  (vec3   a          );
API int   finite3  (vec3   a          );
API vec3  mix3  (vec3 a,vec3 b,float t);
API vec3  clamp3(vec3 v,float a,float b);
//vec3 tricross3 (vec3 a, vec3 b, vec3 c);
API void  ortho3   (vec3 *left, vec3 *up, vec3 v);

// ----------------------------------------------------------------------------

API vec4  ptr4     (const float *a    );
API vec4  vec34    (vec3   a, float w );
//
API vec4  neg4     (vec4   a          );
API vec4  add4     (vec4   a, vec4   b);
API vec4  sub4     (vec4   a, vec4   b);
API vec4  mul4     (vec4   a, vec4   b);
API vec4  inc4     (vec4   a, float  b);
API vec4  dec4     (vec4   a, float  b);
API vec4  scale4   (vec4   a, float  b);
API vec4  div4     (vec4   a, float  b);
API vec4  pmod4    (vec4   a, float  b);
API vec4  min4     (vec4   a, vec4   b);
API vec4  max4     (vec4   a, vec4   b);
API vec4  abs4     (vec4   a          );
API vec4  floor4   (vec4   a          );
API vec4  fract4   (vec4   a          );
API vec4  ceil4    (vec4   a          );
API float dot4     (vec4   a, vec4   b);
API vec4  refl4    (vec4   a, vec4   b);
API float len4sq   (vec4   a          );
API float len4     (vec4   a          );
API vec4  norm4    (vec4   a          );
API vec4  norm4sq  (vec4   a          );
API int   finite4  (vec4   a          );
API vec4  mix4  (vec4 a,vec4 b,float t);
API vec4  clamp4(vec4 v,float a,float b);
// vec4 cross4(vec4 v0, vec4 v1);

// ----------------------------------------------------------------------------

API quat  idq      (                  );
API quat  ptrq     (const float *a    );
API quat  vec3q    (vec3   a, float w );
API quat  vec4q    (vec4   a          );
//
API quat  negq     (quat   a          );
API quat  conjq    (quat   a          );
API quat  addq     (quat   a, quat   b);
API quat  subq     (quat   a, quat   b);
API quat  mulq     (quat   p, quat   q);
API quat  scaleq   (quat   a, float  s);
API quat  normq    (quat   a          );
API float dotq     (quat   a, quat   b);
API quat  mixq(quat a, quat b, float t);
/* quat lerpq(quat a, quat b, float s);
    return norm(quat((1-s)*a.x + s*b.x, (1-s)*a.y + s*b.y, (1-s)*a.z + s*b.z, (1-s)*a.w + s*b.w));
}*/
API quat slerpq(quat a, quat b, float s);

API quat  rotationq(float deg,float x,float y,float z);
API quat  mat44q   (mat44 M);

API vec3 rotate3q_2(vec3 v, quat q);
API vec3 rotate3q(vec3 v, quat r);

// euler <-> quat
API vec3  euler    (quat q);
API quat  eulerq   (vec3 pyr_degrees);

// ----------------------------------------------------------------------------

API void scaling33(mat33 m, float x, float y, float z);
API void scale33(mat33 m, float x, float y, float z);
API void id33(mat33 m);
API void extract33(mat33 m, const mat44 m4);
API void copy33(mat33 m, const mat33 a);//
API vec3 mulv33(mat33 m, vec3 v);
API void multiply33x2(mat33 m, const mat33 a, const mat33 b);

API void rotation33(mat33 m, float degrees, float x,float y,float z);
API void rotationq33(mat33 m, quat q);
API void rotate33(mat33 r, float degrees, float x,float y,float z);
API void compose33(mat33 m, quat r, vec3 s);

// ----------------------------------------------------------------------------

API void id34(mat34 m);
API void copy34(mat34 m, const mat34 a);
API void scale34(mat34 m, float s);
API void add34(mat34 m, mat34 n);
API void muladd34(mat34 m, mat34 n, float s);
API void add34x2(mat34 m, mat34 n, mat34 o);
API void lerp34(mat34 m, mat34 n, mat34 o, float alpha); // mix34?
API void multiply34x2(mat34 m, const mat34 m0, const mat34 m1);
API void multiply34(mat34 m, const mat34 a);
API void multiply34x3(mat34 m, const mat34 a, const mat34 b, const mat34 c);
API void compose34(mat34 m, vec3 t, quat q, vec3 s);
API void invert34(mat34 m, const mat34 o);

// ----------------------------------------------------------------------------

API void scaling44(mat44 m, float x, float y, float z);
API void id44(mat44 m);
API void identity44(mat44 m);
API void copy44(mat44 m, const mat44 a);
API void multiply44x2(mat44 m, const mat44 a, const mat44 b);
API void multiply44x3(mat44 m, const mat44 a, const mat44 b, const mat44 c);
API void multiply44(mat44 m, const mat44 a);
// ---
API void ortho44(mat44 m, float l, float r, float b, float t, float n, float f);
API void frustum44(mat44 m, float l, float r, float b, float t, float n, float f);
API void perspective44(mat44 m, float fovy_degrees, float aspect, float nearp, float farp);
API void lookat44(mat44 m, vec3 eye, vec3 center, vec3 up);
// ---
API void translation44(mat44 m, float x, float y, float z);
API void translate44(mat44 m, float x, float y, float z);
API void relocate44(mat44 m, float x, float y, float z);
API void rotationq44(mat44 m, quat q);
API void rotation44(mat44 m, float degrees, float x, float y, float z);
API void rotate44(mat44 m, float degrees, float x, float y, float z);
API void scaling44(mat44 m, float x, float y, float z);
API void scale44(mat44 m, float x, float y, float z);
// ---
API void transpose44(mat44 m, const mat44 a);
API float det44(const mat44 M);
API bool invert44(mat44 T, const mat44 M);

API vec4 transform444(const mat44, const vec4);
API bool unproject44(vec3 *out, vec3 xyd, vec4 viewport, mat44 mvp);

API vec3 transform_axis(const coord_system, const coord_axis);
API void rebase44(mat44 m, const coord_system src_basis, const coord_system dst_basis);

API void compose44(mat44 m, vec3 t, quat q, vec3 s);

// ----------------------------------------------------------------------------

API vec3 transform33(mat33 m, vec3 p);

API vec4 transform444(const mat44 m, const vec4 p);

API vec3 transform344(const mat44 m, const vec3 p);

API vec3 transformq(const quat q, const vec3 v);

API vec3 transform_axis(const coord_system basis, const coord_axis to);

// A vector is the difference between two points in 3D space, possessing both direction and magnitude
API vec3 transform_vector  (const mat44 m, const vec3 vector)   ;

// A point is a specific location within a 3D space
API vec3 transform_point   (const mat44 m, const vec3 p)    ; // return (m * vec4{point,1).xyz()/r.w;

// A tangent is a unit-length vector which is parallel to a piece of geometry, such as a surface or a curve
API vec3 transform_tangent (const mat44 m, const vec3 tangent)  ;//{ return norm3(transform_vector(m, tangent)); }

// A normal is a unit-length bivector which is perpendicular to a piece of geometry, such as a surface or a curve
API vec3 transform_normal  (const mat44 m, const vec3 normal)   ;

// A quaternion can describe both a rotation and a uniform scaling in 3D space
API quat transform_quat     (const mat44 m, const quat q)      ;

// A matrix can describe a general transformation of homogeneous coordinates in projective space
API float* transform_matrix(mat44 out, const mat44 m, const mat44 matrix);

// Scaling factors are not a vector, they are a compact representation of a scaling matrix
API vec3 transform_scaling (const mat44 m, const vec3 scaling);

// ----------------------------------------------------------------------------
// !!! for debugging

API void print2( vec2 v );
API void print3( vec3 v );
API void print4( vec4 v );
API void printq( quat q );
API void print33( float *m );
API void print34( float *m );
API void print44( float *m );
#line 0
#line 1 "fwk_collide.h"
// -----------------------------------------------------------------------------
// original code by @vurtun (PD) and @barerose (CC0).
// [src] https://gist.github.com/vurtun/95f088e4889da2474ad1ce82d7911fee
// - rlyeh, public domain.

#ifndef GJK_H
#define GJK_H

#define GJK_MAX_ITERATIONS 20

typedef struct gjk_support {
    int aid, bid;
    vec3 a;
    vec3 b;
} gjk_support;
typedef struct gjk_vertex {
    vec3 a;
    vec3 b;
    vec3 p;
    int aid, bid;
} gjk_vertex;
typedef struct gjk_simplex {
    int max_iter, iter;
    int hit, cnt;
    gjk_vertex v[4];
    float bc[4], D;
} gjk_simplex;
typedef struct gjk_result {
    int hit;
    vec3 p0;
    vec3 p1;
    float distance_squared;
    int iterations;
} gjk_result;

int gjk(gjk_simplex *s, const gjk_support *sup, vec3 *dv);
gjk_result gjk_analyze(const gjk_simplex *s);
gjk_result gjk_quad(float a_radius, float b_radius);

#endif

//typedef struct gjk_result gjk_result;

typedef struct line     { vec3 a, b;                                                  } line;
typedef struct sphere   { vec3 c; float r;                                            } sphere;
typedef struct aabb     { vec3 min, max;                                              } aabb;
typedef struct plane    { vec3 p, n;                                                  } plane;
typedef struct capsule  { vec3 a, b; float r;                                         } capsule;
typedef struct ray      { vec3 p, d;                                                  } ray;
typedef struct triangle { vec3 p0,p1,p2;                                              } triangle;
typedef struct poly     { vec3* verts; int cnt;                                       } poly;
typedef union  frustum  { struct { vec4 l, r, t, b, n, f; }; vec4 pl[6]; float v[24]; } frustum;

#define line(...)       M_CAST(line, __VA_ARGS__)
#define sphere(...)     M_CAST(sphere, __VA_ARGS__)
#define aabb(...)       M_CAST(aabb, __VA_ARGS__)
#define plane(...)      M_CAST(plane, __VA_ARGS__)
#define capsule(...)    M_CAST(capsule, __VA_ARGS__)
#define ray(p,normdir)  M_CAST(ray, p, normdir)
#define triangle(...)   M_CAST(triangle, __VA_ARGS__)
#define poly(...)       M_CAST(poly, __VA_ARGS__)
#define frustum(...)    M_CAST(frustum, __VA_ARGS__)

// ----------------------------------------------------------------------------

typedef struct hit {
    union {
        // general case
        float depth;
        // rays only: penetration (t0) and extraction (t1) points along ray line
        struct { float t0, t1; };
        // gjk only
        struct { int hits; vec3 p0, p1; float distance2; int iterations; };
    };
    union { vec3 p; vec3 contact_point; };
    union { vec3 n; vec3 normal; };
} hit;

#define hit(...)        M_CAST(hit, __VA_ARGS__)

// ----------------------------------------------------------------------------

/* line/segment */
API float   line_distance2_point(line l, vec3 p);
API vec3    line_closest_point(line l, vec3 p);
/* ray */
API float   ray_test_plane(ray r, vec4 p4);
API float   ray_test_triangle(ray r, triangle t);
API int     ray_test_sphere(float *t0, float *t1, ray r, sphere s);
API int     ray_test_aabb(float *t0, float *t1, ray r, aabb a);
API hit*    ray_hit_plane(ray r, plane p);
API hit*    ray_hit_triangle(ray r, triangle t);
API hit*    ray_hit_sphere(ray r, sphere s);
API hit*    ray_hit_aabb(ray r, aabb a);
/* sphere */
API vec3    sphere_closest_point(sphere s, vec3 p);
API hit*    sphere_hit_aabb(sphere s, aabb a);
API hit*    sphere_hit_capsule(sphere s, capsule c);
API hit*    sphere_hit_sphere(sphere a, sphere b);
API int     sphere_test_aabb(sphere s, aabb a);
API int     sphere_test_capsule(sphere s, capsule c);
API int     sphere_test_poly(sphere s, poly p);
API int     sphere_test_sphere(sphere a, sphere b);
/* aabb */
API vec3    aabb_closest_point(aabb a, vec3 p);
API float   aabb_distance2_point(aabb a, vec3 p);
API int     aabb_contains_point(aabb a, vec3 p);
API hit*    aabb_hit_aabb(aabb a, aabb b);
API hit*    aabb_hit_capsule(aabb a, capsule c);
API hit*    aabb_hit_sphere(aabb a, sphere s);
API int     aabb_test_aabb(aabb a, aabb b);
API int     aabb_test_capsule(aabb a, capsule c);
API int     aabb_test_poly(aabb a, poly p);
API int     aabb_test_sphere(aabb a, sphere s);
/* capsule */
API float   capsule_distance2_point(capsule c, vec3 p);
API vec3    capsule_closest_point(capsule c, vec3 p);
API hit*    capsule_hit_aabb(capsule c, aabb a);
API hit*    capsule_hit_capsule(capsule a, capsule b);
API hit*    capsule_hit_sphere(capsule c, sphere s);
API int     capsule_test_aabb(capsule c, aabb a);
API int     capsule_test_capsule(capsule a, capsule b);
API int     capsule_test_poly(capsule c, poly p);
API int     capsule_test_sphere(capsule c, sphere s);
/* poly: query */
API int     poly_test_sphere(poly p, sphere s);
API int     poly_test_aabb(poly p, aabb a);
API int     poly_test_capsule(poly p, capsule c);
API int     poly_test_poly(poly a, poly b);
/* poly: query transformed */
API int     poly_test_sphere_transform(poly p, vec3 pos3, mat33 rot33, sphere s);
API int     poly_test_aabb_transform(poly p, vec3 apos3, mat33 arot33, aabb a);
API int     poly_test_capsule_transform(poly p, vec3 pos3, mat33 rot33, capsule c);
API int     poly_test_poly_transform(poly a, vec3 apos3, mat33 arot33, poly b, vec3 bpos3, mat33 brot33);
/* poly: gjk result */
API int     poly_hit_sphere(struct gjk_result *res, poly p, sphere s);
API int     poly_hit_aabb(struct gjk_result *res, poly p, aabb a);
API int     poly_hit_capsule(struct gjk_result *res, poly p, capsule c);
API int     poly_hit_poly(struct gjk_result *res, poly a, poly b);
/* poly: gjk result transformed */
API int     poly_hit_sphere_transform(struct gjk_result *res, poly p, vec3 pos3, mat33 rot33, sphere s);
API int     poly_hit_aabb_transform(struct gjk_result *res, poly p, vec3 pos3, mat33 rot33, aabb a);
API int     poly_hit_capsule_transform(struct gjk_result *res, poly p, vec3 pos3, mat33 rot33, capsule c);
API int     poly_hit_poly_transform(struct gjk_result *res, poly a, vec3 at3, mat33 ar33, poly b, vec3 bt3, mat33 br33);

API vec4    plane4(vec3 p, vec3 n);

API frustum frustum_build(mat44 projview);
API int     frustum_test_sphere(frustum f, sphere s);
API int     frustum_test_aabb(frustum f, aabb a);

API poly    poly_alloc(int cnt);
API void    poly_free(poly *p);

API poly    pyramid(vec3 from, vec3 to, float size); // poly_free() required
API poly    diamond(vec3 from, vec3 to, float size); // poly_free() required
#line 0
//---
#line 1 "fwk_audio.h"
// -----------------------------------------------------------------------------
// audio framework
// - rlyeh, public domain
//
// fixme: leaks, audio_delete
// fixme: sfxr (hack)
// @todo: audio_volume_fx, audio_volume_bgm, audio_volume_master instead?
// @todo: destroystream()    if( ss->type == WAV ) drwav_uninit(&ss->wav);
// @todo: destroystream()    if( ss->type == MOD ) jar_mod_unload(&ss->mod);
// @todo: destroystream()    if( ss->type == XM && ss->xm ) jar_xm_free_context(ss->xm);

typedef struct audio_handle* audio_t;

API audio_t audio_clip( const char *pathfile );
API audio_t audio_stream( const char *pathfile );
API     int audio_play( audio_t s, int flags );

API float   audio_volume_clip(float gain);   // set     fx volume if gain is in [0..1] range. return current     fx volume in any case
API float   audio_volume_stream(float gain); // set    bgm volume if gain is in [0..1] range. return current    bgm volume in any case
API float   audio_volume_master(float gain); // set master volume if gain is in [0..1] range. return current master volume in any case

enum {
    AUDIO_1CH = 0, // default
    AUDIO_2CH = 1,

    AUDIO_8 = 2,
    AUDIO_16 = 0, // default
    AUDIO_32 = 4,
    AUDIO_FLOAT = 8,

    AUDIO_22KHZ = 0, // default
    AUDIO_44KHZ = 16,
};

API int audio_queue( const void *samples, int num_samples, int flags );
#line 0
#line 1 "fwk_cooker.h"
// -----------------------------------------------------------------------------
// asset pipeline framework
// - rlyeh, public domain.
//
// all cooked assets are stored inside .cook.zip file at root folder, which acts as an asset database.
// during game boot, the database gets rebuilt as follows: (note: step 0 is an optional optimization)
// 0. for N given cores, split list of infiles into N zipfiles. then, parallelize cookers.
// 1. compare local disk files against file in zip database. for each mismatch do:
// 2. - invalidate its entry in database, if local file was removed from disk.
// 3. - write its *cooked* contents into database, if local file was created or modified from disk.
// 4. mount any existing zipfile(s) after cooking.
//
// notes: meta-datas from every raw asset are stored into comment field, inside .cook.zip archive.
// @todo: fix leaks
// @todo: symlink exact files
// @todo: idle threads should steal jobs from busy threads (maybe use jobs/coroutines for this?)
// @todo: ... and should compress them in the original cook[N] bucket

enum {
    COOKER_ASYNC = 1,
};

// user defined callback for asset cooking:
// must read infile, process data, and write it to outfile
// must set errno on exit if errors are found
// must return compression level if archive needs to be cooked, else return <0
typedef int (*cooker_callback_t)(char *filename, const char *ext, const char header[16], FILE *in, FILE *out, const char *info, int threadid);

API int  cooker_progress(); // [0..100]
API bool cooker( const char *masks, cooker_callback_t cb, int flags );
#line 0
#line 1 "fwk_data.h"
// -----------------------------------------------------------------------------
// data framework (json5) @todo:xml,kvdb
// - rlyeh, public domain
//
// @todo: vec2,vec3,vec4

// data api

API bool    data_push(const char *source);
API int         data_count(const char *keypath);
#define         data_int(...)    data_get(0,stringf(__VA_ARGS__)).i
#define         data_float(...)  data_get(0,stringf(__VA_ARGS__)).f
#define         data_string(...) data_get(1,stringf(__VA_ARGS__)).s
API bool    data_pop();

// internal api

typedef union data_t {
    char* s;
    double f;
    int64_t i;
    uintptr_t p;
    array(union data_t) arr;
} data_t;

API data_t data_get(bool is_string, const char *keypath); // @todo, array(data_t) data_array();
#line 0
#line 1 "fwk_editor.h"
// -----------------------------------------------------------------------------
// in-game editor
// - rlyeh, public domain.
//
// when editor is on:
// - [ ] ESC, toggles editor on/off
// - [ ] ESC (long press), sends escape key to game
// - [ ] F5, toggles game pause on/off
// - [ ] F5 (long press), restarts game
// - [ ] TAB, see console/log
// - [ ] LMB, select item, display hierarchy @ left sidebar, status @ right sidebar. right click to unselect
// - [ ] RMB, on selected item, display contextual menu: edit, edit script...
// - [ ] RMB, camera move if no gameobj is selected + WASDEC + wheel
// - [ ] SPC, on selected item, cycle transform: locate, rotate, scale
// - [ ] CTRL+S save, CTRL+L load restore point
// - [ ] IMPR save capture bug/info
//
// roadmap
// - [x] ray/object picking
// - [x] basic gizmos (@todo: fixed screen size, snapping)
// - [ ] introspect properties on selected object
// - [ ] add/rem entities, add/rem components, add/rem/pause/resume systems
// - [ ] widgets for (bool,str,flt,int,vec2,vec3,vec4,range,mesh,texture,audio,any other asset,combo of anything)
// - [ ] osc server for properties and editor behavior
// - [ ] multiple selections/select all
// - [ ] undo/redo
// - [ ] cut/copy/paste (ctrl-c to serialize)
// - [ ] menu: open, save, save as, save all, reload

API void editor();
API bool editor_active();
#line 0
#line 1 "fwk_file.h"
// -----------------------------------------------------------------------------
// files, cache and virtual filesystem (registered directories and/or compressed zip archives).
// - rlyeh, public domain.
//
// - note: vfs_mount() order matters (last mounts have higher priority).
// - note: directory/with/trailing/slash/ as mount_point, or zip/tar/pak archive otherwise.
//
// @todo: file_mmap

// physical filesystem. files

API const char** file_list(const char *masks); // **.png;*.c
API char *       file_read(const char *filename);
API char *       file_load(const char *filename, int *len);
API uint64_t     file_size(const char *pathfile);
API bool         file_directory(const char *pathfile);

API char *       file_path(const char *pathfile); // c:/prj/dir/file.ext -> c:/prj/dir/
API char *       file_name(const char *pathfile); // c:/prj/dir/file.ext -> file.ext
API char *       file_ext(const char *pathfile); // c:/prj/dir/file.ext -> .ext
API char *       file_id(const char *pathfile); // c:/prj/dir/file.ext -> file/dir/prj (name then alphabetical)
API char *       file_normalize(const char *pathfile); // c:/prj/dir/file.ext -> c/prj/dir/file_ext
//API char *     file_normalize_with_folder(const char *pathfile); // c:/prj/dir/file.ext -> dir/file_ext

API uint64_t     file_stamp(const char *pathfile); // 1616153596 (seconds since unix epoch)
API uint64_t     file_stamp_human(const char *pathfile); // 20210319113316 (datetime in base10)

API bool         file_copy(const char *src, const char *dst);

API FILE*        file_temp();
API char*        file_tempname();

// @todo file_find() from first file_scan()


// virtual filesystem

API bool         vfs_mount(const char *mount_point);
API const char** vfs_list(const char *masks); // **.png;*.c

API char *       vfs_read(const char *pathfile);
API char *       vfs_load(const char *pathfile, int *size);
API int          vfs_size(const char *pathfile);

API const char * vfs_resolve(const char *fuzzyname); // guess best match. @todo: fuzzy path
API const char * vfs_find(const char *pathfile); // returns filename to extracted temporary file, so it can be read by foreign/3rd party libs
API FILE*        vfs_handle(const char *pathfile); // same as above, but returns file handle instead. preferred way, will clean descriptors at exit

// cache

API void *       cache_insert(const char *key, void *value, int size);
API void *       cache_lookup(const char *key, int *size);
#line 0
#line 1 "fwk_input.h"
// -----------------------------------------------------------------------------
// input framework
// - rlyeh, public domain
//
// @todo: touch, window
// @todo: for extra savings (168->72 bytes), promote bits to real bits (/8 %8) & normalized floats [-1,+1] to shorts or chars
// @todo: GAMEPAD_A|2, MOUSE_L|1, KEY_C|3
// @todo: load/save
// @todo: send virtual presses & outputs (rumble, light, led, text, etc)
// @todo: fix if logger !60 hz
// @tofo: fix click2/repeat edge cases

API int         input_use( int controller_id ); // [0..3]

// -- basic polling api (read input at current frame)

API float       input( int vk );
API vec2        input2( int vk );
API float       input_diff( int vk );
API vec2        input_diff2( int vk );

// -- extended polling api (read input at Nth frame ago)

API float       input_frame( int vk, int frame );
API vec2        input_frame2( int vk, int frame );
API const char* input_frames( int vk, int frame );

// -- events api

API int         input_up( int vk ); // ON -> OFF (release)
API int         input_down( int vk ); // OFF -> ON (trigger)
API int         input_held( int vk ); // ON -> ON (pressed)
API int         input_idle( int vk ); // OFF -> OFF

API int         input_click( int vk, int ms ); // OFF -> ON -> OFF
API int         input_click2( int vk, int ms ); // OFF -> ON -> OFF -> ON -> OFF
API int         input_repeat( int vk, int ms ); // [...] ON -> ON -> ON

API int         input_chord2( int vk1, int vk2 ); // all vk1 && vk2 are ON
API int         input_chord3( int vk1, int vk2, int vk3 ); // all vk1 && vk2 && vk3 are ON
API int         input_chord4( int vk1, int vk2, int vk3, int vk4 ); // all vk1 && vk2 && vk3 && vk4 are ON

// -- 1d/2d filters

API float       input_filter_positive( float v ); // [-1..1] -> [0..1]
API vec2        input_filter_positive2( vec2 v ); // [-1..1] -> [0..1]
API vec2        input_filter_deadzone( vec2 v, float deadzone );
API vec2        input_filter_deadzone_4way( vec2 v, float deadzone );

// -- utils

API void        input_demo();
API void        input_send( int vk ); // @todo
API void*       input_save_state( int id, int *size); // @todo
API bool        input_load_state( int id, void *ptr, int size); // @todo


// --

enum {
    // -- bits: x104 keyboard, x3 mouse, x15 gamepad, x7 window
    // keyboard gaming keys (53-bit): first-class keys for gaming
    KEY_ESC,
    KEY_TICK, KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,  KEY_BS,
    KEY_TAB,   KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,
    KEY_CAPS,     KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L, KEY_ENTER,
    KEY_LSHIFT,       KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,        KEY_RSHIFT,            KEY_UP,
    KEY_LCTRL,KEY_LALT,               KEY_SPACE,                KEY_RALT,KEY_RCTRL,  KEY_LEFT,KEY_DOWN,KEY_RIGHT,

    // for completeness, secondary keys below (52-bit). beware!
    KEY_INS,KEY_HOME,KEY_PGUP,KEY_DEL,KEY_END,KEY_PGDN, // beware: different behavior win/osx (also, osx: no home/end).
    KEY_LMETA,KEY_RMETA,KEY_MENU,KEY_PRINT,KEY_PAUSE,KEY_SCROLL,KEY_NUMLOCK, // beware: may trigger unexpected OS behavior. (@todo: add RSHIFT here for win?)
    KEY_MINUS,KEY_EQUAL,KEY_LSQUARE,KEY_RSQUARE,KEY_SEMICOLON,KEY_QUOTE,KEY_HASH,KEY_BAR,KEY_COMMA,KEY_DOT,KEY_SLASH, // beware: non-us keyboard layouts
    KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12, // beware: complicated on laptops/osx
    KEY_PAD1,KEY_PAD2,KEY_PAD3,KEY_PAD4,KEY_PAD5,KEY_PAD6,KEY_PAD7,KEY_PAD8,KEY_PAD9,KEY_PAD0, // beware: complicated on laptops
    KEY_PADADD,KEY_PADSUB,KEY_PADMUL,KEY_PADDIV,KEY_PADDOT,KEY_PADENTER, // beware: complicated on laptops

    MOUSE_L, MOUSE_M, MOUSE_R,
    GAMEPAD_CONNECTED, GAMEPAD_A, GAMEPAD_B, GAMEPAD_X, GAMEPAD_Y,
    GAMEPAD_UP, GAMEPAD_DOWN, GAMEPAD_LEFT, GAMEPAD_RIGHT, GAMEPAD_MENU, GAMEPAD_START,
    GAMEPAD_LB, GAMEPAD_RB, GAMEPAD_LTHUMB, GAMEPAD_RTHUMB,
    WINDOW_BLUR, WINDOW_FOCUS, WINDOW_CLOSE, WINDOW_MINIMIZE, WINDOW_MAXIMIZE, WINDOW_FULLSCREEN, WINDOW_WINDOWED,

    // -- floats: x7 gamepad, x3 mouse, x4 touch, x4 window
    GAMEPAD_LPAD, GAMEPAD_LPADX = GAMEPAD_LPAD, GAMEPAD_LPADY,
    GAMEPAD_RPAD, GAMEPAD_RPADX = GAMEPAD_RPAD, GAMEPAD_RPADY,
    GAMEPAD_LT, GAMEPAD_RT, GAMEPAD_BATTERY,
    MOUSE, MOUSE_X = MOUSE, MOUSE_Y, MOUSE_W,
    TOUCH_X1, TOUCH_Y1, TOUCH_X2, TOUCH_Y2,
    WINDOW_RESIZE, WINDOW_RESIZEX = WINDOW_RESIZE, WINDOW_RESIZEY, WINDOW_ORIENTATION, WINDOW_BATTERY,

    // -- strings: x2 gamepad
    GAMEPAD_GUID, GAMEPAD_NAME,
};
#line 0
#line 1 "fwk_network.h"
// -----------------------------------------------------------------------------
// network framework
// - rlyeh, public domain

API int download( FILE *out, const char *url );
API int portname( const char *service_name, unsigned retries );

// -----------------------------------------------------------------------------
// udp wrapper
// - rlyeh, public domain.

// server
API int   udp_bind(const char *address, const char *port);

// client
API int   udp_open(const char *address, const char *port);

// common
API int   udp_send(int, const void *buf, int len ); // <0 error, >0 bytes sent ok
API int   udp_sendto(int, const char *ip, const char *port, const void *buf, int len ); // <0 error, >0 bytes sent ok
API int   udp_recv(int, void *buf, int len ); // <0 error, 0 orderly shutdown, >0 received bytes
API int   udp_peek(int); // <0 error, 0 timeout, >0 data

// -----------------------------------------------------------------------------
// tcp wrapper
// - rlyeh, public domain

// client
API int   tcp_open(const char *address, const char *port);

// server
API int   tcp_bind(const char *interface_, const char *port, int queue);
API int   tcp_peek(int, int(*callback)(int));

// common
API int   tcp_send(int, const void* buf, int len);
API int   tcp_recv(int, void* buf, int len);
API char* tcp_host(int); // info
API char* tcp_port(int); // info

API int   tcp_close(int);

// extras
API int   tcp_debug(int); // toggle traffic monitoring on/off for given socket
//API int   tcp_printf(int, const char *fmt, ...); // printf message in remote end
//API int   tcp_crypt(int,uint64_t);               // set shared secret
#line 0
#line 1 "fwk_render.h"
// -----------------------------------------------------------------------------
// naive rendering framework
// - rlyeh, public domain
//
// IQM skeletal meshes by @lsalzman (public domain) - https://bit.ly/2OQh0Me
// SH code by @ands (public domain) - https://github.com/ands/spherical_harmonics_playground
// SHM code by @jarikomppa (unlicensed) - https://github.com/jarikomppa/shadertoolkit

typedef unsigned handle; // GLuint

// -----------------------------------------------------------------------------
// colors

API uint32_t rgba( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
API uint32_t bgra( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
API float    alpha( uint32_t rgba );

#define RGBX(rgb,x)   ( ((rgb)&0xFFFFFF) | ((x)<<24) )
#define RGB3(r,g,b)   ( ((r)<<16) | ((g)<<8) | (b) )
#define RGB4(r,g,b,a) RGBX(RGB3(r,g,b),a)

#define BLACK   RGBX(0x000000,255)
#define RED     RGBX(0xFF004D,255)
#define GREEN   RGBX(0x00B543,255)
#define BLUE    RGBX(0x065AB5,255)
#define ORANGE  RGBX(0xFF6C24,255)
#define CYAN    RGBX(0x29ADFF,255)
#define PURPLE  RGBX(0x7E2553,255)
#define YELLOW  RGBX(0xFFEC27,255)
#define WHITE   RGBX(0xFFF1E8,255)
#define GRAY    RGBX(0x725158,255)

// -----------------------------------------------------------------------------
// images

enum {
    IMAGE_R    = 0x01000,
    IMAGE_RG   = 0x02000,
    IMAGE_RGB  = 0x04000,
    IMAGE_RGBA = 0x08000,
    IMAGE_FLIP = 0x10000,
};

typedef struct image_t {
    union { unsigned x, w; };
    union { unsigned y, h; };
    union { unsigned n, comps; };
    union { void *pixels; unsigned char *pixels8; unsigned short *pixels16; unsigned *pixels32; float *pixelsf; };
} image_t;

API image_t image(const char *pathfile, int flags);
API image_t image_from_mem(const char *ptr, int len, int flags);
API void    image_destroy(image_t *img);

// -----------------------------------------------------------------------------
// textures

enum {
    // UNIT[0..7]

    TEXTURE_BC1 = 8,  // DXT1, RGB with 8:1 compression ratio (+ optional 1bpp for alpha)
    TEXTURE_BC2 = 16, // DXT3, RGBA with 4:1 compression ratio (BC1 for RGB + 4bpp for alpha)
    TEXTURE_BC3 = 32, // DXT5, RGBA with 4:1 compression ratio (BC1 for RGB + BC4 for A)
//  TEXTURE_BC4,     // Alpha

    TEXTURE_NEAREST = 0,
    TEXTURE_LINEAR = 64,
    TEXTURE_MIPMAPS = 128,

    TEXTURE_EDGE = 0,
    TEXTURE_BORDER = 0x100,
    TEXTURE_REPEAT = 0x200,

    TEXTURE_BYTE = 0,
    TEXTURE_FLOAT = 0x400,

    TEXTURE_COLOR = 0,
    TEXTURE_DEPTH = 0x800,

    TEXTURE_R = IMAGE_R,
    TEXTURE_RG = IMAGE_RG,
    TEXTURE_RGB = IMAGE_RGB,
    TEXTURE_RGBA = IMAGE_RGBA,
    TEXTURE_FLIP = IMAGE_FLIP,

    // @fixme
    TEXTURE_SRGB = 1 << 24,
    TEXTURE_BGR = 1 << 25,
    TEXTURE_ARRAY = 1 << 26,
};

typedef struct {
    union { unsigned x, w; };
    union { unsigned y, h; };
    union { unsigned n, bpp; };
    handle id;
    unsigned flags;
} texture_t;

API texture_t texture(const char* filename, int flags);
API texture_t texture_from_mem(const char* ptr, int len, int flags);
API texture_t texture_create(unsigned w, unsigned h, unsigned n, void *pixels, int flags);
API texture_t texture_checker();
API void      texture_destroy(texture_t *t);
// textureLod(filename, dir, lod);
// void texture_add_loader( int(*loader)(const char *filename, int *w, int *h, int *bpp, int reqbpp, int flags) );
API unsigned  texture_update(texture_t *t, unsigned w, unsigned h, unsigned n, void *pixels, int flags);

// -----------------------------------------------------------------------------
// fullscreen quads

API void  fullscreen_rgb_quad( texture_t texture_rgb, float gamma );
API void  fullscreen_ycbcr_quad( texture_t texture_YCbCr[3], float gamma );

// -----------------------------------------------------------------------------
// sprites

API void tile( texture_t texture, vec3 position, uint32_t color /*~0u*/, float rotation /*0*/ );

API void sprite( texture_t texture,
    float px, float py, float pz, float rotation, // position(x,y,depth sort), angle
    float ox, float oy, float sx, float sy,       // offset(x,y), scale(x,y)
    int additive, uint32_t rgba,                  // is_additive, tint color
    float frame, float xcells, float ycells       // frame_number in a 8x4 spritesheet
);

API void sprite_update();

// -----------------------------------------------------------------------------
// cubemaps

typedef struct cubemap_t {
    unsigned id;    // texture id
    vec3 sh[9];     // precomputed spherical harmonics coefficients
} cubemap_t;

API cubemap_t  cubemap( const image_t image, int flags ); // 1 equirectangular panorama
API cubemap_t  cubemap6( const image_t images[6], int flags ); // 6 cubemap faces
API void       cubemap_destroy(cubemap_t *c);
API cubemap_t* cubemap_get_active();

// -----------------------------------------------------------------------------
// fbos

API unsigned fbo( unsigned texture_color, unsigned texture_depth, int wr_flags );
API void     fbo_bind(unsigned id);
API void     fbo_unbind();
API void     fbo_destroy(unsigned id);

// -----------------------------------------------------------------------------
// shadowmaps

typedef struct shadowmap_t {
    mat44  shadowmatrix;
    mat44  mvp;
    mat44  mv;
    mat44  proj;
    vec4   light_position;
    int    saved_fb;
    int    saved_viewport[4];
    handle fbo, texture;
    int texture_width;
} shadowmap_t;

API shadowmap_t shadowmap(int texture_width); // = 1024
API void shadowmap_destroy(shadowmap_t *s);

API void shadowmap_set_shadowmatrix(shadowmap_t *s, vec3 aLightPos, vec3 aLightAt, vec3 aLightUp, const mat44 projection);
API void shadowmap_begin(shadowmap_t *s);
API void shadowmap_end(shadowmap_t *s);

// shadowmap utils

API void shadowmatrix_proj(mat44 shm_proj, float aLightFov, float znear, float zfar);
API void shadowmatrix_ortho(mat44 shm_proj, float left, float right, float bottom, float top, float znear, float zfar);

// -----------------------------------------------------------------------------
// shaders

API unsigned shader(const char *vs, const char *fs, const char *attribs, const char *fragcolor);
API unsigned shader_bind(unsigned program);
API     void shader_int(const char *uniform, int i);
API     void shader_float(const char *uniform, float f);
API     void shader_vec2(const char *uniform, vec2 v);
API     void shader_vec3(const char *uniform, vec3 v);
API     void shader_vec4(const char *uniform, vec4 v);
API     void shader_mat44(const char *uniform, mat44 m);
API     void shader_texture(const char *sampler, unsigned texture, unsigned unit);
API unsigned shader_get_active();
API void     shader_destroy(unsigned shader);

// -----------------------------------------------------------------------------
// meshes (@fixme: deprecate?)

enum MESH_FLAGS {
    MESH_STATIC = 0, // STATIC, DYNAMIC, STREAM // zero|single|many updates per frame
    MESH_STREAM = 1,
    MESH_TRIANGLE_STRIP = 2,
};

typedef struct mesh_t {
    handle vao, vbo, ibo;
    unsigned vertex_count;
    unsigned index_count;
    unsigned flags;
} mesh_t;

API mesh_t mesh_create(const char *format, int vertex_stride,int vertex_count,const void *interleaved_vertex_data, int index_count,const void *index_data, int flags);
API   void mesh_upgrade(mesh_t *m, const char *format, int vertex_stride,int vertex_count,const void *interleaved_vertex_data, int index_count,const void *index_data, int flags);
API   void mesh_push_state(mesh_t *m, unsigned program, unsigned texture_id, float model[16], float view[16], float proj[16], unsigned billboard);
API   void mesh_pop_state(mesh_t *m);
API   void mesh_render(mesh_t *m);
API   void mesh_destroy(mesh_t *m);
API   aabb mesh_bounds(mesh_t *m);

// -----------------------------------------------------------------------------
// materials (@todo)
//
// typedef struct material_t {
//     const char *name;
//     texture_t texture;
//     uint32_t color;
// } material_t;

// -----------------------------------------------------------------------------
// models

enum {
    MODEL_NO_ANIMATIONS = 1,
    MODEL_NO_MESHES = 2,
    MODEL_NO_TEXTURES = 4,
};

typedef struct model_t {
    struct iqm_t *iqm;
    unsigned num_meshes;
    unsigned num_triangles;
    unsigned num_joints; // num_poses;
    unsigned num_anims;
    unsigned num_frames;
    handle program;
    float curframe;
    mat44 pivot;
} model_t;

API model_t  model(const char *filename, int flags);
API model_t  model_from_mem(const void *mem, int sz, int flags);
API float    model_animate(model_t, float curframe);
API float    model_animate_clip(model_t, float curframe, int minframe, int maxframe, bool loop);
API aabb     model_aabb(model_t, mat44 transform);
API void     model_render2(model_t, mat44 proj, mat44 view, mat44 model, int shader);
API void     model_render(model_t, mat44 proj, mat44 view, mat44 model);
API void     model_destroy(model_t);

// -----------------------------------------------------------------------------
// skyboxes

typedef struct skybox_t {
    handle program;
    mesh_t geometry;
    cubemap_t cubemap;
    int flags;
} skybox_t;

API skybox_t skybox(const char *panorama_or_cubemap_folder, int flags);
API int      skybox_push_state(skybox_t *sky, mat44 proj, mat44 view);
API int      skybox_pop_state(skybox_t *sky);
API void     skybox_destroy(skybox_t *sky);

// -----------------------------------------------------------------------------
// post-fxs

API void     viewport_color(vec3 color);
API void     viewport_clear(bool color, bool depth);
API void     viewport_clip(vec2 from, vec2 to);

API void     fx_load(const char *file);
API void     fx_begin();
API void     fx_end();
API void     fx_enable(int pass, int enabled);
API int      fx_enabled(int pass);
API void     fx_enable_all(int enabled);
API char *   fx_name(int pass);

// -----------------------------------------------------------------------------
// utils

API void*    screenshot(unsigned components); // 3 RGB, 4 RGBA, -3 BGR, -4 BGRA
#line 0
#line 1 "fwk_renderdd.h"
// -----------------------------------------------------------------------------
// debugdraw framework
// - rlyeh, public domain.
//
// Credits: Based on work by @glampert https://github.com/glampert/debug-draw (PD)
// [x] grid, axis, frustum, cube, sphere, triangle, square, pentagon, hexagon, circle, normal.
// [x] arrow, point, text, capsule, aabb, plane, flotilla-style locator, boid,
// [x] line batching
// [*] line width and stipple
// [*] (proper) gizmo,
// [ ] bone (pyramid?), ring,
// [ ] camera, light bulb, light probe,

API void ddraw_color(unsigned rgb);
API void ddraw_ontop(int enabled);
//
API void ddraw_aabb(vec3 minbb, vec3 maxbb);
API void ddraw_aabb_corners(vec3 minbb, vec3 maxbb);
API void ddraw_arrow(vec3 begin, vec3 end);
API void ddraw_axis(float units);
API void ddraw_boid(vec3 pos, vec3 dir);
API void ddraw_bone(vec3 center, vec3 end); // @todo: use me
API void ddraw_bounds(const vec3 points[8]);
API void ddraw_box(vec3 c, vec3 extents);
API void ddraw_capsule(vec3 from, vec3 to, float radius);
API void ddraw_circle(vec3 pos, vec3 n, float radius);
API void ddraw_cone(vec3 center, vec3 top, float radius);
API void ddraw_cube(vec3 center, float radius);
API void ddraw_diamond(vec3 from, vec3 to, float size);
API void ddraw_frustum(float projview[16]);
API void ddraw_ground(float scale);
API void ddraw_grid(float scale);
API void ddraw_hexagon(vec3 pos, float radius);
API void ddraw_line(vec3 from, vec3 to);
API void ddraw_line_dashed(vec3 from, vec3 to);
API void ddraw_line_thin(vec3 from, vec3 to);
API void ddraw_normal(vec3 pos, vec3 n);
API void ddraw_pentagon(vec3 pos, float radius);
API void ddraw_plane(vec3 p, vec3 n, float scale);
API void ddraw_point(vec3 from);
API void ddraw_position(vec3 pos, float radius);
API void ddraw_position_dir(vec3 pos, vec3 dir, float radius);
API void ddraw_pyramid(vec3 center, float height, int segments);
API void ddraw_sphere(vec3 pos, float radius);
API void ddraw_square(vec3 pos, float radius);
API void ddraw_text(vec3 pos, float scale, const char *text);
API void ddraw_text2d(vec2 pos, float scale, const char *text);
API void ddraw_triangle(vec3 p1, vec3 p2, vec3 p3);
//
API void ddraw_demo();
API void ddraw_flush();

#define ddraw_text(pos, scale, ...) ddraw_text(pos, scale, stringf(__VA_ARGS__))
#define ddraw_text2d(pos, scale, ...) ddraw_text2d(pos, scale, stringf(__VA_ARGS__))
#line 0
#line 1 "fwk_scene.h"
// -----------------------------------------------------------------------------
// scene framework
// - rlyeh, public domain
//
// @todo: remove explicit GL code from here

// camera

typedef struct {
    mat44 view, proj;
    vec3 position, up, look; // position, updir, lookdir
    float yaw, pitch, speed; // mirror_x, mirror_y;
    vec3 last_look, last_move; // used for friction and smoothing
} camera_t;

API camera_t camera();
API void camera_move(camera_t *cam, float x, float y, float z);
API void camera_fps(camera_t *cam, float yaw, float pitch);
API void camera_lookat(camera_t *cam, vec3 target);
API camera_t *camera_get_active();

// object

typedef struct {
    uint64_t renderbucket;
    mat44 transform;
    quat rot;
    vec3 sca, pos, euler, pivot;
    handle texture_id;
    model_t model;
    aabb bounds;
    unsigned billboard; // [0..7] x(4),y(2),z(1) masks
} object_t;

API object_t object();
API void object_rotate(object_t *obj, vec3 euler);
API void object_pivot(object_t *obj, vec3 euler);
API void object_teleport(object_t *obj, vec3 pos);
API void object_move(object_t *obj, vec3 inc);
API vec3 object_position(object_t *obj);
API void object_scale(object_t *obj, vec3 sca);
//
API void object_model(object_t *obj, model_t model);
API void object_diffuse(object_t *obj, texture_t tex);
API void object_billboard(object_t *obj, unsigned mode);

// object_pose(transform); // @todo

// scene

enum {
    SCENE_WIREFRAME = 1,
    SCENE_CULLFACE = 2,
    SCENE_BACKGROUND = 4,
    SCENE_FOREGROUND = 8,
};

typedef struct {
    handle program;

    array(object_t) objs;

    // special objects below:
    skybox_t skybox;
    int u_coefficients_sh;
} scene_t;

API scene_t*  scene_push();
API void      scene_pop();
API scene_t*  scene_get_active();

API int       scene_merge(const char *source);
API void      scene_render(int flags);

API object_t* scene_spawn();
API unsigned  scene_count();
API object_t* scene_index(unsigned index);
#line 0
#line 1 "fwk_script.h"
// -----------------------------------------------------------------------------
// script framework
// - rlyeh, public domain

API void script_init();
API void script_run(const char *script);
API void script_runfile(const char *pathfile);

API void script_bind_class(const char *objname, int num_methods, const char **c_names, void **c_functions);
API void script_bind_function(const char *c_name, void *c_function);
API void script_call(const char *lua_function);
#line 0
#line 1 "fwk_system.h"
// -----------------------------------------------------------------------------
// system framework utils
// - rlyeh, public domain.
//
// Note: Windows users add `/Zi` compilation flags, else add `-g` and/or `-ldl` flags
// Note: If you are linking your binary using GNU ld you need to add --export-dynamic

API int         argc();
API char*       argv(int);

API const char* option(const char *commalist, const char *defaults);
API int         optioni(const char *commalist, int defaults);
API float       optionf(const char *commalist, float defaults);

API char*       os_exec_output();
API int         os_exec(const char *command);
#define         os_exec(...) os_exec(file_normalize(stringf(__VA_ARGS__)))

API void        tty_color(unsigned color);
API void        tty_reset();

API int         cpu_cores(void);

API const char* app_name();
API const char* app_path();
API const char* app_cache();
API const char* app_temp();
API void        app_reload();

API double      time_ss();
API double      time_ms();
API uint64_t    time_us();
API uint64_t    date_human(); // YYYYMMDDhhmmss
API double      sleep_ss(double ss);
API double      sleep_ms(double ms);
API uint64_t    sleep_us(uint64_t us);

API char*       callstack( int traces ); // write callstack into a temporary string. do not delete it.
API int         callstackf( FILE *fp, int traces ); // write callstack to file. <0 traces to invert order.

API void        die(const char *message);
API void        alert(const char *message);
API void        hexdump( const void *ptr, unsigned len );
API void        hexdumpf( FILE *fp, const void *ptr, unsigned len, int width );
API void        breakpoint(const char *reason);
API bool        has_debugger();

API uint16_t    lil16(uint16_t n); // swap16 as lil
API uint32_t    lil32(uint32_t n); // swap32 as lil
API float       lil32f(float n);   // swap32 as lil
API uint64_t    lil64(uint64_t n); // swap64 as lil
API double      lil64f(double n);  // swap64 as lil
API uint16_t    big16(uint16_t n); // swap16 as big
API uint32_t    big32(uint32_t n); // swap32 as big
API float       big32f(float n);   // swap32 as big
API uint64_t    big64(uint64_t n); // swap64 as big
API double      big64f(double n);  // swap64 as big

API uint16_t*   lil16p(void *n, int sz);
API uint32_t*   lil32p(void *n, int sz);
API float*      lil32pf(void *n, int sz);
API uint64_t*   lil64p(void *n, int sz);
API double*     lil64pf(void *n, int sz);
API uint16_t*   big16p(void *n, int sz);
API uint32_t*   big32p(void *n, int sz);
API float*      big32pf(void *n, int sz);
API uint64_t*   big64p(void *n, int sz);
API double*     big64pf(void *n, int sz);

#define alert(...)   alert(stringf(__VA_ARGS__))
#define PANIC(...)   PANIC(stringf(__VA_ARGS__), __FILE__, __LINE__) // die() ?
API int (PRINTF)(const char *text, const char *stack, const char *file, int line, const char *function);
API int (PANIC)(const char *error, const char *file, int line);
#line 0
#line 1 "fwk_ui.h"
// -----------------------------------------------------------------------------
// immediate ui framework
// - rlyeh, public domain
//
// @todo: logger/console

API int  ui_begin(const char *title, int flags);
API int    ui_int(const char *label, int *value);
API int    ui_bool(const char *label, bool *value);
API int    ui_short(const char *label, short *value);
API int    ui_float(const char *label, float *value);
API int    ui_float2(const char *label, float value[2]);
API int    ui_float3(const char *label, float value[3]);
API int    ui_string(const char *label, char *buffer, int buflen);
API int    ui_color3(const char *label, float *color3);
API int    ui_color4(const char *label, float *color4);
API int    ui_button(const char *label);
API int    ui_toggle(const char *label, bool *value);
API int    ui_dialog(const char *title, const char *text, int choices, bool *show); // @fixme: return
API int    ui_list(const char *label, const char **items, int num_items, int *selector);
API int    ui_separator();
API int    ui_label(const char *label);
API int    ui_label2(const char *label, const char *caption);
API int    ui_slider(const char *label, float *value);
API int    ui_slider2(const char *label, float *value, const char *caption);
API int    ui_const_bool(const char *label, const double value);
API int    ui_const_float(const char *label, const double value);
API int    ui_const_string(const char *label, const char *value);
#define    ui_const_stringf(label, ...) ui_const_string(label, stringf(__VA_ARGS__))
API void ui_end();

API int  ui_menu(const char *items); // semicolon- or comma-separated items
API int  ui_item();

API int  ui_hover(); // ui_is_hover()?
API int  ui_active(); // ui_is_active()?
API void ui_demo();
#line 0
#line 1 "fwk_video.h"
// -----------------------------------------------------------------------------
// video decoder (mpeg)
// - rlyeh, public domain
//
// [ref] https://github.com/phoboslab/pl_mpeg/blob/master/pl_mpeg_player.c
// [use] ffmpeg -i infile.mp4 -c:v mpeg1video -c:a mp2 -format mpeg outfile.mpg

typedef struct video_t video_t;

API video_t*   video( const char *filename, int flags );
API texture_t*  video_decode( video_t *v ); // decodes next frame, returns associated texture(s)
API int         video_has_finished(video_t *v);
API double      video_duration(video_t *v);
API int         video_seek(video_t *v, double seek_to);
API double      video_position(video_t *v);
API void       video_destroy( video_t *v );
#line 0
#line 1 "fwk_window.h"
// -----------------------------------------------------------------------------
// window framework
// - rlyeh, public domain
//
// @todo: window_cursor(ico);
// @todo: if WINDOW_PORTRAIT && exist portrait monitor, use that instead of primary one
// @todo: WINDOW_TRAY

enum {
    WINDOW_NO_MOUSE = 0x01,

    WINDOW_MSAA2 = 0x02,
    WINDOW_MSAA4 = 0x04,
    WINDOW_MSAA8 = 0x08,

    WINDOW_SQUARE = 0x20,
    WINDOW_PORTRAIT = 0x40,
    WINDOW_LANDSCAPE = 0x80,
};

API void   window_create(float zoom, int flags);
API void   window_title(const char *title);
API void   window_icon(const char *file_icon);
API void   window_flush();
API int    window_swap();
API void*  window_handle();

API int    window_width();
API int    window_height();
API double window_aspect();
API double window_time();
API double window_delta();
API double window_fps();

API bool   window_hook(void (*func)(), void* userdata);
API void   window_unhook(void (*func)());

API void   window_focus(); // window attribute api using haz catz language for now
API int    window_has_focus();
API void   window_fullscreen(int enabled);
API int    window_has_fullscreen();
API void   window_cursor(int visible);
API int    window_has_cursor();
API void   window_pause();
API int    window_has_pause();
API void   window_visible(int visible);
API int    window_has_visible();
API void   window_videorec(const char* filename_mpg);
API int    window_has_videorec();

API void   window_screenshot(const char* filename_png);

#define window_title(...) window_title(stringf(__VA_ARGS__))
#line 0

#line 1 "fwk_ds.h"
// data structures and utils: array, set, map, hash, sort.
// - rlyeh, public domain

#ifndef DS_H
#define DS_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// less/sort

static inline int less_int(int a, int b) {
    return a - b;
}
static inline int less_u64(uint64_t a, uint64_t b) {
    return a > b ? +1 : -!!(a - b);
}
static inline int less_ptr(void *a, void *b) {
    return (uintptr_t)a > (uintptr_t)b ? +1 : -!!((uintptr_t)a - (uintptr_t)b);
}
static inline int less_str(char *a, char *b) {
    return strcmp((const char *)a, (const char *)b);
}

// -----------------------------------------------------------------------------
// un/hash

static inline uint32_t unhash_32(uint32_t x) {
    // Thomas Mueller at https://stackoverflow.com/questions/664014/ - says no collisions for 32bits!
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = (x >> 16) ^ x;
    return x;
}
static inline uint32_t hash_32(uint32_t x) {
    // Thomas Mueller at https://stackoverflow.com/questions/664014/ - says no collisions for 32bits!
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
static inline uint64_t hash_64(uint64_t x) {
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}
static inline uint64_t hash_flt(double x) {
    union { double d; uint64_t i; } c;
    return c.d = x, hash_64(c.i);
}
static inline uint64_t hash_str(const char* str) {
    uint64_t hash = 14695981039346656037ULL; // hash(0),mul(131) faster than fnv1a, a few more collisions though
    while( *str ) hash = ( (unsigned char)*str++ ^ hash ) * 0x100000001b3ULL;
    return hash;
}
static inline uint64_t hash_int(int key) {
    return hash_32((uint32_t)key);
}
static inline uint64_t hash_ptr(const void *ptr) {
    uint64_t key = (uint64_t)(uintptr_t)ptr;
    return hash_64(key >> 3); // >> 4? needed?
}

// -----------------------------------------------------------------------------
// vector based allocator (x1.75 enlarge factor)

static inline void* vrealloc( void* p, size_t sz ) {
    if( !sz ) {
        if( p ) {
            size_t *ret = (size_t*)p - 2;
            ret[0] = 0;
            ret[1] = 0;
            REALLOC( ret, 0 );
        }
        return 0;
    } else {
        size_t *ret = (size_t*)p - 2;
        if( !p ) {
            ret = (size_t*)REALLOC( 0, sizeof(size_t) * 2 + sz );
            ret[0] = sz;
            ret[1] = 0;
        } else {
            size_t osz = ret[0];
            size_t ocp = ret[1];
            if( sz <= (osz + ocp) ) {
                ret[0] = sz;
                ret[1] = ocp - (sz - osz);
            } else {
                ret = (size_t*)REALLOC( ret, sizeof(size_t) * 2 + sz * 1.75 );
                ret[0] = sz;
                ret[1] = (size_t)(sz * 1.75) - sz;
            }
        }
        return &ret[2];
    }
}
static inline size_t vlen( void* p ) {
    return p ? 0[ (size_t*)p - 2 ] : 0;
}

// -----------------------------------------------------------------------------
// arrays

#ifdef __cplusplus
#define array_cast(x) (decltype x)
#else
#define array_cast(x) (void *)
#endif

#define array(t) t*
#define array_init(t) ( (t) = 0 )
#define array_resize(t, n) ( array_c_ = array_count(t), array_realloc_((t),(n)), ((n)>array_c_? memset(array_c_+(t),0,((n)-array_c_)*sizeof(0[t])) : (void*)0), (t) )
#define array_push(t, ...) ( array_realloc_((t),array_count(t)+1), (t)[ array_count(t) - 1 ] = (__VA_ARGS__) )
#define array_pop(t) ( array_realloc_((t), array_count(t)-1) )
#define array_back(t) ( &(t)[ array_count(t)-1 ] ) // ( (t) ? &(t)[ array_count(t)-1 ] : NULL )
#define array_data(t) (t)
#define array_at(t,i) (t[i])
#define array_count(t) (int)( (t) ? array_vlen_(t) / sizeof(0[t]) : 0u )
#define array_bytes(t) (int)( (t) ? array_vlen_(t) : 0u )
#define array_sort(t, cmpfunc) qsort( t, array_count(t), sizeof(0[t]), cmpfunc )
#define array_empty(t) ( !array_count(t) )
static __thread unsigned array_c_;

#if 0 // original: no reserve support
#define array_reserve(t, n) ((void)0) // not implemented
#define array_clear(t) ( array_realloc_((t), 0), (t) = 0 )
#define array_vlen_(t)  ( vlen(t) - 0 )
#define array_realloc_(t,n)  ( (t) = array_cast(t) vrealloc((t), ((n)+0) * sizeof(0[t])) )
#define array_free(t) array_clear(t)
#else // new: with reserve support (buggy still?)
#define array_reserve(t, n) ( array_realloc_((t),(n)), array_clear(t) )
#define array_clear(t) ( array_realloc_((t),0) ) // -1
#define array_vlen_(t)  ( vlen(t) - sizeof(0[t]) ) // -1
#define array_realloc_(t,n)  ( (t) = array_cast(t) vrealloc((t), ((n)+1) * sizeof(0[t])) ) // +1
#define array_free(t) ( array_realloc_((t), -1), (t) = 0 ) // -1
#endif

#define array_reverse(t) \
    do if( array_count(t) ) { \
        for(int l = array_count(t), e = l-1, i = (array_push(t, 0[t]), 0); i <= e/2; ++i ) \
            { l[t] = i[t]; i[t] = (e-i)[t]; (e-i)[t] = l[t]; } \
        array_pop(t); \
    } while(0)

//#define array_foreach2(t,val_t,v) \
//    for( val_t *v = &0[t]; v < (&0[t] + array_count(t)); ++v )

//#define array_foreach(t, it) \
//    for( void *end__ = (it = &0[t]) + array_count(t); it != end__; ++it )

#define array_foreach(t,val_t,v) for each_array(t,val_t,v)
#define each_array(t,val_t,v) \
    ( val_t *it__ = &0[t]; it__ < (&0[t] + array_count(t)); ++it__ ) \
        for( val_t v = *it__, *on__ = &v; on__; on__ = 0 )

#define array_foreach_ptr(t,val_t,v) for each_array_ptr(t,val_t,v)
#define each_array_ptr(t,val_t,v) \
    ( val_t *it__ = &0[t]; it__ < (&0[t] + array_count(t)); ++it__ ) \
        for( val_t *v = it__; v; v = 0 )

#define array_search(t, key, cmpfn) /* requires sorted array beforehand */ \
    bsearch(&key, t, array_count(t), sizeof(t[0]), cmpfn )

#define array_insert(t, i, n) do { \
    int ac = array_count(t); \
    if( i >= ac ) { \
        array_push(t, n); \
    } else { \
        array_push(t, array_back(t)); \
        memmove( &(t)[(i)+1], &(t)[i], (ac - (i)) * sizeof(t[0]) ); \
        (t)[ i ] = (n); \
    } \
} while(0)

#define array_copy(t, src) do { /*todo: review old vrealloc call!*/ \
    array_free(t); \
    (t) = vrealloc( (t), array_count(src) * sizeof(0[t])); \
    memcpy( (t), src, array_count(src) * sizeof(0[t])); \
} while(0)

#define array_erase(t, i) do { \
    memcpy( &(t)[i], &(t)[array_count(t) - 1], sizeof(0[t])); \
    array_pop(t); \
} while(0)

#define array_unique(t, cmpfunc) do { /*todo: review old vrealloc call!*/ \
    int cnt = array_count(t), dupes = 0; \
    if( cnt > 1 ) { \
        const void *prev = &(t)[0]; \
        array_sort(t, cmpfunc); \
        for( int i = 1; i < cnt; ) { \
            if( cmpfunc(&t[i], prev) == 0 ) { \
                memmove( &t[i], &t[i+1], (cnt - 1 - i) * sizeof(t[0]) ) ; \
                --cnt; \
                ++dupes; \
            } else { \
                prev = &(t)[i]; \
                ++i; \
            } \
        } \
        if( dupes ) { \
            (t) = vrealloc((t), (array_count(t) - dupes) * sizeof(0[t])); \
        } \
    } \
} while(0)

// -----------------------------------------------------------------------------
// set<K>
// ideas from: https://en.wikipedia.org/wiki/Hash_table
// ideas from: https://probablydance.com/2017/02/26/i-wrote-the-fastest-hashtable/
// ideas from: http://www.idryman.org/blog/2017/05/03/writing-a-damn-fast-hash-table-with-tiny-memory-footprints/

// config
#ifndef SET_REALLOC
#define SET_REALLOC REALLOC
#endif
#ifndef SET_HASHSIZE
#define SET_HASHSIZE (4096 << 4)
#endif
#ifndef SET_DONT_ERASE
#define SET_DONT_ERASE 1
#endif

// public api
#define set(K) \
    struct { set base; struct { set_item p; K key; } tmp, *ptr; \
        int (*typed_cmp)(K, K); uint64_t (*typed_hash)(K); } *

#define set_init(m, cmpfn, hashfn) ( \
    (m) = set_cast(m) SET_REALLOC(0, sizeof(*m)), \
    set_init(&(m)->base), \
    (m)->base.cmp = (int(*)(void*,void*))( (m)->typed_cmp = cmpfn), \
    (m)->base.hash = (uint64_t(*)(void*))( (m)->typed_hash = hashfn ) \
    )

#define set_free(m) ( \
    set_clear(m), \
    set_free(&(m)->base), \
    (m) = set_cast(m) SET_REALLOC((m), 0), \
    (m) = 0 \
    )

#define set_insert(m, k) ( \
    (m)->ptr = set_cast((m)->ptr) SET_REALLOC(0, sizeof((m)->tmp)), \
    (m)->ptr->p.keyhash = (m)->typed_hash((m)->ptr->key = (k)), \
    set_insert(&(m)->base, &(m)->ptr->p, &(m)->ptr->key, (m)->ptr->p.keyhash, (m)->ptr), \
    &(m)->ptr->key \
    )

#define set_find(m, k) ( \
    (m)->ptr = &(m)->tmp, \
    (m)->ptr->p.keyhash = (m)->typed_hash((m)->ptr->key = (k)), \
    (m)->ptr = set_cast((m)->ptr) set_find(&(m)->base, &(m)->ptr->key, (m)->ptr->p.keyhash), \
    (m)->ptr ? &(m)->ptr->key : 0 \
    )

#define set_find_or_add(m, k) ( \
    (m)->tmp.key = (k), \
    (m)->tmpval = set_find((m), ((m)->tmp.key)), \
    (m)->tmpval = (m)->tmpval ? (m)->tmpval : set_insert((m), ((m)->tmp.key)) \
    )

#define set_erase(m, k) ( \
    (m)->ptr = &(m)->tmp, \
    (m)->ptr->p.keyhash = (m)->typed_hash((m)->ptr->key = (k)), \
    set_erase(&(m)->base, &(m)->ptr->key, (m)->ptr->p.keyhash) \
    )

#define set_foreach(m,key_t,k) for each_set(m,key_t,k)
#define each_set(m,key_t,k) \
    ( int i_ = (m)->set.count ? 0 : SET_HASHSIZE; i_ < SET_HASHSIZE; ++i_) \
        for( set_item *cur_ = (m)->base.array[i_], *on_ = cur_; cur_; on_ = cur_ = cur_->next ) \
            for( key_t k = *(key_t *)cur_->key; on_; on_ = 0 )

#define set_foreach_ptr(m,key_t,k) for each_set_ptr(m,key_t,k)
#define each_set_ptr(m,key_t,k) \
    ( int i_ = (m)->set.count ? 0 : SET_HASHSIZE; i_ < SET_HASHSIZE; ++i_) \
        for( set_item *cur_ = (m)->base.array[i_], *on_ = cur_; cur_; on_ = cur_ = cur_->next ) \
            for( key_t *k = (key_t *)cur_->key; on_; on_ = 0 )

#define set_clear(m) ( \
    set_clear(&(m)->base) \
    )

#define set_count(m)        set_count(&(m)->base)
#define set_gc(m)           set_gc(&(m)->base)

// private:

#ifdef __cplusplus
#define set_cast(t) (decltype(t))
#else
#define set_cast(t) (void *)
#endif

typedef struct set_item {
    struct set_item *next;

    uint64_t keyhash;
    void *key;
    void *super;
} set_item;

typedef struct set {
    array(set_item*) array;
    int (*cmp)(void *, void *);
    uint64_t (*hash)(void *);
    int count;
} set;

API void  (set_init)(set *m);
API void  (set_free)(set *m);

API void  (set_insert)(set *m, set_item *p, void *key, uint64_t keyhash, void *super);
API void  (set_erase)(set *m, void *key, uint64_t keyhash);
API void* (set_find)(const set *m, void *key, uint64_t keyhash);
API int   (set_count)(const set *m);
API void  (set_gc)(set *m); // only if using SET_DONT_ERASE

// -----------------------------------------------------------------------------
// map<K,V>
// ideas from: https://en.wikipedia.org/wiki/Hash_table
// ideas from: https://probablydance.com/2017/02/26/i-wrote-the-fastest-hashtable/
// ideas from: http://www.idryman.org/blog/2017/05/03/writing-a-damn-fast-hash-table-with-tiny-memory-footprints/

// config
#ifndef MAP_REALLOC
#define MAP_REALLOC REALLOC
#endif
#ifndef MAP_HASHSIZE
#define MAP_HASHSIZE (4096 << 4)
#endif
#ifndef MAP_DONT_ERASE
#define MAP_DONT_ERASE 1
#endif

// public api
#define map(K,V) \
    struct { map base; struct { pair p; K key; V val; } tmp, *ptr; V* tmpval; \
        int (*typed_cmp)(K, K); uint64_t (*typed_hash)(K); } *

#define map_init(m, cmpfn, hashfn) ( \
    (m) = map_cast(m) MAP_REALLOC(0, sizeof(*(m))), \
    map_init(&(m)->base), \
    (m)->base.cmp = (int(*)(void*,void*))( (m)->typed_cmp = map_cast((m)->typed_cmp) cmpfn), \
    (m)->base.hash = (uint64_t(*)(void*))( (m)->typed_hash = map_cast((m)->typed_hash) hashfn ) \
    )

#define map_free(m) ( \
    map_free(&(m)->base), \
    map_cast(m) MAP_REALLOC((m), sizeof(*(m))), (m) = 0 \
    )

#define map_insert(m, k, v) ( \
    (m)->ptr = map_cast((m)->ptr) MAP_REALLOC(0, sizeof((m)->tmp)), \
    (m)->ptr->val = (v), \
    (m)->ptr->p.keyhash = (m)->typed_hash((m)->ptr->key = (k)), \
    map_insert(&(m)->base, &(m)->ptr->p, &(m)->ptr->key, &(m)->ptr->val, (m)->ptr->p.keyhash, (m)->ptr), \
    &(m)->ptr->val \
    )

#define map_find(m, k) ( \
    (m)->ptr = &(m)->tmp, \
    (m)->ptr->p.keyhash = (m)->typed_hash((m)->ptr->key = (k)), \
    (m)->ptr = map_cast((m)->ptr) map_find(&(m)->base, &(m)->ptr->key, (m)->ptr->p.keyhash), \
    (m)->ptr ? &(m)->ptr->val : 0 \
    )

#define map_find_or_add(m, k, v) ( \
    (m)->tmp.key = (k), (m)->tmp.val = (v), \
    (m)->tmpval = map_find((m), ((m)->tmp.key)), \
    (m)->tmpval = (m)->tmpval ? (m)->tmpval : map_insert((m), ((m)->tmp.key), ((m)->tmp.val)) \
    )

#define map_erase(m, k) ( \
    (m)->ptr = &(m)->tmp, \
    (m)->ptr->p.keyhash = (m)->typed_hash((m)->ptr->key = (k)), \
    map_erase(&(m)->base, &(m)->ptr->key, (m)->ptr->p.keyhash) \
    )

#define map_foreach(m,key_t,k,val_t,v) for each_map(m,key_t,k,val_t,v)
#define each_map(m,key_t,k,val_t,v) \
    ( int i_ = (m)->base.count ? 0 : MAP_HASHSIZE; i_ < MAP_HASHSIZE; ++i_) \
        for( pair *cur_ = (m)->base.array[i_], *on_ = cur_; cur_; on_ = cur_ = cur_->next ) \
            for( key_t k = *(key_t *)cur_->key; on_; ) \
                for( val_t v = *(val_t *)cur_->value; on_; on_ = 0 )

#define map_foreach_ptr(m,key_t,k,val_t,v) for each_map_ptr(m,key_t,k,val_t,v)
#define each_map_ptr(m,key_t,k,val_t,v) \
    ( int i_ = (m)->base.count ? 0 : MAP_HASHSIZE; i_ < MAP_HASHSIZE; ++i_) \
        for( pair *cur_ = (m)->base.array[i_], *on_ = cur_; cur_; on_ = cur_ = cur_->next ) \
            for( key_t *k = (key_t *)cur_->key; on_; ) \
                for( val_t *v = (val_t *)cur_->value; on_; on_ = 0 )

#define map_clear(m) ( \
    map_clear(&(m)->base) \
    )

#define map_count(m)        map_count(&(m)->base)
#define map_gc(m)           map_gc(&(m)->base)

// private:

#ifdef __cplusplus
#define map_cast(t) (decltype(t))
#else
#define map_cast(t) (void *)
#endif

typedef struct pair {
    struct pair *next;

    uint64_t keyhash;
    void *key;
    void *value;
    void *super;
} pair;

typedef struct map {
    array(pair*) array;
    int (*cmp)(void *, void *);
    uint64_t (*hash)(void *);
    int count;
} map;

API void  (map_init)(map *m);
API void  (map_free)(map *m);

API void  (map_insert)(map *m, pair *p, void *key, void *value, uint64_t keyhash, void *super);
API void  (map_erase)(map *m, void *key, uint64_t keyhash);
API void* (map_find)(map *m, void *key, uint64_t keyhash);
API int   (map_count)(map *m);
API void  (map_gc)(map *m); // only if using MAP_DONT_ERASE

#endif // DS_H
#line 0
#line 1 "fwk_string.h"
// string framework
// - rlyeh, public domain

#ifndef STRING_H
#define STRING_H

// string: temporary api (stack)
API char*   stringf(const char *fmt, ...);
#define     stringf(...) (printf || printf(__VA_ARGS__), stringf(__VA_ARGS__))  // vs2015 check trick

#if 1
// string: allocated api (heap)
API char*   stringf_cat(char *x, const char *buf);
#define     stringf_cat(s,fmt,...)  stringf_cat((s), stringf(fmt, __VA_ARGS__)) // stringfcat ?
#define     stringf_del(s)         ((REALLOC((s), 0)), (s)=0) // stringfdel ?
#endif

#if defined _MSC_VER || (defined __TINYC__ && defined _WIN32)
#define strtok_r strtok_s
#endif

#define each_substring(str, delims, keyname) \
    ( int len_ = strlen(str) + 1; len_; len_ = 0 ) \
    for( char buf_[1024], *ptr_ = len_ < 1024 ? buf_ : REALLOC(0, len_), *lit_ = (char*)(str), *_bak = (snprintf(ptr_, len_, "%s", lit_), ptr_); _bak; _bak = 0, (ptr_ == buf_ ? 0 : REALLOC(ptr_, 0)) ) \
    for( char *next_token = 0, *keyname = strtok_r(_bak, delims, &next_token); keyname; keyname = strtok_r(NULL, delims, &next_token) )

// utils

API int          strmatch(const char *s, const char *wildcard);

API int          strcmp_qsort(const void *a, const void *b);
API int          strcmpi_qsort(const void *a, const void *b);

API bool         strbegi(const char *src, const char *sub);  // returns true if both strings match at beginning. case insensitive
API bool         strendi(const char *src, const char *sub);  // returns true if both strings match at end. case insensitive
API const char * strstri(const char *src, const char *sub);  // returns find first substring in string. case insensitive.
#define          strcmpi  ifdef(msc, _stricmp, strcasecmp)

API char *       strrepl(char **copy, const char *target, const char *replace); // replace any 'target' as 'repl' in 'copy'. returns 'copy'
API char *       strswap(char *copy, const char *target, const char *replace);  // replaced only if repl is shorter than target. no allocations.
API char *       strcut(char *copy, const char *target);                        // remove any 'target' in 'copy'. returns 'copy'

API char *       str16to8(const wchar_t *str); // convert from wchar16(win) to utf8/ascii

API const char * strlerp(unsigned numpairs, const char **pairs, const char *str); // using key-value pairs, null-terminated

#ifndef __APPLE__ // BSD provides these
API size_t       strlcat(char *dst, const char *src, size_t dstcap); // concat 2 strings safely. always NUL terminates. may truncate.
API size_t       strlcpy(char *dst, const char *src, size_t dstcap); // copy 2 strings safely. always NUL terminates. truncates if retval>=dstcap
#endif

API array(char*) strsplit(const char *string, const char *delimiters);
API char*        strjoin(array(char*) list, const char *separator);

#endif // STRING_H
#line 0
#line 1 "fwk_profile.h"
// -----------------------------------------------------------------------------
// profiler & stats (@fixme: threadsafe)

#if WITH_PROFILE
#   define profile_init() do { map_init(profiler, less_str, hash_str); } while(0)
#   define profile(...) for( \
        struct profile_t *found = map_find_or_add(profiler, #__VA_ARGS__ "@" FILELINE, (struct profile_t){NAN} ), *dummy = (\
        found->cost = -time_ms() * 1000, found); found->cost < 0; found->cost += time_ms() * 1000, found->avg = found->cost * 0.25 + found->avg * 0.75)
#   define profile_incstat(name, accum) do { if(profiler) { \
        struct profile_t *found = map_find(profiler, name); \
        if(!found) found = map_insert(profiler, name, (struct profile_t){0}); \
        found->stat += accum; \
        } } while(0)
#   define profile_render() if(profiler) do { \
        for(float _i = ui_begin("Profiler",0), _r; _i ; ui_end(), _i = 0) { \
            for each_map_ptr(profiler, const char *, key, struct profile_t, val ) \
                if( !isnan(val->stat) ) ui_slider2(stringf("Stat: %s", *key), (_r = val->stat, &_r), stringf("%.2f", val->stat)), val->stat = 0; \
            ui_separator(); \
            for each_map_ptr(profiler, const char *, key, struct profile_t, val ) \
                if( isnan(val->stat) ) ui_slider2(*key, (_r = val->avg/1000.0, &_r), stringf("%.2f ms", val->avg/1000.0)); \
        } } while(0)
struct profile_t { double stat; int32_t cost, avg; };
typedef map(char *, struct profile_t) profiler_t;
extern profiler_t profiler;
#else
#   define profile_init() do {} while(0)
#   define profile_incstat(name, accum) do {} while(0)
#   define profile(...) if(1) // for(int _p = 1; _p; _p = 0)
#   define profile_render()
#endif
#line 0

// -----------------------------------------------------------------------------
// if/n/def hell

#define ifdef(macro, yes, /*no*/...)   ifdef_##macro(yes, __VA_ARGS__)
#define ifndef(macro, yes, /*no*/...)  ifdef_##macro(__VA_ARGS__, yes)
#define is(macro)                      ifdef_##macro(1,0)
#define isnt(macro)                    ifdef_##macro(0,1)
#define ifdef_true(yes, /*no*/...)     yes
#define ifdef_false(yes, /*no*/...)    __VA_ARGS__

#ifdef _MSC_VER
#define ifdef_gcc                      ifdef_false
#define ifdef_mingw                    ifdef_false
#define ifdef_tcc                      ifdef_false
#define ifdef_msc                      ifdef_true
#elif defined __TINYC__
#define ifdef_gcc                      ifdef_false
#define ifdef_mingw                    ifdef_false
#define ifdef_tcc                      ifdef_true
#define ifdef_msc                      ifdef_false
#elif defined __MINGW64__ // __MINGW__ ?
#define ifdef_gcc                      ifdef_true
#define ifdef_mingw                    ifdef_true
#define ifdef_tcc                      ifdef_false
#define ifdef_msc                      ifdef_false
#else
#define ifdef_gcc                      ifdef_true
#define ifdef_mingw                    ifdef_false
#define ifdef_tcc                      ifdef_false
#define ifdef_msc                      ifdef_false
#endif

#ifdef __cplusplus
#define ifdef_cpp                      ifdef_true
#define ifdef_c                        ifdef_false
#else
#define ifdef_c                        ifdef_true
#define ifdef_cpp                      ifdef_false
#endif

#if defined _WIN32
#define ifdef_win32                    ifdef_true
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_false
#elif defined __linux__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_true
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_false
#elif defined __APPLE__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_true
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_false
#elif defined __EMSCRIPTEN__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_true
#else // __FreeBSD__ || @todo: __ANDROID_API__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_true
#define ifdef_ems                      ifdef_false
#endif

#if 0
#  if defined __ANDROID_API__
#define _AND
#elif defined __APPLE__
#define _OSX
#elif defined __FreeBSD__
#define _BSD
#elif defined _WIN32
#define _WIN
#else
#define _LIN
#endif

#  if defined _MSC_VER
#define _MSC
#elif defined __clang__
#define _CLA
#elif defined __TINYC__
#define _TCC
#else
#define _GCC
#endif
#endif

// -----------------------------------------------------------------------------
// new C keywords
// @todo: autorun (needed?)

#define countof(x)       (sizeof (x) / sizeof 0[x])

#define macro(name)      concat(name, __LINE__)
#define concat(a,b)      conc4t(a,b)
#define conc4t(a,b)      a##b

#define benchmark        for(double macro(t) = -time_ss(); macro(t) < 0; printf("%.2fs\n", macro(t)+=time_ss()))
#define do_once          static int macro(once) = 0; for(;!macro(once);macro(once)=1)
#define defer(begin,end) for(int macro(i) = ((begin), 0); !macro(i); macro(i) = ((end), 1))
#define scope(end)       defer((void)0, end)

//-----------------------------------------------------------------------------
// new C macros

#define ASSERT(expr, ...)   do { int fool_msvc[] = {0,}; if(!(expr)) { fool_msvc[0]++; breakpoint(stringf("!Expression failed: " #expr " " FILELINE "\n" __VA_ARGS__)); } } while(0)
#define PRINTF(...)         PRINTF(stringf(__VA_ARGS__), 1[#__VA_ARGS__] == '!' ? callstack(+48) : "", __FILE__, __LINE__, __FUNCTION__)

#define FILELINE            __FILE__ ":" STRINGIZE(__LINE__)
#define STRINGIZE(x)        STRINGIZ3(x)
#define STRINGIZ3(x)        #x

#define EXPAND(name, ...)          EXPAND_QUOTE(EXPAND_JOIN(name, EXPAND_COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define EXPAND_QUOTE(x, y)         x y
#define EXPAND_JOIN(name, count)   EXPAND_J0IN(name, count)
#define EXPAND_J0IN(name, count)   EXPAND_J01N(name, count)
#define EXPAND_J01N(name, count)   name##count
#define EXPAND_COUNT_ARGS(...)     EXPAND_ARGS((__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define EXPAND_ARGS(args)          EXPAND_RETURN_COUNT args
#define EXPAND_RETURN_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, count, ...) count

// ----

#endif // FWK_H

#line 1 "3rd_glad.h"
/**
 * Loader generated by glad 2.0.0-beta on Sat Mar 28 10:21:14 2020
 *
 * Generator: C/C++
 * Specification: gl
 * Extensions: 183
 *
 * APIs:
 *  - gl:compatibility=3.3
 *
 * Options:
 *  - MX_GLOBAL = False
 *  - ON_DEMAND = False
 *  - LOADER = False
 *  - ALIAS = False
 *  - HEADER_ONLY = True
 *  - DEBUG = False
 *  - MX = False
 *  - MERGE = True
 *
 * Commandline:
 *    --api='gl:compatibility=3.3' --extensions='GL_ARB_ES2_compatibility,GL_ARB_ES3_1_compatibility,GL_ARB_ES3_2_compatibility,GL_ARB_ES3_compatibility,GL_ARB_arrays_of_arrays,GL_ARB_base_instance,GL_ARB_bindless_texture,GL_ARB_blend_func_extended,GL_ARB_buffer_storage,GL_ARB_cl_event,GL_ARB_clear_buffer_object,GL_ARB_clear_texture,GL_ARB_clip_control,GL_ARB_color_buffer_float,GL_ARB_compatibility,GL_ARB_compressed_texture_pixel_storage,GL_ARB_compute_shader,GL_ARB_compute_variable_group_size,GL_ARB_conditional_render_inverted,GL_ARB_conservative_depth,GL_ARB_copy_buffer,GL_ARB_copy_image,GL_ARB_cull_distance,GL_ARB_debug_output,GL_ARB_depth_buffer_float,GL_ARB_depth_clamp,GL_ARB_depth_texture,GL_ARB_derivative_control,GL_ARB_direct_state_access,GL_ARB_draw_buffers,GL_ARB_draw_buffers_blend,GL_ARB_draw_elements_base_vertex,GL_ARB_draw_indirect,GL_ARB_draw_instanced,GL_ARB_enhanced_layouts,GL_ARB_explicit_attrib_location,GL_ARB_explicit_uniform_location,GL_ARB_fragment_coord_conventions,GL_ARB_fragment_layer_viewport,GL_ARB_fragment_program,GL_ARB_fragment_program_shadow,GL_ARB_fragment_shader,GL_ARB_fragment_shader_interlock,GL_ARB_framebuffer_no_attachments,GL_ARB_framebuffer_object,GL_ARB_framebuffer_sRGB,GL_ARB_geometry_shader4,GL_ARB_get_program_binary,GL_ARB_get_texture_sub_image,GL_ARB_gl_spirv,GL_ARB_gpu_shader5,GL_ARB_gpu_shader_fp64,GL_ARB_gpu_shader_int64,GL_ARB_half_float_pixel,GL_ARB_half_float_vertex,GL_ARB_imaging,GL_ARB_indirect_parameters,GL_ARB_instanced_arrays,GL_ARB_internalformat_query,GL_ARB_internalformat_query2,GL_ARB_invalidate_subdata,GL_ARB_map_buffer_alignment,GL_ARB_map_buffer_range,GL_ARB_matrix_palette,GL_ARB_multi_bind,GL_ARB_multi_draw_indirect,GL_ARB_multisample,GL_ARB_multitexture,GL_ARB_occlusion_query,GL_ARB_occlusion_query2,GL_ARB_parallel_shader_compile,GL_ARB_pipeline_statistics_query,GL_ARB_pixel_buffer_object,GL_ARB_point_parameters,GL_ARB_point_sprite,GL_ARB_polygon_offset_clamp,GL_ARB_post_depth_coverage,GL_ARB_program_interface_query,GL_ARB_provoking_vertex,GL_ARB_query_buffer_object,GL_ARB_robust_buffer_access_behavior,GL_ARB_robustness,GL_ARB_robustness_isolation,GL_ARB_sample_locations,GL_ARB_sample_shading,GL_ARB_sampler_objects,GL_ARB_seamless_cube_map,GL_ARB_seamless_cubemap_per_texture,GL_ARB_separate_shader_objects,GL_ARB_shader_atomic_counter_ops,GL_ARB_shader_atomic_counters,GL_ARB_shader_ballot,GL_ARB_shader_bit_encoding,GL_ARB_shader_clock,GL_ARB_shader_draw_parameters,GL_ARB_shader_group_vote,GL_ARB_shader_image_load_store,GL_ARB_shader_image_size,GL_ARB_shader_objects,GL_ARB_shader_precision,GL_ARB_shader_stencil_export,GL_ARB_shader_storage_buffer_object,GL_ARB_shader_subroutine,GL_ARB_shader_texture_image_samples,GL_ARB_shader_texture_lod,GL_ARB_shader_viewport_layer_array,GL_ARB_shading_language_100,GL_ARB_shading_language_420pack,GL_ARB_shading_language_include,GL_ARB_shading_language_packing,GL_ARB_shadow,GL_ARB_shadow_ambient,GL_ARB_sparse_buffer,GL_ARB_sparse_texture,GL_ARB_sparse_texture2,GL_ARB_sparse_texture_clamp,GL_ARB_spirv_extensions,GL_ARB_stencil_texturing,GL_ARB_sync,GL_ARB_tessellation_shader,GL_ARB_texture_barrier,GL_ARB_texture_border_clamp,GL_ARB_texture_buffer_object,GL_ARB_texture_buffer_object_rgb32,GL_ARB_texture_buffer_range,GL_ARB_texture_compression,GL_ARB_texture_compression_bptc,GL_ARB_texture_compression_rgtc,GL_ARB_texture_cube_map,GL_ARB_texture_cube_map_array,GL_ARB_texture_env_add,GL_ARB_texture_env_combine,GL_ARB_texture_env_crossbar,GL_ARB_texture_env_dot3,GL_ARB_texture_filter_anisotropic,GL_ARB_texture_filter_minmax,GL_ARB_texture_float,GL_ARB_texture_gather,GL_ARB_texture_mirror_clamp_to_edge,GL_ARB_texture_mirrored_repeat,GL_ARB_texture_multisample,GL_ARB_texture_non_power_of_two,GL_ARB_texture_query_levels,GL_ARB_texture_query_lod,GL_ARB_texture_rectangle,GL_ARB_texture_rg,GL_ARB_texture_rgb10_a2ui,GL_ARB_texture_stencil8,GL_ARB_texture_storage,GL_ARB_texture_storage_multisample,GL_ARB_texture_swizzle,GL_ARB_texture_view,GL_ARB_timer_query,GL_ARB_transform_feedback2,GL_ARB_transform_feedback3,GL_ARB_transform_feedback_instanced,GL_ARB_transform_feedback_overflow_query,GL_ARB_transpose_matrix,GL_ARB_uniform_buffer_object,GL_ARB_vertex_array_bgra,GL_ARB_vertex_array_object,GL_ARB_vertex_attrib_64bit,GL_ARB_vertex_attrib_binding,GL_ARB_vertex_blend,GL_ARB_vertex_buffer_object,GL_ARB_vertex_program,GL_ARB_vertex_shader,GL_ARB_vertex_type_10f_11f_11f_rev,GL_ARB_vertex_type_2_10_10_10_rev,GL_ARB_viewport_array,GL_ARB_window_pos,GL_KHR_blend_equation_advanced,GL_KHR_blend_equation_advanced_coherent,GL_KHR_context_flush_control,GL_KHR_debug,GL_KHR_no_error,GL_KHR_parallel_shader_compile,GL_KHR_robust_buffer_access_behavior,GL_KHR_robustness,GL_KHR_shader_subgroup,GL_KHR_texture_compression_astc_hdr,GL_KHR_texture_compression_astc_ldr,GL_KHR_texture_compression_astc_sliced_3d' c --header-only
 *
 * Online:
 *    http://glad.sh/#api=gl%3Acompatibility%3D3.3&generator=c&options=HEADER_ONLY
 *
 */

#ifndef GLAD_GL_H_
#define GLAD_GL_H_

#ifdef __gl_h_
    #error OpenGL header already included (API: gl), remove previous include!
#endif
#define __gl_h_ 1


#define GLAD_GL
#define GLAD_OPTION_GL_HEADER_ONLY

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GLAD_PLATFORM_H_
#define GLAD_PLATFORM_H_

#ifndef GLAD_PLATFORM_WIN32
  #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
    #define GLAD_PLATFORM_WIN32 1
  #else
    #define GLAD_PLATFORM_WIN32 0
  #endif
#endif

#ifndef GLAD_PLATFORM_APPLE
  #ifdef __APPLE__
    #define GLAD_PLATFORM_APPLE 1
  #else
    #define GLAD_PLATFORM_APPLE 0
  #endif
#endif

#ifndef GLAD_PLATFORM_EMSCRIPTEN
  #ifdef __EMSCRIPTEN__
    #define GLAD_PLATFORM_EMSCRIPTEN 1
  #else
    #define GLAD_PLATFORM_EMSCRIPTEN 0
  #endif
#endif

#ifndef GLAD_PLATFORM_UWP
  #if defined(_MSC_VER) && !defined(GLAD_INTERNAL_HAVE_WINAPIFAMILY)
    #ifdef __has_include
      #if __has_include(<winapifamily.h>)
        #define GLAD_INTERNAL_HAVE_WINAPIFAMILY 1
      #endif
    #elif _MSC_VER >= 1700 && !_USING_V110_SDK71_
      #define GLAD_INTERNAL_HAVE_WINAPIFAMILY 1
    #endif
  #endif

  #ifdef GLAD_INTERNAL_HAVE_WINAPIFAMILY
    #include <winapifamily.h>
    #if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
      #define GLAD_PLATFORM_UWP 1
    #endif
  #endif

  #ifndef GLAD_PLATFORM_UWP
    #define GLAD_PLATFORM_UWP 0
  #endif
#endif

#ifdef __GNUC__
  #define GLAD_GNUC_EXTENSION __extension__
#else
  #define GLAD_GNUC_EXTENSION
#endif

#ifndef GLAD_API_CALL
  #if defined(GLAD_API_CALL_EXPORT)
    #if GLAD_PLATFORM_WIN32 || defined(__CYGWIN__)
      #if defined(GLAD_API_CALL_EXPORT_BUILD)
        #if defined(__GNUC__)
          #define GLAD_API_CALL __attribute__ ((dllexport)) extern
        #else
          #define GLAD_API_CALL __declspec(dllexport) extern
        #endif
      #else
        #if defined(__GNUC__)
          #define GLAD_API_CALL __attribute__ ((dllimport)) extern
        #else
          #define GLAD_API_CALL __declspec(dllimport) extern
        #endif
      #endif
    #elif defined(__GNUC__) && defined(GLAD_API_CALL_EXPORT_BUILD)
      #define GLAD_API_CALL __attribute__ ((visibility ("default"))) extern
    #else
      #define GLAD_API_CALL extern
    #endif
  #else
    #define GLAD_API_CALL extern
  #endif
#endif

#ifdef APIENTRY
  #define GLAD_API_PTR APIENTRY
#elif GLAD_PLATFORM_WIN32
  #define GLAD_API_PTR __stdcall
#else
  #define GLAD_API_PTR
#endif

#ifndef GLAPI
#define GLAPI GLAD_API_CALL
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY GLAD_API_PTR
#endif

#define GLAD_MAKE_VERSION(major, minor) (major * 10000 + minor)
#define GLAD_VERSION_MAJOR(version) (version / 10000)
#define GLAD_VERSION_MINOR(version) (version % 10000)

#define GLAD_GENERATOR_VERSION "2.0.0-beta"

typedef void (*GLADapiproc)(void);

typedef GLADapiproc (*GLADloadfunc)(const char *name);
typedef GLADapiproc (*GLADuserptrloadfunc)(void *userptr, const char *name);

typedef void (*GLADprecallback)(const char *name, GLADapiproc apiproc, int len_args, ...);
typedef void (*GLADpostcallback)(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...);

#endif /* GLAD_PLATFORM_H_ */

#define GL_2D 0x0600
#define GL_2_BYTES 0x1407
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_3_BYTES 0x1408
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_4_BYTES 0x1409
#define GL_ACCUM 0x0100
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_ACTIVE_RESOURCES 0x92F5
#define GL_ACTIVE_SUBROUTINES 0x8DE5
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH 0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORMS 0x8DE6
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_ACTIVE_TEXTURE_ARB 0x84E0
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_ACTIVE_VARIABLES 0x9305
#define GL_ACTIVE_VERTEX_UNITS_ARB 0x86A5
#define GL_ADD 0x0104
#define GL_ADD_SIGNED 0x8574
#define GL_ADD_SIGNED_ARB 0x8574
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALL_ATTRIB_BITS 0xFFFFFFFF
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF
#define GL_ALL_SHADER_BITS 0xFFFFFFFF
#define GL_ALPHA 0x1906
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_ALPHA16F_ARB 0x881C
#define GL_ALPHA32F_ARB 0x8816
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_ALPHA_BITS 0x0D55
#define GL_ALPHA_INTEGER 0x8D97
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_ALREADY_SIGNALED 0x911A
#define GL_ALWAYS 0x0207
#define GL_AMBIENT 0x1200
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_AND 0x1501
#define GL_AND_INVERTED 0x1504
#define GL_AND_REVERSE 0x1502
#define GL_ANY_SAMPLES_PASSED 0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_ARRAY_BUFFER 0x8892
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#define GL_ARRAY_SIZE 0x92FB
#define GL_ARRAY_STRIDE 0x92FE
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x00001000
#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_INDEX 0x9301
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_AUTO_GENERATE_MIPMAP 0x8295
#define GL_AUTO_NORMAL 0x0D80
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C
#define GL_AUX_BUFFERS 0x0C00
#define GL_BACK 0x0405
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_BGRA_INTEGER 0x8D9B
#define GL_BGR_INTEGER 0x8D9A
#define GL_BITMAP 0x1A00
#define GL_BITMAP_TOKEN 0x0704
#define GL_BLEND 0x0BE2
#define GL_BLEND_ADVANCED_COHERENT_KHR 0x9285
#define GL_BLEND_COLOR 0x8005
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_EQUATION 0x8009
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLOCK_INDEX 0x92FD
#define GL_BLUE 0x1905
#define GL_BLUE_BIAS 0x0D1B
#define GL_BLUE_BITS 0x0D54
#define GL_BLUE_INTEGER 0x8D96
#define GL_BLUE_SCALE 0x0D1A
#define GL_BOOL 0x8B56
#define GL_BOOL_ARB 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC2_ARB 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC3_ARB 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_BOOL_VEC4_ARB 0x8B59
#define GL_BUFFER 0x82E0
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_ACCESS_ARB 0x88BB
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_BINDING 0x9302
#define GL_BUFFER_DATA_SIZE 0x9303
#define GL_BUFFER_IMMUTABLE_STORAGE 0x821F
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAPPED_ARB 0x88BC
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_BUFFER_MAP_POINTER_ARB 0x88BD
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_SIZE_ARB 0x8764
#define GL_BUFFER_STORAGE_FLAGS 0x8220
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#define GL_BUFFER_USAGE 0x8765
#define GL_BUFFER_USAGE_ARB 0x8765
#define GL_BUFFER_VARIABLE 0x92E5
#define GL_BYTE 0x1400
#define GL_C3F_V3F 0x2A24
#define GL_C4F_N3F_V3F 0x2A26
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_CAVEAT_SUPPORT 0x82B8
#define GL_CCW 0x0901
#define GL_CLAMP 0x2900
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_FRAGMENT_COLOR_ARB 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_CLAMP_READ_COLOR_ARB 0x891C
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_CLAMP_TO_BORDER_ARB 0x812D
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_VERTEX_COLOR_ARB 0x891A
#define GL_CLEAR 0x1500
#define GL_CLEAR_BUFFER 0x82B4
#define GL_CLEAR_TEXTURE 0x9365
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#define GL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_CLIENT_STORAGE_BIT 0x0200
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_CLIPPING_INPUT_PRIMITIVES 0x82F6
#define GL_CLIPPING_INPUT_PRIMITIVES_ARB 0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES 0x82F7
#define GL_CLIPPING_OUTPUT_PRIMITIVES_ARB 0x82F7
#define GL_CLIP_DEPTH_MODE 0x935D
#define GL_CLIP_DISTANCE0 0x3000
#define GL_CLIP_DISTANCE1 0x3001
#define GL_CLIP_DISTANCE2 0x3002
#define GL_CLIP_DISTANCE3 0x3003
#define GL_CLIP_DISTANCE4 0x3004
#define GL_CLIP_DISTANCE5 0x3005
#define GL_CLIP_DISTANCE6 0x3006
#define GL_CLIP_DISTANCE7 0x3007
#define GL_CLIP_ORIGIN 0x935C
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_COEFF 0x0A00
#define GL_COLOR 0x1800
#define GL_COLORBURN_KHR 0x929A
#define GL_COLORDODGE_KHR 0x9299
#define GL_COLOR_ARRAY 0x8076
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_COLOR_ATTACHMENT16 0x8CF0
#define GL_COLOR_ATTACHMENT17 0x8CF1
#define GL_COLOR_ATTACHMENT18 0x8CF2
#define GL_COLOR_ATTACHMENT19 0x8CF3
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT20 0x8CF4
#define GL_COLOR_ATTACHMENT21 0x8CF5
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_COLOR_ATTACHMENT23 0x8CF7
#define GL_COLOR_ATTACHMENT24 0x8CF8
#define GL_COLOR_ATTACHMENT25 0x8CF9
#define GL_COLOR_ATTACHMENT26 0x8CFA
#define GL_COLOR_ATTACHMENT27 0x8CFB
#define GL_COLOR_ATTACHMENT28 0x8CFC
#define GL_COLOR_ATTACHMENT29 0x8CFD
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT30 0x8CFE
#define GL_COLOR_ATTACHMENT31 0x8CFF
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_COMPONENTS 0x8283
#define GL_COLOR_ENCODING 0x8296
#define GL_COLOR_INDEX 0x1900
#define GL_COLOR_INDEXES 0x1603
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_COLOR_MATERIAL 0x0B57
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_MATRIX 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#define GL_COLOR_RENDERABLE 0x8286
#define GL_COLOR_SUM 0x8458
#define GL_COLOR_SUM_ARB 0x8458
#define GL_COLOR_TABLE 0x80D0
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#define GL_COLOR_TABLE_BIAS 0x80D7
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#define GL_COLOR_TABLE_FORMAT 0x80D8
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#define GL_COLOR_TABLE_SCALE 0x80D6
#define GL_COLOR_TABLE_WIDTH 0x80D9
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_COMBINE 0x8570
#define GL_COMBINE_ALPHA 0x8572
#define GL_COMBINE_ALPHA_ARB 0x8572
#define GL_COMBINE_ARB 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_RGB_ARB 0x8571
#define GL_COMMAND_BARRIER_BIT 0x00000040
#define GL_COMPARE_REF_TO_TEXTURE 0x884E
#define GL_COMPARE_R_TO_TEXTURE 0x884E
#define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E
#define GL_COMPATIBLE_SUBROUTINES 0x8E4B
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_COMPILE_STATUS 0x8B81
#define GL_COMPLETION_STATUS_ARB 0x91B1
#define GL_COMPLETION_STATUS_KHR 0x91B1
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_ALPHA_ARB 0x84E9
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_INTENSITY_ARB 0x84EC
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB 0x84EB
#define GL_COMPRESSED_LUMINANCE_ARB 0x84EA
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#define GL_COMPRESSED_RG 0x8226
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_RGBA_ARB 0x84EE
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#define GL_COMPRESSED_RGB_ARB 0x84ED
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB 0x8E8F
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB 0x8E8D
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A3
#define GL_COMPUTE_SHADER 0x91B9
#define GL_COMPUTE_SHADER_BIT 0x00000020
#define GL_COMPUTE_SHADER_INVOCATIONS 0x82F5
#define GL_COMPUTE_SHADER_INVOCATIONS_ARB 0x82F5
#define GL_COMPUTE_SUBROUTINE 0x92ED
#define GL_COMPUTE_SUBROUTINE_UNIFORM 0x92F3
#define GL_COMPUTE_TEXTURE 0x82A0
#define GL_COMPUTE_WORK_GROUP_SIZE 0x8267
#define GL_CONDITION_SATISFIED 0x911C
#define GL_CONSTANT 0x8576
#define GL_CONSTANT_ALPHA 0x8003
#define GL_CONSTANT_ARB 0x8576
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_CONSTANT_BORDER 0x8151
#define GL_CONSTANT_COLOR 0x8001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_FLAGS 0x821E
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x00000001
#define GL_CONTEXT_FLAG_NO_ERROR_BIT 0x00000008
#define GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR 0x00000008
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB 0x00000004
#define GL_CONTEXT_LOST 0x0507
#define GL_CONTEXT_PROFILE_MASK 0x9126
#define GL_CONTEXT_RELEASE_BEHAVIOR 0x82FB
#define GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH 0x82FC
#define GL_CONTEXT_ROBUST_ACCESS 0x90F3
#define GL_CONVOLUTION_1D 0x8010
#define GL_CONVOLUTION_2D 0x8011
#define GL_CONVOLUTION_BORDER_COLOR 0x8154
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#define GL_CONVOLUTION_FORMAT 0x8017
#define GL_CONVOLUTION_HEIGHT 0x8019
#define GL_CONVOLUTION_WIDTH 0x8018
#define GL_COORD_REPLACE 0x8862
#define GL_COORD_REPLACE_ARB 0x8862
#define GL_COPY 0x1503
#define GL_COPY_INVERTED 0x150C
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_CURRENT_BIT 0x00000001
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_FOG_COORD 0x8453
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_MATRIX_ARB 0x8641
#define GL_CURRENT_MATRIX_INDEX_ARB 0x8845
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB 0x8640
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_PALETTE_MATRIX_ARB 0x8843
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_CURRENT_QUERY 0x8865
#define GL_CURRENT_QUERY_ARB 0x8865
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_CURRENT_VERTEX_ATTRIB_ARB 0x8626
#define GL_CURRENT_WEIGHT_ARB 0x86A8
#define GL_CW 0x0900
#define GL_DARKEN_KHR 0x9297
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_FUNCTION_ARB 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB 0x8245
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_LOGGED_MESSAGES_ARB 0x9145
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB 0x8243
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB 0x8242
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_HIGH_ARB 0x9146
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_SEVERITY_LOW_ARB 0x9148
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_MEDIUM_ARB 0x9147
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_API_ARB 0x8246
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_APPLICATION_ARB 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_SOURCE_OTHER_ARB 0x824B
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB 0x8249
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB 0x8247
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB 0x824D
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_ERROR_ARB 0x824C
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_OTHER_ARB 0x8251
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_PERFORMANCE_ARB 0x8250
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PORTABILITY_ARB 0x824F
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB 0x824E
#define GL_DECAL 0x2101
#define GL_DECR 0x1E03
#define GL_DECR_WRAP 0x8508
#define GL_DELETE_STATUS 0x8B80
#define GL_DEPTH 0x1801
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_DEPTH_BIAS 0x0D1F
#define GL_DEPTH_BITS 0x0D56
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_CLAMP 0x864F
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#define GL_DEPTH_COMPONENTS 0x8284
#define GL_DEPTH_FUNC 0x0B74
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_RENDERABLE 0x8287
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_STENCIL 0x84F9
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DIFFERENCE_KHR 0x929E
#define GL_DIFFUSE 0x1201
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_DISPLAY_LIST 0x82E7
#define GL_DITHER 0x0BD0
#define GL_DOMAIN 0x0A02
#define GL_DONT_CARE 0x1100
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_DOT3_RGBA_ARB 0x86AF
#define GL_DOT3_RGB_ARB 0x86AE
#define GL_DOUBLE 0x140A
#define GL_DOUBLEBUFFER 0x0C32
#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE
#define GL_DRAW_BUFFER 0x0C01
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER0_ARB 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER10_ARB 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER11_ARB 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER12_ARB 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER13_ARB 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER14_ARB 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_DRAW_BUFFER15_ARB 0x8834
#define GL_DRAW_BUFFER1_ARB 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER2_ARB 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER3_ARB 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER4_ARB 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER5_ARB 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER6_ARB 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER7_ARB 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER8_ARB 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER9_ARB 0x882E
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING 0x8F43
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_DST_ALPHA 0x0304
#define GL_DST_COLOR 0x0306
#define GL_DYNAMIC_COPY 0x88EA
#define GL_DYNAMIC_COPY_ARB 0x88EA
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_DRAW_ARB 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_READ_ARB 0x88E9
#define GL_DYNAMIC_STORAGE_BIT 0x0100
#define GL_EDGE_FLAG 0x0B43
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x00000002
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_EMISSION 0x1600
#define GL_ENABLE_BIT 0x00002000
#define GL_EQUAL 0x0202
#define GL_EQUIV 0x1509
#define GL_EVAL_BIT 0x00010000
#define GL_EXCLUSION_KHR 0x92A0
#define GL_EXP 0x0800
#define GL_EXP2 0x0801
#define GL_EXTENSIONS 0x1F03
#define GL_EYE_LINEAR 0x2400
#define GL_EYE_PLANE 0x2502
#define GL_FALSE 0
#define GL_FASTEST 0x1101
#define GL_FEEDBACK 0x1C01
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_FILL 0x1B02
#define GL_FILTER 0x829A
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_FIXED 0x140C
#define GL_FIXED_ONLY 0x891D
#define GL_FIXED_ONLY_ARB 0x891D
#define GL_FLAT 0x1D00
#define GL_FLOAT 0x1406
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT2_ARB 0x8B5A
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT3_ARB 0x8B5B
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4 0x8B5C
#define GL_FLOAT_MAT4_ARB 0x8B5C
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC2_ARB 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC3_ARB 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_FLOAT_VEC4_ARB 0x8B52
#define GL_FOG 0x0B60
#define GL_FOG_BIT 0x00000080
#define GL_FOG_COLOR 0x0B66
#define GL_FOG_COORD 0x8451
#define GL_FOG_COORDINATE 0x8451
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORD_ARRAY 0x8457
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING 0x889D
#define GL_FOG_COORD_ARRAY_POINTER 0x8456
#define GL_FOG_COORD_ARRAY_STRIDE 0x8455
#define GL_FOG_COORD_ARRAY_TYPE 0x8454
#define GL_FOG_COORD_SRC 0x8450
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_END 0x0B64
#define GL_FOG_HINT 0x0C54
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_MODE 0x0B65
#define GL_FOG_START 0x0B63
#define GL_FRACTIONAL_EVEN 0x8E7C
#define GL_FRACTIONAL_ODD 0x8E7B
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FRAGMENT_SHADER_ARB 0x8B30
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B
#define GL_FRAGMENT_SHADER_INVOCATIONS 0x82F4
#define GL_FRAGMENT_SHADER_INVOCATIONS_ARB 0x82F4
#define GL_FRAGMENT_SUBROUTINE 0x92EC
#define GL_FRAGMENT_SUBROUTINE_UNIFORM 0x92F2
#define GL_FRAGMENT_TEXTURE 0x829F
#define GL_FRAMEBUFFER 0x8D40
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_ARB 0x8DA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_BLEND 0x828B
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB 0x8DA9
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_ARB 0x9342
#define GL_FRAMEBUFFER_RENDERABLE 0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_ARB 0x9343
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_FRONT 0x0404
#define GL_FRONT_AND_BACK 0x0408
#define GL_FRONT_FACE 0x0B46
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_FULL_SUPPORT 0x82B7
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_FUNC_SUBTRACT 0x800A
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_INPUT_TYPE_ARB 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_GEOMETRY_OUTPUT_TYPE_ARB 0x8DDC
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_GEOMETRY_SHADER_ARB 0x8DD9
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED 0x82F3
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB 0x82F3
#define GL_GEOMETRY_SUBROUTINE 0x92EB
#define GL_GEOMETRY_SUBROUTINE_UNIFORM 0x92F1
#define GL_GEOMETRY_TEXTURE 0x829E
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_VERTICES_OUT_ARB 0x8DDA
#define GL_GEQUAL 0x0206
#define GL_GET_TEXTURE_IMAGE_FORMAT 0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE 0x8292
#define GL_GREATER 0x0204
#define GL_GREEN 0x1904
#define GL_GREEN_BIAS 0x0D19
#define GL_GREEN_BITS 0x0D53
#define GL_GREEN_INTEGER 0x8D95
#define GL_GREEN_SCALE 0x0D18
#define GL_GUILTY_CONTEXT_RESET 0x8253
#define GL_GUILTY_CONTEXT_RESET_ARB 0x8253
#define GL_HALF_FLOAT 0x140B
#define GL_HALF_FLOAT_ARB 0x140B
#define GL_HARDLIGHT_KHR 0x929B
#define GL_HIGH_FLOAT 0x8DF2
#define GL_HIGH_INT 0x8DF5
#define GL_HINT_BIT 0x00008000
#define GL_HISTOGRAM 0x8024
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#define GL_HISTOGRAM_FORMAT 0x8027
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#define GL_HISTOGRAM_RED_SIZE 0x8028
#define GL_HISTOGRAM_SINK 0x802D
#define GL_HISTOGRAM_WIDTH 0x8026
#define GL_HSL_COLOR_KHR 0x92AF
#define GL_HSL_HUE_KHR 0x92AD
#define GL_HSL_LUMINOSITY_KHR 0x92B0
#define GL_HSL_SATURATION_KHR 0x92AE
#define GL_IMAGE_1D 0x904C
#define GL_IMAGE_1D_ARRAY 0x9052
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_IMAGE_2D_MULTISAMPLE 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY 0x9056
#define GL_IMAGE_2D_RECT 0x904F
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_BINDING_ACCESS 0x8F3E
#define GL_IMAGE_BINDING_FORMAT 0x906E
#define GL_IMAGE_BINDING_LAYER 0x8F3D
#define GL_IMAGE_BINDING_LAYERED 0x8F3C
#define GL_IMAGE_BINDING_LEVEL 0x8F3B
#define GL_IMAGE_BINDING_NAME 0x8F3A
#define GL_IMAGE_BUFFER 0x9051
#define GL_IMAGE_CLASS_10_10_10_2 0x82C3
#define GL_IMAGE_CLASS_11_11_10 0x82C2
#define GL_IMAGE_CLASS_1_X_16 0x82BE
#define GL_IMAGE_CLASS_1_X_32 0x82BB
#define GL_IMAGE_CLASS_1_X_8 0x82C1
#define GL_IMAGE_CLASS_2_X_16 0x82BD
#define GL_IMAGE_CLASS_2_X_32 0x82BA
#define GL_IMAGE_CLASS_2_X_8 0x82C0
#define GL_IMAGE_CLASS_4_X_16 0x82BC
#define GL_IMAGE_CLASS_4_X_32 0x82B9
#define GL_IMAGE_CLASS_4_X_8 0x82BF
#define GL_IMAGE_COMPATIBILITY_CLASS 0x82A8
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_PIXEL_FORMAT 0x82A9
#define GL_IMAGE_PIXEL_TYPE 0x82AA
#define GL_IMAGE_TEXEL_SIZE 0x82A7
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_INCR 0x1E02
#define GL_INCR_WRAP 0x8507
#define GL_INDEX 0x8222
#define GL_INDEX_ARRAY 0x8077
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_BITS 0x0D51
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_INDEX_MODE 0x0C30
#define GL_INDEX_OFFSET 0x0D13
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_INNOCENT_CONTEXT_RESET 0x8254
#define GL_INNOCENT_CONTEXT_RESET_ARB 0x8254
#define GL_INT 0x1404
#define GL_INT64_ARB 0x140E
#define GL_INT64_VEC2_ARB 0x8FE9
#define GL_INT64_VEC3_ARB 0x8FEA
#define GL_INT64_VEC4_ARB 0x8FEB
#define GL_INTENSITY 0x8049
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_INTENSITY16F_ARB 0x881D
#define GL_INTENSITY32F_ARB 0x8817
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_INTERNALFORMAT_ALPHA_SIZE 0x8274
#define GL_INTERNALFORMAT_ALPHA_TYPE 0x827B
#define GL_INTERNALFORMAT_BLUE_SIZE 0x8273
#define GL_INTERNALFORMAT_BLUE_TYPE 0x827A
#define GL_INTERNALFORMAT_DEPTH_SIZE 0x8275
#define GL_INTERNALFORMAT_DEPTH_TYPE 0x827C
#define GL_INTERNALFORMAT_GREEN_SIZE 0x8272
#define GL_INTERNALFORMAT_GREEN_TYPE 0x8279
#define GL_INTERNALFORMAT_PREFERRED 0x8270
#define GL_INTERNALFORMAT_RED_SIZE 0x8271
#define GL_INTERNALFORMAT_RED_TYPE 0x8278
#define GL_INTERNALFORMAT_SHARED_SIZE 0x8277
#define GL_INTERNALFORMAT_STENCIL_SIZE 0x8276
#define GL_INTERNALFORMAT_STENCIL_TYPE 0x827D
#define GL_INTERNALFORMAT_SUPPORTED 0x826F
#define GL_INTERPOLATE 0x8575
#define GL_INTERPOLATE_ARB 0x8575
#define GL_INT_2_10_10_10_REV 0x8D9F
#define GL_INT_IMAGE_1D 0x9057
#define GL_INT_IMAGE_1D_ARRAY 0x905D
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_INT_IMAGE_2D_MULTISAMPLE 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_INT_IMAGE_2D_RECT 0x905A
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900E
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC2_ARB 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC3_ARB 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_INT_VEC4_ARB 0x8B55
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_INVALID_INDEX 0xFFFFFFFF
#define GL_INVALID_OPERATION 0x0502
#define GL_INVALID_VALUE 0x0501
#define GL_INVERT 0x150A
#define GL_ISOLINES 0x8E7A
#define GL_IS_PER_PATCH 0x92E7
#define GL_IS_ROW_MAJOR 0x9300
#define GL_KEEP 0x1E00
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_LAYER_PROVOKING_VERTEX 0x825E
#define GL_LEFT 0x0406
#define GL_LEQUAL 0x0203
#define GL_LESS 0x0201
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_LIGHTEN_KHR 0x9298
#define GL_LIGHTING 0x0B50
#define GL_LIGHTING_BIT 0x00000040
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LINE 0x1B01
#define GL_LINEAR 0x2601
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_LINES 0x0001
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINES_ADJACENCY_ARB 0x000A
#define GL_LINE_BIT 0x00000004
#define GL_LINE_LOOP 0x0002
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LINE_STRIP 0x0003
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_LINE_STRIP_ADJACENCY_ARB 0x000B
#define GL_LINE_TOKEN 0x0702
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINK_STATUS 0x8B82
#define GL_LIST_BASE 0x0B32
#define GL_LIST_BIT 0x00020000
#define GL_LIST_INDEX 0x0B33
#define GL_LIST_MODE 0x0B30
#define GL_LOAD 0x0101
#define GL_LOCATION 0x930E
#define GL_LOCATION_COMPONENT 0x934A
#define GL_LOCATION_INDEX 0x930F
#define GL_LOGIC_OP 0x0BF1
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_LOSE_CONTEXT_ON_RESET 0x8252
#define GL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define GL_LOWER_LEFT 0x8CA1
#define GL_LOW_FLOAT 0x8DF0
#define GL_LOW_INT 0x8DF3
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_LUMINANCE32F_ARB 0x8818
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_LUMINANCE_ALPHA16F_ARB 0x881F
#define GL_LUMINANCE_ALPHA32F_ARB 0x8819
#define GL_MAJOR_VERSION 0x821B
#define GL_MANUAL_GENERATE_MIPMAP 0x8294
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP_COHERENT_BIT 0x0080
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_PERSISTENT_BIT 0x0040
#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_STENCIL 0x0D11
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MATRIX0_ARB 0x88C0
#define GL_MATRIX10_ARB 0x88CA
#define GL_MATRIX11_ARB 0x88CB
#define GL_MATRIX12_ARB 0x88CC
#define GL_MATRIX13_ARB 0x88CD
#define GL_MATRIX14_ARB 0x88CE
#define GL_MATRIX15_ARB 0x88CF
#define GL_MATRIX16_ARB 0x88D0
#define GL_MATRIX17_ARB 0x88D1
#define GL_MATRIX18_ARB 0x88D2
#define GL_MATRIX19_ARB 0x88D3
#define GL_MATRIX1_ARB 0x88C1
#define GL_MATRIX20_ARB 0x88D4
#define GL_MATRIX21_ARB 0x88D5
#define GL_MATRIX22_ARB 0x88D6
#define GL_MATRIX23_ARB 0x88D7
#define GL_MATRIX24_ARB 0x88D8
#define GL_MATRIX25_ARB 0x88D9
#define GL_MATRIX26_ARB 0x88DA
#define GL_MATRIX27_ARB 0x88DB
#define GL_MATRIX28_ARB 0x88DC
#define GL_MATRIX29_ARB 0x88DD
#define GL_MATRIX2_ARB 0x88C2
#define GL_MATRIX30_ARB 0x88DE
#define GL_MATRIX31_ARB 0x88DF
#define GL_MATRIX3_ARB 0x88C3
#define GL_MATRIX4_ARB 0x88C4
#define GL_MATRIX5_ARB 0x88C5
#define GL_MATRIX6_ARB 0x88C6
#define GL_MATRIX7_ARB 0x88C7
#define GL_MATRIX8_ARB 0x88C8
#define GL_MATRIX9_ARB 0x88C9
#define GL_MATRIX_INDEX_ARRAY_ARB 0x8844
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB 0x8849
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB 0x8846
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB 0x8848
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB 0x8847
#define GL_MATRIX_MODE 0x0BA0
#define GL_MATRIX_PALETTE_ARB 0x8840
#define GL_MATRIX_STRIDE 0x92FF
#define GL_MAX 0x8008
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_MAX_CLIP_DISTANCES 0x0D32
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES 0x82FA
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMBINED_DIMENSIONS 0x8282
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB 0x8B4D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB 0x90EB
#define GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB 0x91BF
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB 0x9344
#define GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB 0x9345
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB 0x851C
#define GL_MAX_CULL_DISTANCES 0x82F9
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB 0x9144
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB 0x9143
#define GL_MAX_DEPTH 0x8280
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_MAX_DRAW_BUFFERS_ARB 0x8824
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENT_INDEX 0x8D6B
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB 0x8DE0
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB 0x8C29
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB 0x8DE1
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB 0x8DDF
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB 0x8DDD
#define GL_MAX_HEIGHT 0x827F
#define GL_MAX_IMAGE_SAMPLES 0x906D
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_INTEGER_SAMPLES 0x9110
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_LAYERS 0x8281
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB 0x8841
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_LENGTH 0x92F6
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_NUM_ACTIVE_VARIABLES 0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_MAX_PALETTE_MATRICES_ARB 0x8842
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B1
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#define GL_MAX_PROGRAM_ATTRIBS_ARB 0x88AD
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB 0x88B5
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB 0x88A1
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB 0x88B4
#define GL_MAX_PROGRAM_MATRICES_ARB 0x862F
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB 0x862E
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AF
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A3
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AB
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A7
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#define GL_MAX_PROGRAM_PARAMETERS_ARB 0x88A9
#define GL_MAX_PROGRAM_TEMPORARIES_ARB 0x88A5
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB 0x8F9F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5F
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_MAX_SAMPLES 0x8D57
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_MAX_SHADER_COMPILER_THREADS_ARB 0x91B0
#define GL_MAX_SHADER_COMPILER_THREADS_KHR 0x91B0
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_ARB 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB 0x919A
#define GL_MAX_SPARSE_TEXTURE_SIZE_ARB 0x9198
#define GL_MAX_SUBROUTINES 0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_MAX_TEXTURE_BUFFER_SIZE_ARB 0x8C2B
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_COORDS_ARB 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB 0x8872
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_MAX_TEXTURE_UNITS_ARB 0x84E2
#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_LOCATIONS 0x826E
#define GL_MAX_VARYING_COMPONENTS 0x8B4B
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VARYING_FLOATS_ARB 0x8B4B
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_MAX_VERTEX_ATTRIBS_ARB 0x8869
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_VERTEX_STREAMS 0x8E71
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB 0x8B4A
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VERTEX_UNITS_ARB 0x86A4
#define GL_MAX_VERTEX_VARYING_COMPONENTS_ARB 0x8DDE
#define GL_MAX_VIEWPORTS 0x825B
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_WIDTH 0x827E
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_MEDIUM_INT 0x8DF4
#define GL_MIN 0x8007
#define GL_MINMAX 0x802E
#define GL_MINMAX_FORMAT 0x802F
#define GL_MINMAX_SINK 0x8030
#define GL_MINOR_VERSION 0x821C
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MIN_MAP_BUFFER_ALIGNMENT 0x90BC
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5E
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB 0x8C37
#define GL_MIPMAP 0x8293
#define GL_MIRRORED_REPEAT 0x8370
#define GL_MIRRORED_REPEAT_ARB 0x8370
#define GL_MIRROR_CLAMP_TO_EDGE 0x8743
#define GL_MODELVIEW 0x1700
#define GL_MODELVIEW0_ARB 0x1700
#define GL_MODELVIEW10_ARB 0x872A
#define GL_MODELVIEW11_ARB 0x872B
#define GL_MODELVIEW12_ARB 0x872C
#define GL_MODELVIEW13_ARB 0x872D
#define GL_MODELVIEW14_ARB 0x872E
#define GL_MODELVIEW15_ARB 0x872F
#define GL_MODELVIEW16_ARB 0x8730
#define GL_MODELVIEW17_ARB 0x8731
#define GL_MODELVIEW18_ARB 0x8732
#define GL_MODELVIEW19_ARB 0x8733
#define GL_MODELVIEW1_ARB 0x850A
#define GL_MODELVIEW20_ARB 0x8734
#define GL_MODELVIEW21_ARB 0x8735
#define GL_MODELVIEW22_ARB 0x8736
#define GL_MODELVIEW23_ARB 0x8737
#define GL_MODELVIEW24_ARB 0x8738
#define GL_MODELVIEW25_ARB 0x8739
#define GL_MODELVIEW26_ARB 0x873A
#define GL_MODELVIEW27_ARB 0x873B
#define GL_MODELVIEW28_ARB 0x873C
#define GL_MODELVIEW29_ARB 0x873D
#define GL_MODELVIEW2_ARB 0x8722
#define GL_MODELVIEW30_ARB 0x873E
#define GL_MODELVIEW31_ARB 0x873F
#define GL_MODELVIEW3_ARB 0x8723
#define GL_MODELVIEW4_ARB 0x8724
#define GL_MODELVIEW5_ARB 0x8725
#define GL_MODELVIEW6_ARB 0x8726
#define GL_MODELVIEW7_ARB 0x8727
#define GL_MODELVIEW8_ARB 0x8728
#define GL_MODELVIEW9_ARB 0x8729
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_MODULATE 0x2100
#define GL_MULT 0x0103
#define GL_MULTIPLY_KHR 0x9294
#define GL_MULTISAMPLE 0x809D
#define GL_MULTISAMPLE_ARB 0x809D
#define GL_MULTISAMPLE_BIT 0x20000000
#define GL_MULTISAMPLE_BIT_ARB 0x20000000
#define GL_MULTISAMPLE_LINE_WIDTH_GRANULARITY_ARB 0x9382
#define GL_MULTISAMPLE_LINE_WIDTH_RANGE_ARB 0x9381
#define GL_N3F_V3F 0x2A25
#define GL_NAMED_STRING_LENGTH_ARB 0x8DE9
#define GL_NAMED_STRING_TYPE_ARB 0x8DEA
#define GL_NAME_LENGTH 0x92F9
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_NAND 0x150E
#define GL_NEAREST 0x2600
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_NEGATIVE_ONE_TO_ONE 0x935E
#define GL_NEVER 0x0200
#define GL_NICEST 0x1102
#define GL_NONE 0
#define GL_NOOP 0x1505
#define GL_NOR 0x1508
#define GL_NORMALIZE 0x0BA1
#define GL_NORMAL_ARRAY 0x8075
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_MAP 0x8511
#define GL_NORMAL_MAP_ARB 0x8511
#define GL_NOTEQUAL 0x0205
#define GL_NO_ERROR 0
#define GL_NO_RESET_NOTIFICATION 0x8261
#define GL_NO_RESET_NOTIFICATION_ARB 0x8261
#define GL_NUM_ACTIVE_VARIABLES 0x9304
#define GL_NUM_COMPATIBLE_SUBROUTINES 0x8E4A
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A2
#define GL_NUM_EXTENSIONS 0x821D
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_NUM_SAMPLE_COUNTS 0x9380
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_NUM_SPARSE_LEVELS_ARB 0x91AA
#define GL_NUM_SPIR_V_EXTENSIONS 0x9554
#define GL_NUM_VIRTUAL_PAGE_SIZES_ARB 0x91A8
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB 0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB 0x8B8A
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#define GL_OBJECT_LINEAR 0x2401
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#define GL_OBJECT_PLANE 0x2501
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#define GL_OBJECT_TYPE 0x9112
#define GL_OBJECT_TYPE_ARB 0x8B4E
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#define GL_OFFSET 0x92FC
#define GL_ONE 1
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND0_ALPHA_ARB 0x8598
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND0_RGB_ARB 0x8590
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND1_ALPHA_ARB 0x8599
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND1_RGB_ARB 0x8591
#define GL_OPERAND2_ALPHA 0x859A
#define GL_OPERAND2_ALPHA_ARB 0x859A
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND2_RGB_ARB 0x8592
#define GL_OR 0x1507
#define GL_ORDER 0x0A01
#define GL_OR_INVERTED 0x150D
#define GL_OR_REVERSE 0x150B
#define GL_OUT_OF_MEMORY 0x0505
#define GL_OVERLAY_KHR 0x9296
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_PACK_COMPRESSED_BLOCK_DEPTH 0x912D
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT 0x912C
#define GL_PACK_COMPRESSED_BLOCK_SIZE 0x912E
#define GL_PACK_COMPRESSED_BLOCK_WIDTH 0x912B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PARAMETER_BUFFER 0x80EE
#define GL_PARAMETER_BUFFER_ARB 0x80EE
#define GL_PARAMETER_BUFFER_BINDING 0x80EF
#define GL_PARAMETER_BUFFER_BINDING_ARB 0x80EF
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_PATCHES 0x000E
#define GL_PATCH_DEFAULT_INNER_LEVEL 0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL 0x8E74
#define GL_PATCH_VERTICES 0x8E72
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x00000080
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_PACK_BUFFER_ARB 0x88EB
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB 0x88ED
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_UNPACK_BUFFER_ARB 0x88EC
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB 0x88EF
#define GL_POINT 0x1B00
#define GL_POINTS 0x0000
#define GL_POINT_BIT 0x00000002
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_SIZE_MAX_ARB 0x8127
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MIN_ARB 0x8126
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_POINT_SPRITE 0x8861
#define GL_POINT_SPRITE_ARB 0x8861
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_POINT_TOKEN 0x0701
#define GL_POLYGON 0x0009
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_OFFSET_CLAMP 0x8E1B
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_POLYGON_TOKEN 0x0703
#define GL_POSITION 0x1203
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#define GL_PREVIOUS 0x8578
#define GL_PREVIOUS_ARB 0x8578
#define GL_PRIMARY_COLOR 0x8577
#define GL_PRIMARY_COLOR_ARB 0x8577
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_PRIMITIVES_SUBMITTED 0x82EF
#define GL_PRIMITIVES_SUBMITTED_ARB 0x82EF
#define GL_PRIMITIVE_BOUNDING_BOX_ARB 0x92BE
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_PROGRAM 0x82E2
#define GL_PROGRAMMABLE_SAMPLE_LOCATION_ARB 0x9341
#define GL_PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_ARB 0x9340
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B0
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB 0x8805
#define GL_PROGRAM_ATTRIBS_ARB 0x88AC
#define GL_PROGRAM_BINARY_FORMATS 0x87FF
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINDING_ARB 0x8677
#define GL_PROGRAM_ERROR_POSITION_ARB 0x864B
#define GL_PROGRAM_ERROR_STRING_ARB 0x8874
#define GL_PROGRAM_FORMAT_ARB 0x8876
#define GL_PROGRAM_FORMAT_ASCII_ARB 0x8875
#define GL_PROGRAM_INPUT 0x92E3
#define GL_PROGRAM_INSTRUCTIONS_ARB 0x88A0
#define GL_PROGRAM_LENGTH_ARB 0x8627
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B2
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AE
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A2
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AA
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A6
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#define GL_PROGRAM_OBJECT_ARB 0x8B40
#define GL_PROGRAM_OUTPUT 0x92E4
#define GL_PROGRAM_PARAMETERS_ARB 0x88A8
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_PROGRAM_STRING_ARB 0x8628
#define GL_PROGRAM_TEMPORARIES_ARB 0x88A4
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB 0x8807
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB 0x8806
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB 0x88B6
#define GL_PROJECTION 0x1701
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_PROVOKING_VERTEX 0x8E4F
#define GL_PROXY_COLOR_TABLE 0x80D3
#define GL_PROXY_HISTOGRAM 0x8025
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB 0x900B
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_PROXY_TEXTURE_RECTANGLE_ARB 0x84F7
#define GL_Q 0x2003
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_QUADS 0x0007
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_QUAD_STRIP 0x0008
#define GL_QUERY 0x82E3
#define GL_QUERY_BUFFER 0x9192
#define GL_QUERY_BUFFER_BARRIER_BIT 0x00008000
#define GL_QUERY_BUFFER_BINDING 0x9193
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16
#define GL_QUERY_BY_REGION_NO_WAIT_INVERTED 0x8E1A
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_WAIT_INVERTED 0x8E19
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_QUERY_COUNTER_BITS_ARB 0x8864
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_NO_WAIT_INVERTED 0x8E18
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_ARB 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_QUERY_RESULT_AVAILABLE_ARB 0x8867
#define GL_QUERY_RESULT_NO_WAIT 0x9194
#define GL_QUERY_TARGET 0x82EA
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_WAIT_INVERTED 0x8E17
#define GL_R 0x2002
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_R16 0x822A
#define GL_R16F 0x822D
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R16_SNORM 0x8F98
#define GL_R32F 0x822E
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_R3_G3_B2 0x2A10
#define GL_R8 0x8229
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R8_SNORM 0x8F94
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_READ_BUFFER 0x0C02
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_READ_ONLY 0x88B8
#define GL_READ_ONLY_ARB 0x88B8
#define GL_READ_PIXELS 0x828C
#define GL_READ_PIXELS_FORMAT 0x828D
#define GL_READ_PIXELS_TYPE 0x828E
#define GL_READ_WRITE 0x88BA
#define GL_READ_WRITE_ARB 0x88BA
#define GL_RED 0x1903
#define GL_REDUCE 0x8016
#define GL_RED_BIAS 0x0D15
#define GL_RED_BITS 0x0D52
#define GL_RED_INTEGER 0x8D94
#define GL_RED_SCALE 0x0D14
#define GL_REFERENCED_BY_COMPUTE_SHADER 0x930B
#define GL_REFERENCED_BY_FRAGMENT_SHADER 0x930A
#define GL_REFERENCED_BY_GEOMETRY_SHADER 0x9309
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_VERTEX_SHADER 0x9306
#define GL_REFLECTION_MAP 0x8512
#define GL_REFLECTION_MAP_ARB 0x8512
#define GL_RENDER 0x1C00
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERER 0x1F01
#define GL_RENDER_MODE 0x0C40
#define GL_REPEAT 0x2901
#define GL_REPLACE 0x1E01
#define GL_REPLICATE_BORDER 0x8153
#define GL_RESCALE_NORMAL 0x803A
#define GL_RESET_NOTIFICATION_STRATEGY 0x8256
#define GL_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define GL_RETURN 0x0102
#define GL_RG 0x8227
#define GL_RG16 0x822C
#define GL_RG16F 0x822F
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG16_SNORM 0x8F99
#define GL_RG32F 0x8230
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_RG8 0x822B
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG8_SNORM 0x8F95
#define GL_RGB 0x1907
#define GL_RGB10 0x8052
#define GL_RGB10_A2 0x8059
#define GL_RGB10_A2UI 0x906F
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGB16F 0x881B
#define GL_RGB16F_ARB 0x881B
#define GL_RGB16I 0x8D89
#define GL_RGB16UI 0x8D77
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGB32F 0x8815
#define GL_RGB32F_ARB 0x8815
#define GL_RGB32I 0x8D83
#define GL_RGB32UI 0x8D71
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB565 0x8D62
#define GL_RGB5_A1 0x8057
#define GL_RGB8 0x8051
#define GL_RGB8I 0x8D8F
#define GL_RGB8UI 0x8D7D
#define GL_RGB8_SNORM 0x8F96
#define GL_RGB9_E5 0x8C3D
#define GL_RGBA 0x1908
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_RGBA16F 0x881A
#define GL_RGBA16F_ARB 0x881A
#define GL_RGBA16I 0x8D88
#define GL_RGBA16UI 0x8D76
#define GL_RGBA16_SNORM 0x8F9B
#define GL_RGBA2 0x8055
#define GL_RGBA32F 0x8814
#define GL_RGBA32F_ARB 0x8814
#define GL_RGBA32I 0x8D82
#define GL_RGBA32UI 0x8D70
#define GL_RGBA4 0x8056
#define GL_RGBA8 0x8058
#define GL_RGBA8I 0x8D8E
#define GL_RGBA8UI 0x8D7C
#define GL_RGBA8_SNORM 0x8F97
#define GL_RGBA_FLOAT_MODE_ARB 0x8820
#define GL_RGBA_INTEGER 0x8D99
#define GL_RGBA_MODE 0x0C31
#define GL_RGB_INTEGER 0x8D98
#define GL_RGB_SCALE 0x8573
#define GL_RGB_SCALE_ARB 0x8573
#define GL_RG_INTEGER 0x8228
#define GL_RIGHT 0x0407
#define GL_S 0x2000
#define GL_SAMPLER 0x82E6
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_1D_ARB 0x8B5D
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_2D_ARB 0x8B5E
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_3D_ARB 0x8B5F
#define GL_SAMPLER_BINDING 0x8919
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_CUBE_ARB 0x8B60
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_SAMPLES 0x80A9
#define GL_SAMPLES_ARB 0x80A9
#define GL_SAMPLES_PASSED 0x8914
#define GL_SAMPLES_PASSED_ARB 0x8914
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_ALPHA_TO_ONE_ARB 0x809F
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLE_BUFFERS_ARB 0x80A8
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_COVERAGE_ARB 0x80A0
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_SAMPLE_COVERAGE_INVERT_ARB 0x80AB
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_VALUE_ARB 0x80AA
#define GL_SAMPLE_LOCATION_ARB 0x8E50
#define GL_SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_ARB 0x933F
#define GL_SAMPLE_LOCATION_PIXEL_GRID_WIDTH_ARB 0x933E
#define GL_SAMPLE_LOCATION_SUBPIXEL_BITS_ARB 0x933D
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_SHADING_ARB 0x8C36
#define GL_SCISSOR_BIT 0x00080000
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_SCREEN_KHR 0x9295
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SELECT 0x1C02
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#define GL_SEPARABLE_2D 0x8012
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_SET 0x150F
#define GL_SHADER 0x82E1
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_SHADER_BINARY_FORMAT_SPIR_V 0x9551
#define GL_SHADER_BINARY_FORMAT_SPIR_V_ARB 0x9551
#define GL_SHADER_COMPILER 0x8DFA
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_SHADER_IMAGE_ATOMIC 0x82A6
#define GL_SHADER_IMAGE_LOAD 0x82A4
#define GL_SHADER_IMAGE_STORE 0x82A5
#define GL_SHADER_INCLUDE_ARB 0x8DAE
#define GL_SHADER_OBJECT_ARB 0x8B48
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000
#define GL_SHADER_STORAGE_BLOCK 0x92E6
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_TYPE 0x8B4F
#define GL_SHADE_MODEL 0x0B54
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C
#define GL_SHININESS 0x1601
#define GL_SHORT 0x1402
#define GL_SIGNALED 0x9119
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_SINGLE_COLOR 0x81F9
#define GL_SLUMINANCE 0x8C46
#define GL_SLUMINANCE8 0x8C47
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#define GL_SLUMINANCE_ALPHA 0x8C44
#define GL_SMOOTH 0x1D01
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SOFTLIGHT_KHR 0x929C
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE0_ALPHA_ARB 0x8588
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE0_RGB_ARB 0x8580
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE1_ALPHA_ARB 0x8589
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE1_RGB_ARB 0x8581
#define GL_SOURCE2_ALPHA 0x858A
#define GL_SOURCE2_ALPHA_ARB 0x858A
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE2_RGB_ARB 0x8582
#define GL_SPARSE_BUFFER_PAGE_SIZE_ARB 0x82F8
#define GL_SPARSE_STORAGE_BIT_ARB 0x0400
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB 0x91A9
#define GL_SPECULAR 0x1202
#define GL_SPHERE_MAP 0x2402
#define GL_SPIR_V_BINARY 0x9552
#define GL_SPIR_V_BINARY_ARB 0x9552
#define GL_SPIR_V_EXTENSIONS 0x9553
#define GL_SPOT_CUTOFF 0x1206
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SRC0_ALPHA 0x8588
#define GL_SRC0_RGB 0x8580
#define GL_SRC1_ALPHA 0x8589
#define GL_SRC1_COLOR 0x88F9
#define GL_SRC1_RGB 0x8581
#define GL_SRC2_ALPHA 0x858A
#define GL_SRC2_RGB 0x8582
#define GL_SRC_ALPHA 0x0302
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_SRC_COLOR 0x0300
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB_DECODE_ARB 0x8299
#define GL_SRGB_READ 0x8297
#define GL_SRGB_WRITE 0x8298
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_STATIC_COPY 0x88E6
#define GL_STATIC_COPY_ARB 0x88E6
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_DRAW_ARB 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_READ_ARB 0x88E5
#define GL_STENCIL 0x1802
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#define GL_STENCIL_BITS 0x0D57
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_COMPONENTS 0x8285
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_INDEX 0x1901
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX16 0x8D49
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_RENDERABLE 0x8288
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_STEREO 0x0C33
#define GL_STREAM_COPY 0x88E2
#define GL_STREAM_COPY_ARB 0x88E2
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_DRAW_ARB 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_READ_ARB 0x88E1
#define GL_SUBGROUP_FEATURE_ARITHMETIC_BIT_KHR 0x00000004
#define GL_SUBGROUP_FEATURE_BALLOT_BIT_KHR 0x00000008
#define GL_SUBGROUP_FEATURE_BASIC_BIT_KHR 0x00000001
#define GL_SUBGROUP_FEATURE_CLUSTERED_BIT_KHR 0x00000040
#define GL_SUBGROUP_FEATURE_QUAD_BIT_KHR 0x00000080
#define GL_SUBGROUP_FEATURE_SHUFFLE_BIT_KHR 0x00000010
#define GL_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT_KHR 0x00000020
#define GL_SUBGROUP_FEATURE_VOTE_BIT_KHR 0x00000002
#define GL_SUBGROUP_QUAD_ALL_STAGES_KHR 0x9535
#define GL_SUBGROUP_SIZE_KHR 0x9532
#define GL_SUBGROUP_SUPPORTED_FEATURES_KHR 0x9534
#define GL_SUBGROUP_SUPPORTED_STAGES_KHR 0x9533
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_SUBTRACT 0x84E7
#define GL_SUBTRACT_ARB 0x84E7
#define GL_SYNC_CL_EVENT_ARB 0x8240
#define GL_SYNC_CL_EVENT_COMPLETE_ARB 0x8241
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_SYNC_STATUS 0x9114
#define GL_T 0x2001
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_V3F 0x2A27
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_T4F_V4F 0x2A28
#define GL_TABLE_TOO_LARGE 0x8031
#define GL_TESS_CONTROL_OUTPUT_VERTICES 0x8E75
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_CONTROL_SHADER_PATCHES 0x82F1
#define GL_TESS_CONTROL_SHADER_PATCHES_ARB 0x82F1
#define GL_TESS_CONTROL_SUBROUTINE 0x92E9
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_CONTROL_TEXTURE 0x829C
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS 0x82F2
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB 0x82F2
#define GL_TESS_EVALUATION_SUBROUTINE 0x92EA
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_TESS_EVALUATION_TEXTURE 0x829D
#define GL_TESS_GEN_MODE 0x8E76
#define GL_TESS_GEN_POINT_MODE 0x8E79
#define GL_TESS_GEN_SPACING 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER 0x8E78
#define GL_TEXTURE 0x1702
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE0_ARB 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE10_ARB 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE11_ARB 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE12_ARB 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE13_ARB 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE14_ARB 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE15_ARB 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE16_ARB 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE17_ARB 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE18_ARB 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE19_ARB 0x84D3
#define GL_TEXTURE1_ARB 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE20_ARB 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE21_ARB 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE22_ARB 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE23_ARB 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE24_ARB 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE25_ARB 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE26_ARB 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE27_ARB 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE28_ARB 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE29_ARB 0x84DD
#define GL_TEXTURE2_ARB 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE30_ARB 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_TEXTURE31_ARB 0x84DF
#define GL_TEXTURE3_ARB 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE4_ARB 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE5_ARB 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE6_ARB 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE7_ARB 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE8_ARB 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE9_ARB 0x84C9
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BINDING_BUFFER_ARB 0x8C2C
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB 0x8514
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB 0x900A
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_TEXTURE_BINDING_RECTANGLE_ARB 0x84F6
#define GL_TEXTURE_BIT 0x00040000
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_BLUE_TYPE_ARB 0x8C12
#define GL_TEXTURE_BORDER 0x1005
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_TEXTURE_BUFFER_ARB 0x8C2A
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_ARB 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_ARB 0x8C2E
#define GL_TEXTURE_BUFFER_OFFSET 0x919D
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F
#define GL_TEXTURE_BUFFER_SIZE 0x919E
#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#define GL_TEXTURE_COMPONENTS 0x1003
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_TEXTURE_COMPRESSED_ARB 0x86A1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE 0x82B3
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB 0x86A0
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_TEXTURE_COMPRESSION_HINT_ARB 0x84EF
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB 0x9009
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#define GL_TEXTURE_ENV 0x2300
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_TEXTURE_GATHER 0x82A2
#define GL_TEXTURE_GATHER_SHADOW 0x82A3
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_GREEN_TYPE_ARB 0x8C11
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_IMAGE_FORMAT 0x828F
#define GL_TEXTURE_IMAGE_TYPE 0x8290
#define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_TEXTURE_REDUCTION_MODE_ARB 0x9366
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_RED_TYPE_ARB 0x8C10
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_SHADOW 0x82A1
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TEXTURE_SPARSE_ARB 0x91A6
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
#define GL_TEXTURE_TARGET 0x1006
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x00000100
#define GL_TEXTURE_VIEW 0x82B5
#define GL_TEXTURE_VIEW_MIN_LAYER 0x82DD
#define GL_TEXTURE_VIEW_MIN_LEVEL 0x82DB
#define GL_TEXTURE_VIEW_NUM_LAYERS 0x82DE
#define GL_TEXTURE_VIEW_NUM_LEVELS 0x82DC
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF
#define GL_TIMESTAMP 0x8E28
#define GL_TIME_ELAPSED 0x88BF
#define GL_TOP_LEVEL_ARRAY_SIZE 0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE 0x930D
#define GL_TRANSFORM_BIT 0x00001000
#define GL_TRANSFORM_FEEDBACK 0x8E22
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_TRANSFORM_FEEDBACK_BUFFER_INDEX 0x934B
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE 0x934C
#define GL_TRANSFORM_FEEDBACK_OVERFLOW 0x82EC
#define GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB 0x82EC
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW 0x82ED
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB 0x82ED
#define GL_TRANSFORM_FEEDBACK_VARYING 0x92F4
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_TRANSPOSE_COLOR_MATRIX_ARB 0x84E6
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB 0x88B7
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB 0x84E5
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLES_ADJACENCY_ARB 0x000C
#define GL_TRIANGLE_FAN 0x0006
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_TRIANGLE_STRIP_ADJACENCY_ARB 0x000D
#define GL_TRUE 1
#define GL_TYPE 0x92FA
#define GL_UNDEFINED_VERTEX 0x8260
#define GL_UNIFORM 0x92E1
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNIFORM_BARRIER_BIT 0x00000004
#define GL_UNIFORM_BLOCK 0x92E2
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNKNOWN_CONTEXT_RESET 0x8255
#define GL_UNKNOWN_CONTEXT_RESET_ARB 0x8255
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH 0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE 0x912A
#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH 0x9127
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNSIGNALED 0x9118
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_INT64_ARB 0x140F
#define GL_UNSIGNED_INT64_VEC2_ARB 0x8FF5
#define GL_UNSIGNED_INT64_VEC3_ARB 0x8FF6
#define GL_UNSIGNED_INT64_VEC4_ARB 0x8FF7
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_UNSIGNED_INT_IMAGE_1D 0x9062
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_UNSIGNED_INT_IMAGE_2D_RECT 0x9065
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900F
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_UNSIGNED_NORMALIZED_ARB 0x8C17
#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UPPER_LEFT 0x8CA2
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_VALIDATE_STATUS 0x8B83
#define GL_VENDOR 0x1F00
#define GL_VERSION 0x1F02
#define GL_VERTEX_ARRAY 0x8074
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB 0x88FE
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB 0x8645
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB 0x8625
#define GL_VERTEX_ATTRIB_BINDING 0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D5
#define GL_VERTEX_BINDING_DIVISOR 0x82D6
#define GL_VERTEX_BINDING_OFFSET 0x82D7
#define GL_VERTEX_BINDING_STRIDE 0x82D8
#define GL_VERTEX_BLEND_ARB 0x86A7
#define GL_VERTEX_PROGRAM_ARB 0x8620
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB 0x8643
#define GL_VERTEX_SHADER 0x8B31
#define GL_VERTEX_SHADER_ARB 0x8B31
#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_VERTEX_SHADER_INVOCATIONS 0x82F0
#define GL_VERTEX_SHADER_INVOCATIONS_ARB 0x82F0
#define GL_VERTEX_SUBROUTINE 0x92E8
#define GL_VERTEX_SUBROUTINE_UNIFORM 0x92EE
#define GL_VERTEX_TEXTURE 0x829B
#define GL_VERTICES_SUBMITTED 0x82EE
#define GL_VERTICES_SUBMITTED_ARB 0x82EE
#define GL_VIEWPORT 0x0BA2
#define GL_VIEWPORT_BIT 0x00000800
#define GL_VIEWPORT_BOUNDS_RANGE 0x825D
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_VIEWPORT_SUBPIXEL_BITS 0x825C
#define GL_VIEW_CLASS_128_BITS 0x82C4
#define GL_VIEW_CLASS_16_BITS 0x82CA
#define GL_VIEW_CLASS_24_BITS 0x82C9
#define GL_VIEW_CLASS_32_BITS 0x82C8
#define GL_VIEW_CLASS_48_BITS 0x82C7
#define GL_VIEW_CLASS_64_BITS 0x82C6
#define GL_VIEW_CLASS_8_BITS 0x82CB
#define GL_VIEW_CLASS_96_BITS 0x82C5
#define GL_VIEW_CLASS_ASTC_10x10_RGBA 0x9393
#define GL_VIEW_CLASS_ASTC_10x5_RGBA 0x9390
#define GL_VIEW_CLASS_ASTC_10x6_RGBA 0x9391
#define GL_VIEW_CLASS_ASTC_10x8_RGBA 0x9392
#define GL_VIEW_CLASS_ASTC_12x10_RGBA 0x9394
#define GL_VIEW_CLASS_ASTC_12x12_RGBA 0x9395
#define GL_VIEW_CLASS_ASTC_4x4_RGBA 0x9388
#define GL_VIEW_CLASS_ASTC_5x4_RGBA 0x9389
#define GL_VIEW_CLASS_ASTC_5x5_RGBA 0x938A
#define GL_VIEW_CLASS_ASTC_6x5_RGBA 0x938B
#define GL_VIEW_CLASS_ASTC_6x6_RGBA 0x938C
#define GL_VIEW_CLASS_ASTC_8x5_RGBA 0x938D
#define GL_VIEW_CLASS_ASTC_8x6_RGBA 0x938E
#define GL_VIEW_CLASS_ASTC_8x8_RGBA 0x938F
#define GL_VIEW_CLASS_BPTC_FLOAT 0x82D3
#define GL_VIEW_CLASS_BPTC_UNORM 0x82D2
#define GL_VIEW_CLASS_EAC_R11 0x9383
#define GL_VIEW_CLASS_EAC_RG11 0x9384
#define GL_VIEW_CLASS_ETC2_EAC_RGBA 0x9387
#define GL_VIEW_CLASS_ETC2_RGB 0x9385
#define GL_VIEW_CLASS_ETC2_RGBA 0x9386
#define GL_VIEW_CLASS_RGTC1_RED 0x82D0
#define GL_VIEW_CLASS_RGTC2_RG 0x82D1
#define GL_VIEW_CLASS_S3TC_DXT1_RGB 0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA 0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA 0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA 0x82CF
#define GL_VIEW_COMPATIBILITY_CLASS 0x82B6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_ARB 0x91A7
#define GL_VIRTUAL_PAGE_SIZE_X_ARB 0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_ARB 0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_ARB 0x9197
#define GL_WAIT_FAILED 0x911D
#define GL_WEIGHTED_AVERAGE_ARB 0x9367
#define GL_WEIGHT_ARRAY_ARB 0x86AD
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_WEIGHT_ARRAY_POINTER_ARB 0x86AC
#define GL_WEIGHT_ARRAY_SIZE_ARB 0x86AB
#define GL_WEIGHT_ARRAY_STRIDE_ARB 0x86AA
#define GL_WEIGHT_ARRAY_TYPE_ARB 0x86A9
#define GL_WEIGHT_SUM_UNITY_ARB 0x86A6
#define GL_WRITE_ONLY 0x88B9
#define GL_WRITE_ONLY_ARB 0x88B9
#define GL_XOR 0x1506
#define GL_ZERO 0
#define GL_ZERO_TO_ONE 0x935F
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17


#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Khronos platform-specific types and definitions.
 *
 * The master copy of khrplatform.h is maintained in the Khronos EGL
 * Registry repository at https://github.com/KhronosGroup/EGL-Registry
 * The last semantic modification to khrplatform.h was at commit ID:
 *      67a3e0864c2d75ea5287b9f3d2eb74a745936692
 *
 * Adopters may modify this file to suit their platform. Adopters are
 * encouraged to submit platform specific modifications to the Khronos
 * group so that they can be included in future versions of this file.
 * Please submit changes by filing pull requests or issues on
 * the EGL Registry repository linked above.
 *
 *
 * See the Implementer's Guidelines for information about where this file
 * should be located on your system and for more details of its use:
 *    http://www.khronos.org/registry/implementers_guide.pdf
 *
 * This file should be included as
 *        #include <KHR/khrplatform.h>
 * by Khronos client API header files that use its types and defines.
 *
 * The types in khrplatform.h should only be used to define API-specific types.
 *
 * Types defined in khrplatform.h:
 *    khronos_int8_t              signed   8  bit
 *    khronos_uint8_t             unsigned 8  bit
 *    khronos_int16_t             signed   16 bit
 *    khronos_uint16_t            unsigned 16 bit
 *    khronos_int32_t             signed   32 bit
 *    khronos_uint32_t            unsigned 32 bit
 *    khronos_int64_t             signed   64 bit
 *    khronos_uint64_t            unsigned 64 bit
 *    khronos_intptr_t            signed   same number of bits as a pointer
 *    khronos_uintptr_t           unsigned same number of bits as a pointer
 *    khronos_ssize_t             signed   size
 *    khronos_usize_t             unsigned size
 *    khronos_float_t             signed   32 bit floating point
 *    khronos_time_ns_t           unsigned 64 bit time in nanoseconds
 *    khronos_utime_nanoseconds_t unsigned time interval or absolute time in
 *                                         nanoseconds
 *    khronos_stime_nanoseconds_t signed time interval in nanoseconds
 *    khronos_boolean_enum_t      enumerated boolean type. This should
 *      only be used as a base type when a client API's boolean type is
 *      an enum. Client APIs which use an integer or other type for
 *      booleans cannot use this as the base type for their boolean.
 *
 * Tokens defined in khrplatform.h:
 *
 *    KHRONOS_FALSE, KHRONOS_TRUE Enumerated boolean false/true values.
 *
 *    KHRONOS_SUPPORT_INT64 is 1 if 64 bit integers are supported; otherwise 0.
 *    KHRONOS_SUPPORT_FLOAT is 1 if floats are supported; otherwise 0.
 *
 * Calling convention macros defined in this file:
 *    KHRONOS_APICALL
 *    KHRONOS_GLAD_API_PTR
 *    KHRONOS_APIATTRIBUTES
 *
 * These may be used in function prototypes as:
 *
 *      KHRONOS_APICALL void KHRONOS_GLAD_API_PTR funcname(
 *                                  int arg1,
 *                                  int arg2) KHRONOS_APIATTRIBUTES;
 */

#if defined(__SCITECH_SNAP__) && !defined(KHRONOS_STATIC)
#   define KHRONOS_STATIC 1
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APICALL
 *-------------------------------------------------------------------------
 * This precedes the return type of the function in the function prototype.
 */
#if defined(KHRONOS_STATIC)
    /* If the preprocessor constant KHRONOS_STATIC is defined, make the
     * header compatible with static linking. */
#   define KHRONOS_APICALL
#elif defined(_WIN32)
#   define KHRONOS_APICALL __declspec(dllimport)
#elif defined (__SYMBIAN32__)
#   define KHRONOS_APICALL IMPORT_C
#elif defined(__ANDROID__)
#   define KHRONOS_APICALL __attribute__((visibility("default")))
#else
#   define KHRONOS_APICALL
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_GLAD_API_PTR
 *-------------------------------------------------------------------------
 * This follows the return type of the function  and precedes the function
 * name in the function prototype.
 */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(KHRONOS_STATIC)
    /* Win32 but not WinCE */
#   define KHRONOS_GLAD_API_PTR __stdcall
#else
#   define KHRONOS_GLAD_API_PTR
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIATTRIBUTES
 *-------------------------------------------------------------------------
 * This follows the closing parenthesis of the function prototype arguments.
 */
#if defined (__ARMCC_2__)
#define KHRONOS_APIATTRIBUTES __softfp
#else
#define KHRONOS_APIATTRIBUTES
#endif

/*-------------------------------------------------------------------------
 * basic type definitions
 *-----------------------------------------------------------------------*/
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || defined(__GNUC__) || defined(__SCO__) || defined(__USLC__)


/*
 * Using <stdint.h>
 */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(__VMS ) || defined(__sgi)

/*
 * Using <inttypes.h>
 */
#include <inttypes.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(_WIN32) && !defined(__SCITECH_SNAP__)

/*
 * Win32
 */
typedef __int32                 khronos_int32_t;
typedef unsigned __int32        khronos_uint32_t;
typedef __int64                 khronos_int64_t;
typedef unsigned __int64        khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(__sun__) || defined(__digital__)

/*
 * Sun or Digital
 */
typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
#if defined(__arch64__) || defined(_LP64)
typedef long int                khronos_int64_t;
typedef unsigned long int       khronos_uint64_t;
#else
typedef long long int           khronos_int64_t;
typedef unsigned long long int  khronos_uint64_t;
#endif /* __arch64__ */
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif 0

/*
 * Hypothetical platform with no float or int64 support
 */
typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
#define KHRONOS_SUPPORT_INT64   0
#define KHRONOS_SUPPORT_FLOAT   0

#else

/*
 * Generic fallback
 */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#endif


/*
 * Types that are (so far) the same on all platforms
 */
typedef signed   char          khronos_int8_t;
typedef unsigned char          khronos_uint8_t;
typedef signed   short int     khronos_int16_t;
typedef unsigned short int     khronos_uint16_t;

/*
 * Types that differ between LLP64 and LP64 architectures - in LLP64,
 * pointers are 64 bits, but 'long' is still 32 bits. Win64 appears
 * to be the only LLP64 architecture in current use.
 */
#ifdef _WIN64
typedef signed   long long int khronos_intptr_t;
typedef unsigned long long int khronos_uintptr_t;
typedef signed   long long int khronos_ssize_t;
typedef unsigned long long int khronos_usize_t;
#else
typedef signed   long  int     khronos_intptr_t;
typedef unsigned long  int     khronos_uintptr_t;
typedef signed   long  int     khronos_ssize_t;
typedef unsigned long  int     khronos_usize_t;
#endif

#if KHRONOS_SUPPORT_FLOAT
/*
 * Float type
 */
typedef          float         khronos_float_t;
#endif

#if KHRONOS_SUPPORT_INT64
/* Time types
 *
 * These types can be used to represent a time interval in nanoseconds or
 * an absolute Unadjusted System Time.  Unadjusted System Time is the number
 * of nanoseconds since some arbitrary system event (e.g. since the last
 * time the system booted).  The Unadjusted System Time is an unsigned
 * 64 bit value that wraps back to 0 every 584 years.  Time intervals
 * may be either signed or unsigned.
 */
typedef khronos_uint64_t       khronos_utime_nanoseconds_t;
typedef khronos_int64_t        khronos_stime_nanoseconds_t;
#endif

/*
 * Dummy value used to pad enum types to 32 bits.
 */
#ifndef KHRONOS_MAX_ENUM
#define KHRONOS_MAX_ENUM 0x7FFFFFFF
#endif

/*
 * Enumerated boolean type
 *
 * Values other than zero should be considered to be true.  Therefore
 * comparisons should not be made against KHRONOS_TRUE.
 */
typedef enum {
    KHRONOS_FALSE = 0,
    KHRONOS_TRUE  = 1,
    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = KHRONOS_MAX_ENUM
} khronos_boolean_enum_t;

#endif /* __khrplatform_h_ */
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_intptr_t GLintptr;
#else
typedef khronos_intptr_t GLintptr;
#endif
#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_intptr_t GLintptrARB;
#else
typedef khronos_intptr_t GLintptrARB;
#endif
#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_ssize_t GLsizeiptr;
#else
typedef khronos_ssize_t GLsizeiptr;
#endif
#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_ssize_t GLsizeiptrARB;
#else
typedef khronos_ssize_t GLsizeiptrARB;
#endif
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (GLAD_API_PTR *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (GLAD_API_PTR *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (GLAD_API_PTR *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (GLAD_API_PTR *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;
typedef void (GLAD_API_PTR *GLVULKANPROCNV)(void);


#define GL_VERSION_1_0 1
GLAD_API_CALL int GLAD_GL_VERSION_1_0;
#define GL_VERSION_1_1 1
GLAD_API_CALL int GLAD_GL_VERSION_1_1;
#define GL_VERSION_1_2 1
GLAD_API_CALL int GLAD_GL_VERSION_1_2;
#define GL_VERSION_1_3 1
GLAD_API_CALL int GLAD_GL_VERSION_1_3;
#define GL_VERSION_1_4 1
GLAD_API_CALL int GLAD_GL_VERSION_1_4;
#define GL_VERSION_1_5 1
GLAD_API_CALL int GLAD_GL_VERSION_1_5;
#define GL_VERSION_2_0 1
GLAD_API_CALL int GLAD_GL_VERSION_2_0;
#define GL_VERSION_2_1 1
GLAD_API_CALL int GLAD_GL_VERSION_2_1;
#define GL_VERSION_3_0 1
GLAD_API_CALL int GLAD_GL_VERSION_3_0;
#define GL_VERSION_3_1 1
GLAD_API_CALL int GLAD_GL_VERSION_3_1;
#define GL_VERSION_3_2 1
GLAD_API_CALL int GLAD_GL_VERSION_3_2;
#define GL_VERSION_3_3 1
GLAD_API_CALL int GLAD_GL_VERSION_3_3;
#define GL_ARB_ES2_compatibility 1
GLAD_API_CALL int GLAD_GL_ARB_ES2_compatibility;
#define GL_ARB_ES3_1_compatibility 1
GLAD_API_CALL int GLAD_GL_ARB_ES3_1_compatibility;
#define GL_ARB_ES3_2_compatibility 1
GLAD_API_CALL int GLAD_GL_ARB_ES3_2_compatibility;
#define GL_ARB_ES3_compatibility 1
GLAD_API_CALL int GLAD_GL_ARB_ES3_compatibility;
#define GL_ARB_arrays_of_arrays 1
GLAD_API_CALL int GLAD_GL_ARB_arrays_of_arrays;
#define GL_ARB_base_instance 1
GLAD_API_CALL int GLAD_GL_ARB_base_instance;
#define GL_ARB_bindless_texture 1
GLAD_API_CALL int GLAD_GL_ARB_bindless_texture;
#define GL_ARB_blend_func_extended 1
GLAD_API_CALL int GLAD_GL_ARB_blend_func_extended;
#define GL_ARB_buffer_storage 1
GLAD_API_CALL int GLAD_GL_ARB_buffer_storage;
#define GL_ARB_cl_event 1
GLAD_API_CALL int GLAD_GL_ARB_cl_event;
#define GL_ARB_clear_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_clear_buffer_object;
#define GL_ARB_clear_texture 1
GLAD_API_CALL int GLAD_GL_ARB_clear_texture;
#define GL_ARB_clip_control 1
GLAD_API_CALL int GLAD_GL_ARB_clip_control;
#define GL_ARB_color_buffer_float 1
GLAD_API_CALL int GLAD_GL_ARB_color_buffer_float;
#define GL_ARB_compatibility 1
GLAD_API_CALL int GLAD_GL_ARB_compatibility;
#define GL_ARB_compressed_texture_pixel_storage 1
GLAD_API_CALL int GLAD_GL_ARB_compressed_texture_pixel_storage;
#define GL_ARB_compute_shader 1
GLAD_API_CALL int GLAD_GL_ARB_compute_shader;
#define GL_ARB_compute_variable_group_size 1
GLAD_API_CALL int GLAD_GL_ARB_compute_variable_group_size;
#define GL_ARB_conditional_render_inverted 1
GLAD_API_CALL int GLAD_GL_ARB_conditional_render_inverted;
#define GL_ARB_conservative_depth 1
GLAD_API_CALL int GLAD_GL_ARB_conservative_depth;
#define GL_ARB_copy_buffer 1
GLAD_API_CALL int GLAD_GL_ARB_copy_buffer;
#define GL_ARB_copy_image 1
GLAD_API_CALL int GLAD_GL_ARB_copy_image;
#define GL_ARB_cull_distance 1
GLAD_API_CALL int GLAD_GL_ARB_cull_distance;
#define GL_ARB_debug_output 1
GLAD_API_CALL int GLAD_GL_ARB_debug_output;
#define GL_ARB_depth_buffer_float 1
GLAD_API_CALL int GLAD_GL_ARB_depth_buffer_float;
#define GL_ARB_depth_clamp 1
GLAD_API_CALL int GLAD_GL_ARB_depth_clamp;
#define GL_ARB_depth_texture 1
GLAD_API_CALL int GLAD_GL_ARB_depth_texture;
#define GL_ARB_derivative_control 1
GLAD_API_CALL int GLAD_GL_ARB_derivative_control;
#define GL_ARB_direct_state_access 1
GLAD_API_CALL int GLAD_GL_ARB_direct_state_access;
#define GL_ARB_draw_buffers 1
GLAD_API_CALL int GLAD_GL_ARB_draw_buffers;
#define GL_ARB_draw_buffers_blend 1
GLAD_API_CALL int GLAD_GL_ARB_draw_buffers_blend;
#define GL_ARB_draw_elements_base_vertex 1
GLAD_API_CALL int GLAD_GL_ARB_draw_elements_base_vertex;
#define GL_ARB_draw_indirect 1
GLAD_API_CALL int GLAD_GL_ARB_draw_indirect;
#define GL_ARB_draw_instanced 1
GLAD_API_CALL int GLAD_GL_ARB_draw_instanced;
#define GL_ARB_enhanced_layouts 1
GLAD_API_CALL int GLAD_GL_ARB_enhanced_layouts;
#define GL_ARB_explicit_attrib_location 1
GLAD_API_CALL int GLAD_GL_ARB_explicit_attrib_location;
#define GL_ARB_explicit_uniform_location 1
GLAD_API_CALL int GLAD_GL_ARB_explicit_uniform_location;
#define GL_ARB_fragment_coord_conventions 1
GLAD_API_CALL int GLAD_GL_ARB_fragment_coord_conventions;
#define GL_ARB_fragment_layer_viewport 1
GLAD_API_CALL int GLAD_GL_ARB_fragment_layer_viewport;
#define GL_ARB_fragment_program 1
GLAD_API_CALL int GLAD_GL_ARB_fragment_program;
#define GL_ARB_fragment_program_shadow 1
GLAD_API_CALL int GLAD_GL_ARB_fragment_program_shadow;
#define GL_ARB_fragment_shader 1
GLAD_API_CALL int GLAD_GL_ARB_fragment_shader;
#define GL_ARB_fragment_shader_interlock 1
GLAD_API_CALL int GLAD_GL_ARB_fragment_shader_interlock;
#define GL_ARB_framebuffer_no_attachments 1
GLAD_API_CALL int GLAD_GL_ARB_framebuffer_no_attachments;
#define GL_ARB_framebuffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_framebuffer_object;
#define GL_ARB_framebuffer_sRGB 1
GLAD_API_CALL int GLAD_GL_ARB_framebuffer_sRGB;
#define GL_ARB_geometry_shader4 1
GLAD_API_CALL int GLAD_GL_ARB_geometry_shader4;
#define GL_ARB_get_program_binary 1
GLAD_API_CALL int GLAD_GL_ARB_get_program_binary;
#define GL_ARB_get_texture_sub_image 1
GLAD_API_CALL int GLAD_GL_ARB_get_texture_sub_image;
#define GL_ARB_gl_spirv 1
GLAD_API_CALL int GLAD_GL_ARB_gl_spirv;
#define GL_ARB_gpu_shader5 1
GLAD_API_CALL int GLAD_GL_ARB_gpu_shader5;
#define GL_ARB_gpu_shader_fp64 1
GLAD_API_CALL int GLAD_GL_ARB_gpu_shader_fp64;
#define GL_ARB_gpu_shader_int64 1
GLAD_API_CALL int GLAD_GL_ARB_gpu_shader_int64;
#define GL_ARB_half_float_pixel 1
GLAD_API_CALL int GLAD_GL_ARB_half_float_pixel;
#define GL_ARB_half_float_vertex 1
GLAD_API_CALL int GLAD_GL_ARB_half_float_vertex;
#define GL_ARB_imaging 1
GLAD_API_CALL int GLAD_GL_ARB_imaging;
#define GL_ARB_indirect_parameters 1
GLAD_API_CALL int GLAD_GL_ARB_indirect_parameters;
#define GL_ARB_instanced_arrays 1
GLAD_API_CALL int GLAD_GL_ARB_instanced_arrays;
#define GL_ARB_internalformat_query 1
GLAD_API_CALL int GLAD_GL_ARB_internalformat_query;
#define GL_ARB_internalformat_query2 1
GLAD_API_CALL int GLAD_GL_ARB_internalformat_query2;
#define GL_ARB_invalidate_subdata 1
GLAD_API_CALL int GLAD_GL_ARB_invalidate_subdata;
#define GL_ARB_map_buffer_alignment 1
GLAD_API_CALL int GLAD_GL_ARB_map_buffer_alignment;
#define GL_ARB_map_buffer_range 1
GLAD_API_CALL int GLAD_GL_ARB_map_buffer_range;
#define GL_ARB_matrix_palette 1
GLAD_API_CALL int GLAD_GL_ARB_matrix_palette;
#define GL_ARB_multi_bind 1
GLAD_API_CALL int GLAD_GL_ARB_multi_bind;
#define GL_ARB_multi_draw_indirect 1
GLAD_API_CALL int GLAD_GL_ARB_multi_draw_indirect;
#define GL_ARB_multisample 1
GLAD_API_CALL int GLAD_GL_ARB_multisample;
#define GL_ARB_multitexture 1
GLAD_API_CALL int GLAD_GL_ARB_multitexture;
#define GL_ARB_occlusion_query 1
GLAD_API_CALL int GLAD_GL_ARB_occlusion_query;
#define GL_ARB_occlusion_query2 1
GLAD_API_CALL int GLAD_GL_ARB_occlusion_query2;
#define GL_ARB_parallel_shader_compile 1
GLAD_API_CALL int GLAD_GL_ARB_parallel_shader_compile;
#define GL_ARB_pipeline_statistics_query 1
GLAD_API_CALL int GLAD_GL_ARB_pipeline_statistics_query;
#define GL_ARB_pixel_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_pixel_buffer_object;
#define GL_ARB_point_parameters 1
GLAD_API_CALL int GLAD_GL_ARB_point_parameters;
#define GL_ARB_point_sprite 1
GLAD_API_CALL int GLAD_GL_ARB_point_sprite;
#define GL_ARB_polygon_offset_clamp 1
GLAD_API_CALL int GLAD_GL_ARB_polygon_offset_clamp;
#define GL_ARB_post_depth_coverage 1
GLAD_API_CALL int GLAD_GL_ARB_post_depth_coverage;
#define GL_ARB_program_interface_query 1
GLAD_API_CALL int GLAD_GL_ARB_program_interface_query;
#define GL_ARB_provoking_vertex 1
GLAD_API_CALL int GLAD_GL_ARB_provoking_vertex;
#define GL_ARB_query_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_query_buffer_object;
#define GL_ARB_robust_buffer_access_behavior 1
GLAD_API_CALL int GLAD_GL_ARB_robust_buffer_access_behavior;
#define GL_ARB_robustness 1
GLAD_API_CALL int GLAD_GL_ARB_robustness;
#define GL_ARB_robustness_isolation 1
GLAD_API_CALL int GLAD_GL_ARB_robustness_isolation;
#define GL_ARB_sample_locations 1
GLAD_API_CALL int GLAD_GL_ARB_sample_locations;
#define GL_ARB_sample_shading 1
GLAD_API_CALL int GLAD_GL_ARB_sample_shading;
#define GL_ARB_sampler_objects 1
GLAD_API_CALL int GLAD_GL_ARB_sampler_objects;
#define GL_ARB_seamless_cube_map 1
GLAD_API_CALL int GLAD_GL_ARB_seamless_cube_map;
#define GL_ARB_seamless_cubemap_per_texture 1
GLAD_API_CALL int GLAD_GL_ARB_seamless_cubemap_per_texture;
#define GL_ARB_separate_shader_objects 1
GLAD_API_CALL int GLAD_GL_ARB_separate_shader_objects;
#define GL_ARB_shader_atomic_counter_ops 1
GLAD_API_CALL int GLAD_GL_ARB_shader_atomic_counter_ops;
#define GL_ARB_shader_atomic_counters 1
GLAD_API_CALL int GLAD_GL_ARB_shader_atomic_counters;
#define GL_ARB_shader_ballot 1
GLAD_API_CALL int GLAD_GL_ARB_shader_ballot;
#define GL_ARB_shader_bit_encoding 1
GLAD_API_CALL int GLAD_GL_ARB_shader_bit_encoding;
#define GL_ARB_shader_clock 1
GLAD_API_CALL int GLAD_GL_ARB_shader_clock;
#define GL_ARB_shader_draw_parameters 1
GLAD_API_CALL int GLAD_GL_ARB_shader_draw_parameters;
#define GL_ARB_shader_group_vote 1
GLAD_API_CALL int GLAD_GL_ARB_shader_group_vote;
#define GL_ARB_shader_image_load_store 1
GLAD_API_CALL int GLAD_GL_ARB_shader_image_load_store;
#define GL_ARB_shader_image_size 1
GLAD_API_CALL int GLAD_GL_ARB_shader_image_size;
#define GL_ARB_shader_objects 1
GLAD_API_CALL int GLAD_GL_ARB_shader_objects;
#define GL_ARB_shader_precision 1
GLAD_API_CALL int GLAD_GL_ARB_shader_precision;
#define GL_ARB_shader_stencil_export 1
GLAD_API_CALL int GLAD_GL_ARB_shader_stencil_export;
#define GL_ARB_shader_storage_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_shader_storage_buffer_object;
#define GL_ARB_shader_subroutine 1
GLAD_API_CALL int GLAD_GL_ARB_shader_subroutine;
#define GL_ARB_shader_texture_image_samples 1
GLAD_API_CALL int GLAD_GL_ARB_shader_texture_image_samples;
#define GL_ARB_shader_texture_lod 1
GLAD_API_CALL int GLAD_GL_ARB_shader_texture_lod;
#define GL_ARB_shader_viewport_layer_array 1
GLAD_API_CALL int GLAD_GL_ARB_shader_viewport_layer_array;
#define GL_ARB_shading_language_100 1
GLAD_API_CALL int GLAD_GL_ARB_shading_language_100;
#define GL_ARB_shading_language_420pack 1
GLAD_API_CALL int GLAD_GL_ARB_shading_language_420pack;
#define GL_ARB_shading_language_include 1
GLAD_API_CALL int GLAD_GL_ARB_shading_language_include;
#define GL_ARB_shading_language_packing 1
GLAD_API_CALL int GLAD_GL_ARB_shading_language_packing;
#define GL_ARB_shadow 1
GLAD_API_CALL int GLAD_GL_ARB_shadow;
#define GL_ARB_shadow_ambient 1
GLAD_API_CALL int GLAD_GL_ARB_shadow_ambient;
#define GL_ARB_sparse_buffer 1
GLAD_API_CALL int GLAD_GL_ARB_sparse_buffer;
#define GL_ARB_sparse_texture 1
GLAD_API_CALL int GLAD_GL_ARB_sparse_texture;
#define GL_ARB_sparse_texture2 1
GLAD_API_CALL int GLAD_GL_ARB_sparse_texture2;
#define GL_ARB_sparse_texture_clamp 1
GLAD_API_CALL int GLAD_GL_ARB_sparse_texture_clamp;
#define GL_ARB_spirv_extensions 1
GLAD_API_CALL int GLAD_GL_ARB_spirv_extensions;
#define GL_ARB_stencil_texturing 1
GLAD_API_CALL int GLAD_GL_ARB_stencil_texturing;
#define GL_ARB_sync 1
GLAD_API_CALL int GLAD_GL_ARB_sync;
#define GL_ARB_tessellation_shader 1
GLAD_API_CALL int GLAD_GL_ARB_tessellation_shader;
#define GL_ARB_texture_barrier 1
GLAD_API_CALL int GLAD_GL_ARB_texture_barrier;
#define GL_ARB_texture_border_clamp 1
GLAD_API_CALL int GLAD_GL_ARB_texture_border_clamp;
#define GL_ARB_texture_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_texture_buffer_object;
#define GL_ARB_texture_buffer_object_rgb32 1
GLAD_API_CALL int GLAD_GL_ARB_texture_buffer_object_rgb32;
#define GL_ARB_texture_buffer_range 1
GLAD_API_CALL int GLAD_GL_ARB_texture_buffer_range;
#define GL_ARB_texture_compression 1
GLAD_API_CALL int GLAD_GL_ARB_texture_compression;
#define GL_ARB_texture_compression_bptc 1
GLAD_API_CALL int GLAD_GL_ARB_texture_compression_bptc;
#define GL_ARB_texture_compression_rgtc 1
GLAD_API_CALL int GLAD_GL_ARB_texture_compression_rgtc;
#define GL_ARB_texture_cube_map 1
GLAD_API_CALL int GLAD_GL_ARB_texture_cube_map;
#define GL_ARB_texture_cube_map_array 1
GLAD_API_CALL int GLAD_GL_ARB_texture_cube_map_array;
#define GL_ARB_texture_env_add 1
GLAD_API_CALL int GLAD_GL_ARB_texture_env_add;
#define GL_ARB_texture_env_combine 1
GLAD_API_CALL int GLAD_GL_ARB_texture_env_combine;
#define GL_ARB_texture_env_crossbar 1
GLAD_API_CALL int GLAD_GL_ARB_texture_env_crossbar;
#define GL_ARB_texture_env_dot3 1
GLAD_API_CALL int GLAD_GL_ARB_texture_env_dot3;
#define GL_ARB_texture_filter_anisotropic 1
GLAD_API_CALL int GLAD_GL_ARB_texture_filter_anisotropic;
#define GL_ARB_texture_filter_minmax 1
GLAD_API_CALL int GLAD_GL_ARB_texture_filter_minmax;
#define GL_ARB_texture_float 1
GLAD_API_CALL int GLAD_GL_ARB_texture_float;
#define GL_ARB_texture_gather 1
GLAD_API_CALL int GLAD_GL_ARB_texture_gather;
#define GL_ARB_texture_mirror_clamp_to_edge 1
GLAD_API_CALL int GLAD_GL_ARB_texture_mirror_clamp_to_edge;
#define GL_ARB_texture_mirrored_repeat 1
GLAD_API_CALL int GLAD_GL_ARB_texture_mirrored_repeat;
#define GL_ARB_texture_multisample 1
GLAD_API_CALL int GLAD_GL_ARB_texture_multisample;
#define GL_ARB_texture_non_power_of_two 1
GLAD_API_CALL int GLAD_GL_ARB_texture_non_power_of_two;
#define GL_ARB_texture_query_levels 1
GLAD_API_CALL int GLAD_GL_ARB_texture_query_levels;
#define GL_ARB_texture_query_lod 1
GLAD_API_CALL int GLAD_GL_ARB_texture_query_lod;
#define GL_ARB_texture_rectangle 1
GLAD_API_CALL int GLAD_GL_ARB_texture_rectangle;
#define GL_ARB_texture_rg 1
GLAD_API_CALL int GLAD_GL_ARB_texture_rg;
#define GL_ARB_texture_rgb10_a2ui 1
GLAD_API_CALL int GLAD_GL_ARB_texture_rgb10_a2ui;
#define GL_ARB_texture_stencil8 1
GLAD_API_CALL int GLAD_GL_ARB_texture_stencil8;
#define GL_ARB_texture_storage 1
GLAD_API_CALL int GLAD_GL_ARB_texture_storage;
#define GL_ARB_texture_storage_multisample 1
GLAD_API_CALL int GLAD_GL_ARB_texture_storage_multisample;
#define GL_ARB_texture_swizzle 1
GLAD_API_CALL int GLAD_GL_ARB_texture_swizzle;
#define GL_ARB_texture_view 1
GLAD_API_CALL int GLAD_GL_ARB_texture_view;
#define GL_ARB_timer_query 1
GLAD_API_CALL int GLAD_GL_ARB_timer_query;
#define GL_ARB_transform_feedback2 1
GLAD_API_CALL int GLAD_GL_ARB_transform_feedback2;
#define GL_ARB_transform_feedback3 1
GLAD_API_CALL int GLAD_GL_ARB_transform_feedback3;
#define GL_ARB_transform_feedback_instanced 1
GLAD_API_CALL int GLAD_GL_ARB_transform_feedback_instanced;
#define GL_ARB_transform_feedback_overflow_query 1
GLAD_API_CALL int GLAD_GL_ARB_transform_feedback_overflow_query;
#define GL_ARB_transpose_matrix 1
GLAD_API_CALL int GLAD_GL_ARB_transpose_matrix;
#define GL_ARB_uniform_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_uniform_buffer_object;
#define GL_ARB_vertex_array_bgra 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_array_bgra;
#define GL_ARB_vertex_array_object 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_array_object;
#define GL_ARB_vertex_attrib_64bit 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_attrib_64bit;
#define GL_ARB_vertex_attrib_binding 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_attrib_binding;
#define GL_ARB_vertex_blend 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_blend;
#define GL_ARB_vertex_buffer_object 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_buffer_object;
#define GL_ARB_vertex_program 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_program;
#define GL_ARB_vertex_shader 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_shader;
#define GL_ARB_vertex_type_10f_11f_11f_rev 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_type_10f_11f_11f_rev;
#define GL_ARB_vertex_type_2_10_10_10_rev 1
GLAD_API_CALL int GLAD_GL_ARB_vertex_type_2_10_10_10_rev;
#define GL_ARB_viewport_array 1
GLAD_API_CALL int GLAD_GL_ARB_viewport_array;
#define GL_ARB_window_pos 1
GLAD_API_CALL int GLAD_GL_ARB_window_pos;
#define GL_KHR_blend_equation_advanced 1
GLAD_API_CALL int GLAD_GL_KHR_blend_equation_advanced;
#define GL_KHR_blend_equation_advanced_coherent 1
GLAD_API_CALL int GLAD_GL_KHR_blend_equation_advanced_coherent;
#define GL_KHR_context_flush_control 1
GLAD_API_CALL int GLAD_GL_KHR_context_flush_control;
#define GL_KHR_debug 1
GLAD_API_CALL int GLAD_GL_KHR_debug;
#define GL_KHR_no_error 1
GLAD_API_CALL int GLAD_GL_KHR_no_error;
#define GL_KHR_parallel_shader_compile 1
GLAD_API_CALL int GLAD_GL_KHR_parallel_shader_compile;
#define GL_KHR_robust_buffer_access_behavior 1
GLAD_API_CALL int GLAD_GL_KHR_robust_buffer_access_behavior;
#define GL_KHR_robustness 1
GLAD_API_CALL int GLAD_GL_KHR_robustness;
#define GL_KHR_shader_subgroup 1
GLAD_API_CALL int GLAD_GL_KHR_shader_subgroup;
#define GL_KHR_texture_compression_astc_hdr 1
GLAD_API_CALL int GLAD_GL_KHR_texture_compression_astc_hdr;
#define GL_KHR_texture_compression_astc_ldr 1
GLAD_API_CALL int GLAD_GL_KHR_texture_compression_astc_ldr;
#define GL_KHR_texture_compression_astc_sliced_3d 1
GLAD_API_CALL int GLAD_GL_KHR_texture_compression_astc_sliced_3d;


typedef void (GLAD_API_PTR *PFNGLACCUMPROC)(GLenum op, GLfloat value);
typedef void (GLAD_API_PTR *PFNGLACTIVESHADERPROGRAMPROC)(GLuint pipeline, GLuint program);
typedef void (GLAD_API_PTR *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (GLAD_API_PTR *PFNGLACTIVETEXTUREARBPROC)(GLenum texture);
typedef void (GLAD_API_PTR *PFNGLALPHAFUNCPROC)(GLenum func, GLfloat ref);
typedef GLboolean (GLAD_API_PTR *PFNGLARETEXTURESRESIDENTPROC)(GLsizei n, const GLuint * textures, GLboolean * residences);
typedef void (GLAD_API_PTR *PFNGLARRAYELEMENTPROC)(GLint i);
typedef void (GLAD_API_PTR *PFNGLATTACHOBJECTARBPROC)(GLhandleARB containerObj, GLhandleARB obj);
typedef void (GLAD_API_PTR *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAD_API_PTR *PFNGLBEGINPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLBEGINCONDITIONALRENDERPROC)(GLuint id, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLBEGINQUERYPROC)(GLenum target, GLuint id);
typedef void (GLAD_API_PTR *PFNGLBEGINQUERYARBPROC)(GLenum target, GLuint id);
typedef void (GLAD_API_PTR *PFNGLBEGINQUERYINDEXEDPROC)(GLenum target, GLuint index, GLuint id);
typedef void (GLAD_API_PTR *PFNGLBEGINTRANSFORMFEEDBACKPROC)(GLenum primitiveMode);
typedef void (GLAD_API_PTR *PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLBINDATTRIBLOCATIONARBPROC)(GLhandleARB programObj, GLuint index, const GLcharARB * name);
typedef void (GLAD_API_PTR *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLBINDBUFFERARBPROC)(GLenum target, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLBINDBUFFERRANGEPROC)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAD_API_PTR *PFNGLBINDBUFFERSBASEPROC)(GLenum target, GLuint first, GLsizei count, const GLuint * buffers);
typedef void (GLAD_API_PTR *PFNGLBINDBUFFERSRANGEPROC)(GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes);
typedef void (GLAD_API_PTR *PFNGLBINDFRAGDATALOCATIONPROC)(GLuint program, GLuint color, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (GLAD_API_PTR *PFNGLBINDIMAGETEXTUREPROC)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GLAD_API_PTR *PFNGLBINDIMAGETEXTURESPROC)(GLuint first, GLsizei count, const GLuint * textures);
typedef void (GLAD_API_PTR *PFNGLBINDPROGRAMARBPROC)(GLenum target, GLuint program);
typedef void (GLAD_API_PTR *PFNGLBINDPROGRAMPIPELINEPROC)(GLuint pipeline);
typedef void (GLAD_API_PTR *PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void (GLAD_API_PTR *PFNGLBINDSAMPLERPROC)(GLuint unit, GLuint sampler);
typedef void (GLAD_API_PTR *PFNGLBINDSAMPLERSPROC)(GLuint first, GLsizei count, const GLuint * samplers);
typedef void (GLAD_API_PTR *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (GLAD_API_PTR *PFNGLBINDTEXTUREUNITPROC)(GLuint unit, GLuint texture);
typedef void (GLAD_API_PTR *PFNGLBINDTEXTURESPROC)(GLuint first, GLsizei count, const GLuint * textures);
typedef void (GLAD_API_PTR *PFNGLBINDTRANSFORMFEEDBACKPROC)(GLenum target, GLuint id);
typedef void (GLAD_API_PTR *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (GLAD_API_PTR *PFNGLBINDVERTEXBUFFERPROC)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLBINDVERTEXBUFFERSPROC)(GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides);
typedef void (GLAD_API_PTR *PFNGLBITMAPPROC)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);
typedef void (GLAD_API_PTR *PFNGLBLENDBARRIERPROC)(void);
typedef void (GLAD_API_PTR *PFNGLBLENDBARRIERKHRPROC)(void);
typedef void (GLAD_API_PTR *PFNGLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONSEPARATEIPROC)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONSEPARATEIARBPROC)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONIPROC)(GLuint buf, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONIARBPROC)(GLuint buf, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (GLAD_API_PTR *PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAD_API_PTR *PFNGLBLENDFUNCSEPARATEIPROC)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAD_API_PTR *PFNGLBLENDFUNCSEPARATEIARBPROC)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAD_API_PTR *PFNGLBLENDFUNCIPROC)(GLuint buf, GLenum src, GLenum dst);
typedef void (GLAD_API_PTR *PFNGLBLENDFUNCIARBPROC)(GLuint buf, GLenum src, GLenum dst);
typedef void (GLAD_API_PTR *PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLAD_API_PTR *PFNGLBLITNAMEDFRAMEBUFFERPROC)(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLAD_API_PTR *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
typedef void (GLAD_API_PTR *PFNGLBUFFERDATAARBPROC)(GLenum target, GLsizeiptrARB size, const void * data, GLenum usage);
typedef void (GLAD_API_PTR *PFNGLBUFFERPAGECOMMITMENTARBPROC)(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);
typedef void (GLAD_API_PTR *PFNGLBUFFERSTORAGEPROC)(GLenum target, GLsizeiptr size, const void * data, GLbitfield flags);
typedef void (GLAD_API_PTR *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
typedef void (GLAD_API_PTR *PFNGLBUFFERSUBDATAARBPROC)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void * data);
typedef void (GLAD_API_PTR *PFNGLCALLLISTPROC)(GLuint list);
typedef void (GLAD_API_PTR *PFNGLCALLLISTSPROC)(GLsizei n, GLenum type, const void * lists);
typedef GLenum (GLAD_API_PTR *PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef GLenum (GLAD_API_PTR *PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC)(GLuint framebuffer, GLenum target);
typedef void (GLAD_API_PTR *PFNGLCLAMPCOLORPROC)(GLenum target, GLenum clamp);
typedef void (GLAD_API_PTR *PFNGLCLAMPCOLORARBPROC)(GLenum target, GLenum clamp);
typedef void (GLAD_API_PTR *PFNGLCLEARPROC)(GLbitfield mask);
typedef void (GLAD_API_PTR *PFNGLCLEARACCUMPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERDATAPROC)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERSUBDATAPROC)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERFIPROC)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERFVPROC)(GLenum buffer, GLint drawbuffer, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERIVPROC)(GLenum buffer, GLint drawbuffer, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERUIVPROC)(GLenum buffer, GLint drawbuffer, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAD_API_PTR *PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (GLAD_API_PTR *PFNGLCLEARDEPTHFPROC)(GLfloat d);
typedef void (GLAD_API_PTR *PFNGLCLEARINDEXPROC)(GLfloat c);
typedef void (GLAD_API_PTR *PFNGLCLEARNAMEDBUFFERDATAPROC)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCLEARNAMEDBUFFERSUBDATAPROC)(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCLEARNAMEDFRAMEBUFFERFIPROC)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef void (GLAD_API_PTR *PFNGLCLEARNAMEDFRAMEBUFFERFVPROC)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLCLEARNAMEDFRAMEBUFFERIVPROC)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLCLEARSTENCILPROC)(GLint s);
typedef void (GLAD_API_PTR *PFNGLCLEARTEXIMAGEPROC)(GLuint texture, GLint level, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCLEARTEXSUBIMAGEPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCLIENTACTIVETEXTUREPROC)(GLenum texture);
typedef void (GLAD_API_PTR *PFNGLCLIENTACTIVETEXTUREARBPROC)(GLenum texture);
typedef GLenum (GLAD_API_PTR *PFNGLCLIENTWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (GLAD_API_PTR *PFNGLCLIPCONTROLPROC)(GLenum origin, GLenum depth);
typedef void (GLAD_API_PTR *PFNGLCLIPPLANEPROC)(GLenum plane, const GLdouble * equation);
typedef void (GLAD_API_PTR *PFNGLCOLOR3BPROC)(GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3BVPROC)(const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3DPROC)(GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3FPROC)(GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3IPROC)(GLint red, GLint green, GLint blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3SPROC)(GLshort red, GLshort green, GLshort blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3UBPROC)(GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3UBVPROC)(const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3UIPROC)(GLuint red, GLuint green, GLuint blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3UIVPROC)(const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR3USPROC)(GLushort red, GLushort green, GLushort blue);
typedef void (GLAD_API_PTR *PFNGLCOLOR3USVPROC)(const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4BPROC)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4BVPROC)(const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4DPROC)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4FPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4IPROC)(GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4SPROC)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4UBPROC)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4UBVPROC)(const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4UIPROC)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4UIVPROC)(const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLCOLOR4USPROC)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
typedef void (GLAD_API_PTR *PFNGLCOLOR4USVPROC)(const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (GLAD_API_PTR *PFNGLCOLORMASKIPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAD_API_PTR *PFNGLCOLORMATERIALPROC)(GLenum face, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLCOLORP3UIPROC)(GLenum type, GLuint color);
typedef void (GLAD_API_PTR *PFNGLCOLORP3UIVPROC)(GLenum type, const GLuint * color);
typedef void (GLAD_API_PTR *PFNGLCOLORP4UIPROC)(GLenum type, GLuint color);
typedef void (GLAD_API_PTR *PFNGLCOLORP4UIVPROC)(GLenum type, const GLuint * color);
typedef void (GLAD_API_PTR *PFNGLCOLORPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLCOLORSUBTABLEPROC)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOLORTABLEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * table);
typedef void (GLAD_API_PTR *PFNGLCOLORTABLEPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLCOLORTABLEPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (GLAD_API_PTR *PFNGLCOMPILESHADERARBPROC)(GLhandleARB shaderObj);
typedef void (GLAD_API_PTR *PFNGLCOMPILESHADERINCLUDEARBPROC)(GLuint shader, GLsizei count, const GLchar *const* path, const GLint * length);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE1DARBPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE3DARBPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAD_API_PTR *PFNGLCONVOLUTIONFILTER1DPROC)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * image);
typedef void (GLAD_API_PTR *PFNGLCONVOLUTIONFILTER2DPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * image);
typedef void (GLAD_API_PTR *PFNGLCONVOLUTIONPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat params);
typedef void (GLAD_API_PTR *PFNGLCONVOLUTIONPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLCONVOLUTIONPARAMETERIPROC)(GLenum target, GLenum pname, GLint params);
typedef void (GLAD_API_PTR *PFNGLCONVOLUTIONPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLCOPYBUFFERSUBDATAPROC)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAD_API_PTR *PFNGLCOPYCOLORSUBTABLEPROC)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLCOPYCOLORTABLEPROC)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLCOPYCONVOLUTIONFILTER1DPROC)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLCOPYCONVOLUTIONFILTER2DPROC)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLCOPYIMAGESUBDATAPROC)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
typedef void (GLAD_API_PTR *PFNGLCOPYNAMEDBUFFERSUBDATAPROC)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAD_API_PTR *PFNGLCOPYPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXTURESUBIMAGE1DPROC)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXTURESUBIMAGE2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLCOPYTEXTURESUBIMAGE3DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLCREATEBUFFERSPROC)(GLsizei n, GLuint * buffers);
typedef void (GLAD_API_PTR *PFNGLCREATEFRAMEBUFFERSPROC)(GLsizei n, GLuint * framebuffers);
typedef GLuint (GLAD_API_PTR *PFNGLCREATEPROGRAMPROC)(void);
typedef GLhandleARB (GLAD_API_PTR *PFNGLCREATEPROGRAMOBJECTARBPROC)(void);
typedef void (GLAD_API_PTR *PFNGLCREATEPROGRAMPIPELINESPROC)(GLsizei n, GLuint * pipelines);
typedef void (GLAD_API_PTR *PFNGLCREATEQUERIESPROC)(GLenum target, GLsizei n, GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLCREATERENDERBUFFERSPROC)(GLsizei n, GLuint * renderbuffers);
typedef void (GLAD_API_PTR *PFNGLCREATESAMPLERSPROC)(GLsizei n, GLuint * samplers);
typedef GLuint (GLAD_API_PTR *PFNGLCREATESHADERPROC)(GLenum type);
typedef GLhandleARB (GLAD_API_PTR *PFNGLCREATESHADEROBJECTARBPROC)(GLenum shaderType);
typedef GLuint (GLAD_API_PTR *PFNGLCREATESHADERPROGRAMVPROC)(GLenum type, GLsizei count, const GLchar *const* strings);
typedef GLsync (GLAD_API_PTR *PFNGLCREATESYNCFROMCLEVENTARBPROC)(struct _cl_context * context, struct _cl_event * event, GLbitfield flags);
typedef void (GLAD_API_PTR *PFNGLCREATETEXTURESPROC)(GLenum target, GLsizei n, GLuint * textures);
typedef void (GLAD_API_PTR *PFNGLCREATETRANSFORMFEEDBACKSPROC)(GLsizei n, GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLCREATEVERTEXARRAYSPROC)(GLsizei n, GLuint * arrays);
typedef void (GLAD_API_PTR *PFNGLCULLFACEPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLCURRENTPALETTEMATRIXARBPROC)(GLint index);
typedef void (GLAD_API_PTR *PFNGLDEBUGMESSAGECALLBACKPROC)(GLDEBUGPROC callback, const void * userParam);
typedef void (GLAD_API_PTR *PFNGLDEBUGMESSAGECALLBACKARBPROC)(GLDEBUGPROCARB callback, const void * userParam);
typedef void (GLAD_API_PTR *PFNGLDEBUGMESSAGECONTROLPROC)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
typedef void (GLAD_API_PTR *PFNGLDEBUGMESSAGECONTROLARBPROC)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
typedef void (GLAD_API_PTR *PFNGLDEBUGMESSAGEINSERTPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf);
typedef void (GLAD_API_PTR *PFNGLDEBUGMESSAGEINSERTARBPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf);
typedef void (GLAD_API_PTR *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint * buffers);
typedef void (GLAD_API_PTR *PFNGLDELETEBUFFERSARBPROC)(GLsizei n, const GLuint * buffers);
typedef void (GLAD_API_PTR *PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint * framebuffers);
typedef void (GLAD_API_PTR *PFNGLDELETELISTSPROC)(GLuint list, GLsizei range);
typedef void (GLAD_API_PTR *PFNGLDELETENAMEDSTRINGARBPROC)(GLint namelen, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLDELETEOBJECTARBPROC)(GLhandleARB obj);
typedef void (GLAD_API_PTR *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (GLAD_API_PTR *PFNGLDELETEPROGRAMPIPELINESPROC)(GLsizei n, const GLuint * pipelines);
typedef void (GLAD_API_PTR *PFNGLDELETEPROGRAMSARBPROC)(GLsizei n, const GLuint * programs);
typedef void (GLAD_API_PTR *PFNGLDELETEQUERIESPROC)(GLsizei n, const GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLDELETEQUERIESARBPROC)(GLsizei n, const GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint * renderbuffers);
typedef void (GLAD_API_PTR *PFNGLDELETESAMPLERSPROC)(GLsizei count, const GLuint * samplers);
typedef void (GLAD_API_PTR *PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (GLAD_API_PTR *PFNGLDELETESYNCPROC)(GLsync sync);
typedef void (GLAD_API_PTR *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint * textures);
typedef void (GLAD_API_PTR *PFNGLDELETETRANSFORMFEEDBACKSPROC)(GLsizei n, const GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint * arrays);
typedef void (GLAD_API_PTR *PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (GLAD_API_PTR *PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEARRAYDVNVPROC)(GLuint first, GLsizei count, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEARRAYVPROC)(GLuint first, GLsizei count, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEINDEXEDPROC)(GLuint index, GLdouble n, GLdouble f);
typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEINDEXEDDNVPROC)(GLuint index, GLdouble n, GLdouble f);
typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEFPROC)(GLfloat n, GLfloat f);
typedef void (GLAD_API_PTR *PFNGLDETACHOBJECTARBPROC)(GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (GLAD_API_PTR *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAD_API_PTR *PFNGLDISABLEPROC)(GLenum cap);
typedef void (GLAD_API_PTR *PFNGLDISABLECLIENTSTATEPROC)(GLenum array);
typedef void (GLAD_API_PTR *PFNGLDISABLEVERTEXARRAYATTRIBPROC)(GLuint vaobj, GLuint index);
typedef void (GLAD_API_PTR *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (GLAD_API_PTR *PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)(GLuint index);
typedef void (GLAD_API_PTR *PFNGLDISABLEIPROC)(GLenum target, GLuint index);
typedef void (GLAD_API_PTR *PFNGLDISPATCHCOMPUTEPROC)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (GLAD_API_PTR *PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
typedef void (GLAD_API_PTR *PFNGLDISPATCHCOMPUTEINDIRECTPROC)(GLintptr indirect);
typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSINDIRECTPROC)(GLenum mode, const void * indirect);
typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSINSTANCEDARBPROC)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
typedef void (GLAD_API_PTR *PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (GLAD_API_PTR *PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum * bufs);
typedef void (GLAD_API_PTR *PFNGLDRAWBUFFERSARBPROC)(GLsizei n, const GLenum * bufs);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINDIRECTPROC)(GLenum mode, GLenum type, const void * indirect);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDARBPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLuint baseinstance);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex);
typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
typedef void (GLAD_API_PTR *PFNGLDRAWPIXELSPROC)(GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices);
typedef void (GLAD_API_PTR *PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex);
typedef void (GLAD_API_PTR *PFNGLDRAWTRANSFORMFEEDBACKPROC)(GLenum mode, GLuint id);
typedef void (GLAD_API_PTR *PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)(GLenum mode, GLuint id, GLsizei instancecount);
typedef void (GLAD_API_PTR *PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)(GLenum mode, GLuint id, GLuint stream);
typedef void (GLAD_API_PTR *PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
typedef void (GLAD_API_PTR *PFNGLEDGEFLAGPROC)(GLboolean flag);
typedef void (GLAD_API_PTR *PFNGLEDGEFLAGPOINTERPROC)(GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLEDGEFLAGVPROC)(const GLboolean * flag);
typedef void (GLAD_API_PTR *PFNGLENABLEPROC)(GLenum cap);
typedef void (GLAD_API_PTR *PFNGLENABLECLIENTSTATEPROC)(GLenum array);
typedef void (GLAD_API_PTR *PFNGLENABLEVERTEXARRAYATTRIBPROC)(GLuint vaobj, GLuint index);
typedef void (GLAD_API_PTR *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (GLAD_API_PTR *PFNGLENABLEVERTEXATTRIBARRAYARBPROC)(GLuint index);
typedef void (GLAD_API_PTR *PFNGLENABLEIPROC)(GLenum target, GLuint index);
typedef void (GLAD_API_PTR *PFNGLENDPROC)(void);
typedef void (GLAD_API_PTR *PFNGLENDCONDITIONALRENDERPROC)(void);
typedef void (GLAD_API_PTR *PFNGLENDLISTPROC)(void);
typedef void (GLAD_API_PTR *PFNGLENDQUERYPROC)(GLenum target);
typedef void (GLAD_API_PTR *PFNGLENDQUERYARBPROC)(GLenum target);
typedef void (GLAD_API_PTR *PFNGLENDQUERYINDEXEDPROC)(GLenum target, GLuint index);
typedef void (GLAD_API_PTR *PFNGLENDTRANSFORMFEEDBACKPROC)(void);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD1DPROC)(GLdouble u);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD1DVPROC)(const GLdouble * u);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD1FPROC)(GLfloat u);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD1FVPROC)(const GLfloat * u);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD2DPROC)(GLdouble u, GLdouble v);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD2DVPROC)(const GLdouble * u);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD2FPROC)(GLfloat u, GLfloat v);
typedef void (GLAD_API_PTR *PFNGLEVALCOORD2FVPROC)(const GLfloat * u);
typedef void (GLAD_API_PTR *PFNGLEVALMESH1PROC)(GLenum mode, GLint i1, GLint i2);
typedef void (GLAD_API_PTR *PFNGLEVALMESH2PROC)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef void (GLAD_API_PTR *PFNGLEVALPOINT1PROC)(GLint i);
typedef void (GLAD_API_PTR *PFNGLEVALPOINT2PROC)(GLint i, GLint j);
typedef void (GLAD_API_PTR *PFNGLEVALUATEDEPTHVALUESARBPROC)(void);
typedef void (GLAD_API_PTR *PFNGLFEEDBACKBUFFERPROC)(GLsizei size, GLenum type, GLfloat * buffer);
typedef GLsync (GLAD_API_PTR *PFNGLFENCESYNCPROC)(GLenum condition, GLbitfield flags);
typedef void (GLAD_API_PTR *PFNGLFINISHPROC)(void);
typedef void (GLAD_API_PTR *PFNGLFLUSHPROC)(void);
typedef void (GLAD_API_PTR *PFNGLFLUSHMAPPEDBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length);
typedef void (GLAD_API_PTR *PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC)(GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAD_API_PTR *PFNGLFOGCOORDPOINTERPROC)(GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLFOGCOORDDPROC)(GLdouble coord);
typedef void (GLAD_API_PTR *PFNGLFOGCOORDDVPROC)(const GLdouble * coord);
typedef void (GLAD_API_PTR *PFNGLFOGCOORDFPROC)(GLfloat coord);
typedef void (GLAD_API_PTR *PFNGLFOGCOORDFVPROC)(const GLfloat * coord);
typedef void (GLAD_API_PTR *PFNGLFOGFPROC)(GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLFOGFVPROC)(GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLFOGIPROC)(GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLFOGIVPROC)(GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERSAMPLELOCATIONSFVARBPROC)(GLenum target, GLuint start, GLsizei count, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURE1DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURE3DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTUREARBPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTUREFACEARBPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURELAYERPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURELAYERARBPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAD_API_PTR *PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLFRUSTUMPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAD_API_PTR *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint * buffers);
typedef void (GLAD_API_PTR *PFNGLGENBUFFERSARBPROC)(GLsizei n, GLuint * buffers);
typedef void (GLAD_API_PTR *PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint * framebuffers);
typedef GLuint (GLAD_API_PTR *PFNGLGENLISTSPROC)(GLsizei range);
typedef void (GLAD_API_PTR *PFNGLGENPROGRAMPIPELINESPROC)(GLsizei n, GLuint * pipelines);
typedef void (GLAD_API_PTR *PFNGLGENPROGRAMSARBPROC)(GLsizei n, GLuint * programs);
typedef void (GLAD_API_PTR *PFNGLGENQUERIESPROC)(GLsizei n, GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLGENQUERIESARBPROC)(GLsizei n, GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint * renderbuffers);
typedef void (GLAD_API_PTR *PFNGLGENSAMPLERSPROC)(GLsizei count, GLuint * samplers);
typedef void (GLAD_API_PTR *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint * textures);
typedef void (GLAD_API_PTR *PFNGLGENTRANSFORMFEEDBACKSPROC)(GLsizei n, GLuint * ids);
typedef void (GLAD_API_PTR *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint * arrays);
typedef void (GLAD_API_PTR *PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (GLAD_API_PTR *PFNGLGENERATETEXTUREMIPMAPPROC)(GLuint texture);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)(GLuint program, GLuint bufferIndex, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEATTRIBARBPROC)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);
typedef void (GLAD_API_PTR *PFNGLGETACTIVESUBROUTINENAMEPROC)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMARBPROC)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMBLOCKIVPROC)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMNAMEPROC)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName);
typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMSIVPROC)(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETATTACHEDOBJECTSARBPROC)(GLhandleARB containerObj, GLsizei maxCount, GLsizei * count, GLhandleARB * obj);
typedef void (GLAD_API_PTR *PFNGLGETATTACHEDSHADERSPROC)(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders);
typedef GLint (GLAD_API_PTR *PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar * name);
typedef GLint (GLAD_API_PTR *PFNGLGETATTRIBLOCATIONARBPROC)(GLhandleARB programObj, const GLcharARB * name);
typedef void (GLAD_API_PTR *PFNGLGETBOOLEANI_VPROC)(GLenum target, GLuint index, GLboolean * data);
typedef void (GLAD_API_PTR *PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean * data);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERPARAMETERI64VPROC)(GLenum target, GLenum pname, GLint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERPARAMETERIVARBPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERPOINTERVPROC)(GLenum target, GLenum pname, void ** params);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERPOINTERVARBPROC)(GLenum target, GLenum pname, void ** params);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, void * data);
typedef void (GLAD_API_PTR *PFNGLGETBUFFERSUBDATAARBPROC)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, void * data);
typedef void (GLAD_API_PTR *PFNGLGETCLIPPLANEPROC)(GLenum plane, GLdouble * equation);
typedef void (GLAD_API_PTR *PFNGLGETCOLORTABLEPROC)(GLenum target, GLenum format, GLenum type, void * table);
typedef void (GLAD_API_PTR *PFNGLGETCOLORTABLEPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETCOLORTABLEPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum target, GLint level, void * img);
typedef void (GLAD_API_PTR *PFNGLGETCOMPRESSEDTEXIMAGEARBPROC)(GLenum target, GLint level, void * img);
typedef void (GLAD_API_PTR *PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC)(GLuint texture, GLint level, GLsizei bufSize, void * pixels);
typedef void (GLAD_API_PTR *PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void * pixels);
typedef void (GLAD_API_PTR *PFNGLGETCONVOLUTIONFILTERPROC)(GLenum target, GLenum format, GLenum type, void * image);
typedef void (GLAD_API_PTR *PFNGLGETCONVOLUTIONPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETCONVOLUTIONPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef GLuint (GLAD_API_PTR *PFNGLGETDEBUGMESSAGELOGPROC)(GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
typedef GLuint (GLAD_API_PTR *PFNGLGETDEBUGMESSAGELOGARBPROC)(GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
typedef void (GLAD_API_PTR *PFNGLGETDOUBLEI_VPROC)(GLenum target, GLuint index, GLdouble * data);
typedef void (GLAD_API_PTR *PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble * data);
typedef GLenum (GLAD_API_PTR *PFNGLGETERRORPROC)(void);
typedef void (GLAD_API_PTR *PFNGLGETFLOATI_VPROC)(GLenum target, GLuint index, GLfloat * data);
typedef void (GLAD_API_PTR *PFNGLGETFLOATVPROC)(GLenum pname, GLfloat * data);
typedef GLint (GLAD_API_PTR *PFNGLGETFRAGDATAINDEXPROC)(GLuint program, const GLchar * name);
typedef GLint (GLAD_API_PTR *PFNGLGETFRAGDATALOCATIONPROC)(GLuint program, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum target, GLenum attachment, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETFRAMEBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef GLenum (GLAD_API_PTR *PFNGLGETGRAPHICSRESETSTATUSPROC)(void);
typedef GLenum (GLAD_API_PTR *PFNGLGETGRAPHICSRESETSTATUSARBPROC)(void);
typedef GLhandleARB (GLAD_API_PTR *PFNGLGETHANDLEARBPROC)(GLenum pname);
typedef void (GLAD_API_PTR *PFNGLGETHISTOGRAMPROC)(GLenum target, GLboolean reset, GLenum format, GLenum type, void * values);
typedef void (GLAD_API_PTR *PFNGLGETHISTOGRAMPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETHISTOGRAMPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef GLuint64 (GLAD_API_PTR *PFNGLGETIMAGEHANDLEARBPROC)(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
typedef void (GLAD_API_PTR *PFNGLGETINFOLOGARBPROC)(GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog);
typedef void (GLAD_API_PTR *PFNGLGETINTEGER64I_VPROC)(GLenum target, GLuint index, GLint64 * data);
typedef void (GLAD_API_PTR *PFNGLGETINTEGER64VPROC)(GLenum pname, GLint64 * data);
typedef void (GLAD_API_PTR *PFNGLGETINTEGERI_VPROC)(GLenum target, GLuint index, GLint * data);
typedef void (GLAD_API_PTR *PFNGLGETINTEGERVPROC)(GLenum pname, GLint * data);
typedef void (GLAD_API_PTR *PFNGLGETINTERNALFORMATI64VPROC)(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETINTERNALFORMATIVPROC)(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETLIGHTFVPROC)(GLenum light, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETLIGHTIVPROC)(GLenum light, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETMAPDVPROC)(GLenum target, GLenum query, GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLGETMAPFVPROC)(GLenum target, GLenum query, GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLGETMAPIVPROC)(GLenum target, GLenum query, GLint * v);
typedef void (GLAD_API_PTR *PFNGLGETMATERIALFVPROC)(GLenum face, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETMATERIALIVPROC)(GLenum face, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETMINMAXPROC)(GLenum target, GLboolean reset, GLenum format, GLenum type, void * values);
typedef void (GLAD_API_PTR *PFNGLGETMINMAXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETMINMAXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETMULTISAMPLEFVPROC)(GLenum pname, GLuint index, GLfloat * val);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDBUFFERPARAMETERI64VPROC)(GLuint buffer, GLenum pname, GLint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDBUFFERPARAMETERIVPROC)(GLuint buffer, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDBUFFERPOINTERVPROC)(GLuint buffer, GLenum pname, void ** params);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDBUFFERSUBDATAPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, void * data);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC)(GLuint framebuffer, GLenum pname, GLint * param);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC)(GLuint renderbuffer, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDSTRINGARBPROC)(GLint namelen, const GLchar * name, GLsizei bufSize, GLint * stringlen, GLchar * string);
typedef void (GLAD_API_PTR *PFNGLGETNAMEDSTRINGIVARBPROC)(GLint namelen, const GLchar * name, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETOBJECTLABELPROC)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label);
typedef void (GLAD_API_PTR *PFNGLGETOBJECTPARAMETERFVARBPROC)(GLhandleARB obj, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETOBJECTPARAMETERIVARBPROC)(GLhandleARB obj, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETOBJECTPTRLABELPROC)(const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label);
typedef void (GLAD_API_PTR *PFNGLGETPIXELMAPFVPROC)(GLenum map, GLfloat * values);
typedef void (GLAD_API_PTR *PFNGLGETPIXELMAPUIVPROC)(GLenum map, GLuint * values);
typedef void (GLAD_API_PTR *PFNGLGETPIXELMAPUSVPROC)(GLenum map, GLushort * values);
typedef void (GLAD_API_PTR *PFNGLGETPOINTERVPROC)(GLenum pname, void ** params);
typedef void (GLAD_API_PTR *PFNGLGETPOLYGONSTIPPLEPROC)(GLubyte * mask);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMBINARYPROC)(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, void * binary);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMENVPARAMETERDVARBPROC)(GLenum target, GLuint index, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMENVPARAMETERFVARBPROC)(GLenum target, GLuint index, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMINTERFACEIVPROC)(GLuint program, GLenum programInterface, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)(GLenum target, GLuint index, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)(GLenum target, GLuint index, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMPIPELINEINFOLOGPROC)(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMPIPELINEIVPROC)(GLuint pipeline, GLenum pname, GLint * params);
typedef GLuint (GLAD_API_PTR *PFNGLGETPROGRAMRESOURCEINDEXPROC)(GLuint program, GLenum programInterface, const GLchar * name);
typedef GLint (GLAD_API_PTR *PFNGLGETPROGRAMRESOURCELOCATIONPROC)(GLuint program, GLenum programInterface, const GLchar * name);
typedef GLint (GLAD_API_PTR *PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)(GLuint program, GLenum programInterface, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMRESOURCENAMEPROC)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMRESOURCEIVPROC)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei count, GLsizei * length, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMSTAGEIVPROC)(GLuint program, GLenum shadertype, GLenum pname, GLint * values);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMSTRINGARBPROC)(GLenum target, GLenum pname, void * string);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETPROGRAMIVARBPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYBUFFEROBJECTI64VPROC)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAD_API_PTR *PFNGLGETQUERYBUFFEROBJECTIVPROC)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAD_API_PTR *PFNGLGETQUERYBUFFEROBJECTUI64VPROC)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAD_API_PTR *PFNGLGETQUERYBUFFEROBJECTUIVPROC)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAD_API_PTR *PFNGLGETQUERYINDEXEDIVPROC)(GLenum target, GLuint index, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTI64VPROC)(GLuint id, GLenum pname, GLint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTIVPROC)(GLuint id, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTIVARBPROC)(GLuint id, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTUI64VPROC)(GLuint id, GLenum pname, GLuint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTUIVPROC)(GLuint id, GLenum pname, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTUIVARBPROC)(GLuint id, GLenum pname, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETQUERYIVARBPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETRENDERBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETSEPARABLEFILTERPROC)(GLenum target, GLenum format, GLenum type, void * row, void * column, void * span);
typedef void (GLAD_API_PTR *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
typedef void (GLAD_API_PTR *PFNGLGETSHADERPRECISIONFORMATPROC)(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision);
typedef void (GLAD_API_PTR *PFNGLGETSHADERSOURCEPROC)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);
typedef void (GLAD_API_PTR *PFNGLGETSHADERSOURCEARBPROC)(GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * source);
typedef void (GLAD_API_PTR *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint * params);
typedef const GLubyte * (GLAD_API_PTR *PFNGLGETSTRINGPROC)(GLenum name);
typedef const GLubyte * (GLAD_API_PTR *PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
typedef GLuint (GLAD_API_PTR *PFNGLGETSUBROUTINEINDEXPROC)(GLuint program, GLenum shadertype, const GLchar * name);
typedef GLint (GLAD_API_PTR *PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)(GLuint program, GLenum shadertype, const GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETSYNCIVPROC)(GLsync sync, GLenum pname, GLsizei count, GLsizei * length, GLint * values);
typedef void (GLAD_API_PTR *PFNGLGETTEXENVFVPROC)(GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXENVIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXGENDVPROC)(GLenum coord, GLenum pname, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXGENFVPROC)(GLenum coord, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXGENIVPROC)(GLenum coord, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
typedef void (GLAD_API_PTR *PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
typedef GLuint64 (GLAD_API_PTR *PFNGLGETTEXTUREHANDLEARBPROC)(GLuint texture);
typedef void (GLAD_API_PTR *PFNGLGETTEXTUREIMAGEPROC)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels);
typedef void (GLAD_API_PTR *PFNGLGETTEXTURELEVELPARAMETERFVPROC)(GLuint texture, GLint level, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXTURELEVELPARAMETERIVPROC)(GLuint texture, GLint level, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXTUREPARAMETERIIVPROC)(GLuint texture, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXTUREPARAMETERIUIVPROC)(GLuint texture, GLenum pname, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXTUREPARAMETERFVPROC)(GLuint texture, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETTEXTUREPARAMETERIVPROC)(GLuint texture, GLenum pname, GLint * params);
typedef GLuint64 (GLAD_API_PTR *PFNGLGETTEXTURESAMPLERHANDLEARBPROC)(GLuint texture, GLuint sampler);
typedef void (GLAD_API_PTR *PFNGLGETTEXTURESUBIMAGEPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void * pixels);
typedef void (GLAD_API_PTR *PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
typedef void (GLAD_API_PTR *PFNGLGETTRANSFORMFEEDBACKI64_VPROC)(GLuint xfb, GLenum pname, GLuint index, GLint64 * param);
typedef void (GLAD_API_PTR *PFNGLGETTRANSFORMFEEDBACKI_VPROC)(GLuint xfb, GLenum pname, GLuint index, GLint * param);
typedef void (GLAD_API_PTR *PFNGLGETTRANSFORMFEEDBACKIVPROC)(GLuint xfb, GLenum pname, GLint * param);
typedef GLuint (GLAD_API_PTR *PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar * uniformBlockName);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMINDICESPROC)(GLuint program, GLsizei uniformCount, const GLchar *const* uniformNames, GLuint * uniformIndices);
typedef GLint (GLAD_API_PTR *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar * name);
typedef GLint (GLAD_API_PTR *PFNGLGETUNIFORMLOCATIONARBPROC)(GLhandleARB programObj, const GLcharARB * name);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMSUBROUTINEUIVPROC)(GLenum shadertype, GLint location, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMDVPROC)(GLuint program, GLint location, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMFVPROC)(GLuint program, GLint location, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMFVARBPROC)(GLhandleARB programObj, GLint location, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMI64VARBPROC)(GLuint program, GLint location, GLint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMIVPROC)(GLuint program, GLint location, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMIVARBPROC)(GLhandleARB programObj, GLint location, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMUI64VARBPROC)(GLuint program, GLint location, GLuint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETUNIFORMUIVPROC)(GLuint program, GLint location, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXARRAYINDEXED64IVPROC)(GLuint vaobj, GLuint index, GLenum pname, GLint64 * param);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXARRAYINDEXEDIVPROC)(GLuint vaobj, GLuint index, GLenum pname, GLint * param);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXARRAYIVPROC)(GLuint vaobj, GLenum pname, GLint * param);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIIVPROC)(GLuint index, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIUIVPROC)(GLuint index, GLenum pname, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBLDVPROC)(GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBLUI64VARBPROC)(GLuint index, GLenum pname, GLuint64EXT * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBPOINTERVPROC)(GLuint index, GLenum pname, void ** pointer);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBPOINTERVARBPROC)(GLuint index, GLenum pname, void ** pointer);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBDVPROC)(GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBDVARBPROC)(GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBFVPROC)(GLuint index, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBFVARBPROC)(GLuint index, GLenum pname, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIVPROC)(GLuint index, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIVARBPROC)(GLuint index, GLenum pname, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETNCOLORTABLEARBPROC)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * table);
typedef void (GLAD_API_PTR *PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC)(GLenum target, GLint lod, GLsizei bufSize, void * img);
typedef void (GLAD_API_PTR *PFNGLGETNCONVOLUTIONFILTERARBPROC)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * image);
typedef void (GLAD_API_PTR *PFNGLGETNHISTOGRAMARBPROC)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values);
typedef void (GLAD_API_PTR *PFNGLGETNMAPDVARBPROC)(GLenum target, GLenum query, GLsizei bufSize, GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLGETNMAPFVARBPROC)(GLenum target, GLenum query, GLsizei bufSize, GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLGETNMAPIVARBPROC)(GLenum target, GLenum query, GLsizei bufSize, GLint * v);
typedef void (GLAD_API_PTR *PFNGLGETNMINMAXARBPROC)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values);
typedef void (GLAD_API_PTR *PFNGLGETNPIXELMAPFVARBPROC)(GLenum map, GLsizei bufSize, GLfloat * values);
typedef void (GLAD_API_PTR *PFNGLGETNPIXELMAPUIVARBPROC)(GLenum map, GLsizei bufSize, GLuint * values);
typedef void (GLAD_API_PTR *PFNGLGETNPIXELMAPUSVARBPROC)(GLenum map, GLsizei bufSize, GLushort * values);
typedef void (GLAD_API_PTR *PFNGLGETNPOLYGONSTIPPLEARBPROC)(GLsizei bufSize, GLubyte * pattern);
typedef void (GLAD_API_PTR *PFNGLGETNSEPARABLEFILTERARBPROC)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void * row, GLsizei columnBufSize, void * column, void * span);
typedef void (GLAD_API_PTR *PFNGLGETNTEXIMAGEARBPROC)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * img);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMDVARBPROC)(GLuint program, GLint location, GLsizei bufSize, GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMFVPROC)(GLuint program, GLint location, GLsizei bufSize, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMFVARBPROC)(GLuint program, GLint location, GLsizei bufSize, GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMI64VARBPROC)(GLuint program, GLint location, GLsizei bufSize, GLint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMIVPROC)(GLuint program, GLint location, GLsizei bufSize, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMIVARBPROC)(GLuint program, GLint location, GLsizei bufSize, GLint * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMUI64VARBPROC)(GLuint program, GLint location, GLsizei bufSize, GLuint64 * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMUIVPROC)(GLuint program, GLint location, GLsizei bufSize, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLGETNUNIFORMUIVARBPROC)(GLuint program, GLint location, GLsizei bufSize, GLuint * params);
typedef void (GLAD_API_PTR *PFNGLHINTPROC)(GLenum target, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLHISTOGRAMPROC)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAD_API_PTR *PFNGLINDEXMASKPROC)(GLuint mask);
typedef void (GLAD_API_PTR *PFNGLINDEXPOINTERPROC)(GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLINDEXDPROC)(GLdouble c);
typedef void (GLAD_API_PTR *PFNGLINDEXDVPROC)(const GLdouble * c);
typedef void (GLAD_API_PTR *PFNGLINDEXFPROC)(GLfloat c);
typedef void (GLAD_API_PTR *PFNGLINDEXFVPROC)(const GLfloat * c);
typedef void (GLAD_API_PTR *PFNGLINDEXIPROC)(GLint c);
typedef void (GLAD_API_PTR *PFNGLINDEXIVPROC)(const GLint * c);
typedef void (GLAD_API_PTR *PFNGLINDEXSPROC)(GLshort c);
typedef void (GLAD_API_PTR *PFNGLINDEXSVPROC)(const GLshort * c);
typedef void (GLAD_API_PTR *PFNGLINDEXUBPROC)(GLubyte c);
typedef void (GLAD_API_PTR *PFNGLINDEXUBVPROC)(const GLubyte * c);
typedef void (GLAD_API_PTR *PFNGLINITNAMESPROC)(void);
typedef void (GLAD_API_PTR *PFNGLINTERLEAVEDARRAYSPROC)(GLenum format, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLINVALIDATEBUFFERDATAPROC)(GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLINVALIDATEBUFFERSUBDATAPROC)(GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAD_API_PTR *PFNGLINVALIDATEFRAMEBUFFERPROC)(GLenum target, GLsizei numAttachments, const GLenum * attachments);
typedef void (GLAD_API_PTR *PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC)(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments);
typedef void (GLAD_API_PTR *PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC)(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLINVALIDATESUBFRAMEBUFFERPROC)(GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLINVALIDATETEXIMAGEPROC)(GLuint texture, GLint level);
typedef void (GLAD_API_PTR *PFNGLINVALIDATETEXSUBIMAGEPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
typedef GLboolean (GLAD_API_PTR *PFNGLISBUFFERPROC)(GLuint buffer);
typedef GLboolean (GLAD_API_PTR *PFNGLISBUFFERARBPROC)(GLuint buffer);
typedef GLboolean (GLAD_API_PTR *PFNGLISENABLEDPROC)(GLenum cap);
typedef GLboolean (GLAD_API_PTR *PFNGLISENABLEDIPROC)(GLenum target, GLuint index);
typedef GLboolean (GLAD_API_PTR *PFNGLISFRAMEBUFFERPROC)(GLuint framebuffer);
typedef GLboolean (GLAD_API_PTR *PFNGLISIMAGEHANDLERESIDENTARBPROC)(GLuint64 handle);
typedef GLboolean (GLAD_API_PTR *PFNGLISLISTPROC)(GLuint list);
typedef GLboolean (GLAD_API_PTR *PFNGLISNAMEDSTRINGARBPROC)(GLint namelen, const GLchar * name);
typedef GLboolean (GLAD_API_PTR *PFNGLISPROGRAMPROC)(GLuint program);
typedef GLboolean (GLAD_API_PTR *PFNGLISPROGRAMARBPROC)(GLuint program);
typedef GLboolean (GLAD_API_PTR *PFNGLISPROGRAMPIPELINEPROC)(GLuint pipeline);
typedef GLboolean (GLAD_API_PTR *PFNGLISQUERYPROC)(GLuint id);
typedef GLboolean (GLAD_API_PTR *PFNGLISQUERYARBPROC)(GLuint id);
typedef GLboolean (GLAD_API_PTR *PFNGLISRENDERBUFFERPROC)(GLuint renderbuffer);
typedef GLboolean (GLAD_API_PTR *PFNGLISSAMPLERPROC)(GLuint sampler);
typedef GLboolean (GLAD_API_PTR *PFNGLISSHADERPROC)(GLuint shader);
typedef GLboolean (GLAD_API_PTR *PFNGLISSYNCPROC)(GLsync sync);
typedef GLboolean (GLAD_API_PTR *PFNGLISTEXTUREPROC)(GLuint texture);
typedef GLboolean (GLAD_API_PTR *PFNGLISTEXTUREHANDLERESIDENTARBPROC)(GLuint64 handle);
typedef GLboolean (GLAD_API_PTR *PFNGLISTRANSFORMFEEDBACKPROC)(GLuint id);
typedef GLboolean (GLAD_API_PTR *PFNGLISVERTEXARRAYPROC)(GLuint array);
typedef void (GLAD_API_PTR *PFNGLLIGHTMODELFPROC)(GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLLIGHTMODELFVPROC)(GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLLIGHTMODELIPROC)(GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLLIGHTMODELIVPROC)(GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLLIGHTFPROC)(GLenum light, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLLIGHTFVPROC)(GLenum light, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLLIGHTIPROC)(GLenum light, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLLIGHTIVPROC)(GLenum light, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLLINESTIPPLEPROC)(GLint factor, GLushort pattern);
typedef void (GLAD_API_PTR *PFNGLLINEWIDTHPROC)(GLfloat width);
typedef void (GLAD_API_PTR *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (GLAD_API_PTR *PFNGLLINKPROGRAMARBPROC)(GLhandleARB programObj);
typedef void (GLAD_API_PTR *PFNGLLISTBASEPROC)(GLuint base);
typedef void (GLAD_API_PTR *PFNGLLOADIDENTITYPROC)(void);
typedef void (GLAD_API_PTR *PFNGLLOADMATRIXDPROC)(const GLdouble * m);
typedef void (GLAD_API_PTR *PFNGLLOADMATRIXFPROC)(const GLfloat * m);
typedef void (GLAD_API_PTR *PFNGLLOADNAMEPROC)(GLuint name);
typedef void (GLAD_API_PTR *PFNGLLOADTRANSPOSEMATRIXDPROC)(const GLdouble * m);
typedef void (GLAD_API_PTR *PFNGLLOADTRANSPOSEMATRIXDARBPROC)(const GLdouble * m);
typedef void (GLAD_API_PTR *PFNGLLOADTRANSPOSEMATRIXFPROC)(const GLfloat * m);
typedef void (GLAD_API_PTR *PFNGLLOADTRANSPOSEMATRIXFARBPROC)(const GLfloat * m);
typedef void (GLAD_API_PTR *PFNGLLOGICOPPROC)(GLenum opcode);
typedef void (GLAD_API_PTR *PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC)(GLuint64 handle);
typedef void (GLAD_API_PTR *PFNGLMAKEIMAGEHANDLERESIDENTARBPROC)(GLuint64 handle, GLenum access);
typedef void (GLAD_API_PTR *PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC)(GLuint64 handle);
typedef void (GLAD_API_PTR *PFNGLMAKETEXTUREHANDLERESIDENTARBPROC)(GLuint64 handle);
typedef void (GLAD_API_PTR *PFNGLMAP1DPROC)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points);
typedef void (GLAD_API_PTR *PFNGLMAP1FPROC)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points);
typedef void (GLAD_API_PTR *PFNGLMAP2DPROC)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points);
typedef void (GLAD_API_PTR *PFNGLMAP2FPROC)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points);
typedef void * (GLAD_API_PTR *PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef void * (GLAD_API_PTR *PFNGLMAPBUFFERARBPROC)(GLenum target, GLenum access);
typedef void * (GLAD_API_PTR *PFNGLMAPBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAD_API_PTR *PFNGLMAPGRID1DPROC)(GLint un, GLdouble u1, GLdouble u2);
typedef void (GLAD_API_PTR *PFNGLMAPGRID1FPROC)(GLint un, GLfloat u1, GLfloat u2);
typedef void (GLAD_API_PTR *PFNGLMAPGRID2DPROC)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
typedef void (GLAD_API_PTR *PFNGLMAPGRID2FPROC)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
typedef void * (GLAD_API_PTR *PFNGLMAPNAMEDBUFFERPROC)(GLuint buffer, GLenum access);
typedef void * (GLAD_API_PTR *PFNGLMAPNAMEDBUFFERRANGEPROC)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAD_API_PTR *PFNGLMATERIALFPROC)(GLenum face, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLMATERIALFVPROC)(GLenum face, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLMATERIALIPROC)(GLenum face, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLMATERIALIVPROC)(GLenum face, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLMATRIXINDEXPOINTERARBPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLMATRIXINDEXUBVARBPROC)(GLint size, const GLubyte * indices);
typedef void (GLAD_API_PTR *PFNGLMATRIXINDEXUIVARBPROC)(GLint size, const GLuint * indices);
typedef void (GLAD_API_PTR *PFNGLMATRIXINDEXUSVARBPROC)(GLint size, const GLushort * indices);
typedef void (GLAD_API_PTR *PFNGLMATRIXMODEPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLMAXSHADERCOMPILERTHREADSARBPROC)(GLuint count);
typedef void (GLAD_API_PTR *PFNGLMAXSHADERCOMPILERTHREADSKHRPROC)(GLuint count);
typedef void (GLAD_API_PTR *PFNGLMEMORYBARRIERPROC)(GLbitfield barriers);
typedef void (GLAD_API_PTR *PFNGLMEMORYBARRIERBYREGIONPROC)(GLbitfield barriers);
typedef void (GLAD_API_PTR *PFNGLMINSAMPLESHADINGPROC)(GLfloat value);
typedef void (GLAD_API_PTR *PFNGLMINSAMPLESHADINGARBPROC)(GLfloat value);
typedef void (GLAD_API_PTR *PFNGLMINMAXPROC)(GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAD_API_PTR *PFNGLMULTMATRIXDPROC)(const GLdouble * m);
typedef void (GLAD_API_PTR *PFNGLMULTMATRIXFPROC)(const GLfloat * m);
typedef void (GLAD_API_PTR *PFNGLMULTTRANSPOSEMATRIXDPROC)(const GLdouble * m);
typedef void (GLAD_API_PTR *PFNGLMULTTRANSPOSEMATRIXDARBPROC)(const GLdouble * m);
typedef void (GLAD_API_PTR *PFNGLMULTTRANSPOSEMATRIXFPROC)(const GLfloat * m);
typedef void (GLAD_API_PTR *PFNGLMULTTRANSPOSEMATRIXFARBPROC)(const GLfloat * m);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWARRAYSPROC)(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWARRAYSINDIRECTPROC)(GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC)(GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC)(GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSPROC)(GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount, const GLint * basevertex);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSINDIRECTPROC)(GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC)(GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC)(GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1DPROC)(GLenum target, GLdouble s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1DARBPROC)(GLenum target, GLdouble s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1DVPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1DVARBPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1FPROC)(GLenum target, GLfloat s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1FARBPROC)(GLenum target, GLfloat s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1FVPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1FVARBPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1IPROC)(GLenum target, GLint s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1IARBPROC)(GLenum target, GLint s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1IVPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1IVARBPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1SPROC)(GLenum target, GLshort s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1SARBPROC)(GLenum target, GLshort s);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1SVPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD1SVARBPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2DPROC)(GLenum target, GLdouble s, GLdouble t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2DARBPROC)(GLenum target, GLdouble s, GLdouble t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2DVPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2DVARBPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2FPROC)(GLenum target, GLfloat s, GLfloat t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2FARBPROC)(GLenum target, GLfloat s, GLfloat t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2FVPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2FVARBPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2IPROC)(GLenum target, GLint s, GLint t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2IARBPROC)(GLenum target, GLint s, GLint t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2IVPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2IVARBPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2SPROC)(GLenum target, GLshort s, GLshort t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2SARBPROC)(GLenum target, GLshort s, GLshort t);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2SVPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD2SVARBPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3DPROC)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3DARBPROC)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3DVPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3DVARBPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3FPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3FARBPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3FVPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3FVARBPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3IPROC)(GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3IARBPROC)(GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3IVPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3IVARBPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3SPROC)(GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3SARBPROC)(GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3SVPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD3SVARBPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4DPROC)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4DARBPROC)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4DVPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4DVARBPROC)(GLenum target, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4FPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4FARBPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4FVPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4FVARBPROC)(GLenum target, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4IPROC)(GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4IARBPROC)(GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4IVPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4IVARBPROC)(GLenum target, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4SPROC)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4SARBPROC)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4SVPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORD4SVARBPROC)(GLenum target, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP1UIPROC)(GLenum texture, GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP1UIVPROC)(GLenum texture, GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP2UIPROC)(GLenum texture, GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP2UIVPROC)(GLenum texture, GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP3UIPROC)(GLenum texture, GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP3UIVPROC)(GLenum texture, GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP4UIPROC)(GLenum texture, GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLMULTITEXCOORDP4UIVPROC)(GLenum texture, GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLNAMEDBUFFERDATAPROC)(GLuint buffer, GLsizeiptr size, const void * data, GLenum usage);
typedef void (GLAD_API_PTR *PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
typedef void (GLAD_API_PTR *PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
typedef void (GLAD_API_PTR *PFNGLNAMEDBUFFERSTORAGEPROC)(GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags);
typedef void (GLAD_API_PTR *PFNGLNAMEDBUFFERSUBDATAPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC)(GLuint framebuffer, GLenum buf);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC)(GLuint framebuffer, GLsizei n, const GLenum * bufs);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC)(GLuint framebuffer, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC)(GLuint framebuffer, GLenum src);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVARBPROC)(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERTEXTUREPROC)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAD_API_PTR *PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAD_API_PTR *PFNGLNAMEDRENDERBUFFERSTORAGEPROC)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLNAMEDSTRINGARBPROC)(GLenum type, GLint namelen, const GLchar * name, GLint stringlen, const GLchar * string);
typedef void (GLAD_API_PTR *PFNGLNEWLISTPROC)(GLuint list, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLNORMAL3BPROC)(GLbyte nx, GLbyte ny, GLbyte nz);
typedef void (GLAD_API_PTR *PFNGLNORMAL3BVPROC)(const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLNORMAL3DPROC)(GLdouble nx, GLdouble ny, GLdouble nz);
typedef void (GLAD_API_PTR *PFNGLNORMAL3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLNORMAL3FPROC)(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (GLAD_API_PTR *PFNGLNORMAL3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLNORMAL3IPROC)(GLint nx, GLint ny, GLint nz);
typedef void (GLAD_API_PTR *PFNGLNORMAL3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLNORMAL3SPROC)(GLshort nx, GLshort ny, GLshort nz);
typedef void (GLAD_API_PTR *PFNGLNORMAL3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLNORMALP3UIPROC)(GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLNORMALP3UIVPROC)(GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLNORMALPOINTERPROC)(GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLOBJECTLABELPROC)(GLenum identifier, GLuint name, GLsizei length, const GLchar * label);
typedef void (GLAD_API_PTR *PFNGLOBJECTPTRLABELPROC)(const void * ptr, GLsizei length, const GLchar * label);
typedef void (GLAD_API_PTR *PFNGLORTHOPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAD_API_PTR *PFNGLPASSTHROUGHPROC)(GLfloat token);
typedef void (GLAD_API_PTR *PFNGLPATCHPARAMETERFVPROC)(GLenum pname, const GLfloat * values);
typedef void (GLAD_API_PTR *PFNGLPATCHPARAMETERIPROC)(GLenum pname, GLint value);
typedef void (GLAD_API_PTR *PFNGLPAUSETRANSFORMFEEDBACKPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPIXELMAPFVPROC)(GLenum map, GLsizei mapsize, const GLfloat * values);
typedef void (GLAD_API_PTR *PFNGLPIXELMAPUIVPROC)(GLenum map, GLsizei mapsize, const GLuint * values);
typedef void (GLAD_API_PTR *PFNGLPIXELMAPUSVPROC)(GLenum map, GLsizei mapsize, const GLushort * values);
typedef void (GLAD_API_PTR *PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLPIXELTRANSFERFPROC)(GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLPIXELTRANSFERIPROC)(GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLPIXELZOOMPROC)(GLfloat xfactor, GLfloat yfactor);
typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERFARBPROC)(GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERFVPROC)(GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERFVARBPROC)(GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERIPROC)(GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERIVPROC)(GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLPOINTSIZEPROC)(GLfloat size);
typedef void (GLAD_API_PTR *PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (GLAD_API_PTR *PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
typedef void (GLAD_API_PTR *PFNGLPOLYGONOFFSETCLAMPPROC)(GLfloat factor, GLfloat units, GLfloat clamp);
typedef void (GLAD_API_PTR *PFNGLPOLYGONSTIPPLEPROC)(const GLubyte * mask);
typedef void (GLAD_API_PTR *PFNGLPOPATTRIBPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPOPCLIENTATTRIBPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPOPDEBUGGROUPPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPOPMATRIXPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPOPNAMEPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPRIMITIVEBOUNDINGBOXPROC)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
typedef void (GLAD_API_PTR *PFNGLPRIMITIVEBOUNDINGBOXARBPROC)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
typedef void (GLAD_API_PTR *PFNGLPRIMITIVERESTARTINDEXPROC)(GLuint index);
typedef void (GLAD_API_PTR *PFNGLPRIORITIZETEXTURESPROC)(GLsizei n, const GLuint * textures, const GLfloat * priorities);
typedef void (GLAD_API_PTR *PFNGLPROGRAMBINARYPROC)(GLuint program, GLenum binaryFormat, const void * binary, GLsizei length);
typedef void (GLAD_API_PTR *PFNGLPROGRAMENVPARAMETER4DARBPROC)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLPROGRAMENVPARAMETER4DVARBPROC)(GLenum target, GLuint index, const GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLPROGRAMENVPARAMETER4FARBPROC)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAD_API_PTR *PFNGLPROGRAMENVPARAMETER4FVARBPROC)(GLenum target, GLuint index, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLPROGRAMLOCALPARAMETER4DARBPROC)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)(GLenum target, GLuint index, const GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLPROGRAMLOCALPARAMETER4FARBPROC)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAD_API_PTR *PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)(GLenum target, GLuint index, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLPROGRAMPARAMETERIPROC)(GLuint program, GLenum pname, GLint value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMPARAMETERIARBPROC)(GLuint program, GLenum pname, GLint value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMSTRINGARBPROC)(GLenum target, GLenum format, GLsizei len, const void * string);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1DPROC)(GLuint program, GLint location, GLdouble v0);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1DVPROC)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1FPROC)(GLuint program, GLint location, GLfloat v0);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1IPROC)(GLuint program, GLint location, GLint v0);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1I64ARBPROC)(GLuint program, GLint location, GLint64 x);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1I64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1IVPROC)(GLuint program, GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1UIPROC)(GLuint program, GLint location, GLuint v0);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1UI64ARBPROC)(GLuint program, GLint location, GLuint64 x);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1UI64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM1UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2DPROC)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2DVPROC)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2FPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2IPROC)(GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2I64ARBPROC)(GLuint program, GLint location, GLint64 x, GLint64 y);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2I64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2IVPROC)(GLuint program, GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2UIPROC)(GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2UI64ARBPROC)(GLuint program, GLint location, GLuint64 x, GLuint64 y);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2UI64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM2UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3DPROC)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3DVPROC)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3FPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3IPROC)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3I64ARBPROC)(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3I64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3IVPROC)(GLuint program, GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3UIPROC)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3UI64ARBPROC)(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3UI64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM3UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4DPROC)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4DVPROC)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4FPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4IPROC)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4I64ARBPROC)(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4I64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4IVPROC)(GLuint program, GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4UIPROC)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4UI64ARBPROC)(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4UI64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORM4UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC)(GLuint program, GLint location, GLuint64 value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 * values);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX2DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX2FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX3DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX3FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX4DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX4FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLPROVOKINGVERTEXPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLPUSHATTRIBPROC)(GLbitfield mask);
typedef void (GLAD_API_PTR *PFNGLPUSHCLIENTATTRIBPROC)(GLbitfield mask);
typedef void (GLAD_API_PTR *PFNGLPUSHDEBUGGROUPPROC)(GLenum source, GLuint id, GLsizei length, const GLchar * message);
typedef void (GLAD_API_PTR *PFNGLPUSHMATRIXPROC)(void);
typedef void (GLAD_API_PTR *PFNGLPUSHNAMEPROC)(GLuint name);
typedef void (GLAD_API_PTR *PFNGLQUERYCOUNTERPROC)(GLuint id, GLenum target);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2DPROC)(GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2FPROC)(GLfloat x, GLfloat y);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2IPROC)(GLint x, GLint y);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2SPROC)(GLshort x, GLshort y);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS2SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3DPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3FPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3IPROC)(GLint x, GLint y, GLint z);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3SPROC)(GLshort x, GLshort y, GLshort z);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4DPROC)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4FPROC)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4IPROC)(GLint x, GLint y, GLint z, GLint w);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4SPROC)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAD_API_PTR *PFNGLRASTERPOS4SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLREADBUFFERPROC)(GLenum src);
typedef void (GLAD_API_PTR *PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels);
typedef void (GLAD_API_PTR *PFNGLREADNPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data);
typedef void (GLAD_API_PTR *PFNGLREADNPIXELSARBPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data);
typedef void (GLAD_API_PTR *PFNGLRECTDPROC)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
typedef void (GLAD_API_PTR *PFNGLRECTDVPROC)(const GLdouble * v1, const GLdouble * v2);
typedef void (GLAD_API_PTR *PFNGLRECTFPROC)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
typedef void (GLAD_API_PTR *PFNGLRECTFVPROC)(const GLfloat * v1, const GLfloat * v2);
typedef void (GLAD_API_PTR *PFNGLRECTIPROC)(GLint x1, GLint y1, GLint x2, GLint y2);
typedef void (GLAD_API_PTR *PFNGLRECTIVPROC)(const GLint * v1, const GLint * v2);
typedef void (GLAD_API_PTR *PFNGLRECTSPROC)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
typedef void (GLAD_API_PTR *PFNGLRECTSVPROC)(const GLshort * v1, const GLshort * v2);
typedef void (GLAD_API_PTR *PFNGLRELEASESHADERCOMPILERPROC)(void);
typedef GLint (GLAD_API_PTR *PFNGLRENDERMODEPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLRESETHISTOGRAMPROC)(GLenum target);
typedef void (GLAD_API_PTR *PFNGLRESETMINMAXPROC)(GLenum target);
typedef void (GLAD_API_PTR *PFNGLRESUMETRANSFORMFEEDBACKPROC)(void);
typedef void (GLAD_API_PTR *PFNGLROTATEDPROC)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLROTATEFPROC)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);
typedef void (GLAD_API_PTR *PFNGLSAMPLECOVERAGEARBPROC)(GLfloat value, GLboolean invert);
typedef void (GLAD_API_PTR *PFNGLSAMPLEMASKIPROC)(GLuint maskNumber, GLbitfield mask);
typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, const GLint * param);
typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, const GLuint * param);
typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERFPROC)(GLuint sampler, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, const GLfloat * param);
typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIPROC)(GLuint sampler, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, const GLint * param);
typedef void (GLAD_API_PTR *PFNGLSCALEDPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLSCALEFPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLSCISSORARRAYVPROC)(GLuint first, GLsizei count, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLSCISSORINDEXEDPROC)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLSCISSORINDEXEDVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3BPROC)(GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3BVPROC)(const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3DPROC)(GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3FPROC)(GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3IPROC)(GLint red, GLint green, GLint blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3SPROC)(GLshort red, GLshort green, GLshort blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3UBPROC)(GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3UBVPROC)(const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3UIPROC)(GLuint red, GLuint green, GLuint blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3UIVPROC)(const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3USPROC)(GLushort red, GLushort green, GLushort blue);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLOR3USVPROC)(const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLORP3UIPROC)(GLenum type, GLuint color);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLORP3UIVPROC)(GLenum type, const GLuint * color);
typedef void (GLAD_API_PTR *PFNGLSECONDARYCOLORPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLSELECTBUFFERPROC)(GLsizei size, GLuint * buffer);
typedef void (GLAD_API_PTR *PFNGLSEPARABLEFILTER2DPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * row, const void * column);
typedef void (GLAD_API_PTR *PFNGLSHADEMODELPROC)(GLenum mode);
typedef void (GLAD_API_PTR *PFNGLSHADERBINARYPROC)(GLsizei count, const GLuint * shaders, GLenum binaryformat, const void * binary, GLsizei length);
typedef void (GLAD_API_PTR *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const* string, const GLint * length);
typedef void (GLAD_API_PTR *PFNGLSHADERSOURCEARBPROC)(GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint * length);
typedef void (GLAD_API_PTR *PFNGLSHADERSTORAGEBLOCKBINDINGPROC)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
typedef void (GLAD_API_PTR *PFNGLSPECIALIZESHADERPROC)(GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue);
typedef void (GLAD_API_PTR *PFNGLSPECIALIZESHADERARBPROC)(GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue);
typedef void (GLAD_API_PTR *PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
typedef void (GLAD_API_PTR *PFNGLSTENCILFUNCSEPARATEPROC)(GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (GLAD_API_PTR *PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void (GLAD_API_PTR *PFNGLSTENCILMASKSEPARATEPROC)(GLenum face, GLuint mask);
typedef void (GLAD_API_PTR *PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (GLAD_API_PTR *PFNGLSTENCILOPSEPARATEPROC)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (GLAD_API_PTR *PFNGLTEXBUFFERPROC)(GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLTEXBUFFERARBPROC)(GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLTEXBUFFERRANGEPROC)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1DPROC)(GLdouble s);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1FPROC)(GLfloat s);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1IPROC)(GLint s);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1SPROC)(GLshort s);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD1SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2DPROC)(GLdouble s, GLdouble t);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2FPROC)(GLfloat s, GLfloat t);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2IPROC)(GLint s, GLint t);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2SPROC)(GLshort s, GLshort t);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD2SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3DPROC)(GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3FPROC)(GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3IPROC)(GLint s, GLint t, GLint r);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3SPROC)(GLshort s, GLshort t, GLshort r);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4DPROC)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4FPROC)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4IPROC)(GLint s, GLint t, GLint r, GLint q);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4SPROC)(GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAD_API_PTR *PFNGLTEXCOORD4SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP1UIPROC)(GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP1UIVPROC)(GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP2UIPROC)(GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP2UIVPROC)(GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP3UIPROC)(GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP3UIVPROC)(GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP4UIPROC)(GLenum type, GLuint coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDP4UIVPROC)(GLenum type, const GLuint * coords);
typedef void (GLAD_API_PTR *PFNGLTEXCOORDPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLTEXENVFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLTEXENVFVPROC)(GLenum target, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLTEXENVIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLTEXENVIVPROC)(GLenum target, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLTEXGENDPROC)(GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAD_API_PTR *PFNGLTEXGENDVPROC)(GLenum coord, GLenum pname, const GLdouble * params);
typedef void (GLAD_API_PTR *PFNGLTEXGENFPROC)(GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLTEXGENFVPROC)(GLenum coord, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLTEXGENIPROC)(GLenum coord, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLTEXGENIVPROC)(GLenum coord, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXIMAGE2DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAD_API_PTR *PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXIMAGE3DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAD_API_PTR *PFNGLTEXPAGECOMMITMENTARBPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, const GLuint * params);
typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat * params);
typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLTEXSTORAGE1DPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLTEXSTORAGE2DPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLTEXSTORAGE2DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAD_API_PTR *PFNGLTEXSTORAGE3DPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAD_API_PTR *PFNGLTEXSTORAGE3DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAD_API_PTR *PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXTUREBARRIERPROC)(void);
typedef void (GLAD_API_PTR *PFNGLTEXTUREBUFFERPROC)(GLuint texture, GLenum internalformat, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLTEXTUREBUFFERRANGEPROC)(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAD_API_PTR *PFNGLTEXTUREPARAMETERIIVPROC)(GLuint texture, GLenum pname, const GLint * params);
typedef void (GLAD_API_PTR *PFNGLTEXTUREPARAMETERIUIVPROC)(GLuint texture, GLenum pname, const GLuint * params);
typedef void (GLAD_API_PTR *PFNGLTEXTUREPARAMETERFPROC)(GLuint texture, GLenum pname, GLfloat param);
typedef void (GLAD_API_PTR *PFNGLTEXTUREPARAMETERFVPROC)(GLuint texture, GLenum pname, const GLfloat * param);
typedef void (GLAD_API_PTR *PFNGLTEXTUREPARAMETERIPROC)(GLuint texture, GLenum pname, GLint param);
typedef void (GLAD_API_PTR *PFNGLTEXTUREPARAMETERIVPROC)(GLuint texture, GLenum pname, const GLint * param);
typedef void (GLAD_API_PTR *PFNGLTEXTURESTORAGE1DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAD_API_PTR *PFNGLTEXTURESTORAGE2DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAD_API_PTR *PFNGLTEXTURESTORAGE3DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAD_API_PTR *PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAD_API_PTR *PFNGLTEXTURESUBIMAGE1DPROC)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXTURESUBIMAGE2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXTURESUBIMAGE3DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
typedef void (GLAD_API_PTR *PFNGLTEXTUREVIEWPROC)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
typedef void (GLAD_API_PTR *PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC)(GLuint xfb, GLuint index, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC)(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAD_API_PTR *PFNGLTRANSFORMFEEDBACKVARYINGSPROC)(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);
typedef void (GLAD_API_PTR *PFNGLTRANSLATEDPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLTRANSLATEFPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1DPROC)(GLint location, GLdouble x);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1DVPROC)(GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1FARBPROC)(GLint location, GLfloat v0);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1FVARBPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1I64ARBPROC)(GLint location, GLint64 x);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1I64VARBPROC)(GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1IARBPROC)(GLint location, GLint v0);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1IVARBPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1UIPROC)(GLint location, GLuint v0);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1UI64ARBPROC)(GLint location, GLuint64 x);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1UI64VARBPROC)(GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM1UIVPROC)(GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2DPROC)(GLint location, GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2DVPROC)(GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2FARBPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2FVARBPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2I64ARBPROC)(GLint location, GLint64 x, GLint64 y);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2I64VARBPROC)(GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2IARBPROC)(GLint location, GLint v0, GLint v1);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2IVARBPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2UIPROC)(GLint location, GLuint v0, GLuint v1);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2UI64ARBPROC)(GLint location, GLuint64 x, GLuint64 y);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2UI64VARBPROC)(GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM2UIVPROC)(GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3DPROC)(GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3DVPROC)(GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3FARBPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3FVARBPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3I64ARBPROC)(GLint location, GLint64 x, GLint64 y, GLint64 z);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3I64VARBPROC)(GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3IARBPROC)(GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3IVARBPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3UI64ARBPROC)(GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3UI64VARBPROC)(GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM3UIVPROC)(GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4DPROC)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4DVPROC)(GLint location, GLsizei count, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4FARBPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4FVARBPROC)(GLint location, GLsizei count, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4I64ARBPROC)(GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4I64VARBPROC)(GLint location, GLsizei count, const GLint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4IARBPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4IVARBPROC)(GLint location, GLsizei count, const GLint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4UI64ARBPROC)(GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4UI64VARBPROC)(GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORM4UIVPROC)(GLint location, GLsizei count, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (GLAD_API_PTR *PFNGLUNIFORMHANDLEUI64ARBPROC)(GLint location, GLuint64 value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMHANDLEUI64VARBPROC)(GLint location, GLsizei count, const GLuint64 * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2FVARBPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2X3DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2X4DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3FVARBPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3X2DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3X4DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4FVARBPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4X2DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4X3DVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
typedef void (GLAD_API_PTR *PFNGLUNIFORMSUBROUTINESUIVPROC)(GLenum shadertype, GLsizei count, const GLuint * indices);
typedef GLboolean (GLAD_API_PTR *PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef GLboolean (GLAD_API_PTR *PFNGLUNMAPBUFFERARBPROC)(GLenum target);
typedef GLboolean (GLAD_API_PTR *PFNGLUNMAPNAMEDBUFFERPROC)(GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (GLAD_API_PTR *PFNGLUSEPROGRAMOBJECTARBPROC)(GLhandleARB programObj);
typedef void (GLAD_API_PTR *PFNGLUSEPROGRAMSTAGESPROC)(GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (GLAD_API_PTR *PFNGLVALIDATEPROGRAMPROC)(GLuint program);
typedef void (GLAD_API_PTR *PFNGLVALIDATEPROGRAMARBPROC)(GLhandleARB programObj);
typedef void (GLAD_API_PTR *PFNGLVALIDATEPROGRAMPIPELINEPROC)(GLuint pipeline);
typedef void (GLAD_API_PTR *PFNGLVERTEX2DPROC)(GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLVERTEX2DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX2FPROC)(GLfloat x, GLfloat y);
typedef void (GLAD_API_PTR *PFNGLVERTEX2FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX2IPROC)(GLint x, GLint y);
typedef void (GLAD_API_PTR *PFNGLVERTEX2IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX2SPROC)(GLshort x, GLshort y);
typedef void (GLAD_API_PTR *PFNGLVERTEX2SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX3DPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLVERTEX3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX3FPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLVERTEX3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX3IPROC)(GLint x, GLint y, GLint z);
typedef void (GLAD_API_PTR *PFNGLVERTEX3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX3SPROC)(GLshort x, GLshort y, GLshort z);
typedef void (GLAD_API_PTR *PFNGLVERTEX3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX4DPROC)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLVERTEX4DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX4FPROC)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAD_API_PTR *PFNGLVERTEX4FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX4IPROC)(GLint x, GLint y, GLint z, GLint w);
typedef void (GLAD_API_PTR *PFNGLVERTEX4IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEX4SPROC)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAD_API_PTR *PFNGLVERTEX4SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYATTRIBBINDINGPROC)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYATTRIBFORMATPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYATTRIBIFORMATPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYATTRIBLFORMATPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYBINDINGDIVISORPROC)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYELEMENTBUFFERPROC)(GLuint vaobj, GLuint buffer);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYVERTEXBUFFERPROC)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAD_API_PTR *PFNGLVERTEXARRAYVERTEXBUFFERSPROC)(GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1DPROC)(GLuint index, GLdouble x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1DARBPROC)(GLuint index, GLdouble x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1DVARBPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1FPROC)(GLuint index, GLfloat x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1FARBPROC)(GLuint index, GLfloat x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1FVPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1FVARBPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1SPROC)(GLuint index, GLshort x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1SARBPROC)(GLuint index, GLshort x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1SVPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1SVARBPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2DPROC)(GLuint index, GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2DARBPROC)(GLuint index, GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2DVARBPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2FPROC)(GLuint index, GLfloat x, GLfloat y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2FARBPROC)(GLuint index, GLfloat x, GLfloat y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2FVPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2FVARBPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2SPROC)(GLuint index, GLshort x, GLshort y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2SARBPROC)(GLuint index, GLshort x, GLshort y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2SVPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2SVARBPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3DARBPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3DVARBPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3FARBPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3FVPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3FVARBPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3SPROC)(GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3SARBPROC)(GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3SVPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3SVARBPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NBVPROC)(GLuint index, const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NBVARBPROC)(GLuint index, const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NIVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NIVARBPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NSVPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NSVARBPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUBPROC)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUBARBPROC)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUBVPROC)(GLuint index, const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUBVARBPROC)(GLuint index, const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUIVPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUIVARBPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUSVPROC)(GLuint index, const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUSVARBPROC)(GLuint index, const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4BVPROC)(GLuint index, const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4BVARBPROC)(GLuint index, const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4DARBPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4DVARBPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4FARBPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4FVPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4FVARBPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4IVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4IVARBPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4SPROC)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4SARBPROC)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4SVPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4SVARBPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4UBVPROC)(GLuint index, const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4UBVARBPROC)(GLuint index, const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4UIVPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4UIVARBPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4USVPROC)(GLuint index, const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4USVARBPROC)(GLuint index, const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBBINDINGPROC)(GLuint attribindex, GLuint bindingindex);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBDIVISORARBPROC)(GLuint index, GLuint divisor);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBFORMATPROC)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1IPROC)(GLuint index, GLint x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1IVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1UIPROC)(GLuint index, GLuint x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1UIVPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2IPROC)(GLuint index, GLint x, GLint y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2IVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2UIPROC)(GLuint index, GLuint x, GLuint y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2UIVPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3IPROC)(GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3IVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3UIVPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4BVPROC)(GLuint index, const GLbyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4IPROC)(GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4IVPROC)(GLuint index, const GLint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4SVPROC)(GLuint index, const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4UBVPROC)(GLuint index, const GLubyte * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4UIVPROC)(GLuint index, const GLuint * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4USVPROC)(GLuint index, const GLushort * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBIFORMATPROC)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL1DPROC)(GLuint index, GLdouble x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL1DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL1UI64ARBPROC)(GLuint index, GLuint64EXT x);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL1UI64VARBPROC)(GLuint index, const GLuint64EXT * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL2DPROC)(GLuint index, GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL2DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL3DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL3DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL4DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBL4DVPROC)(GLuint index, const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBLFORMATPROC)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBLPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP1UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP1UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP2UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP2UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP3UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP3UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP4UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP4UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBPOINTERARBPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLVERTEXBINDINGDIVISORPROC)(GLuint bindingindex, GLuint divisor);
typedef void (GLAD_API_PTR *PFNGLVERTEXBLENDARBPROC)(GLint count);
typedef void (GLAD_API_PTR *PFNGLVERTEXP2UIPROC)(GLenum type, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXP2UIVPROC)(GLenum type, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXP3UIPROC)(GLenum type, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXP3UIVPROC)(GLenum type, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXP4UIPROC)(GLenum type, GLuint value);
typedef void (GLAD_API_PTR *PFNGLVERTEXP4UIVPROC)(GLenum type, const GLuint * value);
typedef void (GLAD_API_PTR *PFNGLVERTEXPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_API_PTR *PFNGLVIEWPORTARRAYVPROC)(GLuint first, GLsizei count, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLVIEWPORTINDEXEDFPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (GLAD_API_PTR *PFNGLVIEWPORTINDEXEDFVPROC)(GLuint index, const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (GLAD_API_PTR *PFNGLWEIGHTPOINTERARBPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAD_API_PTR *PFNGLWEIGHTBVARBPROC)(GLint size, const GLbyte * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTDVARBPROC)(GLint size, const GLdouble * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTFVARBPROC)(GLint size, const GLfloat * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTIVARBPROC)(GLint size, const GLint * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTSVARBPROC)(GLint size, const GLshort * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTUBVARBPROC)(GLint size, const GLubyte * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTUIVARBPROC)(GLint size, const GLuint * weights);
typedef void (GLAD_API_PTR *PFNGLWEIGHTUSVARBPROC)(GLint size, const GLushort * weights);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2DPROC)(GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2DARBPROC)(GLdouble x, GLdouble y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2DVARBPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2FPROC)(GLfloat x, GLfloat y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2FARBPROC)(GLfloat x, GLfloat y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2FVARBPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2IPROC)(GLint x, GLint y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2IARBPROC)(GLint x, GLint y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2IVARBPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2SPROC)(GLshort x, GLshort y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2SARBPROC)(GLshort x, GLshort y);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS2SVARBPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3DPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3DARBPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3DVPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3DVARBPROC)(const GLdouble * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3FPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3FARBPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3FVPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3FVARBPROC)(const GLfloat * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3IPROC)(GLint x, GLint y, GLint z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3IARBPROC)(GLint x, GLint y, GLint z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3IVPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3IVARBPROC)(const GLint * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3SPROC)(GLshort x, GLshort y, GLshort z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3SARBPROC)(GLshort x, GLshort y, GLshort z);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3SVPROC)(const GLshort * v);
typedef void (GLAD_API_PTR *PFNGLWINDOWPOS3SVARBPROC)(const GLshort * v);

GLAD_API_CALL PFNGLACCUMPROC glad_glAccum;
#define glAccum glad_glAccum
GLAD_API_CALL PFNGLACTIVESHADERPROGRAMPROC glad_glActiveShaderProgram;
#define glActiveShaderProgram glad_glActiveShaderProgram
GLAD_API_CALL PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
#define glActiveTexture glad_glActiveTexture
GLAD_API_CALL PFNGLACTIVETEXTUREARBPROC glad_glActiveTextureARB;
#define glActiveTextureARB glad_glActiveTextureARB
GLAD_API_CALL PFNGLALPHAFUNCPROC glad_glAlphaFunc;
#define glAlphaFunc glad_glAlphaFunc
GLAD_API_CALL PFNGLARETEXTURESRESIDENTPROC glad_glAreTexturesResident;
#define glAreTexturesResident glad_glAreTexturesResident
GLAD_API_CALL PFNGLARRAYELEMENTPROC glad_glArrayElement;
#define glArrayElement glad_glArrayElement
GLAD_API_CALL PFNGLATTACHOBJECTARBPROC glad_glAttachObjectARB;
#define glAttachObjectARB glad_glAttachObjectARB
GLAD_API_CALL PFNGLATTACHSHADERPROC glad_glAttachShader;
#define glAttachShader glad_glAttachShader
GLAD_API_CALL PFNGLBEGINPROC glad_glBegin;
#define glBegin glad_glBegin
GLAD_API_CALL PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender;
#define glBeginConditionalRender glad_glBeginConditionalRender
GLAD_API_CALL PFNGLBEGINQUERYPROC glad_glBeginQuery;
#define glBeginQuery glad_glBeginQuery
GLAD_API_CALL PFNGLBEGINQUERYARBPROC glad_glBeginQueryARB;
#define glBeginQueryARB glad_glBeginQueryARB
GLAD_API_CALL PFNGLBEGINQUERYINDEXEDPROC glad_glBeginQueryIndexed;
#define glBeginQueryIndexed glad_glBeginQueryIndexed
GLAD_API_CALL PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback;
#define glBeginTransformFeedback glad_glBeginTransformFeedback
GLAD_API_CALL PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation;
#define glBindAttribLocation glad_glBindAttribLocation
GLAD_API_CALL PFNGLBINDATTRIBLOCATIONARBPROC glad_glBindAttribLocationARB;
#define glBindAttribLocationARB glad_glBindAttribLocationARB
GLAD_API_CALL PFNGLBINDBUFFERPROC glad_glBindBuffer;
#define glBindBuffer glad_glBindBuffer
GLAD_API_CALL PFNGLBINDBUFFERARBPROC glad_glBindBufferARB;
#define glBindBufferARB glad_glBindBufferARB
GLAD_API_CALL PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase;
#define glBindBufferBase glad_glBindBufferBase
GLAD_API_CALL PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange;
#define glBindBufferRange glad_glBindBufferRange
GLAD_API_CALL PFNGLBINDBUFFERSBASEPROC glad_glBindBuffersBase;
#define glBindBuffersBase glad_glBindBuffersBase
GLAD_API_CALL PFNGLBINDBUFFERSRANGEPROC glad_glBindBuffersRange;
#define glBindBuffersRange glad_glBindBuffersRange
GLAD_API_CALL PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation;
#define glBindFragDataLocation glad_glBindFragDataLocation
GLAD_API_CALL PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed;
#define glBindFragDataLocationIndexed glad_glBindFragDataLocationIndexed
GLAD_API_CALL PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
#define glBindFramebuffer glad_glBindFramebuffer
GLAD_API_CALL PFNGLBINDIMAGETEXTUREPROC glad_glBindImageTexture;
#define glBindImageTexture glad_glBindImageTexture
GLAD_API_CALL PFNGLBINDIMAGETEXTURESPROC glad_glBindImageTextures;
#define glBindImageTextures glad_glBindImageTextures
GLAD_API_CALL PFNGLBINDPROGRAMARBPROC glad_glBindProgramARB;
#define glBindProgramARB glad_glBindProgramARB
GLAD_API_CALL PFNGLBINDPROGRAMPIPELINEPROC glad_glBindProgramPipeline;
#define glBindProgramPipeline glad_glBindProgramPipeline
GLAD_API_CALL PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;
#define glBindRenderbuffer glad_glBindRenderbuffer
GLAD_API_CALL PFNGLBINDSAMPLERPROC glad_glBindSampler;
#define glBindSampler glad_glBindSampler
GLAD_API_CALL PFNGLBINDSAMPLERSPROC glad_glBindSamplers;
#define glBindSamplers glad_glBindSamplers
GLAD_API_CALL PFNGLBINDTEXTUREPROC glad_glBindTexture;
#define glBindTexture glad_glBindTexture
GLAD_API_CALL PFNGLBINDTEXTUREUNITPROC glad_glBindTextureUnit;
#define glBindTextureUnit glad_glBindTextureUnit
GLAD_API_CALL PFNGLBINDTEXTURESPROC glad_glBindTextures;
#define glBindTextures glad_glBindTextures
GLAD_API_CALL PFNGLBINDTRANSFORMFEEDBACKPROC glad_glBindTransformFeedback;
#define glBindTransformFeedback glad_glBindTransformFeedback
GLAD_API_CALL PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
#define glBindVertexArray glad_glBindVertexArray
GLAD_API_CALL PFNGLBINDVERTEXBUFFERPROC glad_glBindVertexBuffer;
#define glBindVertexBuffer glad_glBindVertexBuffer
GLAD_API_CALL PFNGLBINDVERTEXBUFFERSPROC glad_glBindVertexBuffers;
#define glBindVertexBuffers glad_glBindVertexBuffers
GLAD_API_CALL PFNGLBITMAPPROC glad_glBitmap;
#define glBitmap glad_glBitmap
GLAD_API_CALL PFNGLBLENDBARRIERPROC glad_glBlendBarrier;
#define glBlendBarrier glad_glBlendBarrier
GLAD_API_CALL PFNGLBLENDBARRIERKHRPROC glad_glBlendBarrierKHR;
#define glBlendBarrierKHR glad_glBlendBarrierKHR
GLAD_API_CALL PFNGLBLENDCOLORPROC glad_glBlendColor;
#define glBlendColor glad_glBlendColor
GLAD_API_CALL PFNGLBLENDEQUATIONPROC glad_glBlendEquation;
#define glBlendEquation glad_glBlendEquation
GLAD_API_CALL PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate;
#define glBlendEquationSeparate glad_glBlendEquationSeparate
GLAD_API_CALL PFNGLBLENDEQUATIONSEPARATEIPROC glad_glBlendEquationSeparatei;
#define glBlendEquationSeparatei glad_glBlendEquationSeparatei
GLAD_API_CALL PFNGLBLENDEQUATIONSEPARATEIARBPROC glad_glBlendEquationSeparateiARB;
#define glBlendEquationSeparateiARB glad_glBlendEquationSeparateiARB
GLAD_API_CALL PFNGLBLENDEQUATIONIPROC glad_glBlendEquationi;
#define glBlendEquationi glad_glBlendEquationi
GLAD_API_CALL PFNGLBLENDEQUATIONIARBPROC glad_glBlendEquationiARB;
#define glBlendEquationiARB glad_glBlendEquationiARB
GLAD_API_CALL PFNGLBLENDFUNCPROC glad_glBlendFunc;
#define glBlendFunc glad_glBlendFunc
GLAD_API_CALL PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate;
#define glBlendFuncSeparate glad_glBlendFuncSeparate
GLAD_API_CALL PFNGLBLENDFUNCSEPARATEIPROC glad_glBlendFuncSeparatei;
#define glBlendFuncSeparatei glad_glBlendFuncSeparatei
GLAD_API_CALL PFNGLBLENDFUNCSEPARATEIARBPROC glad_glBlendFuncSeparateiARB;
#define glBlendFuncSeparateiARB glad_glBlendFuncSeparateiARB
GLAD_API_CALL PFNGLBLENDFUNCIPROC glad_glBlendFunci;
#define glBlendFunci glad_glBlendFunci
GLAD_API_CALL PFNGLBLENDFUNCIARBPROC glad_glBlendFunciARB;
#define glBlendFunciARB glad_glBlendFunciARB
GLAD_API_CALL PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer;
#define glBlitFramebuffer glad_glBlitFramebuffer
GLAD_API_CALL PFNGLBLITNAMEDFRAMEBUFFERPROC glad_glBlitNamedFramebuffer;
#define glBlitNamedFramebuffer glad_glBlitNamedFramebuffer
GLAD_API_CALL PFNGLBUFFERDATAPROC glad_glBufferData;
#define glBufferData glad_glBufferData
GLAD_API_CALL PFNGLBUFFERDATAARBPROC glad_glBufferDataARB;
#define glBufferDataARB glad_glBufferDataARB
GLAD_API_CALL PFNGLBUFFERPAGECOMMITMENTARBPROC glad_glBufferPageCommitmentARB;
#define glBufferPageCommitmentARB glad_glBufferPageCommitmentARB
GLAD_API_CALL PFNGLBUFFERSTORAGEPROC glad_glBufferStorage;
#define glBufferStorage glad_glBufferStorage
GLAD_API_CALL PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
#define glBufferSubData glad_glBufferSubData
GLAD_API_CALL PFNGLBUFFERSUBDATAARBPROC glad_glBufferSubDataARB;
#define glBufferSubDataARB glad_glBufferSubDataARB
GLAD_API_CALL PFNGLCALLLISTPROC glad_glCallList;
#define glCallList glad_glCallList
GLAD_API_CALL PFNGLCALLLISTSPROC glad_glCallLists;
#define glCallLists glad_glCallLists
GLAD_API_CALL PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
GLAD_API_CALL PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC glad_glCheckNamedFramebufferStatus;
#define glCheckNamedFramebufferStatus glad_glCheckNamedFramebufferStatus
GLAD_API_CALL PFNGLCLAMPCOLORPROC glad_glClampColor;
#define glClampColor glad_glClampColor
GLAD_API_CALL PFNGLCLAMPCOLORARBPROC glad_glClampColorARB;
#define glClampColorARB glad_glClampColorARB
GLAD_API_CALL PFNGLCLEARPROC glad_glClear;
#define glClear glad_glClear
GLAD_API_CALL PFNGLCLEARACCUMPROC glad_glClearAccum;
#define glClearAccum glad_glClearAccum
GLAD_API_CALL PFNGLCLEARBUFFERDATAPROC glad_glClearBufferData;
#define glClearBufferData glad_glClearBufferData
GLAD_API_CALL PFNGLCLEARBUFFERSUBDATAPROC glad_glClearBufferSubData;
#define glClearBufferSubData glad_glClearBufferSubData
GLAD_API_CALL PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi;
#define glClearBufferfi glad_glClearBufferfi
GLAD_API_CALL PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv;
#define glClearBufferfv glad_glClearBufferfv
GLAD_API_CALL PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv;
#define glClearBufferiv glad_glClearBufferiv
GLAD_API_CALL PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv;
#define glClearBufferuiv glad_glClearBufferuiv
GLAD_API_CALL PFNGLCLEARCOLORPROC glad_glClearColor;
#define glClearColor glad_glClearColor
GLAD_API_CALL PFNGLCLEARDEPTHPROC glad_glClearDepth;
#define glClearDepth glad_glClearDepth
GLAD_API_CALL PFNGLCLEARDEPTHFPROC glad_glClearDepthf;
#define glClearDepthf glad_glClearDepthf
GLAD_API_CALL PFNGLCLEARINDEXPROC glad_glClearIndex;
#define glClearIndex glad_glClearIndex
GLAD_API_CALL PFNGLCLEARNAMEDBUFFERDATAPROC glad_glClearNamedBufferData;
#define glClearNamedBufferData glad_glClearNamedBufferData
GLAD_API_CALL PFNGLCLEARNAMEDBUFFERSUBDATAPROC glad_glClearNamedBufferSubData;
#define glClearNamedBufferSubData glad_glClearNamedBufferSubData
GLAD_API_CALL PFNGLCLEARNAMEDFRAMEBUFFERFIPROC glad_glClearNamedFramebufferfi;
#define glClearNamedFramebufferfi glad_glClearNamedFramebufferfi
GLAD_API_CALL PFNGLCLEARNAMEDFRAMEBUFFERFVPROC glad_glClearNamedFramebufferfv;
#define glClearNamedFramebufferfv glad_glClearNamedFramebufferfv
GLAD_API_CALL PFNGLCLEARNAMEDFRAMEBUFFERIVPROC glad_glClearNamedFramebufferiv;
#define glClearNamedFramebufferiv glad_glClearNamedFramebufferiv
GLAD_API_CALL PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC glad_glClearNamedFramebufferuiv;
#define glClearNamedFramebufferuiv glad_glClearNamedFramebufferuiv
GLAD_API_CALL PFNGLCLEARSTENCILPROC glad_glClearStencil;
#define glClearStencil glad_glClearStencil
GLAD_API_CALL PFNGLCLEARTEXIMAGEPROC glad_glClearTexImage;
#define glClearTexImage glad_glClearTexImage
GLAD_API_CALL PFNGLCLEARTEXSUBIMAGEPROC glad_glClearTexSubImage;
#define glClearTexSubImage glad_glClearTexSubImage
GLAD_API_CALL PFNGLCLIENTACTIVETEXTUREPROC glad_glClientActiveTexture;
#define glClientActiveTexture glad_glClientActiveTexture
GLAD_API_CALL PFNGLCLIENTACTIVETEXTUREARBPROC glad_glClientActiveTextureARB;
#define glClientActiveTextureARB glad_glClientActiveTextureARB
GLAD_API_CALL PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync;
#define glClientWaitSync glad_glClientWaitSync
GLAD_API_CALL PFNGLCLIPCONTROLPROC glad_glClipControl;
#define glClipControl glad_glClipControl
GLAD_API_CALL PFNGLCLIPPLANEPROC glad_glClipPlane;
#define glClipPlane glad_glClipPlane
GLAD_API_CALL PFNGLCOLOR3BPROC glad_glColor3b;
#define glColor3b glad_glColor3b
GLAD_API_CALL PFNGLCOLOR3BVPROC glad_glColor3bv;
#define glColor3bv glad_glColor3bv
GLAD_API_CALL PFNGLCOLOR3DPROC glad_glColor3d;
#define glColor3d glad_glColor3d
GLAD_API_CALL PFNGLCOLOR3DVPROC glad_glColor3dv;
#define glColor3dv glad_glColor3dv
GLAD_API_CALL PFNGLCOLOR3FPROC glad_glColor3f;
#define glColor3f glad_glColor3f
GLAD_API_CALL PFNGLCOLOR3FVPROC glad_glColor3fv;
#define glColor3fv glad_glColor3fv
GLAD_API_CALL PFNGLCOLOR3IPROC glad_glColor3i;
#define glColor3i glad_glColor3i
GLAD_API_CALL PFNGLCOLOR3IVPROC glad_glColor3iv;
#define glColor3iv glad_glColor3iv
GLAD_API_CALL PFNGLCOLOR3SPROC glad_glColor3s;
#define glColor3s glad_glColor3s
GLAD_API_CALL PFNGLCOLOR3SVPROC glad_glColor3sv;
#define glColor3sv glad_glColor3sv
GLAD_API_CALL PFNGLCOLOR3UBPROC glad_glColor3ub;
#define glColor3ub glad_glColor3ub
GLAD_API_CALL PFNGLCOLOR3UBVPROC glad_glColor3ubv;
#define glColor3ubv glad_glColor3ubv
GLAD_API_CALL PFNGLCOLOR3UIPROC glad_glColor3ui;
#define glColor3ui glad_glColor3ui
GLAD_API_CALL PFNGLCOLOR3UIVPROC glad_glColor3uiv;
#define glColor3uiv glad_glColor3uiv
GLAD_API_CALL PFNGLCOLOR3USPROC glad_glColor3us;
#define glColor3us glad_glColor3us
GLAD_API_CALL PFNGLCOLOR3USVPROC glad_glColor3usv;
#define glColor3usv glad_glColor3usv
GLAD_API_CALL PFNGLCOLOR4BPROC glad_glColor4b;
#define glColor4b glad_glColor4b
GLAD_API_CALL PFNGLCOLOR4BVPROC glad_glColor4bv;
#define glColor4bv glad_glColor4bv
GLAD_API_CALL PFNGLCOLOR4DPROC glad_glColor4d;
#define glColor4d glad_glColor4d
GLAD_API_CALL PFNGLCOLOR4DVPROC glad_glColor4dv;
#define glColor4dv glad_glColor4dv
GLAD_API_CALL PFNGLCOLOR4FPROC glad_glColor4f;
#define glColor4f glad_glColor4f
GLAD_API_CALL PFNGLCOLOR4FVPROC glad_glColor4fv;
#define glColor4fv glad_glColor4fv
GLAD_API_CALL PFNGLCOLOR4IPROC glad_glColor4i;
#define glColor4i glad_glColor4i
GLAD_API_CALL PFNGLCOLOR4IVPROC glad_glColor4iv;
#define glColor4iv glad_glColor4iv
GLAD_API_CALL PFNGLCOLOR4SPROC glad_glColor4s;
#define glColor4s glad_glColor4s
GLAD_API_CALL PFNGLCOLOR4SVPROC glad_glColor4sv;
#define glColor4sv glad_glColor4sv
GLAD_API_CALL PFNGLCOLOR4UBPROC glad_glColor4ub;
#define glColor4ub glad_glColor4ub
GLAD_API_CALL PFNGLCOLOR4UBVPROC glad_glColor4ubv;
#define glColor4ubv glad_glColor4ubv
GLAD_API_CALL PFNGLCOLOR4UIPROC glad_glColor4ui;
#define glColor4ui glad_glColor4ui
GLAD_API_CALL PFNGLCOLOR4UIVPROC glad_glColor4uiv;
#define glColor4uiv glad_glColor4uiv
GLAD_API_CALL PFNGLCOLOR4USPROC glad_glColor4us;
#define glColor4us glad_glColor4us
GLAD_API_CALL PFNGLCOLOR4USVPROC glad_glColor4usv;
#define glColor4usv glad_glColor4usv
GLAD_API_CALL PFNGLCOLORMASKPROC glad_glColorMask;
#define glColorMask glad_glColorMask
GLAD_API_CALL PFNGLCOLORMASKIPROC glad_glColorMaski;
#define glColorMaski glad_glColorMaski
GLAD_API_CALL PFNGLCOLORMATERIALPROC glad_glColorMaterial;
#define glColorMaterial glad_glColorMaterial
GLAD_API_CALL PFNGLCOLORP3UIPROC glad_glColorP3ui;
#define glColorP3ui glad_glColorP3ui
GLAD_API_CALL PFNGLCOLORP3UIVPROC glad_glColorP3uiv;
#define glColorP3uiv glad_glColorP3uiv
GLAD_API_CALL PFNGLCOLORP4UIPROC glad_glColorP4ui;
#define glColorP4ui glad_glColorP4ui
GLAD_API_CALL PFNGLCOLORP4UIVPROC glad_glColorP4uiv;
#define glColorP4uiv glad_glColorP4uiv
GLAD_API_CALL PFNGLCOLORPOINTERPROC glad_glColorPointer;
#define glColorPointer glad_glColorPointer
GLAD_API_CALL PFNGLCOLORSUBTABLEPROC glad_glColorSubTable;
#define glColorSubTable glad_glColorSubTable
GLAD_API_CALL PFNGLCOLORTABLEPROC glad_glColorTable;
#define glColorTable glad_glColorTable
GLAD_API_CALL PFNGLCOLORTABLEPARAMETERFVPROC glad_glColorTableParameterfv;
#define glColorTableParameterfv glad_glColorTableParameterfv
GLAD_API_CALL PFNGLCOLORTABLEPARAMETERIVPROC glad_glColorTableParameteriv;
#define glColorTableParameteriv glad_glColorTableParameteriv
GLAD_API_CALL PFNGLCOMPILESHADERPROC glad_glCompileShader;
#define glCompileShader glad_glCompileShader
GLAD_API_CALL PFNGLCOMPILESHADERARBPROC glad_glCompileShaderARB;
#define glCompileShaderARB glad_glCompileShaderARB
GLAD_API_CALL PFNGLCOMPILESHADERINCLUDEARBPROC glad_glCompileShaderIncludeARB;
#define glCompileShaderIncludeARB glad_glCompileShaderIncludeARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D;
#define glCompressedTexImage1D glad_glCompressedTexImage1D
GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glad_glCompressedTexImage1DARB;
#define glCompressedTexImage1DARB glad_glCompressedTexImage1DARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D;
#define glCompressedTexImage2D glad_glCompressedTexImage2D
GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glad_glCompressedTexImage2DARB;
#define glCompressedTexImage2DARB glad_glCompressedTexImage2DARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D;
#define glCompressedTexImage3D glad_glCompressedTexImage3D
GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glad_glCompressedTexImage3DARB;
#define glCompressedTexImage3DARB glad_glCompressedTexImage3DARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D;
#define glCompressedTexSubImage1D glad_glCompressedTexSubImage1D
GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glad_glCompressedTexSubImage1DARB;
#define glCompressedTexSubImage1DARB glad_glCompressedTexSubImage1DARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D;
#define glCompressedTexSubImage2D glad_glCompressedTexSubImage2D
GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glad_glCompressedTexSubImage2DARB;
#define glCompressedTexSubImage2DARB glad_glCompressedTexSubImage2DARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D;
#define glCompressedTexSubImage3D glad_glCompressedTexSubImage3D
GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glad_glCompressedTexSubImage3DARB;
#define glCompressedTexSubImage3DARB glad_glCompressedTexSubImage3DARB
GLAD_API_CALL PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC glad_glCompressedTextureSubImage1D;
#define glCompressedTextureSubImage1D glad_glCompressedTextureSubImage1D
GLAD_API_CALL PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC glad_glCompressedTextureSubImage2D;
#define glCompressedTextureSubImage2D glad_glCompressedTextureSubImage2D
GLAD_API_CALL PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC glad_glCompressedTextureSubImage3D;
#define glCompressedTextureSubImage3D glad_glCompressedTextureSubImage3D
GLAD_API_CALL PFNGLCONVOLUTIONFILTER1DPROC glad_glConvolutionFilter1D;
#define glConvolutionFilter1D glad_glConvolutionFilter1D
GLAD_API_CALL PFNGLCONVOLUTIONFILTER2DPROC glad_glConvolutionFilter2D;
#define glConvolutionFilter2D glad_glConvolutionFilter2D
GLAD_API_CALL PFNGLCONVOLUTIONPARAMETERFPROC glad_glConvolutionParameterf;
#define glConvolutionParameterf glad_glConvolutionParameterf
GLAD_API_CALL PFNGLCONVOLUTIONPARAMETERFVPROC glad_glConvolutionParameterfv;
#define glConvolutionParameterfv glad_glConvolutionParameterfv
GLAD_API_CALL PFNGLCONVOLUTIONPARAMETERIPROC glad_glConvolutionParameteri;
#define glConvolutionParameteri glad_glConvolutionParameteri
GLAD_API_CALL PFNGLCONVOLUTIONPARAMETERIVPROC glad_glConvolutionParameteriv;
#define glConvolutionParameteriv glad_glConvolutionParameteriv
GLAD_API_CALL PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData;
#define glCopyBufferSubData glad_glCopyBufferSubData
GLAD_API_CALL PFNGLCOPYCOLORSUBTABLEPROC glad_glCopyColorSubTable;
#define glCopyColorSubTable glad_glCopyColorSubTable
GLAD_API_CALL PFNGLCOPYCOLORTABLEPROC glad_glCopyColorTable;
#define glCopyColorTable glad_glCopyColorTable
GLAD_API_CALL PFNGLCOPYCONVOLUTIONFILTER1DPROC glad_glCopyConvolutionFilter1D;
#define glCopyConvolutionFilter1D glad_glCopyConvolutionFilter1D
GLAD_API_CALL PFNGLCOPYCONVOLUTIONFILTER2DPROC glad_glCopyConvolutionFilter2D;
#define glCopyConvolutionFilter2D glad_glCopyConvolutionFilter2D
GLAD_API_CALL PFNGLCOPYIMAGESUBDATAPROC glad_glCopyImageSubData;
#define glCopyImageSubData glad_glCopyImageSubData
GLAD_API_CALL PFNGLCOPYNAMEDBUFFERSUBDATAPROC glad_glCopyNamedBufferSubData;
#define glCopyNamedBufferSubData glad_glCopyNamedBufferSubData
GLAD_API_CALL PFNGLCOPYPIXELSPROC glad_glCopyPixels;
#define glCopyPixels glad_glCopyPixels
GLAD_API_CALL PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D;
#define glCopyTexImage1D glad_glCopyTexImage1D
GLAD_API_CALL PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D;
#define glCopyTexImage2D glad_glCopyTexImage2D
GLAD_API_CALL PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D;
#define glCopyTexSubImage1D glad_glCopyTexSubImage1D
GLAD_API_CALL PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D;
#define glCopyTexSubImage2D glad_glCopyTexSubImage2D
GLAD_API_CALL PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D;
#define glCopyTexSubImage3D glad_glCopyTexSubImage3D
GLAD_API_CALL PFNGLCOPYTEXTURESUBIMAGE1DPROC glad_glCopyTextureSubImage1D;
#define glCopyTextureSubImage1D glad_glCopyTextureSubImage1D
GLAD_API_CALL PFNGLCOPYTEXTURESUBIMAGE2DPROC glad_glCopyTextureSubImage2D;
#define glCopyTextureSubImage2D glad_glCopyTextureSubImage2D
GLAD_API_CALL PFNGLCOPYTEXTURESUBIMAGE3DPROC glad_glCopyTextureSubImage3D;
#define glCopyTextureSubImage3D glad_glCopyTextureSubImage3D
GLAD_API_CALL PFNGLCREATEBUFFERSPROC glad_glCreateBuffers;
#define glCreateBuffers glad_glCreateBuffers
GLAD_API_CALL PFNGLCREATEFRAMEBUFFERSPROC glad_glCreateFramebuffers;
#define glCreateFramebuffers glad_glCreateFramebuffers
GLAD_API_CALL PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
#define glCreateProgram glad_glCreateProgram
GLAD_API_CALL PFNGLCREATEPROGRAMOBJECTARBPROC glad_glCreateProgramObjectARB;
#define glCreateProgramObjectARB glad_glCreateProgramObjectARB
GLAD_API_CALL PFNGLCREATEPROGRAMPIPELINESPROC glad_glCreateProgramPipelines;
#define glCreateProgramPipelines glad_glCreateProgramPipelines
GLAD_API_CALL PFNGLCREATEQUERIESPROC glad_glCreateQueries;
#define glCreateQueries glad_glCreateQueries
GLAD_API_CALL PFNGLCREATERENDERBUFFERSPROC glad_glCreateRenderbuffers;
#define glCreateRenderbuffers glad_glCreateRenderbuffers
GLAD_API_CALL PFNGLCREATESAMPLERSPROC glad_glCreateSamplers;
#define glCreateSamplers glad_glCreateSamplers
GLAD_API_CALL PFNGLCREATESHADERPROC glad_glCreateShader;
#define glCreateShader glad_glCreateShader
GLAD_API_CALL PFNGLCREATESHADEROBJECTARBPROC glad_glCreateShaderObjectARB;
#define glCreateShaderObjectARB glad_glCreateShaderObjectARB
GLAD_API_CALL PFNGLCREATESHADERPROGRAMVPROC glad_glCreateShaderProgramv;
#define glCreateShaderProgramv glad_glCreateShaderProgramv
GLAD_API_CALL PFNGLCREATESYNCFROMCLEVENTARBPROC glad_glCreateSyncFromCLeventARB;
#define glCreateSyncFromCLeventARB glad_glCreateSyncFromCLeventARB
GLAD_API_CALL PFNGLCREATETEXTURESPROC glad_glCreateTextures;
#define glCreateTextures glad_glCreateTextures
GLAD_API_CALL PFNGLCREATETRANSFORMFEEDBACKSPROC glad_glCreateTransformFeedbacks;
#define glCreateTransformFeedbacks glad_glCreateTransformFeedbacks
GLAD_API_CALL PFNGLCREATEVERTEXARRAYSPROC glad_glCreateVertexArrays;
#define glCreateVertexArrays glad_glCreateVertexArrays
GLAD_API_CALL PFNGLCULLFACEPROC glad_glCullFace;
#define glCullFace glad_glCullFace
GLAD_API_CALL PFNGLCURRENTPALETTEMATRIXARBPROC glad_glCurrentPaletteMatrixARB;
#define glCurrentPaletteMatrixARB glad_glCurrentPaletteMatrixARB
GLAD_API_CALL PFNGLDEBUGMESSAGECALLBACKPROC glad_glDebugMessageCallback;
#define glDebugMessageCallback glad_glDebugMessageCallback
GLAD_API_CALL PFNGLDEBUGMESSAGECALLBACKARBPROC glad_glDebugMessageCallbackARB;
#define glDebugMessageCallbackARB glad_glDebugMessageCallbackARB
GLAD_API_CALL PFNGLDEBUGMESSAGECONTROLPROC glad_glDebugMessageControl;
#define glDebugMessageControl glad_glDebugMessageControl
GLAD_API_CALL PFNGLDEBUGMESSAGECONTROLARBPROC glad_glDebugMessageControlARB;
#define glDebugMessageControlARB glad_glDebugMessageControlARB
GLAD_API_CALL PFNGLDEBUGMESSAGEINSERTPROC glad_glDebugMessageInsert;
#define glDebugMessageInsert glad_glDebugMessageInsert
GLAD_API_CALL PFNGLDEBUGMESSAGEINSERTARBPROC glad_glDebugMessageInsertARB;
#define glDebugMessageInsertARB glad_glDebugMessageInsertARB
GLAD_API_CALL PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
#define glDeleteBuffers glad_glDeleteBuffers
GLAD_API_CALL PFNGLDELETEBUFFERSARBPROC glad_glDeleteBuffersARB;
#define glDeleteBuffersARB glad_glDeleteBuffersARB
GLAD_API_CALL PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
#define glDeleteFramebuffers glad_glDeleteFramebuffers
GLAD_API_CALL PFNGLDELETELISTSPROC glad_glDeleteLists;
#define glDeleteLists glad_glDeleteLists
GLAD_API_CALL PFNGLDELETENAMEDSTRINGARBPROC glad_glDeleteNamedStringARB;
#define glDeleteNamedStringARB glad_glDeleteNamedStringARB
GLAD_API_CALL PFNGLDELETEOBJECTARBPROC glad_glDeleteObjectARB;
#define glDeleteObjectARB glad_glDeleteObjectARB
GLAD_API_CALL PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
#define glDeleteProgram glad_glDeleteProgram
GLAD_API_CALL PFNGLDELETEPROGRAMPIPELINESPROC glad_glDeleteProgramPipelines;
#define glDeleteProgramPipelines glad_glDeleteProgramPipelines
GLAD_API_CALL PFNGLDELETEPROGRAMSARBPROC glad_glDeleteProgramsARB;
#define glDeleteProgramsARB glad_glDeleteProgramsARB
GLAD_API_CALL PFNGLDELETEQUERIESPROC glad_glDeleteQueries;
#define glDeleteQueries glad_glDeleteQueries
GLAD_API_CALL PFNGLDELETEQUERIESARBPROC glad_glDeleteQueriesARB;
#define glDeleteQueriesARB glad_glDeleteQueriesARB
GLAD_API_CALL PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;
#define glDeleteRenderbuffers glad_glDeleteRenderbuffers
GLAD_API_CALL PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers;
#define glDeleteSamplers glad_glDeleteSamplers
GLAD_API_CALL PFNGLDELETESHADERPROC glad_glDeleteShader;
#define glDeleteShader glad_glDeleteShader
GLAD_API_CALL PFNGLDELETESYNCPROC glad_glDeleteSync;
#define glDeleteSync glad_glDeleteSync
GLAD_API_CALL PFNGLDELETETEXTURESPROC glad_glDeleteTextures;
#define glDeleteTextures glad_glDeleteTextures
GLAD_API_CALL PFNGLDELETETRANSFORMFEEDBACKSPROC glad_glDeleteTransformFeedbacks;
#define glDeleteTransformFeedbacks glad_glDeleteTransformFeedbacks
GLAD_API_CALL PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;
#define glDeleteVertexArrays glad_glDeleteVertexArrays
GLAD_API_CALL PFNGLDEPTHFUNCPROC glad_glDepthFunc;
#define glDepthFunc glad_glDepthFunc
GLAD_API_CALL PFNGLDEPTHMASKPROC glad_glDepthMask;
#define glDepthMask glad_glDepthMask
GLAD_API_CALL PFNGLDEPTHRANGEPROC glad_glDepthRange;
#define glDepthRange glad_glDepthRange
GLAD_API_CALL PFNGLDEPTHRANGEARRAYDVNVPROC glad_glDepthRangeArraydvNV;
#define glDepthRangeArraydvNV glad_glDepthRangeArraydvNV
GLAD_API_CALL PFNGLDEPTHRANGEARRAYVPROC glad_glDepthRangeArrayv;
#define glDepthRangeArrayv glad_glDepthRangeArrayv
GLAD_API_CALL PFNGLDEPTHRANGEINDEXEDPROC glad_glDepthRangeIndexed;
#define glDepthRangeIndexed glad_glDepthRangeIndexed
GLAD_API_CALL PFNGLDEPTHRANGEINDEXEDDNVPROC glad_glDepthRangeIndexeddNV;
#define glDepthRangeIndexeddNV glad_glDepthRangeIndexeddNV
GLAD_API_CALL PFNGLDEPTHRANGEFPROC glad_glDepthRangef;
#define glDepthRangef glad_glDepthRangef
GLAD_API_CALL PFNGLDETACHOBJECTARBPROC glad_glDetachObjectARB;
#define glDetachObjectARB glad_glDetachObjectARB
GLAD_API_CALL PFNGLDETACHSHADERPROC glad_glDetachShader;
#define glDetachShader glad_glDetachShader
GLAD_API_CALL PFNGLDISABLEPROC glad_glDisable;
#define glDisable glad_glDisable
GLAD_API_CALL PFNGLDISABLECLIENTSTATEPROC glad_glDisableClientState;
#define glDisableClientState glad_glDisableClientState
GLAD_API_CALL PFNGLDISABLEVERTEXARRAYATTRIBPROC glad_glDisableVertexArrayAttrib;
#define glDisableVertexArrayAttrib glad_glDisableVertexArrayAttrib
GLAD_API_CALL PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
GLAD_API_CALL PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glad_glDisableVertexAttribArrayARB;
#define glDisableVertexAttribArrayARB glad_glDisableVertexAttribArrayARB
GLAD_API_CALL PFNGLDISABLEIPROC glad_glDisablei;
#define glDisablei glad_glDisablei
GLAD_API_CALL PFNGLDISPATCHCOMPUTEPROC glad_glDispatchCompute;
#define glDispatchCompute glad_glDispatchCompute
GLAD_API_CALL PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC glad_glDispatchComputeGroupSizeARB;
#define glDispatchComputeGroupSizeARB glad_glDispatchComputeGroupSizeARB
GLAD_API_CALL PFNGLDISPATCHCOMPUTEINDIRECTPROC glad_glDispatchComputeIndirect;
#define glDispatchComputeIndirect glad_glDispatchComputeIndirect
GLAD_API_CALL PFNGLDRAWARRAYSPROC glad_glDrawArrays;
#define glDrawArrays glad_glDrawArrays
GLAD_API_CALL PFNGLDRAWARRAYSINDIRECTPROC glad_glDrawArraysIndirect;
#define glDrawArraysIndirect glad_glDrawArraysIndirect
GLAD_API_CALL PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced;
#define glDrawArraysInstanced glad_glDrawArraysInstanced
GLAD_API_CALL PFNGLDRAWARRAYSINSTANCEDARBPROC glad_glDrawArraysInstancedARB;
#define glDrawArraysInstancedARB glad_glDrawArraysInstancedARB
GLAD_API_CALL PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glad_glDrawArraysInstancedBaseInstance;
#define glDrawArraysInstancedBaseInstance glad_glDrawArraysInstancedBaseInstance
GLAD_API_CALL PFNGLDRAWBUFFERPROC glad_glDrawBuffer;
#define glDrawBuffer glad_glDrawBuffer
GLAD_API_CALL PFNGLDRAWBUFFERSPROC glad_glDrawBuffers;
#define glDrawBuffers glad_glDrawBuffers
GLAD_API_CALL PFNGLDRAWBUFFERSARBPROC glad_glDrawBuffersARB;
#define glDrawBuffersARB glad_glDrawBuffersARB
GLAD_API_CALL PFNGLDRAWELEMENTSPROC glad_glDrawElements;
#define glDrawElements glad_glDrawElements
GLAD_API_CALL PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex;
#define glDrawElementsBaseVertex glad_glDrawElementsBaseVertex
GLAD_API_CALL PFNGLDRAWELEMENTSINDIRECTPROC glad_glDrawElementsIndirect;
#define glDrawElementsIndirect glad_glDrawElementsIndirect
GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced;
#define glDrawElementsInstanced glad_glDrawElementsInstanced
GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDARBPROC glad_glDrawElementsInstancedARB;
#define glDrawElementsInstancedARB glad_glDrawElementsInstancedARB
GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glad_glDrawElementsInstancedBaseInstance;
#define glDrawElementsInstancedBaseInstance glad_glDrawElementsInstancedBaseInstance
GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex;
#define glDrawElementsInstancedBaseVertex glad_glDrawElementsInstancedBaseVertex
GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glad_glDrawElementsInstancedBaseVertexBaseInstance;
#define glDrawElementsInstancedBaseVertexBaseInstance glad_glDrawElementsInstancedBaseVertexBaseInstance
GLAD_API_CALL PFNGLDRAWPIXELSPROC glad_glDrawPixels;
#define glDrawPixels glad_glDrawPixels
GLAD_API_CALL PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements;
#define glDrawRangeElements glad_glDrawRangeElements
GLAD_API_CALL PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex;
#define glDrawRangeElementsBaseVertex glad_glDrawRangeElementsBaseVertex
GLAD_API_CALL PFNGLDRAWTRANSFORMFEEDBACKPROC glad_glDrawTransformFeedback;
#define glDrawTransformFeedback glad_glDrawTransformFeedback
GLAD_API_CALL PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glad_glDrawTransformFeedbackInstanced;
#define glDrawTransformFeedbackInstanced glad_glDrawTransformFeedbackInstanced
GLAD_API_CALL PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glad_glDrawTransformFeedbackStream;
#define glDrawTransformFeedbackStream glad_glDrawTransformFeedbackStream
GLAD_API_CALL PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glad_glDrawTransformFeedbackStreamInstanced;
#define glDrawTransformFeedbackStreamInstanced glad_glDrawTransformFeedbackStreamInstanced
GLAD_API_CALL PFNGLEDGEFLAGPROC glad_glEdgeFlag;
#define glEdgeFlag glad_glEdgeFlag
GLAD_API_CALL PFNGLEDGEFLAGPOINTERPROC glad_glEdgeFlagPointer;
#define glEdgeFlagPointer glad_glEdgeFlagPointer
GLAD_API_CALL PFNGLEDGEFLAGVPROC glad_glEdgeFlagv;
#define glEdgeFlagv glad_glEdgeFlagv
GLAD_API_CALL PFNGLENABLEPROC glad_glEnable;
#define glEnable glad_glEnable
GLAD_API_CALL PFNGLENABLECLIENTSTATEPROC glad_glEnableClientState;
#define glEnableClientState glad_glEnableClientState
GLAD_API_CALL PFNGLENABLEVERTEXARRAYATTRIBPROC glad_glEnableVertexArrayAttrib;
#define glEnableVertexArrayAttrib glad_glEnableVertexArrayAttrib
GLAD_API_CALL PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
GLAD_API_CALL PFNGLENABLEVERTEXATTRIBARRAYARBPROC glad_glEnableVertexAttribArrayARB;
#define glEnableVertexAttribArrayARB glad_glEnableVertexAttribArrayARB
GLAD_API_CALL PFNGLENABLEIPROC glad_glEnablei;
#define glEnablei glad_glEnablei
GLAD_API_CALL PFNGLENDPROC glad_glEnd;
#define glEnd glad_glEnd
GLAD_API_CALL PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender;
#define glEndConditionalRender glad_glEndConditionalRender
GLAD_API_CALL PFNGLENDLISTPROC glad_glEndList;
#define glEndList glad_glEndList
GLAD_API_CALL PFNGLENDQUERYPROC glad_glEndQuery;
#define glEndQuery glad_glEndQuery
GLAD_API_CALL PFNGLENDQUERYARBPROC glad_glEndQueryARB;
#define glEndQueryARB glad_glEndQueryARB
GLAD_API_CALL PFNGLENDQUERYINDEXEDPROC glad_glEndQueryIndexed;
#define glEndQueryIndexed glad_glEndQueryIndexed
GLAD_API_CALL PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback;
#define glEndTransformFeedback glad_glEndTransformFeedback
GLAD_API_CALL PFNGLEVALCOORD1DPROC glad_glEvalCoord1d;
#define glEvalCoord1d glad_glEvalCoord1d
GLAD_API_CALL PFNGLEVALCOORD1DVPROC glad_glEvalCoord1dv;
#define glEvalCoord1dv glad_glEvalCoord1dv
GLAD_API_CALL PFNGLEVALCOORD1FPROC glad_glEvalCoord1f;
#define glEvalCoord1f glad_glEvalCoord1f
GLAD_API_CALL PFNGLEVALCOORD1FVPROC glad_glEvalCoord1fv;
#define glEvalCoord1fv glad_glEvalCoord1fv
GLAD_API_CALL PFNGLEVALCOORD2DPROC glad_glEvalCoord2d;
#define glEvalCoord2d glad_glEvalCoord2d
GLAD_API_CALL PFNGLEVALCOORD2DVPROC glad_glEvalCoord2dv;
#define glEvalCoord2dv glad_glEvalCoord2dv
GLAD_API_CALL PFNGLEVALCOORD2FPROC glad_glEvalCoord2f;
#define glEvalCoord2f glad_glEvalCoord2f
GLAD_API_CALL PFNGLEVALCOORD2FVPROC glad_glEvalCoord2fv;
#define glEvalCoord2fv glad_glEvalCoord2fv
GLAD_API_CALL PFNGLEVALMESH1PROC glad_glEvalMesh1;
#define glEvalMesh1 glad_glEvalMesh1
GLAD_API_CALL PFNGLEVALMESH2PROC glad_glEvalMesh2;
#define glEvalMesh2 glad_glEvalMesh2
GLAD_API_CALL PFNGLEVALPOINT1PROC glad_glEvalPoint1;
#define glEvalPoint1 glad_glEvalPoint1
GLAD_API_CALL PFNGLEVALPOINT2PROC glad_glEvalPoint2;
#define glEvalPoint2 glad_glEvalPoint2
GLAD_API_CALL PFNGLEVALUATEDEPTHVALUESARBPROC glad_glEvaluateDepthValuesARB;
#define glEvaluateDepthValuesARB glad_glEvaluateDepthValuesARB
GLAD_API_CALL PFNGLFEEDBACKBUFFERPROC glad_glFeedbackBuffer;
#define glFeedbackBuffer glad_glFeedbackBuffer
GLAD_API_CALL PFNGLFENCESYNCPROC glad_glFenceSync;
#define glFenceSync glad_glFenceSync
GLAD_API_CALL PFNGLFINISHPROC glad_glFinish;
#define glFinish glad_glFinish
GLAD_API_CALL PFNGLFLUSHPROC glad_glFlush;
#define glFlush glad_glFlush
GLAD_API_CALL PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange;
#define glFlushMappedBufferRange glad_glFlushMappedBufferRange
GLAD_API_CALL PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glad_glFlushMappedNamedBufferRange;
#define glFlushMappedNamedBufferRange glad_glFlushMappedNamedBufferRange
GLAD_API_CALL PFNGLFOGCOORDPOINTERPROC glad_glFogCoordPointer;
#define glFogCoordPointer glad_glFogCoordPointer
GLAD_API_CALL PFNGLFOGCOORDDPROC glad_glFogCoordd;
#define glFogCoordd glad_glFogCoordd
GLAD_API_CALL PFNGLFOGCOORDDVPROC glad_glFogCoorddv;
#define glFogCoorddv glad_glFogCoorddv
GLAD_API_CALL PFNGLFOGCOORDFPROC glad_glFogCoordf;
#define glFogCoordf glad_glFogCoordf
GLAD_API_CALL PFNGLFOGCOORDFVPROC glad_glFogCoordfv;
#define glFogCoordfv glad_glFogCoordfv
GLAD_API_CALL PFNGLFOGFPROC glad_glFogf;
#define glFogf glad_glFogf
GLAD_API_CALL PFNGLFOGFVPROC glad_glFogfv;
#define glFogfv glad_glFogfv
GLAD_API_CALL PFNGLFOGIPROC glad_glFogi;
#define glFogi glad_glFogi
GLAD_API_CALL PFNGLFOGIVPROC glad_glFogiv;
#define glFogiv glad_glFogiv
GLAD_API_CALL PFNGLFRAMEBUFFERPARAMETERIPROC glad_glFramebufferParameteri;
#define glFramebufferParameteri glad_glFramebufferParameteri
GLAD_API_CALL PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
#define glFramebufferRenderbuffer glad_glFramebufferRenderbuffer
GLAD_API_CALL PFNGLFRAMEBUFFERSAMPLELOCATIONSFVARBPROC glad_glFramebufferSampleLocationsfvARB;
#define glFramebufferSampleLocationsfvARB glad_glFramebufferSampleLocationsfvARB
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture;
#define glFramebufferTexture glad_glFramebufferTexture
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D;
#define glFramebufferTexture1D glad_glFramebufferTexture1D
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
#define glFramebufferTexture2D glad_glFramebufferTexture2D
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D;
#define glFramebufferTexture3D glad_glFramebufferTexture3D
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTUREARBPROC glad_glFramebufferTextureARB;
#define glFramebufferTextureARB glad_glFramebufferTextureARB
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTUREFACEARBPROC glad_glFramebufferTextureFaceARB;
#define glFramebufferTextureFaceARB glad_glFramebufferTextureFaceARB
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer;
#define glFramebufferTextureLayer glad_glFramebufferTextureLayer
GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURELAYERARBPROC glad_glFramebufferTextureLayerARB;
#define glFramebufferTextureLayerARB glad_glFramebufferTextureLayerARB
GLAD_API_CALL PFNGLFRONTFACEPROC glad_glFrontFace;
#define glFrontFace glad_glFrontFace
GLAD_API_CALL PFNGLFRUSTUMPROC glad_glFrustum;
#define glFrustum glad_glFrustum
GLAD_API_CALL PFNGLGENBUFFERSPROC glad_glGenBuffers;
#define glGenBuffers glad_glGenBuffers
GLAD_API_CALL PFNGLGENBUFFERSARBPROC glad_glGenBuffersARB;
#define glGenBuffersARB glad_glGenBuffersARB
GLAD_API_CALL PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
#define glGenFramebuffers glad_glGenFramebuffers
GLAD_API_CALL PFNGLGENLISTSPROC glad_glGenLists;
#define glGenLists glad_glGenLists
GLAD_API_CALL PFNGLGENPROGRAMPIPELINESPROC glad_glGenProgramPipelines;
#define glGenProgramPipelines glad_glGenProgramPipelines
GLAD_API_CALL PFNGLGENPROGRAMSARBPROC glad_glGenProgramsARB;
#define glGenProgramsARB glad_glGenProgramsARB
GLAD_API_CALL PFNGLGENQUERIESPROC glad_glGenQueries;
#define glGenQueries glad_glGenQueries
GLAD_API_CALL PFNGLGENQUERIESARBPROC glad_glGenQueriesARB;
#define glGenQueriesARB glad_glGenQueriesARB
GLAD_API_CALL PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;
#define glGenRenderbuffers glad_glGenRenderbuffers
GLAD_API_CALL PFNGLGENSAMPLERSPROC glad_glGenSamplers;
#define glGenSamplers glad_glGenSamplers
GLAD_API_CALL PFNGLGENTEXTURESPROC glad_glGenTextures;
#define glGenTextures glad_glGenTextures
GLAD_API_CALL PFNGLGENTRANSFORMFEEDBACKSPROC glad_glGenTransformFeedbacks;
#define glGenTransformFeedbacks glad_glGenTransformFeedbacks
GLAD_API_CALL PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
#define glGenVertexArrays glad_glGenVertexArrays
GLAD_API_CALL PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
#define glGenerateMipmap glad_glGenerateMipmap
GLAD_API_CALL PFNGLGENERATETEXTUREMIPMAPPROC glad_glGenerateTextureMipmap;
#define glGenerateTextureMipmap glad_glGenerateTextureMipmap
GLAD_API_CALL PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glad_glGetActiveAtomicCounterBufferiv;
#define glGetActiveAtomicCounterBufferiv glad_glGetActiveAtomicCounterBufferiv
GLAD_API_CALL PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib;
#define glGetActiveAttrib glad_glGetActiveAttrib
GLAD_API_CALL PFNGLGETACTIVEATTRIBARBPROC glad_glGetActiveAttribARB;
#define glGetActiveAttribARB glad_glGetActiveAttribARB
GLAD_API_CALL PFNGLGETACTIVESUBROUTINENAMEPROC glad_glGetActiveSubroutineName;
#define glGetActiveSubroutineName glad_glGetActiveSubroutineName
GLAD_API_CALL PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glad_glGetActiveSubroutineUniformName;
#define glGetActiveSubroutineUniformName glad_glGetActiveSubroutineUniformName
GLAD_API_CALL PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glad_glGetActiveSubroutineUniformiv;
#define glGetActiveSubroutineUniformiv glad_glGetActiveSubroutineUniformiv
GLAD_API_CALL PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform;
#define glGetActiveUniform glad_glGetActiveUniform
GLAD_API_CALL PFNGLGETACTIVEUNIFORMARBPROC glad_glGetActiveUniformARB;
#define glGetActiveUniformARB glad_glGetActiveUniformARB
GLAD_API_CALL PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName;
#define glGetActiveUniformBlockName glad_glGetActiveUniformBlockName
GLAD_API_CALL PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv;
#define glGetActiveUniformBlockiv glad_glGetActiveUniformBlockiv
GLAD_API_CALL PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName;
#define glGetActiveUniformName glad_glGetActiveUniformName
GLAD_API_CALL PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv;
#define glGetActiveUniformsiv glad_glGetActiveUniformsiv
GLAD_API_CALL PFNGLGETATTACHEDOBJECTSARBPROC glad_glGetAttachedObjectsARB;
#define glGetAttachedObjectsARB glad_glGetAttachedObjectsARB
GLAD_API_CALL PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders;
#define glGetAttachedShaders glad_glGetAttachedShaders
GLAD_API_CALL PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation;
#define glGetAttribLocation glad_glGetAttribLocation
GLAD_API_CALL PFNGLGETATTRIBLOCATIONARBPROC glad_glGetAttribLocationARB;
#define glGetAttribLocationARB glad_glGetAttribLocationARB
GLAD_API_CALL PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v;
#define glGetBooleani_v glad_glGetBooleani_v
GLAD_API_CALL PFNGLGETBOOLEANVPROC glad_glGetBooleanv;
#define glGetBooleanv glad_glGetBooleanv
GLAD_API_CALL PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v;
#define glGetBufferParameteri64v glad_glGetBufferParameteri64v
GLAD_API_CALL PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv;
#define glGetBufferParameteriv glad_glGetBufferParameteriv
GLAD_API_CALL PFNGLGETBUFFERPARAMETERIVARBPROC glad_glGetBufferParameterivARB;
#define glGetBufferParameterivARB glad_glGetBufferParameterivARB
GLAD_API_CALL PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv;
#define glGetBufferPointerv glad_glGetBufferPointerv
GLAD_API_CALL PFNGLGETBUFFERPOINTERVARBPROC glad_glGetBufferPointervARB;
#define glGetBufferPointervARB glad_glGetBufferPointervARB
GLAD_API_CALL PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData;
#define glGetBufferSubData glad_glGetBufferSubData
GLAD_API_CALL PFNGLGETBUFFERSUBDATAARBPROC glad_glGetBufferSubDataARB;
#define glGetBufferSubDataARB glad_glGetBufferSubDataARB
GLAD_API_CALL PFNGLGETCLIPPLANEPROC glad_glGetClipPlane;
#define glGetClipPlane glad_glGetClipPlane
GLAD_API_CALL PFNGLGETCOLORTABLEPROC glad_glGetColorTable;
#define glGetColorTable glad_glGetColorTable
GLAD_API_CALL PFNGLGETCOLORTABLEPARAMETERFVPROC glad_glGetColorTableParameterfv;
#define glGetColorTableParameterfv glad_glGetColorTableParameterfv
GLAD_API_CALL PFNGLGETCOLORTABLEPARAMETERIVPROC glad_glGetColorTableParameteriv;
#define glGetColorTableParameteriv glad_glGetColorTableParameteriv
GLAD_API_CALL PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage;
#define glGetCompressedTexImage glad_glGetCompressedTexImage
GLAD_API_CALL PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glad_glGetCompressedTexImageARB;
#define glGetCompressedTexImageARB glad_glGetCompressedTexImageARB
GLAD_API_CALL PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC glad_glGetCompressedTextureImage;
#define glGetCompressedTextureImage glad_glGetCompressedTextureImage
GLAD_API_CALL PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC glad_glGetCompressedTextureSubImage;
#define glGetCompressedTextureSubImage glad_glGetCompressedTextureSubImage
GLAD_API_CALL PFNGLGETCONVOLUTIONFILTERPROC glad_glGetConvolutionFilter;
#define glGetConvolutionFilter glad_glGetConvolutionFilter
GLAD_API_CALL PFNGLGETCONVOLUTIONPARAMETERFVPROC glad_glGetConvolutionParameterfv;
#define glGetConvolutionParameterfv glad_glGetConvolutionParameterfv
GLAD_API_CALL PFNGLGETCONVOLUTIONPARAMETERIVPROC glad_glGetConvolutionParameteriv;
#define glGetConvolutionParameteriv glad_glGetConvolutionParameteriv
GLAD_API_CALL PFNGLGETDEBUGMESSAGELOGPROC glad_glGetDebugMessageLog;
#define glGetDebugMessageLog glad_glGetDebugMessageLog
GLAD_API_CALL PFNGLGETDEBUGMESSAGELOGARBPROC glad_glGetDebugMessageLogARB;
#define glGetDebugMessageLogARB glad_glGetDebugMessageLogARB
GLAD_API_CALL PFNGLGETDOUBLEI_VPROC glad_glGetDoublei_v;
#define glGetDoublei_v glad_glGetDoublei_v
GLAD_API_CALL PFNGLGETDOUBLEVPROC glad_glGetDoublev;
#define glGetDoublev glad_glGetDoublev
GLAD_API_CALL PFNGLGETERRORPROC glad_glGetError;
#define glGetError glad_glGetError
GLAD_API_CALL PFNGLGETFLOATI_VPROC glad_glGetFloati_v;
#define glGetFloati_v glad_glGetFloati_v
GLAD_API_CALL PFNGLGETFLOATVPROC glad_glGetFloatv;
#define glGetFloatv glad_glGetFloatv
GLAD_API_CALL PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex;
#define glGetFragDataIndex glad_glGetFragDataIndex
GLAD_API_CALL PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation;
#define glGetFragDataLocation glad_glGetFragDataLocation
GLAD_API_CALL PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv;
#define glGetFramebufferAttachmentParameteriv glad_glGetFramebufferAttachmentParameteriv
GLAD_API_CALL PFNGLGETFRAMEBUFFERPARAMETERIVPROC glad_glGetFramebufferParameteriv;
#define glGetFramebufferParameteriv glad_glGetFramebufferParameteriv
GLAD_API_CALL PFNGLGETGRAPHICSRESETSTATUSPROC glad_glGetGraphicsResetStatus;
#define glGetGraphicsResetStatus glad_glGetGraphicsResetStatus
GLAD_API_CALL PFNGLGETGRAPHICSRESETSTATUSARBPROC glad_glGetGraphicsResetStatusARB;
#define glGetGraphicsResetStatusARB glad_glGetGraphicsResetStatusARB
GLAD_API_CALL PFNGLGETHANDLEARBPROC glad_glGetHandleARB;
#define glGetHandleARB glad_glGetHandleARB
GLAD_API_CALL PFNGLGETHISTOGRAMPROC glad_glGetHistogram;
#define glGetHistogram glad_glGetHistogram
GLAD_API_CALL PFNGLGETHISTOGRAMPARAMETERFVPROC glad_glGetHistogramParameterfv;
#define glGetHistogramParameterfv glad_glGetHistogramParameterfv
GLAD_API_CALL PFNGLGETHISTOGRAMPARAMETERIVPROC glad_glGetHistogramParameteriv;
#define glGetHistogramParameteriv glad_glGetHistogramParameteriv
GLAD_API_CALL PFNGLGETIMAGEHANDLEARBPROC glad_glGetImageHandleARB;
#define glGetImageHandleARB glad_glGetImageHandleARB
GLAD_API_CALL PFNGLGETINFOLOGARBPROC glad_glGetInfoLogARB;
#define glGetInfoLogARB glad_glGetInfoLogARB
GLAD_API_CALL PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v;
#define glGetInteger64i_v glad_glGetInteger64i_v
GLAD_API_CALL PFNGLGETINTEGER64VPROC glad_glGetInteger64v;
#define glGetInteger64v glad_glGetInteger64v
GLAD_API_CALL PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v;
#define glGetIntegeri_v glad_glGetIntegeri_v
GLAD_API_CALL PFNGLGETINTEGERVPROC glad_glGetIntegerv;
#define glGetIntegerv glad_glGetIntegerv
GLAD_API_CALL PFNGLGETINTERNALFORMATI64VPROC glad_glGetInternalformati64v;
#define glGetInternalformati64v glad_glGetInternalformati64v
GLAD_API_CALL PFNGLGETINTERNALFORMATIVPROC glad_glGetInternalformativ;
#define glGetInternalformativ glad_glGetInternalformativ
GLAD_API_CALL PFNGLGETLIGHTFVPROC glad_glGetLightfv;
#define glGetLightfv glad_glGetLightfv
GLAD_API_CALL PFNGLGETLIGHTIVPROC glad_glGetLightiv;
#define glGetLightiv glad_glGetLightiv
GLAD_API_CALL PFNGLGETMAPDVPROC glad_glGetMapdv;
#define glGetMapdv glad_glGetMapdv
GLAD_API_CALL PFNGLGETMAPFVPROC glad_glGetMapfv;
#define glGetMapfv glad_glGetMapfv
GLAD_API_CALL PFNGLGETMAPIVPROC glad_glGetMapiv;
#define glGetMapiv glad_glGetMapiv
GLAD_API_CALL PFNGLGETMATERIALFVPROC glad_glGetMaterialfv;
#define glGetMaterialfv glad_glGetMaterialfv
GLAD_API_CALL PFNGLGETMATERIALIVPROC glad_glGetMaterialiv;
#define glGetMaterialiv glad_glGetMaterialiv
GLAD_API_CALL PFNGLGETMINMAXPROC glad_glGetMinmax;
#define glGetMinmax glad_glGetMinmax
GLAD_API_CALL PFNGLGETMINMAXPARAMETERFVPROC glad_glGetMinmaxParameterfv;
#define glGetMinmaxParameterfv glad_glGetMinmaxParameterfv
GLAD_API_CALL PFNGLGETMINMAXPARAMETERIVPROC glad_glGetMinmaxParameteriv;
#define glGetMinmaxParameteriv glad_glGetMinmaxParameteriv
GLAD_API_CALL PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv;
#define glGetMultisamplefv glad_glGetMultisamplefv
GLAD_API_CALL PFNGLGETNAMEDBUFFERPARAMETERI64VPROC glad_glGetNamedBufferParameteri64v;
#define glGetNamedBufferParameteri64v glad_glGetNamedBufferParameteri64v
GLAD_API_CALL PFNGLGETNAMEDBUFFERPARAMETERIVPROC glad_glGetNamedBufferParameteriv;
#define glGetNamedBufferParameteriv glad_glGetNamedBufferParameteriv
GLAD_API_CALL PFNGLGETNAMEDBUFFERPOINTERVPROC glad_glGetNamedBufferPointerv;
#define glGetNamedBufferPointerv glad_glGetNamedBufferPointerv
GLAD_API_CALL PFNGLGETNAMEDBUFFERSUBDATAPROC glad_glGetNamedBufferSubData;
#define glGetNamedBufferSubData glad_glGetNamedBufferSubData
GLAD_API_CALL PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetNamedFramebufferAttachmentParameteriv;
#define glGetNamedFramebufferAttachmentParameteriv glad_glGetNamedFramebufferAttachmentParameteriv
GLAD_API_CALL PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC glad_glGetNamedFramebufferParameteriv;
#define glGetNamedFramebufferParameteriv glad_glGetNamedFramebufferParameteriv
GLAD_API_CALL PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC glad_glGetNamedRenderbufferParameteriv;
#define glGetNamedRenderbufferParameteriv glad_glGetNamedRenderbufferParameteriv
GLAD_API_CALL PFNGLGETNAMEDSTRINGARBPROC glad_glGetNamedStringARB;
#define glGetNamedStringARB glad_glGetNamedStringARB
GLAD_API_CALL PFNGLGETNAMEDSTRINGIVARBPROC glad_glGetNamedStringivARB;
#define glGetNamedStringivARB glad_glGetNamedStringivARB
GLAD_API_CALL PFNGLGETOBJECTLABELPROC glad_glGetObjectLabel;
#define glGetObjectLabel glad_glGetObjectLabel
GLAD_API_CALL PFNGLGETOBJECTPARAMETERFVARBPROC glad_glGetObjectParameterfvARB;
#define glGetObjectParameterfvARB glad_glGetObjectParameterfvARB
GLAD_API_CALL PFNGLGETOBJECTPARAMETERIVARBPROC glad_glGetObjectParameterivARB;
#define glGetObjectParameterivARB glad_glGetObjectParameterivARB
GLAD_API_CALL PFNGLGETOBJECTPTRLABELPROC glad_glGetObjectPtrLabel;
#define glGetObjectPtrLabel glad_glGetObjectPtrLabel
GLAD_API_CALL PFNGLGETPIXELMAPFVPROC glad_glGetPixelMapfv;
#define glGetPixelMapfv glad_glGetPixelMapfv
GLAD_API_CALL PFNGLGETPIXELMAPUIVPROC glad_glGetPixelMapuiv;
#define glGetPixelMapuiv glad_glGetPixelMapuiv
GLAD_API_CALL PFNGLGETPIXELMAPUSVPROC glad_glGetPixelMapusv;
#define glGetPixelMapusv glad_glGetPixelMapusv
GLAD_API_CALL PFNGLGETPOINTERVPROC glad_glGetPointerv;
#define glGetPointerv glad_glGetPointerv
GLAD_API_CALL PFNGLGETPOLYGONSTIPPLEPROC glad_glGetPolygonStipple;
#define glGetPolygonStipple glad_glGetPolygonStipple
GLAD_API_CALL PFNGLGETPROGRAMBINARYPROC glad_glGetProgramBinary;
#define glGetProgramBinary glad_glGetProgramBinary
GLAD_API_CALL PFNGLGETPROGRAMENVPARAMETERDVARBPROC glad_glGetProgramEnvParameterdvARB;
#define glGetProgramEnvParameterdvARB glad_glGetProgramEnvParameterdvARB
GLAD_API_CALL PFNGLGETPROGRAMENVPARAMETERFVARBPROC glad_glGetProgramEnvParameterfvARB;
#define glGetProgramEnvParameterfvARB glad_glGetProgramEnvParameterfvARB
GLAD_API_CALL PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
#define glGetProgramInfoLog glad_glGetProgramInfoLog
GLAD_API_CALL PFNGLGETPROGRAMINTERFACEIVPROC glad_glGetProgramInterfaceiv;
#define glGetProgramInterfaceiv glad_glGetProgramInterfaceiv
GLAD_API_CALL PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glad_glGetProgramLocalParameterdvARB;
#define glGetProgramLocalParameterdvARB glad_glGetProgramLocalParameterdvARB
GLAD_API_CALL PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glad_glGetProgramLocalParameterfvARB;
#define glGetProgramLocalParameterfvARB glad_glGetProgramLocalParameterfvARB
GLAD_API_CALL PFNGLGETPROGRAMPIPELINEINFOLOGPROC glad_glGetProgramPipelineInfoLog;
#define glGetProgramPipelineInfoLog glad_glGetProgramPipelineInfoLog
GLAD_API_CALL PFNGLGETPROGRAMPIPELINEIVPROC glad_glGetProgramPipelineiv;
#define glGetProgramPipelineiv glad_glGetProgramPipelineiv
GLAD_API_CALL PFNGLGETPROGRAMRESOURCEINDEXPROC glad_glGetProgramResourceIndex;
#define glGetProgramResourceIndex glad_glGetProgramResourceIndex
GLAD_API_CALL PFNGLGETPROGRAMRESOURCELOCATIONPROC glad_glGetProgramResourceLocation;
#define glGetProgramResourceLocation glad_glGetProgramResourceLocation
GLAD_API_CALL PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glad_glGetProgramResourceLocationIndex;
#define glGetProgramResourceLocationIndex glad_glGetProgramResourceLocationIndex
GLAD_API_CALL PFNGLGETPROGRAMRESOURCENAMEPROC glad_glGetProgramResourceName;
#define glGetProgramResourceName glad_glGetProgramResourceName
GLAD_API_CALL PFNGLGETPROGRAMRESOURCEIVPROC glad_glGetProgramResourceiv;
#define glGetProgramResourceiv glad_glGetProgramResourceiv
GLAD_API_CALL PFNGLGETPROGRAMSTAGEIVPROC glad_glGetProgramStageiv;
#define glGetProgramStageiv glad_glGetProgramStageiv
GLAD_API_CALL PFNGLGETPROGRAMSTRINGARBPROC glad_glGetProgramStringARB;
#define glGetProgramStringARB glad_glGetProgramStringARB
GLAD_API_CALL PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
#define glGetProgramiv glad_glGetProgramiv
GLAD_API_CALL PFNGLGETPROGRAMIVARBPROC glad_glGetProgramivARB;
#define glGetProgramivARB glad_glGetProgramivARB
GLAD_API_CALL PFNGLGETQUERYBUFFEROBJECTI64VPROC glad_glGetQueryBufferObjecti64v;
#define glGetQueryBufferObjecti64v glad_glGetQueryBufferObjecti64v
GLAD_API_CALL PFNGLGETQUERYBUFFEROBJECTIVPROC glad_glGetQueryBufferObjectiv;
#define glGetQueryBufferObjectiv glad_glGetQueryBufferObjectiv
GLAD_API_CALL PFNGLGETQUERYBUFFEROBJECTUI64VPROC glad_glGetQueryBufferObjectui64v;
#define glGetQueryBufferObjectui64v glad_glGetQueryBufferObjectui64v
GLAD_API_CALL PFNGLGETQUERYBUFFEROBJECTUIVPROC glad_glGetQueryBufferObjectuiv;
#define glGetQueryBufferObjectuiv glad_glGetQueryBufferObjectuiv
GLAD_API_CALL PFNGLGETQUERYINDEXEDIVPROC glad_glGetQueryIndexediv;
#define glGetQueryIndexediv glad_glGetQueryIndexediv
GLAD_API_CALL PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v;
#define glGetQueryObjecti64v glad_glGetQueryObjecti64v
GLAD_API_CALL PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv;
#define glGetQueryObjectiv glad_glGetQueryObjectiv
GLAD_API_CALL PFNGLGETQUERYOBJECTIVARBPROC glad_glGetQueryObjectivARB;
#define glGetQueryObjectivARB glad_glGetQueryObjectivARB
GLAD_API_CALL PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v;
#define glGetQueryObjectui64v glad_glGetQueryObjectui64v
GLAD_API_CALL PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv;
#define glGetQueryObjectuiv glad_glGetQueryObjectuiv
GLAD_API_CALL PFNGLGETQUERYOBJECTUIVARBPROC glad_glGetQueryObjectuivARB;
#define glGetQueryObjectuivARB glad_glGetQueryObjectuivARB
GLAD_API_CALL PFNGLGETQUERYIVPROC glad_glGetQueryiv;
#define glGetQueryiv glad_glGetQueryiv
GLAD_API_CALL PFNGLGETQUERYIVARBPROC glad_glGetQueryivARB;
#define glGetQueryivARB glad_glGetQueryivARB
GLAD_API_CALL PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv;
#define glGetRenderbufferParameteriv glad_glGetRenderbufferParameteriv
GLAD_API_CALL PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv;
#define glGetSamplerParameterIiv glad_glGetSamplerParameterIiv
GLAD_API_CALL PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv;
#define glGetSamplerParameterIuiv glad_glGetSamplerParameterIuiv
GLAD_API_CALL PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv;
#define glGetSamplerParameterfv glad_glGetSamplerParameterfv
GLAD_API_CALL PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv;
#define glGetSamplerParameteriv glad_glGetSamplerParameteriv
GLAD_API_CALL PFNGLGETSEPARABLEFILTERPROC glad_glGetSeparableFilter;
#define glGetSeparableFilter glad_glGetSeparableFilter
GLAD_API_CALL PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
#define glGetShaderInfoLog glad_glGetShaderInfoLog
GLAD_API_CALL PFNGLGETSHADERPRECISIONFORMATPROC glad_glGetShaderPrecisionFormat;
#define glGetShaderPrecisionFormat glad_glGetShaderPrecisionFormat
GLAD_API_CALL PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource;
#define glGetShaderSource glad_glGetShaderSource
GLAD_API_CALL PFNGLGETSHADERSOURCEARBPROC glad_glGetShaderSourceARB;
#define glGetShaderSourceARB glad_glGetShaderSourceARB
GLAD_API_CALL PFNGLGETSHADERIVPROC glad_glGetShaderiv;
#define glGetShaderiv glad_glGetShaderiv
GLAD_API_CALL PFNGLGETSTRINGPROC glad_glGetString;
#define glGetString glad_glGetString
GLAD_API_CALL PFNGLGETSTRINGIPROC glad_glGetStringi;
#define glGetStringi glad_glGetStringi
GLAD_API_CALL PFNGLGETSUBROUTINEINDEXPROC glad_glGetSubroutineIndex;
#define glGetSubroutineIndex glad_glGetSubroutineIndex
GLAD_API_CALL PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glad_glGetSubroutineUniformLocation;
#define glGetSubroutineUniformLocation glad_glGetSubroutineUniformLocation
GLAD_API_CALL PFNGLGETSYNCIVPROC glad_glGetSynciv;
#define glGetSynciv glad_glGetSynciv
GLAD_API_CALL PFNGLGETTEXENVFVPROC glad_glGetTexEnvfv;
#define glGetTexEnvfv glad_glGetTexEnvfv
GLAD_API_CALL PFNGLGETTEXENVIVPROC glad_glGetTexEnviv;
#define glGetTexEnviv glad_glGetTexEnviv
GLAD_API_CALL PFNGLGETTEXGENDVPROC glad_glGetTexGendv;
#define glGetTexGendv glad_glGetTexGendv
GLAD_API_CALL PFNGLGETTEXGENFVPROC glad_glGetTexGenfv;
#define glGetTexGenfv glad_glGetTexGenfv
GLAD_API_CALL PFNGLGETTEXGENIVPROC glad_glGetTexGeniv;
#define glGetTexGeniv glad_glGetTexGeniv
GLAD_API_CALL PFNGLGETTEXIMAGEPROC glad_glGetTexImage;
#define glGetTexImage glad_glGetTexImage
GLAD_API_CALL PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv;
#define glGetTexLevelParameterfv glad_glGetTexLevelParameterfv
GLAD_API_CALL PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv;
#define glGetTexLevelParameteriv glad_glGetTexLevelParameteriv
GLAD_API_CALL PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv;
#define glGetTexParameterIiv glad_glGetTexParameterIiv
GLAD_API_CALL PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv;
#define glGetTexParameterIuiv glad_glGetTexParameterIuiv
GLAD_API_CALL PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv;
#define glGetTexParameterfv glad_glGetTexParameterfv
GLAD_API_CALL PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv;
#define glGetTexParameteriv glad_glGetTexParameteriv
GLAD_API_CALL PFNGLGETTEXTUREHANDLEARBPROC glad_glGetTextureHandleARB;
#define glGetTextureHandleARB glad_glGetTextureHandleARB
GLAD_API_CALL PFNGLGETTEXTUREIMAGEPROC glad_glGetTextureImage;
#define glGetTextureImage glad_glGetTextureImage
GLAD_API_CALL PFNGLGETTEXTURELEVELPARAMETERFVPROC glad_glGetTextureLevelParameterfv;
#define glGetTextureLevelParameterfv glad_glGetTextureLevelParameterfv
GLAD_API_CALL PFNGLGETTEXTURELEVELPARAMETERIVPROC glad_glGetTextureLevelParameteriv;
#define glGetTextureLevelParameteriv glad_glGetTextureLevelParameteriv
GLAD_API_CALL PFNGLGETTEXTUREPARAMETERIIVPROC glad_glGetTextureParameterIiv;
#define glGetTextureParameterIiv glad_glGetTextureParameterIiv
GLAD_API_CALL PFNGLGETTEXTUREPARAMETERIUIVPROC glad_glGetTextureParameterIuiv;
#define glGetTextureParameterIuiv glad_glGetTextureParameterIuiv
GLAD_API_CALL PFNGLGETTEXTUREPARAMETERFVPROC glad_glGetTextureParameterfv;
#define glGetTextureParameterfv glad_glGetTextureParameterfv
GLAD_API_CALL PFNGLGETTEXTUREPARAMETERIVPROC glad_glGetTextureParameteriv;
#define glGetTextureParameteriv glad_glGetTextureParameteriv
GLAD_API_CALL PFNGLGETTEXTURESAMPLERHANDLEARBPROC glad_glGetTextureSamplerHandleARB;
#define glGetTextureSamplerHandleARB glad_glGetTextureSamplerHandleARB
GLAD_API_CALL PFNGLGETTEXTURESUBIMAGEPROC glad_glGetTextureSubImage;
#define glGetTextureSubImage glad_glGetTextureSubImage
GLAD_API_CALL PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying;
#define glGetTransformFeedbackVarying glad_glGetTransformFeedbackVarying
GLAD_API_CALL PFNGLGETTRANSFORMFEEDBACKI64_VPROC glad_glGetTransformFeedbacki64_v;
#define glGetTransformFeedbacki64_v glad_glGetTransformFeedbacki64_v
GLAD_API_CALL PFNGLGETTRANSFORMFEEDBACKI_VPROC glad_glGetTransformFeedbacki_v;
#define glGetTransformFeedbacki_v glad_glGetTransformFeedbacki_v
GLAD_API_CALL PFNGLGETTRANSFORMFEEDBACKIVPROC glad_glGetTransformFeedbackiv;
#define glGetTransformFeedbackiv glad_glGetTransformFeedbackiv
GLAD_API_CALL PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex;
#define glGetUniformBlockIndex glad_glGetUniformBlockIndex
GLAD_API_CALL PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices;
#define glGetUniformIndices glad_glGetUniformIndices
GLAD_API_CALL PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
#define glGetUniformLocation glad_glGetUniformLocation
GLAD_API_CALL PFNGLGETUNIFORMLOCATIONARBPROC glad_glGetUniformLocationARB;
#define glGetUniformLocationARB glad_glGetUniformLocationARB
GLAD_API_CALL PFNGLGETUNIFORMSUBROUTINEUIVPROC glad_glGetUniformSubroutineuiv;
#define glGetUniformSubroutineuiv glad_glGetUniformSubroutineuiv
GLAD_API_CALL PFNGLGETUNIFORMDVPROC glad_glGetUniformdv;
#define glGetUniformdv glad_glGetUniformdv
GLAD_API_CALL PFNGLGETUNIFORMFVPROC glad_glGetUniformfv;
#define glGetUniformfv glad_glGetUniformfv
GLAD_API_CALL PFNGLGETUNIFORMFVARBPROC glad_glGetUniformfvARB;
#define glGetUniformfvARB glad_glGetUniformfvARB
GLAD_API_CALL PFNGLGETUNIFORMI64VARBPROC glad_glGetUniformi64vARB;
#define glGetUniformi64vARB glad_glGetUniformi64vARB
GLAD_API_CALL PFNGLGETUNIFORMIVPROC glad_glGetUniformiv;
#define glGetUniformiv glad_glGetUniformiv
GLAD_API_CALL PFNGLGETUNIFORMIVARBPROC glad_glGetUniformivARB;
#define glGetUniformivARB glad_glGetUniformivARB
GLAD_API_CALL PFNGLGETUNIFORMUI64VARBPROC glad_glGetUniformui64vARB;
#define glGetUniformui64vARB glad_glGetUniformui64vARB
GLAD_API_CALL PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv;
#define glGetUniformuiv glad_glGetUniformuiv
GLAD_API_CALL PFNGLGETVERTEXARRAYINDEXED64IVPROC glad_glGetVertexArrayIndexed64iv;
#define glGetVertexArrayIndexed64iv glad_glGetVertexArrayIndexed64iv
GLAD_API_CALL PFNGLGETVERTEXARRAYINDEXEDIVPROC glad_glGetVertexArrayIndexediv;
#define glGetVertexArrayIndexediv glad_glGetVertexArrayIndexediv
GLAD_API_CALL PFNGLGETVERTEXARRAYIVPROC glad_glGetVertexArrayiv;
#define glGetVertexArrayiv glad_glGetVertexArrayiv
GLAD_API_CALL PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv;
#define glGetVertexAttribIiv glad_glGetVertexAttribIiv
GLAD_API_CALL PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv;
#define glGetVertexAttribIuiv glad_glGetVertexAttribIuiv
GLAD_API_CALL PFNGLGETVERTEXATTRIBLDVPROC glad_glGetVertexAttribLdv;
#define glGetVertexAttribLdv glad_glGetVertexAttribLdv
GLAD_API_CALL PFNGLGETVERTEXATTRIBLUI64VARBPROC glad_glGetVertexAttribLui64vARB;
#define glGetVertexAttribLui64vARB glad_glGetVertexAttribLui64vARB
GLAD_API_CALL PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv;
#define glGetVertexAttribPointerv glad_glGetVertexAttribPointerv
GLAD_API_CALL PFNGLGETVERTEXATTRIBPOINTERVARBPROC glad_glGetVertexAttribPointervARB;
#define glGetVertexAttribPointervARB glad_glGetVertexAttribPointervARB
GLAD_API_CALL PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv;
#define glGetVertexAttribdv glad_glGetVertexAttribdv
GLAD_API_CALL PFNGLGETVERTEXATTRIBDVARBPROC glad_glGetVertexAttribdvARB;
#define glGetVertexAttribdvARB glad_glGetVertexAttribdvARB
GLAD_API_CALL PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv;
#define glGetVertexAttribfv glad_glGetVertexAttribfv
GLAD_API_CALL PFNGLGETVERTEXATTRIBFVARBPROC glad_glGetVertexAttribfvARB;
#define glGetVertexAttribfvARB glad_glGetVertexAttribfvARB
GLAD_API_CALL PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv;
#define glGetVertexAttribiv glad_glGetVertexAttribiv
GLAD_API_CALL PFNGLGETVERTEXATTRIBIVARBPROC glad_glGetVertexAttribivARB;
#define glGetVertexAttribivARB glad_glGetVertexAttribivARB
GLAD_API_CALL PFNGLGETNCOLORTABLEARBPROC glad_glGetnColorTableARB;
#define glGetnColorTableARB glad_glGetnColorTableARB
GLAD_API_CALL PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC glad_glGetnCompressedTexImageARB;
#define glGetnCompressedTexImageARB glad_glGetnCompressedTexImageARB
GLAD_API_CALL PFNGLGETNCONVOLUTIONFILTERARBPROC glad_glGetnConvolutionFilterARB;
#define glGetnConvolutionFilterARB glad_glGetnConvolutionFilterARB
GLAD_API_CALL PFNGLGETNHISTOGRAMARBPROC glad_glGetnHistogramARB;
#define glGetnHistogramARB glad_glGetnHistogramARB
GLAD_API_CALL PFNGLGETNMAPDVARBPROC glad_glGetnMapdvARB;
#define glGetnMapdvARB glad_glGetnMapdvARB
GLAD_API_CALL PFNGLGETNMAPFVARBPROC glad_glGetnMapfvARB;
#define glGetnMapfvARB glad_glGetnMapfvARB
GLAD_API_CALL PFNGLGETNMAPIVARBPROC glad_glGetnMapivARB;
#define glGetnMapivARB glad_glGetnMapivARB
GLAD_API_CALL PFNGLGETNMINMAXARBPROC glad_glGetnMinmaxARB;
#define glGetnMinmaxARB glad_glGetnMinmaxARB
GLAD_API_CALL PFNGLGETNPIXELMAPFVARBPROC glad_glGetnPixelMapfvARB;
#define glGetnPixelMapfvARB glad_glGetnPixelMapfvARB
GLAD_API_CALL PFNGLGETNPIXELMAPUIVARBPROC glad_glGetnPixelMapuivARB;
#define glGetnPixelMapuivARB glad_glGetnPixelMapuivARB
GLAD_API_CALL PFNGLGETNPIXELMAPUSVARBPROC glad_glGetnPixelMapusvARB;
#define glGetnPixelMapusvARB glad_glGetnPixelMapusvARB
GLAD_API_CALL PFNGLGETNPOLYGONSTIPPLEARBPROC glad_glGetnPolygonStippleARB;
#define glGetnPolygonStippleARB glad_glGetnPolygonStippleARB
GLAD_API_CALL PFNGLGETNSEPARABLEFILTERARBPROC glad_glGetnSeparableFilterARB;
#define glGetnSeparableFilterARB glad_glGetnSeparableFilterARB
GLAD_API_CALL PFNGLGETNTEXIMAGEARBPROC glad_glGetnTexImageARB;
#define glGetnTexImageARB glad_glGetnTexImageARB
GLAD_API_CALL PFNGLGETNUNIFORMDVARBPROC glad_glGetnUniformdvARB;
#define glGetnUniformdvARB glad_glGetnUniformdvARB
GLAD_API_CALL PFNGLGETNUNIFORMFVPROC glad_glGetnUniformfv;
#define glGetnUniformfv glad_glGetnUniformfv
GLAD_API_CALL PFNGLGETNUNIFORMFVARBPROC glad_glGetnUniformfvARB;
#define glGetnUniformfvARB glad_glGetnUniformfvARB
GLAD_API_CALL PFNGLGETNUNIFORMI64VARBPROC glad_glGetnUniformi64vARB;
#define glGetnUniformi64vARB glad_glGetnUniformi64vARB
GLAD_API_CALL PFNGLGETNUNIFORMIVPROC glad_glGetnUniformiv;
#define glGetnUniformiv glad_glGetnUniformiv
GLAD_API_CALL PFNGLGETNUNIFORMIVARBPROC glad_glGetnUniformivARB;
#define glGetnUniformivARB glad_glGetnUniformivARB
GLAD_API_CALL PFNGLGETNUNIFORMUI64VARBPROC glad_glGetnUniformui64vARB;
#define glGetnUniformui64vARB glad_glGetnUniformui64vARB
GLAD_API_CALL PFNGLGETNUNIFORMUIVPROC glad_glGetnUniformuiv;
#define glGetnUniformuiv glad_glGetnUniformuiv
GLAD_API_CALL PFNGLGETNUNIFORMUIVARBPROC glad_glGetnUniformuivARB;
#define glGetnUniformuivARB glad_glGetnUniformuivARB
GLAD_API_CALL PFNGLHINTPROC glad_glHint;
#define glHint glad_glHint
GLAD_API_CALL PFNGLHISTOGRAMPROC glad_glHistogram;
#define glHistogram glad_glHistogram
GLAD_API_CALL PFNGLINDEXMASKPROC glad_glIndexMask;
#define glIndexMask glad_glIndexMask
GLAD_API_CALL PFNGLINDEXPOINTERPROC glad_glIndexPointer;
#define glIndexPointer glad_glIndexPointer
GLAD_API_CALL PFNGLINDEXDPROC glad_glIndexd;
#define glIndexd glad_glIndexd
GLAD_API_CALL PFNGLINDEXDVPROC glad_glIndexdv;
#define glIndexdv glad_glIndexdv
GLAD_API_CALL PFNGLINDEXFPROC glad_glIndexf;
#define glIndexf glad_glIndexf
GLAD_API_CALL PFNGLINDEXFVPROC glad_glIndexfv;
#define glIndexfv glad_glIndexfv
GLAD_API_CALL PFNGLINDEXIPROC glad_glIndexi;
#define glIndexi glad_glIndexi
GLAD_API_CALL PFNGLINDEXIVPROC glad_glIndexiv;
#define glIndexiv glad_glIndexiv
GLAD_API_CALL PFNGLINDEXSPROC glad_glIndexs;
#define glIndexs glad_glIndexs
GLAD_API_CALL PFNGLINDEXSVPROC glad_glIndexsv;
#define glIndexsv glad_glIndexsv
GLAD_API_CALL PFNGLINDEXUBPROC glad_glIndexub;
#define glIndexub glad_glIndexub
GLAD_API_CALL PFNGLINDEXUBVPROC glad_glIndexubv;
#define glIndexubv glad_glIndexubv
GLAD_API_CALL PFNGLINITNAMESPROC glad_glInitNames;
#define glInitNames glad_glInitNames
GLAD_API_CALL PFNGLINTERLEAVEDARRAYSPROC glad_glInterleavedArrays;
#define glInterleavedArrays glad_glInterleavedArrays
GLAD_API_CALL PFNGLINVALIDATEBUFFERDATAPROC glad_glInvalidateBufferData;
#define glInvalidateBufferData glad_glInvalidateBufferData
GLAD_API_CALL PFNGLINVALIDATEBUFFERSUBDATAPROC glad_glInvalidateBufferSubData;
#define glInvalidateBufferSubData glad_glInvalidateBufferSubData
GLAD_API_CALL PFNGLINVALIDATEFRAMEBUFFERPROC glad_glInvalidateFramebuffer;
#define glInvalidateFramebuffer glad_glInvalidateFramebuffer
GLAD_API_CALL PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC glad_glInvalidateNamedFramebufferData;
#define glInvalidateNamedFramebufferData glad_glInvalidateNamedFramebufferData
GLAD_API_CALL PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC glad_glInvalidateNamedFramebufferSubData;
#define glInvalidateNamedFramebufferSubData glad_glInvalidateNamedFramebufferSubData
GLAD_API_CALL PFNGLINVALIDATESUBFRAMEBUFFERPROC glad_glInvalidateSubFramebuffer;
#define glInvalidateSubFramebuffer glad_glInvalidateSubFramebuffer
GLAD_API_CALL PFNGLINVALIDATETEXIMAGEPROC glad_glInvalidateTexImage;
#define glInvalidateTexImage glad_glInvalidateTexImage
GLAD_API_CALL PFNGLINVALIDATETEXSUBIMAGEPROC glad_glInvalidateTexSubImage;
#define glInvalidateTexSubImage glad_glInvalidateTexSubImage
GLAD_API_CALL PFNGLISBUFFERPROC glad_glIsBuffer;
#define glIsBuffer glad_glIsBuffer
GLAD_API_CALL PFNGLISBUFFERARBPROC glad_glIsBufferARB;
#define glIsBufferARB glad_glIsBufferARB
GLAD_API_CALL PFNGLISENABLEDPROC glad_glIsEnabled;
#define glIsEnabled glad_glIsEnabled
GLAD_API_CALL PFNGLISENABLEDIPROC glad_glIsEnabledi;
#define glIsEnabledi glad_glIsEnabledi
GLAD_API_CALL PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer;
#define glIsFramebuffer glad_glIsFramebuffer
GLAD_API_CALL PFNGLISIMAGEHANDLERESIDENTARBPROC glad_glIsImageHandleResidentARB;
#define glIsImageHandleResidentARB glad_glIsImageHandleResidentARB
GLAD_API_CALL PFNGLISLISTPROC glad_glIsList;
#define glIsList glad_glIsList
GLAD_API_CALL PFNGLISNAMEDSTRINGARBPROC glad_glIsNamedStringARB;
#define glIsNamedStringARB glad_glIsNamedStringARB
GLAD_API_CALL PFNGLISPROGRAMPROC glad_glIsProgram;
#define glIsProgram glad_glIsProgram
GLAD_API_CALL PFNGLISPROGRAMARBPROC glad_glIsProgramARB;
#define glIsProgramARB glad_glIsProgramARB
GLAD_API_CALL PFNGLISPROGRAMPIPELINEPROC glad_glIsProgramPipeline;
#define glIsProgramPipeline glad_glIsProgramPipeline
GLAD_API_CALL PFNGLISQUERYPROC glad_glIsQuery;
#define glIsQuery glad_glIsQuery
GLAD_API_CALL PFNGLISQUERYARBPROC glad_glIsQueryARB;
#define glIsQueryARB glad_glIsQueryARB
GLAD_API_CALL PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer;
#define glIsRenderbuffer glad_glIsRenderbuffer
GLAD_API_CALL PFNGLISSAMPLERPROC glad_glIsSampler;
#define glIsSampler glad_glIsSampler
GLAD_API_CALL PFNGLISSHADERPROC glad_glIsShader;
#define glIsShader glad_glIsShader
GLAD_API_CALL PFNGLISSYNCPROC glad_glIsSync;
#define glIsSync glad_glIsSync
GLAD_API_CALL PFNGLISTEXTUREPROC glad_glIsTexture;
#define glIsTexture glad_glIsTexture
GLAD_API_CALL PFNGLISTEXTUREHANDLERESIDENTARBPROC glad_glIsTextureHandleResidentARB;
#define glIsTextureHandleResidentARB glad_glIsTextureHandleResidentARB
GLAD_API_CALL PFNGLISTRANSFORMFEEDBACKPROC glad_glIsTransformFeedback;
#define glIsTransformFeedback glad_glIsTransformFeedback
GLAD_API_CALL PFNGLISVERTEXARRAYPROC glad_glIsVertexArray;
#define glIsVertexArray glad_glIsVertexArray
GLAD_API_CALL PFNGLLIGHTMODELFPROC glad_glLightModelf;
#define glLightModelf glad_glLightModelf
GLAD_API_CALL PFNGLLIGHTMODELFVPROC glad_glLightModelfv;
#define glLightModelfv glad_glLightModelfv
GLAD_API_CALL PFNGLLIGHTMODELIPROC glad_glLightModeli;
#define glLightModeli glad_glLightModeli
GLAD_API_CALL PFNGLLIGHTMODELIVPROC glad_glLightModeliv;
#define glLightModeliv glad_glLightModeliv
GLAD_API_CALL PFNGLLIGHTFPROC glad_glLightf;
#define glLightf glad_glLightf
GLAD_API_CALL PFNGLLIGHTFVPROC glad_glLightfv;
#define glLightfv glad_glLightfv
GLAD_API_CALL PFNGLLIGHTIPROC glad_glLighti;
#define glLighti glad_glLighti
GLAD_API_CALL PFNGLLIGHTIVPROC glad_glLightiv;
#define glLightiv glad_glLightiv
GLAD_API_CALL PFNGLLINESTIPPLEPROC glad_glLineStipple;
#define glLineStipple glad_glLineStipple
GLAD_API_CALL PFNGLLINEWIDTHPROC glad_glLineWidth;
#define glLineWidth glad_glLineWidth
GLAD_API_CALL PFNGLLINKPROGRAMPROC glad_glLinkProgram;
#define glLinkProgram glad_glLinkProgram
GLAD_API_CALL PFNGLLINKPROGRAMARBPROC glad_glLinkProgramARB;
#define glLinkProgramARB glad_glLinkProgramARB
GLAD_API_CALL PFNGLLISTBASEPROC glad_glListBase;
#define glListBase glad_glListBase
GLAD_API_CALL PFNGLLOADIDENTITYPROC glad_glLoadIdentity;
#define glLoadIdentity glad_glLoadIdentity
GLAD_API_CALL PFNGLLOADMATRIXDPROC glad_glLoadMatrixd;
#define glLoadMatrixd glad_glLoadMatrixd
GLAD_API_CALL PFNGLLOADMATRIXFPROC glad_glLoadMatrixf;
#define glLoadMatrixf glad_glLoadMatrixf
GLAD_API_CALL PFNGLLOADNAMEPROC glad_glLoadName;
#define glLoadName glad_glLoadName
GLAD_API_CALL PFNGLLOADTRANSPOSEMATRIXDPROC glad_glLoadTransposeMatrixd;
#define glLoadTransposeMatrixd glad_glLoadTransposeMatrixd
GLAD_API_CALL PFNGLLOADTRANSPOSEMATRIXDARBPROC glad_glLoadTransposeMatrixdARB;
#define glLoadTransposeMatrixdARB glad_glLoadTransposeMatrixdARB
GLAD_API_CALL PFNGLLOADTRANSPOSEMATRIXFPROC glad_glLoadTransposeMatrixf;
#define glLoadTransposeMatrixf glad_glLoadTransposeMatrixf
GLAD_API_CALL PFNGLLOADTRANSPOSEMATRIXFARBPROC glad_glLoadTransposeMatrixfARB;
#define glLoadTransposeMatrixfARB glad_glLoadTransposeMatrixfARB
GLAD_API_CALL PFNGLLOGICOPPROC glad_glLogicOp;
#define glLogicOp glad_glLogicOp
GLAD_API_CALL PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC glad_glMakeImageHandleNonResidentARB;
#define glMakeImageHandleNonResidentARB glad_glMakeImageHandleNonResidentARB
GLAD_API_CALL PFNGLMAKEIMAGEHANDLERESIDENTARBPROC glad_glMakeImageHandleResidentARB;
#define glMakeImageHandleResidentARB glad_glMakeImageHandleResidentARB
GLAD_API_CALL PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC glad_glMakeTextureHandleNonResidentARB;
#define glMakeTextureHandleNonResidentARB glad_glMakeTextureHandleNonResidentARB
GLAD_API_CALL PFNGLMAKETEXTUREHANDLERESIDENTARBPROC glad_glMakeTextureHandleResidentARB;
#define glMakeTextureHandleResidentARB glad_glMakeTextureHandleResidentARB
GLAD_API_CALL PFNGLMAP1DPROC glad_glMap1d;
#define glMap1d glad_glMap1d
GLAD_API_CALL PFNGLMAP1FPROC glad_glMap1f;
#define glMap1f glad_glMap1f
GLAD_API_CALL PFNGLMAP2DPROC glad_glMap2d;
#define glMap2d glad_glMap2d
GLAD_API_CALL PFNGLMAP2FPROC glad_glMap2f;
#define glMap2f glad_glMap2f
GLAD_API_CALL PFNGLMAPBUFFERPROC glad_glMapBuffer;
#define glMapBuffer glad_glMapBuffer
GLAD_API_CALL PFNGLMAPBUFFERARBPROC glad_glMapBufferARB;
#define glMapBufferARB glad_glMapBufferARB
GLAD_API_CALL PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange;
#define glMapBufferRange glad_glMapBufferRange
GLAD_API_CALL PFNGLMAPGRID1DPROC glad_glMapGrid1d;
#define glMapGrid1d glad_glMapGrid1d
GLAD_API_CALL PFNGLMAPGRID1FPROC glad_glMapGrid1f;
#define glMapGrid1f glad_glMapGrid1f
GLAD_API_CALL PFNGLMAPGRID2DPROC glad_glMapGrid2d;
#define glMapGrid2d glad_glMapGrid2d
GLAD_API_CALL PFNGLMAPGRID2FPROC glad_glMapGrid2f;
#define glMapGrid2f glad_glMapGrid2f
GLAD_API_CALL PFNGLMAPNAMEDBUFFERPROC glad_glMapNamedBuffer;
#define glMapNamedBuffer glad_glMapNamedBuffer
GLAD_API_CALL PFNGLMAPNAMEDBUFFERRANGEPROC glad_glMapNamedBufferRange;
#define glMapNamedBufferRange glad_glMapNamedBufferRange
GLAD_API_CALL PFNGLMATERIALFPROC glad_glMaterialf;
#define glMaterialf glad_glMaterialf
GLAD_API_CALL PFNGLMATERIALFVPROC glad_glMaterialfv;
#define glMaterialfv glad_glMaterialfv
GLAD_API_CALL PFNGLMATERIALIPROC glad_glMateriali;
#define glMateriali glad_glMateriali
GLAD_API_CALL PFNGLMATERIALIVPROC glad_glMaterialiv;
#define glMaterialiv glad_glMaterialiv
GLAD_API_CALL PFNGLMATRIXINDEXPOINTERARBPROC glad_glMatrixIndexPointerARB;
#define glMatrixIndexPointerARB glad_glMatrixIndexPointerARB
GLAD_API_CALL PFNGLMATRIXINDEXUBVARBPROC glad_glMatrixIndexubvARB;
#define glMatrixIndexubvARB glad_glMatrixIndexubvARB
GLAD_API_CALL PFNGLMATRIXINDEXUIVARBPROC glad_glMatrixIndexuivARB;
#define glMatrixIndexuivARB glad_glMatrixIndexuivARB
GLAD_API_CALL PFNGLMATRIXINDEXUSVARBPROC glad_glMatrixIndexusvARB;
#define glMatrixIndexusvARB glad_glMatrixIndexusvARB
GLAD_API_CALL PFNGLMATRIXMODEPROC glad_glMatrixMode;
#define glMatrixMode glad_glMatrixMode
GLAD_API_CALL PFNGLMAXSHADERCOMPILERTHREADSARBPROC glad_glMaxShaderCompilerThreadsARB;
#define glMaxShaderCompilerThreadsARB glad_glMaxShaderCompilerThreadsARB
GLAD_API_CALL PFNGLMAXSHADERCOMPILERTHREADSKHRPROC glad_glMaxShaderCompilerThreadsKHR;
#define glMaxShaderCompilerThreadsKHR glad_glMaxShaderCompilerThreadsKHR
GLAD_API_CALL PFNGLMEMORYBARRIERPROC glad_glMemoryBarrier;
#define glMemoryBarrier glad_glMemoryBarrier
GLAD_API_CALL PFNGLMEMORYBARRIERBYREGIONPROC glad_glMemoryBarrierByRegion;
#define glMemoryBarrierByRegion glad_glMemoryBarrierByRegion
GLAD_API_CALL PFNGLMINSAMPLESHADINGPROC glad_glMinSampleShading;
#define glMinSampleShading glad_glMinSampleShading
GLAD_API_CALL PFNGLMINSAMPLESHADINGARBPROC glad_glMinSampleShadingARB;
#define glMinSampleShadingARB glad_glMinSampleShadingARB
GLAD_API_CALL PFNGLMINMAXPROC glad_glMinmax;
#define glMinmax glad_glMinmax
GLAD_API_CALL PFNGLMULTMATRIXDPROC glad_glMultMatrixd;
#define glMultMatrixd glad_glMultMatrixd
GLAD_API_CALL PFNGLMULTMATRIXFPROC glad_glMultMatrixf;
#define glMultMatrixf glad_glMultMatrixf
GLAD_API_CALL PFNGLMULTTRANSPOSEMATRIXDPROC glad_glMultTransposeMatrixd;
#define glMultTransposeMatrixd glad_glMultTransposeMatrixd
GLAD_API_CALL PFNGLMULTTRANSPOSEMATRIXDARBPROC glad_glMultTransposeMatrixdARB;
#define glMultTransposeMatrixdARB glad_glMultTransposeMatrixdARB
GLAD_API_CALL PFNGLMULTTRANSPOSEMATRIXFPROC glad_glMultTransposeMatrixf;
#define glMultTransposeMatrixf glad_glMultTransposeMatrixf
GLAD_API_CALL PFNGLMULTTRANSPOSEMATRIXFARBPROC glad_glMultTransposeMatrixfARB;
#define glMultTransposeMatrixfARB glad_glMultTransposeMatrixfARB
GLAD_API_CALL PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays;
#define glMultiDrawArrays glad_glMultiDrawArrays
GLAD_API_CALL PFNGLMULTIDRAWARRAYSINDIRECTPROC glad_glMultiDrawArraysIndirect;
#define glMultiDrawArraysIndirect glad_glMultiDrawArraysIndirect
GLAD_API_CALL PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC glad_glMultiDrawArraysIndirectCount;
#define glMultiDrawArraysIndirectCount glad_glMultiDrawArraysIndirectCount
GLAD_API_CALL PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC glad_glMultiDrawArraysIndirectCountARB;
#define glMultiDrawArraysIndirectCountARB glad_glMultiDrawArraysIndirectCountARB
GLAD_API_CALL PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements;
#define glMultiDrawElements glad_glMultiDrawElements
GLAD_API_CALL PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex;
#define glMultiDrawElementsBaseVertex glad_glMultiDrawElementsBaseVertex
GLAD_API_CALL PFNGLMULTIDRAWELEMENTSINDIRECTPROC glad_glMultiDrawElementsIndirect;
#define glMultiDrawElementsIndirect glad_glMultiDrawElementsIndirect
GLAD_API_CALL PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC glad_glMultiDrawElementsIndirectCount;
#define glMultiDrawElementsIndirectCount glad_glMultiDrawElementsIndirectCount
GLAD_API_CALL PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC glad_glMultiDrawElementsIndirectCountARB;
#define glMultiDrawElementsIndirectCountARB glad_glMultiDrawElementsIndirectCountARB
GLAD_API_CALL PFNGLMULTITEXCOORD1DPROC glad_glMultiTexCoord1d;
#define glMultiTexCoord1d glad_glMultiTexCoord1d
GLAD_API_CALL PFNGLMULTITEXCOORD1DARBPROC glad_glMultiTexCoord1dARB;
#define glMultiTexCoord1dARB glad_glMultiTexCoord1dARB
GLAD_API_CALL PFNGLMULTITEXCOORD1DVPROC glad_glMultiTexCoord1dv;
#define glMultiTexCoord1dv glad_glMultiTexCoord1dv
GLAD_API_CALL PFNGLMULTITEXCOORD1DVARBPROC glad_glMultiTexCoord1dvARB;
#define glMultiTexCoord1dvARB glad_glMultiTexCoord1dvARB
GLAD_API_CALL PFNGLMULTITEXCOORD1FPROC glad_glMultiTexCoord1f;
#define glMultiTexCoord1f glad_glMultiTexCoord1f
GLAD_API_CALL PFNGLMULTITEXCOORD1FARBPROC glad_glMultiTexCoord1fARB;
#define glMultiTexCoord1fARB glad_glMultiTexCoord1fARB
GLAD_API_CALL PFNGLMULTITEXCOORD1FVPROC glad_glMultiTexCoord1fv;
#define glMultiTexCoord1fv glad_glMultiTexCoord1fv
GLAD_API_CALL PFNGLMULTITEXCOORD1FVARBPROC glad_glMultiTexCoord1fvARB;
#define glMultiTexCoord1fvARB glad_glMultiTexCoord1fvARB
GLAD_API_CALL PFNGLMULTITEXCOORD1IPROC glad_glMultiTexCoord1i;
#define glMultiTexCoord1i glad_glMultiTexCoord1i
GLAD_API_CALL PFNGLMULTITEXCOORD1IARBPROC glad_glMultiTexCoord1iARB;
#define glMultiTexCoord1iARB glad_glMultiTexCoord1iARB
GLAD_API_CALL PFNGLMULTITEXCOORD1IVPROC glad_glMultiTexCoord1iv;
#define glMultiTexCoord1iv glad_glMultiTexCoord1iv
GLAD_API_CALL PFNGLMULTITEXCOORD1IVARBPROC glad_glMultiTexCoord1ivARB;
#define glMultiTexCoord1ivARB glad_glMultiTexCoord1ivARB
GLAD_API_CALL PFNGLMULTITEXCOORD1SPROC glad_glMultiTexCoord1s;
#define glMultiTexCoord1s glad_glMultiTexCoord1s
GLAD_API_CALL PFNGLMULTITEXCOORD1SARBPROC glad_glMultiTexCoord1sARB;
#define glMultiTexCoord1sARB glad_glMultiTexCoord1sARB
GLAD_API_CALL PFNGLMULTITEXCOORD1SVPROC glad_glMultiTexCoord1sv;
#define glMultiTexCoord1sv glad_glMultiTexCoord1sv
GLAD_API_CALL PFNGLMULTITEXCOORD1SVARBPROC glad_glMultiTexCoord1svARB;
#define glMultiTexCoord1svARB glad_glMultiTexCoord1svARB
GLAD_API_CALL PFNGLMULTITEXCOORD2DPROC glad_glMultiTexCoord2d;
#define glMultiTexCoord2d glad_glMultiTexCoord2d
GLAD_API_CALL PFNGLMULTITEXCOORD2DARBPROC glad_glMultiTexCoord2dARB;
#define glMultiTexCoord2dARB glad_glMultiTexCoord2dARB
GLAD_API_CALL PFNGLMULTITEXCOORD2DVPROC glad_glMultiTexCoord2dv;
#define glMultiTexCoord2dv glad_glMultiTexCoord2dv
GLAD_API_CALL PFNGLMULTITEXCOORD2DVARBPROC glad_glMultiTexCoord2dvARB;
#define glMultiTexCoord2dvARB glad_glMultiTexCoord2dvARB
GLAD_API_CALL PFNGLMULTITEXCOORD2FPROC glad_glMultiTexCoord2f;
#define glMultiTexCoord2f glad_glMultiTexCoord2f
GLAD_API_CALL PFNGLMULTITEXCOORD2FARBPROC glad_glMultiTexCoord2fARB;
#define glMultiTexCoord2fARB glad_glMultiTexCoord2fARB
GLAD_API_CALL PFNGLMULTITEXCOORD2FVPROC glad_glMultiTexCoord2fv;
#define glMultiTexCoord2fv glad_glMultiTexCoord2fv
GLAD_API_CALL PFNGLMULTITEXCOORD2FVARBPROC glad_glMultiTexCoord2fvARB;
#define glMultiTexCoord2fvARB glad_glMultiTexCoord2fvARB
GLAD_API_CALL PFNGLMULTITEXCOORD2IPROC glad_glMultiTexCoord2i;
#define glMultiTexCoord2i glad_glMultiTexCoord2i
GLAD_API_CALL PFNGLMULTITEXCOORD2IARBPROC glad_glMultiTexCoord2iARB;
#define glMultiTexCoord2iARB glad_glMultiTexCoord2iARB
GLAD_API_CALL PFNGLMULTITEXCOORD2IVPROC glad_glMultiTexCoord2iv;
#define glMultiTexCoord2iv glad_glMultiTexCoord2iv
GLAD_API_CALL PFNGLMULTITEXCOORD2IVARBPROC glad_glMultiTexCoord2ivARB;
#define glMultiTexCoord2ivARB glad_glMultiTexCoord2ivARB
GLAD_API_CALL PFNGLMULTITEXCOORD2SPROC glad_glMultiTexCoord2s;
#define glMultiTexCoord2s glad_glMultiTexCoord2s
GLAD_API_CALL PFNGLMULTITEXCOORD2SARBPROC glad_glMultiTexCoord2sARB;
#define glMultiTexCoord2sARB glad_glMultiTexCoord2sARB
GLAD_API_CALL PFNGLMULTITEXCOORD2SVPROC glad_glMultiTexCoord2sv;
#define glMultiTexCoord2sv glad_glMultiTexCoord2sv
GLAD_API_CALL PFNGLMULTITEXCOORD2SVARBPROC glad_glMultiTexCoord2svARB;
#define glMultiTexCoord2svARB glad_glMultiTexCoord2svARB
GLAD_API_CALL PFNGLMULTITEXCOORD3DPROC glad_glMultiTexCoord3d;
#define glMultiTexCoord3d glad_glMultiTexCoord3d
GLAD_API_CALL PFNGLMULTITEXCOORD3DARBPROC glad_glMultiTexCoord3dARB;
#define glMultiTexCoord3dARB glad_glMultiTexCoord3dARB
GLAD_API_CALL PFNGLMULTITEXCOORD3DVPROC glad_glMultiTexCoord3dv;
#define glMultiTexCoord3dv glad_glMultiTexCoord3dv
GLAD_API_CALL PFNGLMULTITEXCOORD3DVARBPROC glad_glMultiTexCoord3dvARB;
#define glMultiTexCoord3dvARB glad_glMultiTexCoord3dvARB
GLAD_API_CALL PFNGLMULTITEXCOORD3FPROC glad_glMultiTexCoord3f;
#define glMultiTexCoord3f glad_glMultiTexCoord3f
GLAD_API_CALL PFNGLMULTITEXCOORD3FARBPROC glad_glMultiTexCoord3fARB;
#define glMultiTexCoord3fARB glad_glMultiTexCoord3fARB
GLAD_API_CALL PFNGLMULTITEXCOORD3FVPROC glad_glMultiTexCoord3fv;
#define glMultiTexCoord3fv glad_glMultiTexCoord3fv
GLAD_API_CALL PFNGLMULTITEXCOORD3FVARBPROC glad_glMultiTexCoord3fvARB;
#define glMultiTexCoord3fvARB glad_glMultiTexCoord3fvARB
GLAD_API_CALL PFNGLMULTITEXCOORD3IPROC glad_glMultiTexCoord3i;
#define glMultiTexCoord3i glad_glMultiTexCoord3i
GLAD_API_CALL PFNGLMULTITEXCOORD3IARBPROC glad_glMultiTexCoord3iARB;
#define glMultiTexCoord3iARB glad_glMultiTexCoord3iARB
GLAD_API_CALL PFNGLMULTITEXCOORD3IVPROC glad_glMultiTexCoord3iv;
#define glMultiTexCoord3iv glad_glMultiTexCoord3iv
GLAD_API_CALL PFNGLMULTITEXCOORD3IVARBPROC glad_glMultiTexCoord3ivARB;
#define glMultiTexCoord3ivARB glad_glMultiTexCoord3ivARB
GLAD_API_CALL PFNGLMULTITEXCOORD3SPROC glad_glMultiTexCoord3s;
#define glMultiTexCoord3s glad_glMultiTexCoord3s
GLAD_API_CALL PFNGLMULTITEXCOORD3SARBPROC glad_glMultiTexCoord3sARB;
#define glMultiTexCoord3sARB glad_glMultiTexCoord3sARB
GLAD_API_CALL PFNGLMULTITEXCOORD3SVPROC glad_glMultiTexCoord3sv;
#define glMultiTexCoord3sv glad_glMultiTexCoord3sv
GLAD_API_CALL PFNGLMULTITEXCOORD3SVARBPROC glad_glMultiTexCoord3svARB;
#define glMultiTexCoord3svARB glad_glMultiTexCoord3svARB
GLAD_API_CALL PFNGLMULTITEXCOORD4DPROC glad_glMultiTexCoord4d;
#define glMultiTexCoord4d glad_glMultiTexCoord4d
GLAD_API_CALL PFNGLMULTITEXCOORD4DARBPROC glad_glMultiTexCoord4dARB;
#define glMultiTexCoord4dARB glad_glMultiTexCoord4dARB
GLAD_API_CALL PFNGLMULTITEXCOORD4DVPROC glad_glMultiTexCoord4dv;
#define glMultiTexCoord4dv glad_glMultiTexCoord4dv
GLAD_API_CALL PFNGLMULTITEXCOORD4DVARBPROC glad_glMultiTexCoord4dvARB;
#define glMultiTexCoord4dvARB glad_glMultiTexCoord4dvARB
GLAD_API_CALL PFNGLMULTITEXCOORD4FPROC glad_glMultiTexCoord4f;
#define glMultiTexCoord4f glad_glMultiTexCoord4f
GLAD_API_CALL PFNGLMULTITEXCOORD4FARBPROC glad_glMultiTexCoord4fARB;
#define glMultiTexCoord4fARB glad_glMultiTexCoord4fARB
GLAD_API_CALL PFNGLMULTITEXCOORD4FVPROC glad_glMultiTexCoord4fv;
#define glMultiTexCoord4fv glad_glMultiTexCoord4fv
GLAD_API_CALL PFNGLMULTITEXCOORD4FVARBPROC glad_glMultiTexCoord4fvARB;
#define glMultiTexCoord4fvARB glad_glMultiTexCoord4fvARB
GLAD_API_CALL PFNGLMULTITEXCOORD4IPROC glad_glMultiTexCoord4i;
#define glMultiTexCoord4i glad_glMultiTexCoord4i
GLAD_API_CALL PFNGLMULTITEXCOORD4IARBPROC glad_glMultiTexCoord4iARB;
#define glMultiTexCoord4iARB glad_glMultiTexCoord4iARB
GLAD_API_CALL PFNGLMULTITEXCOORD4IVPROC glad_glMultiTexCoord4iv;
#define glMultiTexCoord4iv glad_glMultiTexCoord4iv
GLAD_API_CALL PFNGLMULTITEXCOORD4IVARBPROC glad_glMultiTexCoord4ivARB;
#define glMultiTexCoord4ivARB glad_glMultiTexCoord4ivARB
GLAD_API_CALL PFNGLMULTITEXCOORD4SPROC glad_glMultiTexCoord4s;
#define glMultiTexCoord4s glad_glMultiTexCoord4s
GLAD_API_CALL PFNGLMULTITEXCOORD4SARBPROC glad_glMultiTexCoord4sARB;
#define glMultiTexCoord4sARB glad_glMultiTexCoord4sARB
GLAD_API_CALL PFNGLMULTITEXCOORD4SVPROC glad_glMultiTexCoord4sv;
#define glMultiTexCoord4sv glad_glMultiTexCoord4sv
GLAD_API_CALL PFNGLMULTITEXCOORD4SVARBPROC glad_glMultiTexCoord4svARB;
#define glMultiTexCoord4svARB glad_glMultiTexCoord4svARB
GLAD_API_CALL PFNGLMULTITEXCOORDP1UIPROC glad_glMultiTexCoordP1ui;
#define glMultiTexCoordP1ui glad_glMultiTexCoordP1ui
GLAD_API_CALL PFNGLMULTITEXCOORDP1UIVPROC glad_glMultiTexCoordP1uiv;
#define glMultiTexCoordP1uiv glad_glMultiTexCoordP1uiv
GLAD_API_CALL PFNGLMULTITEXCOORDP2UIPROC glad_glMultiTexCoordP2ui;
#define glMultiTexCoordP2ui glad_glMultiTexCoordP2ui
GLAD_API_CALL PFNGLMULTITEXCOORDP2UIVPROC glad_glMultiTexCoordP2uiv;
#define glMultiTexCoordP2uiv glad_glMultiTexCoordP2uiv
GLAD_API_CALL PFNGLMULTITEXCOORDP3UIPROC glad_glMultiTexCoordP3ui;
#define glMultiTexCoordP3ui glad_glMultiTexCoordP3ui
GLAD_API_CALL PFNGLMULTITEXCOORDP3UIVPROC glad_glMultiTexCoordP3uiv;
#define glMultiTexCoordP3uiv glad_glMultiTexCoordP3uiv
GLAD_API_CALL PFNGLMULTITEXCOORDP4UIPROC glad_glMultiTexCoordP4ui;
#define glMultiTexCoordP4ui glad_glMultiTexCoordP4ui
GLAD_API_CALL PFNGLMULTITEXCOORDP4UIVPROC glad_glMultiTexCoordP4uiv;
#define glMultiTexCoordP4uiv glad_glMultiTexCoordP4uiv
GLAD_API_CALL PFNGLNAMEDBUFFERDATAPROC glad_glNamedBufferData;
#define glNamedBufferData glad_glNamedBufferData
GLAD_API_CALL PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC glad_glNamedBufferPageCommitmentARB;
#define glNamedBufferPageCommitmentARB glad_glNamedBufferPageCommitmentARB
GLAD_API_CALL PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC glad_glNamedBufferPageCommitmentEXT;
#define glNamedBufferPageCommitmentEXT glad_glNamedBufferPageCommitmentEXT
GLAD_API_CALL PFNGLNAMEDBUFFERSTORAGEPROC glad_glNamedBufferStorage;
#define glNamedBufferStorage glad_glNamedBufferStorage
GLAD_API_CALL PFNGLNAMEDBUFFERSUBDATAPROC glad_glNamedBufferSubData;
#define glNamedBufferSubData glad_glNamedBufferSubData
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC glad_glNamedFramebufferDrawBuffer;
#define glNamedFramebufferDrawBuffer glad_glNamedFramebufferDrawBuffer
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC glad_glNamedFramebufferDrawBuffers;
#define glNamedFramebufferDrawBuffers glad_glNamedFramebufferDrawBuffers
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC glad_glNamedFramebufferParameteri;
#define glNamedFramebufferParameteri glad_glNamedFramebufferParameteri
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC glad_glNamedFramebufferReadBuffer;
#define glNamedFramebufferReadBuffer glad_glNamedFramebufferReadBuffer
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC glad_glNamedFramebufferRenderbuffer;
#define glNamedFramebufferRenderbuffer glad_glNamedFramebufferRenderbuffer
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVARBPROC glad_glNamedFramebufferSampleLocationsfvARB;
#define glNamedFramebufferSampleLocationsfvARB glad_glNamedFramebufferSampleLocationsfvARB
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glad_glNamedFramebufferTexture;
#define glNamedFramebufferTexture glad_glNamedFramebufferTexture
GLAD_API_CALL PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC glad_glNamedFramebufferTextureLayer;
#define glNamedFramebufferTextureLayer glad_glNamedFramebufferTextureLayer
GLAD_API_CALL PFNGLNAMEDRENDERBUFFERSTORAGEPROC glad_glNamedRenderbufferStorage;
#define glNamedRenderbufferStorage glad_glNamedRenderbufferStorage
GLAD_API_CALL PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glNamedRenderbufferStorageMultisample;
#define glNamedRenderbufferStorageMultisample glad_glNamedRenderbufferStorageMultisample
GLAD_API_CALL PFNGLNAMEDSTRINGARBPROC glad_glNamedStringARB;
#define glNamedStringARB glad_glNamedStringARB
GLAD_API_CALL PFNGLNEWLISTPROC glad_glNewList;
#define glNewList glad_glNewList
GLAD_API_CALL PFNGLNORMAL3BPROC glad_glNormal3b;
#define glNormal3b glad_glNormal3b
GLAD_API_CALL PFNGLNORMAL3BVPROC glad_glNormal3bv;
#define glNormal3bv glad_glNormal3bv
GLAD_API_CALL PFNGLNORMAL3DPROC glad_glNormal3d;
#define glNormal3d glad_glNormal3d
GLAD_API_CALL PFNGLNORMAL3DVPROC glad_glNormal3dv;
#define glNormal3dv glad_glNormal3dv
GLAD_API_CALL PFNGLNORMAL3FPROC glad_glNormal3f;
#define glNormal3f glad_glNormal3f
GLAD_API_CALL PFNGLNORMAL3FVPROC glad_glNormal3fv;
#define glNormal3fv glad_glNormal3fv
GLAD_API_CALL PFNGLNORMAL3IPROC glad_glNormal3i;
#define glNormal3i glad_glNormal3i
GLAD_API_CALL PFNGLNORMAL3IVPROC glad_glNormal3iv;
#define glNormal3iv glad_glNormal3iv
GLAD_API_CALL PFNGLNORMAL3SPROC glad_glNormal3s;
#define glNormal3s glad_glNormal3s
GLAD_API_CALL PFNGLNORMAL3SVPROC glad_glNormal3sv;
#define glNormal3sv glad_glNormal3sv
GLAD_API_CALL PFNGLNORMALP3UIPROC glad_glNormalP3ui;
#define glNormalP3ui glad_glNormalP3ui
GLAD_API_CALL PFNGLNORMALP3UIVPROC glad_glNormalP3uiv;
#define glNormalP3uiv glad_glNormalP3uiv
GLAD_API_CALL PFNGLNORMALPOINTERPROC glad_glNormalPointer;
#define glNormalPointer glad_glNormalPointer
GLAD_API_CALL PFNGLOBJECTLABELPROC glad_glObjectLabel;
#define glObjectLabel glad_glObjectLabel
GLAD_API_CALL PFNGLOBJECTPTRLABELPROC glad_glObjectPtrLabel;
#define glObjectPtrLabel glad_glObjectPtrLabel
GLAD_API_CALL PFNGLORTHOPROC glad_glOrtho;
#define glOrtho glad_glOrtho
GLAD_API_CALL PFNGLPASSTHROUGHPROC glad_glPassThrough;
#define glPassThrough glad_glPassThrough
GLAD_API_CALL PFNGLPATCHPARAMETERFVPROC glad_glPatchParameterfv;
#define glPatchParameterfv glad_glPatchParameterfv
GLAD_API_CALL PFNGLPATCHPARAMETERIPROC glad_glPatchParameteri;
#define glPatchParameteri glad_glPatchParameteri
GLAD_API_CALL PFNGLPAUSETRANSFORMFEEDBACKPROC glad_glPauseTransformFeedback;
#define glPauseTransformFeedback glad_glPauseTransformFeedback
GLAD_API_CALL PFNGLPIXELMAPFVPROC glad_glPixelMapfv;
#define glPixelMapfv glad_glPixelMapfv
GLAD_API_CALL PFNGLPIXELMAPUIVPROC glad_glPixelMapuiv;
#define glPixelMapuiv glad_glPixelMapuiv
GLAD_API_CALL PFNGLPIXELMAPUSVPROC glad_glPixelMapusv;
#define glPixelMapusv glad_glPixelMapusv
GLAD_API_CALL PFNGLPIXELSTOREFPROC glad_glPixelStoref;
#define glPixelStoref glad_glPixelStoref
GLAD_API_CALL PFNGLPIXELSTOREIPROC glad_glPixelStorei;
#define glPixelStorei glad_glPixelStorei
GLAD_API_CALL PFNGLPIXELTRANSFERFPROC glad_glPixelTransferf;
#define glPixelTransferf glad_glPixelTransferf
GLAD_API_CALL PFNGLPIXELTRANSFERIPROC glad_glPixelTransferi;
#define glPixelTransferi glad_glPixelTransferi
GLAD_API_CALL PFNGLPIXELZOOMPROC glad_glPixelZoom;
#define glPixelZoom glad_glPixelZoom
GLAD_API_CALL PFNGLPOINTPARAMETERFPROC glad_glPointParameterf;
#define glPointParameterf glad_glPointParameterf
GLAD_API_CALL PFNGLPOINTPARAMETERFARBPROC glad_glPointParameterfARB;
#define glPointParameterfARB glad_glPointParameterfARB
GLAD_API_CALL PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv;
#define glPointParameterfv glad_glPointParameterfv
GLAD_API_CALL PFNGLPOINTPARAMETERFVARBPROC glad_glPointParameterfvARB;
#define glPointParameterfvARB glad_glPointParameterfvARB
GLAD_API_CALL PFNGLPOINTPARAMETERIPROC glad_glPointParameteri;
#define glPointParameteri glad_glPointParameteri
GLAD_API_CALL PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv;
#define glPointParameteriv glad_glPointParameteriv
GLAD_API_CALL PFNGLPOINTSIZEPROC glad_glPointSize;
#define glPointSize glad_glPointSize
GLAD_API_CALL PFNGLPOLYGONMODEPROC glad_glPolygonMode;
#define glPolygonMode glad_glPolygonMode
GLAD_API_CALL PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset;
#define glPolygonOffset glad_glPolygonOffset
GLAD_API_CALL PFNGLPOLYGONOFFSETCLAMPPROC glad_glPolygonOffsetClamp;
#define glPolygonOffsetClamp glad_glPolygonOffsetClamp
GLAD_API_CALL PFNGLPOLYGONSTIPPLEPROC glad_glPolygonStipple;
#define glPolygonStipple glad_glPolygonStipple
GLAD_API_CALL PFNGLPOPATTRIBPROC glad_glPopAttrib;
#define glPopAttrib glad_glPopAttrib
GLAD_API_CALL PFNGLPOPCLIENTATTRIBPROC glad_glPopClientAttrib;
#define glPopClientAttrib glad_glPopClientAttrib
GLAD_API_CALL PFNGLPOPDEBUGGROUPPROC glad_glPopDebugGroup;
#define glPopDebugGroup glad_glPopDebugGroup
GLAD_API_CALL PFNGLPOPMATRIXPROC glad_glPopMatrix;
#define glPopMatrix glad_glPopMatrix
GLAD_API_CALL PFNGLPOPNAMEPROC glad_glPopName;
#define glPopName glad_glPopName
GLAD_API_CALL PFNGLPRIMITIVEBOUNDINGBOXPROC glad_glPrimitiveBoundingBox;
#define glPrimitiveBoundingBox glad_glPrimitiveBoundingBox
GLAD_API_CALL PFNGLPRIMITIVEBOUNDINGBOXARBPROC glad_glPrimitiveBoundingBoxARB;
#define glPrimitiveBoundingBoxARB glad_glPrimitiveBoundingBoxARB
GLAD_API_CALL PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex;
#define glPrimitiveRestartIndex glad_glPrimitiveRestartIndex
GLAD_API_CALL PFNGLPRIORITIZETEXTURESPROC glad_glPrioritizeTextures;
#define glPrioritizeTextures glad_glPrioritizeTextures
GLAD_API_CALL PFNGLPROGRAMBINARYPROC glad_glProgramBinary;
#define glProgramBinary glad_glProgramBinary
GLAD_API_CALL PFNGLPROGRAMENVPARAMETER4DARBPROC glad_glProgramEnvParameter4dARB;
#define glProgramEnvParameter4dARB glad_glProgramEnvParameter4dARB
GLAD_API_CALL PFNGLPROGRAMENVPARAMETER4DVARBPROC glad_glProgramEnvParameter4dvARB;
#define glProgramEnvParameter4dvARB glad_glProgramEnvParameter4dvARB
GLAD_API_CALL PFNGLPROGRAMENVPARAMETER4FARBPROC glad_glProgramEnvParameter4fARB;
#define glProgramEnvParameter4fARB glad_glProgramEnvParameter4fARB
GLAD_API_CALL PFNGLPROGRAMENVPARAMETER4FVARBPROC glad_glProgramEnvParameter4fvARB;
#define glProgramEnvParameter4fvARB glad_glProgramEnvParameter4fvARB
GLAD_API_CALL PFNGLPROGRAMLOCALPARAMETER4DARBPROC glad_glProgramLocalParameter4dARB;
#define glProgramLocalParameter4dARB glad_glProgramLocalParameter4dARB
GLAD_API_CALL PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glad_glProgramLocalParameter4dvARB;
#define glProgramLocalParameter4dvARB glad_glProgramLocalParameter4dvARB
GLAD_API_CALL PFNGLPROGRAMLOCALPARAMETER4FARBPROC glad_glProgramLocalParameter4fARB;
#define glProgramLocalParameter4fARB glad_glProgramLocalParameter4fARB
GLAD_API_CALL PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glad_glProgramLocalParameter4fvARB;
#define glProgramLocalParameter4fvARB glad_glProgramLocalParameter4fvARB
GLAD_API_CALL PFNGLPROGRAMPARAMETERIPROC glad_glProgramParameteri;
#define glProgramParameteri glad_glProgramParameteri
GLAD_API_CALL PFNGLPROGRAMPARAMETERIARBPROC glad_glProgramParameteriARB;
#define glProgramParameteriARB glad_glProgramParameteriARB
GLAD_API_CALL PFNGLPROGRAMSTRINGARBPROC glad_glProgramStringARB;
#define glProgramStringARB glad_glProgramStringARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM1DPROC glad_glProgramUniform1d;
#define glProgramUniform1d glad_glProgramUniform1d
GLAD_API_CALL PFNGLPROGRAMUNIFORM1DVPROC glad_glProgramUniform1dv;
#define glProgramUniform1dv glad_glProgramUniform1dv
GLAD_API_CALL PFNGLPROGRAMUNIFORM1FPROC glad_glProgramUniform1f;
#define glProgramUniform1f glad_glProgramUniform1f
GLAD_API_CALL PFNGLPROGRAMUNIFORM1FVPROC glad_glProgramUniform1fv;
#define glProgramUniform1fv glad_glProgramUniform1fv
GLAD_API_CALL PFNGLPROGRAMUNIFORM1IPROC glad_glProgramUniform1i;
#define glProgramUniform1i glad_glProgramUniform1i
GLAD_API_CALL PFNGLPROGRAMUNIFORM1I64ARBPROC glad_glProgramUniform1i64ARB;
#define glProgramUniform1i64ARB glad_glProgramUniform1i64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM1I64VARBPROC glad_glProgramUniform1i64vARB;
#define glProgramUniform1i64vARB glad_glProgramUniform1i64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM1IVPROC glad_glProgramUniform1iv;
#define glProgramUniform1iv glad_glProgramUniform1iv
GLAD_API_CALL PFNGLPROGRAMUNIFORM1UIPROC glad_glProgramUniform1ui;
#define glProgramUniform1ui glad_glProgramUniform1ui
GLAD_API_CALL PFNGLPROGRAMUNIFORM1UI64ARBPROC glad_glProgramUniform1ui64ARB;
#define glProgramUniform1ui64ARB glad_glProgramUniform1ui64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM1UI64VARBPROC glad_glProgramUniform1ui64vARB;
#define glProgramUniform1ui64vARB glad_glProgramUniform1ui64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM1UIVPROC glad_glProgramUniform1uiv;
#define glProgramUniform1uiv glad_glProgramUniform1uiv
GLAD_API_CALL PFNGLPROGRAMUNIFORM2DPROC glad_glProgramUniform2d;
#define glProgramUniform2d glad_glProgramUniform2d
GLAD_API_CALL PFNGLPROGRAMUNIFORM2DVPROC glad_glProgramUniform2dv;
#define glProgramUniform2dv glad_glProgramUniform2dv
GLAD_API_CALL PFNGLPROGRAMUNIFORM2FPROC glad_glProgramUniform2f;
#define glProgramUniform2f glad_glProgramUniform2f
GLAD_API_CALL PFNGLPROGRAMUNIFORM2FVPROC glad_glProgramUniform2fv;
#define glProgramUniform2fv glad_glProgramUniform2fv
GLAD_API_CALL PFNGLPROGRAMUNIFORM2IPROC glad_glProgramUniform2i;
#define glProgramUniform2i glad_glProgramUniform2i
GLAD_API_CALL PFNGLPROGRAMUNIFORM2I64ARBPROC glad_glProgramUniform2i64ARB;
#define glProgramUniform2i64ARB glad_glProgramUniform2i64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM2I64VARBPROC glad_glProgramUniform2i64vARB;
#define glProgramUniform2i64vARB glad_glProgramUniform2i64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM2IVPROC glad_glProgramUniform2iv;
#define glProgramUniform2iv glad_glProgramUniform2iv
GLAD_API_CALL PFNGLPROGRAMUNIFORM2UIPROC glad_glProgramUniform2ui;
#define glProgramUniform2ui glad_glProgramUniform2ui
GLAD_API_CALL PFNGLPROGRAMUNIFORM2UI64ARBPROC glad_glProgramUniform2ui64ARB;
#define glProgramUniform2ui64ARB glad_glProgramUniform2ui64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM2UI64VARBPROC glad_glProgramUniform2ui64vARB;
#define glProgramUniform2ui64vARB glad_glProgramUniform2ui64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM2UIVPROC glad_glProgramUniform2uiv;
#define glProgramUniform2uiv glad_glProgramUniform2uiv
GLAD_API_CALL PFNGLPROGRAMUNIFORM3DPROC glad_glProgramUniform3d;
#define glProgramUniform3d glad_glProgramUniform3d
GLAD_API_CALL PFNGLPROGRAMUNIFORM3DVPROC glad_glProgramUniform3dv;
#define glProgramUniform3dv glad_glProgramUniform3dv
GLAD_API_CALL PFNGLPROGRAMUNIFORM3FPROC glad_glProgramUniform3f;
#define glProgramUniform3f glad_glProgramUniform3f
GLAD_API_CALL PFNGLPROGRAMUNIFORM3FVPROC glad_glProgramUniform3fv;
#define glProgramUniform3fv glad_glProgramUniform3fv
GLAD_API_CALL PFNGLPROGRAMUNIFORM3IPROC glad_glProgramUniform3i;
#define glProgramUniform3i glad_glProgramUniform3i
GLAD_API_CALL PFNGLPROGRAMUNIFORM3I64ARBPROC glad_glProgramUniform3i64ARB;
#define glProgramUniform3i64ARB glad_glProgramUniform3i64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM3I64VARBPROC glad_glProgramUniform3i64vARB;
#define glProgramUniform3i64vARB glad_glProgramUniform3i64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM3IVPROC glad_glProgramUniform3iv;
#define glProgramUniform3iv glad_glProgramUniform3iv
GLAD_API_CALL PFNGLPROGRAMUNIFORM3UIPROC glad_glProgramUniform3ui;
#define glProgramUniform3ui glad_glProgramUniform3ui
GLAD_API_CALL PFNGLPROGRAMUNIFORM3UI64ARBPROC glad_glProgramUniform3ui64ARB;
#define glProgramUniform3ui64ARB glad_glProgramUniform3ui64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM3UI64VARBPROC glad_glProgramUniform3ui64vARB;
#define glProgramUniform3ui64vARB glad_glProgramUniform3ui64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM3UIVPROC glad_glProgramUniform3uiv;
#define glProgramUniform3uiv glad_glProgramUniform3uiv
GLAD_API_CALL PFNGLPROGRAMUNIFORM4DPROC glad_glProgramUniform4d;
#define glProgramUniform4d glad_glProgramUniform4d
GLAD_API_CALL PFNGLPROGRAMUNIFORM4DVPROC glad_glProgramUniform4dv;
#define glProgramUniform4dv glad_glProgramUniform4dv
GLAD_API_CALL PFNGLPROGRAMUNIFORM4FPROC glad_glProgramUniform4f;
#define glProgramUniform4f glad_glProgramUniform4f
GLAD_API_CALL PFNGLPROGRAMUNIFORM4FVPROC glad_glProgramUniform4fv;
#define glProgramUniform4fv glad_glProgramUniform4fv
GLAD_API_CALL PFNGLPROGRAMUNIFORM4IPROC glad_glProgramUniform4i;
#define glProgramUniform4i glad_glProgramUniform4i
GLAD_API_CALL PFNGLPROGRAMUNIFORM4I64ARBPROC glad_glProgramUniform4i64ARB;
#define glProgramUniform4i64ARB glad_glProgramUniform4i64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM4I64VARBPROC glad_glProgramUniform4i64vARB;
#define glProgramUniform4i64vARB glad_glProgramUniform4i64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM4IVPROC glad_glProgramUniform4iv;
#define glProgramUniform4iv glad_glProgramUniform4iv
GLAD_API_CALL PFNGLPROGRAMUNIFORM4UIPROC glad_glProgramUniform4ui;
#define glProgramUniform4ui glad_glProgramUniform4ui
GLAD_API_CALL PFNGLPROGRAMUNIFORM4UI64ARBPROC glad_glProgramUniform4ui64ARB;
#define glProgramUniform4ui64ARB glad_glProgramUniform4ui64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM4UI64VARBPROC glad_glProgramUniform4ui64vARB;
#define glProgramUniform4ui64vARB glad_glProgramUniform4ui64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORM4UIVPROC glad_glProgramUniform4uiv;
#define glProgramUniform4uiv glad_glProgramUniform4uiv
GLAD_API_CALL PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC glad_glProgramUniformHandleui64ARB;
#define glProgramUniformHandleui64ARB glad_glProgramUniformHandleui64ARB
GLAD_API_CALL PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC glad_glProgramUniformHandleui64vARB;
#define glProgramUniformHandleui64vARB glad_glProgramUniformHandleui64vARB
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX2DVPROC glad_glProgramUniformMatrix2dv;
#define glProgramUniformMatrix2dv glad_glProgramUniformMatrix2dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX2FVPROC glad_glProgramUniformMatrix2fv;
#define glProgramUniformMatrix2fv glad_glProgramUniformMatrix2fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glad_glProgramUniformMatrix2x3dv;
#define glProgramUniformMatrix2x3dv glad_glProgramUniformMatrix2x3dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glad_glProgramUniformMatrix2x3fv;
#define glProgramUniformMatrix2x3fv glad_glProgramUniformMatrix2x3fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glad_glProgramUniformMatrix2x4dv;
#define glProgramUniformMatrix2x4dv glad_glProgramUniformMatrix2x4dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glad_glProgramUniformMatrix2x4fv;
#define glProgramUniformMatrix2x4fv glad_glProgramUniformMatrix2x4fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX3DVPROC glad_glProgramUniformMatrix3dv;
#define glProgramUniformMatrix3dv glad_glProgramUniformMatrix3dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX3FVPROC glad_glProgramUniformMatrix3fv;
#define glProgramUniformMatrix3fv glad_glProgramUniformMatrix3fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glad_glProgramUniformMatrix3x2dv;
#define glProgramUniformMatrix3x2dv glad_glProgramUniformMatrix3x2dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glad_glProgramUniformMatrix3x2fv;
#define glProgramUniformMatrix3x2fv glad_glProgramUniformMatrix3x2fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glad_glProgramUniformMatrix3x4dv;
#define glProgramUniformMatrix3x4dv glad_glProgramUniformMatrix3x4dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glad_glProgramUniformMatrix3x4fv;
#define glProgramUniformMatrix3x4fv glad_glProgramUniformMatrix3x4fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX4DVPROC glad_glProgramUniformMatrix4dv;
#define glProgramUniformMatrix4dv glad_glProgramUniformMatrix4dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX4FVPROC glad_glProgramUniformMatrix4fv;
#define glProgramUniformMatrix4fv glad_glProgramUniformMatrix4fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glad_glProgramUniformMatrix4x2dv;
#define glProgramUniformMatrix4x2dv glad_glProgramUniformMatrix4x2dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glad_glProgramUniformMatrix4x2fv;
#define glProgramUniformMatrix4x2fv glad_glProgramUniformMatrix4x2fv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glad_glProgramUniformMatrix4x3dv;
#define glProgramUniformMatrix4x3dv glad_glProgramUniformMatrix4x3dv
GLAD_API_CALL PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glad_glProgramUniformMatrix4x3fv;
#define glProgramUniformMatrix4x3fv glad_glProgramUniformMatrix4x3fv
GLAD_API_CALL PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex;
#define glProvokingVertex glad_glProvokingVertex
GLAD_API_CALL PFNGLPUSHATTRIBPROC glad_glPushAttrib;
#define glPushAttrib glad_glPushAttrib
GLAD_API_CALL PFNGLPUSHCLIENTATTRIBPROC glad_glPushClientAttrib;
#define glPushClientAttrib glad_glPushClientAttrib
GLAD_API_CALL PFNGLPUSHDEBUGGROUPPROC glad_glPushDebugGroup;
#define glPushDebugGroup glad_glPushDebugGroup
GLAD_API_CALL PFNGLPUSHMATRIXPROC glad_glPushMatrix;
#define glPushMatrix glad_glPushMatrix
GLAD_API_CALL PFNGLPUSHNAMEPROC glad_glPushName;
#define glPushName glad_glPushName
GLAD_API_CALL PFNGLQUERYCOUNTERPROC glad_glQueryCounter;
#define glQueryCounter glad_glQueryCounter
GLAD_API_CALL PFNGLRASTERPOS2DPROC glad_glRasterPos2d;
#define glRasterPos2d glad_glRasterPos2d
GLAD_API_CALL PFNGLRASTERPOS2DVPROC glad_glRasterPos2dv;
#define glRasterPos2dv glad_glRasterPos2dv
GLAD_API_CALL PFNGLRASTERPOS2FPROC glad_glRasterPos2f;
#define glRasterPos2f glad_glRasterPos2f
GLAD_API_CALL PFNGLRASTERPOS2FVPROC glad_glRasterPos2fv;
#define glRasterPos2fv glad_glRasterPos2fv
GLAD_API_CALL PFNGLRASTERPOS2IPROC glad_glRasterPos2i;
#define glRasterPos2i glad_glRasterPos2i
GLAD_API_CALL PFNGLRASTERPOS2IVPROC glad_glRasterPos2iv;
#define glRasterPos2iv glad_glRasterPos2iv
GLAD_API_CALL PFNGLRASTERPOS2SPROC glad_glRasterPos2s;
#define glRasterPos2s glad_glRasterPos2s
GLAD_API_CALL PFNGLRASTERPOS2SVPROC glad_glRasterPos2sv;
#define glRasterPos2sv glad_glRasterPos2sv
GLAD_API_CALL PFNGLRASTERPOS3DPROC glad_glRasterPos3d;
#define glRasterPos3d glad_glRasterPos3d
GLAD_API_CALL PFNGLRASTERPOS3DVPROC glad_glRasterPos3dv;
#define glRasterPos3dv glad_glRasterPos3dv
GLAD_API_CALL PFNGLRASTERPOS3FPROC glad_glRasterPos3f;
#define glRasterPos3f glad_glRasterPos3f
GLAD_API_CALL PFNGLRASTERPOS3FVPROC glad_glRasterPos3fv;
#define glRasterPos3fv glad_glRasterPos3fv
GLAD_API_CALL PFNGLRASTERPOS3IPROC glad_glRasterPos3i;
#define glRasterPos3i glad_glRasterPos3i
GLAD_API_CALL PFNGLRASTERPOS3IVPROC glad_glRasterPos3iv;
#define glRasterPos3iv glad_glRasterPos3iv
GLAD_API_CALL PFNGLRASTERPOS3SPROC glad_glRasterPos3s;
#define glRasterPos3s glad_glRasterPos3s
GLAD_API_CALL PFNGLRASTERPOS3SVPROC glad_glRasterPos3sv;
#define glRasterPos3sv glad_glRasterPos3sv
GLAD_API_CALL PFNGLRASTERPOS4DPROC glad_glRasterPos4d;
#define glRasterPos4d glad_glRasterPos4d
GLAD_API_CALL PFNGLRASTERPOS4DVPROC glad_glRasterPos4dv;
#define glRasterPos4dv glad_glRasterPos4dv
GLAD_API_CALL PFNGLRASTERPOS4FPROC glad_glRasterPos4f;
#define glRasterPos4f glad_glRasterPos4f
GLAD_API_CALL PFNGLRASTERPOS4FVPROC glad_glRasterPos4fv;
#define glRasterPos4fv glad_glRasterPos4fv
GLAD_API_CALL PFNGLRASTERPOS4IPROC glad_glRasterPos4i;
#define glRasterPos4i glad_glRasterPos4i
GLAD_API_CALL PFNGLRASTERPOS4IVPROC glad_glRasterPos4iv;
#define glRasterPos4iv glad_glRasterPos4iv
GLAD_API_CALL PFNGLRASTERPOS4SPROC glad_glRasterPos4s;
#define glRasterPos4s glad_glRasterPos4s
GLAD_API_CALL PFNGLRASTERPOS4SVPROC glad_glRasterPos4sv;
#define glRasterPos4sv glad_glRasterPos4sv
GLAD_API_CALL PFNGLREADBUFFERPROC glad_glReadBuffer;
#define glReadBuffer glad_glReadBuffer
GLAD_API_CALL PFNGLREADPIXELSPROC glad_glReadPixels;
#define glReadPixels glad_glReadPixels
GLAD_API_CALL PFNGLREADNPIXELSPROC glad_glReadnPixels;
#define glReadnPixels glad_glReadnPixels
GLAD_API_CALL PFNGLREADNPIXELSARBPROC glad_glReadnPixelsARB;
#define glReadnPixelsARB glad_glReadnPixelsARB
GLAD_API_CALL PFNGLRECTDPROC glad_glRectd;
#define glRectd glad_glRectd
GLAD_API_CALL PFNGLRECTDVPROC glad_glRectdv;
#define glRectdv glad_glRectdv
GLAD_API_CALL PFNGLRECTFPROC glad_glRectf;
#define glRectf glad_glRectf
GLAD_API_CALL PFNGLRECTFVPROC glad_glRectfv;
#define glRectfv glad_glRectfv
GLAD_API_CALL PFNGLRECTIPROC glad_glRecti;
#define glRecti glad_glRecti
GLAD_API_CALL PFNGLRECTIVPROC glad_glRectiv;
#define glRectiv glad_glRectiv
GLAD_API_CALL PFNGLRECTSPROC glad_glRects;
#define glRects glad_glRects
GLAD_API_CALL PFNGLRECTSVPROC glad_glRectsv;
#define glRectsv glad_glRectsv
GLAD_API_CALL PFNGLRELEASESHADERCOMPILERPROC glad_glReleaseShaderCompiler;
#define glReleaseShaderCompiler glad_glReleaseShaderCompiler
GLAD_API_CALL PFNGLRENDERMODEPROC glad_glRenderMode;
#define glRenderMode glad_glRenderMode
GLAD_API_CALL PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;
#define glRenderbufferStorage glad_glRenderbufferStorage
GLAD_API_CALL PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample;
#define glRenderbufferStorageMultisample glad_glRenderbufferStorageMultisample
GLAD_API_CALL PFNGLRESETHISTOGRAMPROC glad_glResetHistogram;
#define glResetHistogram glad_glResetHistogram
GLAD_API_CALL PFNGLRESETMINMAXPROC glad_glResetMinmax;
#define glResetMinmax glad_glResetMinmax
GLAD_API_CALL PFNGLRESUMETRANSFORMFEEDBACKPROC glad_glResumeTransformFeedback;
#define glResumeTransformFeedback glad_glResumeTransformFeedback
GLAD_API_CALL PFNGLROTATEDPROC glad_glRotated;
#define glRotated glad_glRotated
GLAD_API_CALL PFNGLROTATEFPROC glad_glRotatef;
#define glRotatef glad_glRotatef
GLAD_API_CALL PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage;
#define glSampleCoverage glad_glSampleCoverage
GLAD_API_CALL PFNGLSAMPLECOVERAGEARBPROC glad_glSampleCoverageARB;
#define glSampleCoverageARB glad_glSampleCoverageARB
GLAD_API_CALL PFNGLSAMPLEMASKIPROC glad_glSampleMaski;
#define glSampleMaski glad_glSampleMaski
GLAD_API_CALL PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv;
#define glSamplerParameterIiv glad_glSamplerParameterIiv
GLAD_API_CALL PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv;
#define glSamplerParameterIuiv glad_glSamplerParameterIuiv
GLAD_API_CALL PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf;
#define glSamplerParameterf glad_glSamplerParameterf
GLAD_API_CALL PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv;
#define glSamplerParameterfv glad_glSamplerParameterfv
GLAD_API_CALL PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri;
#define glSamplerParameteri glad_glSamplerParameteri
GLAD_API_CALL PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv;
#define glSamplerParameteriv glad_glSamplerParameteriv
GLAD_API_CALL PFNGLSCALEDPROC glad_glScaled;
#define glScaled glad_glScaled
GLAD_API_CALL PFNGLSCALEFPROC glad_glScalef;
#define glScalef glad_glScalef
GLAD_API_CALL PFNGLSCISSORPROC glad_glScissor;
#define glScissor glad_glScissor
GLAD_API_CALL PFNGLSCISSORARRAYVPROC glad_glScissorArrayv;
#define glScissorArrayv glad_glScissorArrayv
GLAD_API_CALL PFNGLSCISSORINDEXEDPROC glad_glScissorIndexed;
#define glScissorIndexed glad_glScissorIndexed
GLAD_API_CALL PFNGLSCISSORINDEXEDVPROC glad_glScissorIndexedv;
#define glScissorIndexedv glad_glScissorIndexedv
GLAD_API_CALL PFNGLSECONDARYCOLOR3BPROC glad_glSecondaryColor3b;
#define glSecondaryColor3b glad_glSecondaryColor3b
GLAD_API_CALL PFNGLSECONDARYCOLOR3BVPROC glad_glSecondaryColor3bv;
#define glSecondaryColor3bv glad_glSecondaryColor3bv
GLAD_API_CALL PFNGLSECONDARYCOLOR3DPROC glad_glSecondaryColor3d;
#define glSecondaryColor3d glad_glSecondaryColor3d
GLAD_API_CALL PFNGLSECONDARYCOLOR3DVPROC glad_glSecondaryColor3dv;
#define glSecondaryColor3dv glad_glSecondaryColor3dv
GLAD_API_CALL PFNGLSECONDARYCOLOR3FPROC glad_glSecondaryColor3f;
#define glSecondaryColor3f glad_glSecondaryColor3f
GLAD_API_CALL PFNGLSECONDARYCOLOR3FVPROC glad_glSecondaryColor3fv;
#define glSecondaryColor3fv glad_glSecondaryColor3fv
GLAD_API_CALL PFNGLSECONDARYCOLOR3IPROC glad_glSecondaryColor3i;
#define glSecondaryColor3i glad_glSecondaryColor3i
GLAD_API_CALL PFNGLSECONDARYCOLOR3IVPROC glad_glSecondaryColor3iv;
#define glSecondaryColor3iv glad_glSecondaryColor3iv
GLAD_API_CALL PFNGLSECONDARYCOLOR3SPROC glad_glSecondaryColor3s;
#define glSecondaryColor3s glad_glSecondaryColor3s
GLAD_API_CALL PFNGLSECONDARYCOLOR3SVPROC glad_glSecondaryColor3sv;
#define glSecondaryColor3sv glad_glSecondaryColor3sv
GLAD_API_CALL PFNGLSECONDARYCOLOR3UBPROC glad_glSecondaryColor3ub;
#define glSecondaryColor3ub glad_glSecondaryColor3ub
GLAD_API_CALL PFNGLSECONDARYCOLOR3UBVPROC glad_glSecondaryColor3ubv;
#define glSecondaryColor3ubv glad_glSecondaryColor3ubv
GLAD_API_CALL PFNGLSECONDARYCOLOR3UIPROC glad_glSecondaryColor3ui;
#define glSecondaryColor3ui glad_glSecondaryColor3ui
GLAD_API_CALL PFNGLSECONDARYCOLOR3UIVPROC glad_glSecondaryColor3uiv;
#define glSecondaryColor3uiv glad_glSecondaryColor3uiv
GLAD_API_CALL PFNGLSECONDARYCOLOR3USPROC glad_glSecondaryColor3us;
#define glSecondaryColor3us glad_glSecondaryColor3us
GLAD_API_CALL PFNGLSECONDARYCOLOR3USVPROC glad_glSecondaryColor3usv;
#define glSecondaryColor3usv glad_glSecondaryColor3usv
GLAD_API_CALL PFNGLSECONDARYCOLORP3UIPROC glad_glSecondaryColorP3ui;
#define glSecondaryColorP3ui glad_glSecondaryColorP3ui
GLAD_API_CALL PFNGLSECONDARYCOLORP3UIVPROC glad_glSecondaryColorP3uiv;
#define glSecondaryColorP3uiv glad_glSecondaryColorP3uiv
GLAD_API_CALL PFNGLSECONDARYCOLORPOINTERPROC glad_glSecondaryColorPointer;
#define glSecondaryColorPointer glad_glSecondaryColorPointer
GLAD_API_CALL PFNGLSELECTBUFFERPROC glad_glSelectBuffer;
#define glSelectBuffer glad_glSelectBuffer
GLAD_API_CALL PFNGLSEPARABLEFILTER2DPROC glad_glSeparableFilter2D;
#define glSeparableFilter2D glad_glSeparableFilter2D
GLAD_API_CALL PFNGLSHADEMODELPROC glad_glShadeModel;
#define glShadeModel glad_glShadeModel
GLAD_API_CALL PFNGLSHADERBINARYPROC glad_glShaderBinary;
#define glShaderBinary glad_glShaderBinary
GLAD_API_CALL PFNGLSHADERSOURCEPROC glad_glShaderSource;
#define glShaderSource glad_glShaderSource
GLAD_API_CALL PFNGLSHADERSOURCEARBPROC glad_glShaderSourceARB;
#define glShaderSourceARB glad_glShaderSourceARB
GLAD_API_CALL PFNGLSHADERSTORAGEBLOCKBINDINGPROC glad_glShaderStorageBlockBinding;
#define glShaderStorageBlockBinding glad_glShaderStorageBlockBinding
GLAD_API_CALL PFNGLSPECIALIZESHADERPROC glad_glSpecializeShader;
#define glSpecializeShader glad_glSpecializeShader
GLAD_API_CALL PFNGLSPECIALIZESHADERARBPROC glad_glSpecializeShaderARB;
#define glSpecializeShaderARB glad_glSpecializeShaderARB
GLAD_API_CALL PFNGLSTENCILFUNCPROC glad_glStencilFunc;
#define glStencilFunc glad_glStencilFunc
GLAD_API_CALL PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate;
#define glStencilFuncSeparate glad_glStencilFuncSeparate
GLAD_API_CALL PFNGLSTENCILMASKPROC glad_glStencilMask;
#define glStencilMask glad_glStencilMask
GLAD_API_CALL PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate;
#define glStencilMaskSeparate glad_glStencilMaskSeparate
GLAD_API_CALL PFNGLSTENCILOPPROC glad_glStencilOp;
#define glStencilOp glad_glStencilOp
GLAD_API_CALL PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate;
#define glStencilOpSeparate glad_glStencilOpSeparate
GLAD_API_CALL PFNGLTEXBUFFERPROC glad_glTexBuffer;
#define glTexBuffer glad_glTexBuffer
GLAD_API_CALL PFNGLTEXBUFFERARBPROC glad_glTexBufferARB;
#define glTexBufferARB glad_glTexBufferARB
GLAD_API_CALL PFNGLTEXBUFFERRANGEPROC glad_glTexBufferRange;
#define glTexBufferRange glad_glTexBufferRange
GLAD_API_CALL PFNGLTEXCOORD1DPROC glad_glTexCoord1d;
#define glTexCoord1d glad_glTexCoord1d
GLAD_API_CALL PFNGLTEXCOORD1DVPROC glad_glTexCoord1dv;
#define glTexCoord1dv glad_glTexCoord1dv
GLAD_API_CALL PFNGLTEXCOORD1FPROC glad_glTexCoord1f;
#define glTexCoord1f glad_glTexCoord1f
GLAD_API_CALL PFNGLTEXCOORD1FVPROC glad_glTexCoord1fv;
#define glTexCoord1fv glad_glTexCoord1fv
GLAD_API_CALL PFNGLTEXCOORD1IPROC glad_glTexCoord1i;
#define glTexCoord1i glad_glTexCoord1i
GLAD_API_CALL PFNGLTEXCOORD1IVPROC glad_glTexCoord1iv;
#define glTexCoord1iv glad_glTexCoord1iv
GLAD_API_CALL PFNGLTEXCOORD1SPROC glad_glTexCoord1s;
#define glTexCoord1s glad_glTexCoord1s
GLAD_API_CALL PFNGLTEXCOORD1SVPROC glad_glTexCoord1sv;
#define glTexCoord1sv glad_glTexCoord1sv
GLAD_API_CALL PFNGLTEXCOORD2DPROC glad_glTexCoord2d;
#define glTexCoord2d glad_glTexCoord2d
GLAD_API_CALL PFNGLTEXCOORD2DVPROC glad_glTexCoord2dv;
#define glTexCoord2dv glad_glTexCoord2dv
GLAD_API_CALL PFNGLTEXCOORD2FPROC glad_glTexCoord2f;
#define glTexCoord2f glad_glTexCoord2f
GLAD_API_CALL PFNGLTEXCOORD2FVPROC glad_glTexCoord2fv;
#define glTexCoord2fv glad_glTexCoord2fv
GLAD_API_CALL PFNGLTEXCOORD2IPROC glad_glTexCoord2i;
#define glTexCoord2i glad_glTexCoord2i
GLAD_API_CALL PFNGLTEXCOORD2IVPROC glad_glTexCoord2iv;
#define glTexCoord2iv glad_glTexCoord2iv
GLAD_API_CALL PFNGLTEXCOORD2SPROC glad_glTexCoord2s;
#define glTexCoord2s glad_glTexCoord2s
GLAD_API_CALL PFNGLTEXCOORD2SVPROC glad_glTexCoord2sv;
#define glTexCoord2sv glad_glTexCoord2sv
GLAD_API_CALL PFNGLTEXCOORD3DPROC glad_glTexCoord3d;
#define glTexCoord3d glad_glTexCoord3d
GLAD_API_CALL PFNGLTEXCOORD3DVPROC glad_glTexCoord3dv;
#define glTexCoord3dv glad_glTexCoord3dv
GLAD_API_CALL PFNGLTEXCOORD3FPROC glad_glTexCoord3f;
#define glTexCoord3f glad_glTexCoord3f
GLAD_API_CALL PFNGLTEXCOORD3FVPROC glad_glTexCoord3fv;
#define glTexCoord3fv glad_glTexCoord3fv
GLAD_API_CALL PFNGLTEXCOORD3IPROC glad_glTexCoord3i;
#define glTexCoord3i glad_glTexCoord3i
GLAD_API_CALL PFNGLTEXCOORD3IVPROC glad_glTexCoord3iv;
#define glTexCoord3iv glad_glTexCoord3iv
GLAD_API_CALL PFNGLTEXCOORD3SPROC glad_glTexCoord3s;
#define glTexCoord3s glad_glTexCoord3s
GLAD_API_CALL PFNGLTEXCOORD3SVPROC glad_glTexCoord3sv;
#define glTexCoord3sv glad_glTexCoord3sv
GLAD_API_CALL PFNGLTEXCOORD4DPROC glad_glTexCoord4d;
#define glTexCoord4d glad_glTexCoord4d
GLAD_API_CALL PFNGLTEXCOORD4DVPROC glad_glTexCoord4dv;
#define glTexCoord4dv glad_glTexCoord4dv
GLAD_API_CALL PFNGLTEXCOORD4FPROC glad_glTexCoord4f;
#define glTexCoord4f glad_glTexCoord4f
GLAD_API_CALL PFNGLTEXCOORD4FVPROC glad_glTexCoord4fv;
#define glTexCoord4fv glad_glTexCoord4fv
GLAD_API_CALL PFNGLTEXCOORD4IPROC glad_glTexCoord4i;
#define glTexCoord4i glad_glTexCoord4i
GLAD_API_CALL PFNGLTEXCOORD4IVPROC glad_glTexCoord4iv;
#define glTexCoord4iv glad_glTexCoord4iv
GLAD_API_CALL PFNGLTEXCOORD4SPROC glad_glTexCoord4s;
#define glTexCoord4s glad_glTexCoord4s
GLAD_API_CALL PFNGLTEXCOORD4SVPROC glad_glTexCoord4sv;
#define glTexCoord4sv glad_glTexCoord4sv
GLAD_API_CALL PFNGLTEXCOORDP1UIPROC glad_glTexCoordP1ui;
#define glTexCoordP1ui glad_glTexCoordP1ui
GLAD_API_CALL PFNGLTEXCOORDP1UIVPROC glad_glTexCoordP1uiv;
#define glTexCoordP1uiv glad_glTexCoordP1uiv
GLAD_API_CALL PFNGLTEXCOORDP2UIPROC glad_glTexCoordP2ui;
#define glTexCoordP2ui glad_glTexCoordP2ui
GLAD_API_CALL PFNGLTEXCOORDP2UIVPROC glad_glTexCoordP2uiv;
#define glTexCoordP2uiv glad_glTexCoordP2uiv
GLAD_API_CALL PFNGLTEXCOORDP3UIPROC glad_glTexCoordP3ui;
#define glTexCoordP3ui glad_glTexCoordP3ui
GLAD_API_CALL PFNGLTEXCOORDP3UIVPROC glad_glTexCoordP3uiv;
#define glTexCoordP3uiv glad_glTexCoordP3uiv
GLAD_API_CALL PFNGLTEXCOORDP4UIPROC glad_glTexCoordP4ui;
#define glTexCoordP4ui glad_glTexCoordP4ui
GLAD_API_CALL PFNGLTEXCOORDP4UIVPROC glad_glTexCoordP4uiv;
#define glTexCoordP4uiv glad_glTexCoordP4uiv
GLAD_API_CALL PFNGLTEXCOORDPOINTERPROC glad_glTexCoordPointer;
#define glTexCoordPointer glad_glTexCoordPointer
GLAD_API_CALL PFNGLTEXENVFPROC glad_glTexEnvf;
#define glTexEnvf glad_glTexEnvf
GLAD_API_CALL PFNGLTEXENVFVPROC glad_glTexEnvfv;
#define glTexEnvfv glad_glTexEnvfv
GLAD_API_CALL PFNGLTEXENVIPROC glad_glTexEnvi;
#define glTexEnvi glad_glTexEnvi
GLAD_API_CALL PFNGLTEXENVIVPROC glad_glTexEnviv;
#define glTexEnviv glad_glTexEnviv
GLAD_API_CALL PFNGLTEXGENDPROC glad_glTexGend;
#define glTexGend glad_glTexGend
GLAD_API_CALL PFNGLTEXGENDVPROC glad_glTexGendv;
#define glTexGendv glad_glTexGendv
GLAD_API_CALL PFNGLTEXGENFPROC glad_glTexGenf;
#define glTexGenf glad_glTexGenf
GLAD_API_CALL PFNGLTEXGENFVPROC glad_glTexGenfv;
#define glTexGenfv glad_glTexGenfv
GLAD_API_CALL PFNGLTEXGENIPROC glad_glTexGeni;
#define glTexGeni glad_glTexGeni
GLAD_API_CALL PFNGLTEXGENIVPROC glad_glTexGeniv;
#define glTexGeniv glad_glTexGeniv
GLAD_API_CALL PFNGLTEXIMAGE1DPROC glad_glTexImage1D;
#define glTexImage1D glad_glTexImage1D
GLAD_API_CALL PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
#define glTexImage2D glad_glTexImage2D
GLAD_API_CALL PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample;
#define glTexImage2DMultisample glad_glTexImage2DMultisample
GLAD_API_CALL PFNGLTEXIMAGE3DPROC glad_glTexImage3D;
#define glTexImage3D glad_glTexImage3D
GLAD_API_CALL PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample;
#define glTexImage3DMultisample glad_glTexImage3DMultisample
GLAD_API_CALL PFNGLTEXPAGECOMMITMENTARBPROC glad_glTexPageCommitmentARB;
#define glTexPageCommitmentARB glad_glTexPageCommitmentARB
GLAD_API_CALL PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv;
#define glTexParameterIiv glad_glTexParameterIiv
GLAD_API_CALL PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv;
#define glTexParameterIuiv glad_glTexParameterIuiv
GLAD_API_CALL PFNGLTEXPARAMETERFPROC glad_glTexParameterf;
#define glTexParameterf glad_glTexParameterf
GLAD_API_CALL PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv;
#define glTexParameterfv glad_glTexParameterfv
GLAD_API_CALL PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
#define glTexParameteri glad_glTexParameteri
GLAD_API_CALL PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv;
#define glTexParameteriv glad_glTexParameteriv
GLAD_API_CALL PFNGLTEXSTORAGE1DPROC glad_glTexStorage1D;
#define glTexStorage1D glad_glTexStorage1D
GLAD_API_CALL PFNGLTEXSTORAGE2DPROC glad_glTexStorage2D;
#define glTexStorage2D glad_glTexStorage2D
GLAD_API_CALL PFNGLTEXSTORAGE2DMULTISAMPLEPROC glad_glTexStorage2DMultisample;
#define glTexStorage2DMultisample glad_glTexStorage2DMultisample
GLAD_API_CALL PFNGLTEXSTORAGE3DPROC glad_glTexStorage3D;
#define glTexStorage3D glad_glTexStorage3D
GLAD_API_CALL PFNGLTEXSTORAGE3DMULTISAMPLEPROC glad_glTexStorage3DMultisample;
#define glTexStorage3DMultisample glad_glTexStorage3DMultisample
GLAD_API_CALL PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D;
#define glTexSubImage1D glad_glTexSubImage1D
GLAD_API_CALL PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D;
#define glTexSubImage2D glad_glTexSubImage2D
GLAD_API_CALL PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D;
#define glTexSubImage3D glad_glTexSubImage3D
GLAD_API_CALL PFNGLTEXTUREBARRIERPROC glad_glTextureBarrier;
#define glTextureBarrier glad_glTextureBarrier
GLAD_API_CALL PFNGLTEXTUREBUFFERPROC glad_glTextureBuffer;
#define glTextureBuffer glad_glTextureBuffer
GLAD_API_CALL PFNGLTEXTUREBUFFERRANGEPROC glad_glTextureBufferRange;
#define glTextureBufferRange glad_glTextureBufferRange
GLAD_API_CALL PFNGLTEXTUREPARAMETERIIVPROC glad_glTextureParameterIiv;
#define glTextureParameterIiv glad_glTextureParameterIiv
GLAD_API_CALL PFNGLTEXTUREPARAMETERIUIVPROC glad_glTextureParameterIuiv;
#define glTextureParameterIuiv glad_glTextureParameterIuiv
GLAD_API_CALL PFNGLTEXTUREPARAMETERFPROC glad_glTextureParameterf;
#define glTextureParameterf glad_glTextureParameterf
GLAD_API_CALL PFNGLTEXTUREPARAMETERFVPROC glad_glTextureParameterfv;
#define glTextureParameterfv glad_glTextureParameterfv
GLAD_API_CALL PFNGLTEXTUREPARAMETERIPROC glad_glTextureParameteri;
#define glTextureParameteri glad_glTextureParameteri
GLAD_API_CALL PFNGLTEXTUREPARAMETERIVPROC glad_glTextureParameteriv;
#define glTextureParameteriv glad_glTextureParameteriv
GLAD_API_CALL PFNGLTEXTURESTORAGE1DPROC glad_glTextureStorage1D;
#define glTextureStorage1D glad_glTextureStorage1D
GLAD_API_CALL PFNGLTEXTURESTORAGE2DPROC glad_glTextureStorage2D;
#define glTextureStorage2D glad_glTextureStorage2D
GLAD_API_CALL PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC glad_glTextureStorage2DMultisample;
#define glTextureStorage2DMultisample glad_glTextureStorage2DMultisample
GLAD_API_CALL PFNGLTEXTURESTORAGE3DPROC glad_glTextureStorage3D;
#define glTextureStorage3D glad_glTextureStorage3D
GLAD_API_CALL PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC glad_glTextureStorage3DMultisample;
#define glTextureStorage3DMultisample glad_glTextureStorage3DMultisample
GLAD_API_CALL PFNGLTEXTURESUBIMAGE1DPROC glad_glTextureSubImage1D;
#define glTextureSubImage1D glad_glTextureSubImage1D
GLAD_API_CALL PFNGLTEXTURESUBIMAGE2DPROC glad_glTextureSubImage2D;
#define glTextureSubImage2D glad_glTextureSubImage2D
GLAD_API_CALL PFNGLTEXTURESUBIMAGE3DPROC glad_glTextureSubImage3D;
#define glTextureSubImage3D glad_glTextureSubImage3D
GLAD_API_CALL PFNGLTEXTUREVIEWPROC glad_glTextureView;
#define glTextureView glad_glTextureView
GLAD_API_CALL PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glad_glTransformFeedbackBufferBase;
#define glTransformFeedbackBufferBase glad_glTransformFeedbackBufferBase
GLAD_API_CALL PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glad_glTransformFeedbackBufferRange;
#define glTransformFeedbackBufferRange glad_glTransformFeedbackBufferRange
GLAD_API_CALL PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings;
#define glTransformFeedbackVaryings glad_glTransformFeedbackVaryings
GLAD_API_CALL PFNGLTRANSLATEDPROC glad_glTranslated;
#define glTranslated glad_glTranslated
GLAD_API_CALL PFNGLTRANSLATEFPROC glad_glTranslatef;
#define glTranslatef glad_glTranslatef
GLAD_API_CALL PFNGLUNIFORM1DPROC glad_glUniform1d;
#define glUniform1d glad_glUniform1d
GLAD_API_CALL PFNGLUNIFORM1DVPROC glad_glUniform1dv;
#define glUniform1dv glad_glUniform1dv
GLAD_API_CALL PFNGLUNIFORM1FPROC glad_glUniform1f;
#define glUniform1f glad_glUniform1f
GLAD_API_CALL PFNGLUNIFORM1FARBPROC glad_glUniform1fARB;
#define glUniform1fARB glad_glUniform1fARB
GLAD_API_CALL PFNGLUNIFORM1FVPROC glad_glUniform1fv;
#define glUniform1fv glad_glUniform1fv
GLAD_API_CALL PFNGLUNIFORM1FVARBPROC glad_glUniform1fvARB;
#define glUniform1fvARB glad_glUniform1fvARB
GLAD_API_CALL PFNGLUNIFORM1IPROC glad_glUniform1i;
#define glUniform1i glad_glUniform1i
GLAD_API_CALL PFNGLUNIFORM1I64ARBPROC glad_glUniform1i64ARB;
#define glUniform1i64ARB glad_glUniform1i64ARB
GLAD_API_CALL PFNGLUNIFORM1I64VARBPROC glad_glUniform1i64vARB;
#define glUniform1i64vARB glad_glUniform1i64vARB
GLAD_API_CALL PFNGLUNIFORM1IARBPROC glad_glUniform1iARB;
#define glUniform1iARB glad_glUniform1iARB
GLAD_API_CALL PFNGLUNIFORM1IVPROC glad_glUniform1iv;
#define glUniform1iv glad_glUniform1iv
GLAD_API_CALL PFNGLUNIFORM1IVARBPROC glad_glUniform1ivARB;
#define glUniform1ivARB glad_glUniform1ivARB
GLAD_API_CALL PFNGLUNIFORM1UIPROC glad_glUniform1ui;
#define glUniform1ui glad_glUniform1ui
GLAD_API_CALL PFNGLUNIFORM1UI64ARBPROC glad_glUniform1ui64ARB;
#define glUniform1ui64ARB glad_glUniform1ui64ARB
GLAD_API_CALL PFNGLUNIFORM1UI64VARBPROC glad_glUniform1ui64vARB;
#define glUniform1ui64vARB glad_glUniform1ui64vARB
GLAD_API_CALL PFNGLUNIFORM1UIVPROC glad_glUniform1uiv;
#define glUniform1uiv glad_glUniform1uiv
GLAD_API_CALL PFNGLUNIFORM2DPROC glad_glUniform2d;
#define glUniform2d glad_glUniform2d
GLAD_API_CALL PFNGLUNIFORM2DVPROC glad_glUniform2dv;
#define glUniform2dv glad_glUniform2dv
GLAD_API_CALL PFNGLUNIFORM2FPROC glad_glUniform2f;
#define glUniform2f glad_glUniform2f
GLAD_API_CALL PFNGLUNIFORM2FARBPROC glad_glUniform2fARB;
#define glUniform2fARB glad_glUniform2fARB
GLAD_API_CALL PFNGLUNIFORM2FVPROC glad_glUniform2fv;
#define glUniform2fv glad_glUniform2fv
GLAD_API_CALL PFNGLUNIFORM2FVARBPROC glad_glUniform2fvARB;
#define glUniform2fvARB glad_glUniform2fvARB
GLAD_API_CALL PFNGLUNIFORM2IPROC glad_glUniform2i;
#define glUniform2i glad_glUniform2i
GLAD_API_CALL PFNGLUNIFORM2I64ARBPROC glad_glUniform2i64ARB;
#define glUniform2i64ARB glad_glUniform2i64ARB
GLAD_API_CALL PFNGLUNIFORM2I64VARBPROC glad_glUniform2i64vARB;
#define glUniform2i64vARB glad_glUniform2i64vARB
GLAD_API_CALL PFNGLUNIFORM2IARBPROC glad_glUniform2iARB;
#define glUniform2iARB glad_glUniform2iARB
GLAD_API_CALL PFNGLUNIFORM2IVPROC glad_glUniform2iv;
#define glUniform2iv glad_glUniform2iv
GLAD_API_CALL PFNGLUNIFORM2IVARBPROC glad_glUniform2ivARB;
#define glUniform2ivARB glad_glUniform2ivARB
GLAD_API_CALL PFNGLUNIFORM2UIPROC glad_glUniform2ui;
#define glUniform2ui glad_glUniform2ui
GLAD_API_CALL PFNGLUNIFORM2UI64ARBPROC glad_glUniform2ui64ARB;
#define glUniform2ui64ARB glad_glUniform2ui64ARB
GLAD_API_CALL PFNGLUNIFORM2UI64VARBPROC glad_glUniform2ui64vARB;
#define glUniform2ui64vARB glad_glUniform2ui64vARB
GLAD_API_CALL PFNGLUNIFORM2UIVPROC glad_glUniform2uiv;
#define glUniform2uiv glad_glUniform2uiv
GLAD_API_CALL PFNGLUNIFORM3DPROC glad_glUniform3d;
#define glUniform3d glad_glUniform3d
GLAD_API_CALL PFNGLUNIFORM3DVPROC glad_glUniform3dv;
#define glUniform3dv glad_glUniform3dv
GLAD_API_CALL PFNGLUNIFORM3FPROC glad_glUniform3f;
#define glUniform3f glad_glUniform3f
GLAD_API_CALL PFNGLUNIFORM3FARBPROC glad_glUniform3fARB;
#define glUniform3fARB glad_glUniform3fARB
GLAD_API_CALL PFNGLUNIFORM3FVPROC glad_glUniform3fv;
#define glUniform3fv glad_glUniform3fv
GLAD_API_CALL PFNGLUNIFORM3FVARBPROC glad_glUniform3fvARB;
#define glUniform3fvARB glad_glUniform3fvARB
GLAD_API_CALL PFNGLUNIFORM3IPROC glad_glUniform3i;
#define glUniform3i glad_glUniform3i
GLAD_API_CALL PFNGLUNIFORM3I64ARBPROC glad_glUniform3i64ARB;
#define glUniform3i64ARB glad_glUniform3i64ARB
GLAD_API_CALL PFNGLUNIFORM3I64VARBPROC glad_glUniform3i64vARB;
#define glUniform3i64vARB glad_glUniform3i64vARB
GLAD_API_CALL PFNGLUNIFORM3IARBPROC glad_glUniform3iARB;
#define glUniform3iARB glad_glUniform3iARB
GLAD_API_CALL PFNGLUNIFORM3IVPROC glad_glUniform3iv;
#define glUniform3iv glad_glUniform3iv
GLAD_API_CALL PFNGLUNIFORM3IVARBPROC glad_glUniform3ivARB;
#define glUniform3ivARB glad_glUniform3ivARB
GLAD_API_CALL PFNGLUNIFORM3UIPROC glad_glUniform3ui;
#define glUniform3ui glad_glUniform3ui
GLAD_API_CALL PFNGLUNIFORM3UI64ARBPROC glad_glUniform3ui64ARB;
#define glUniform3ui64ARB glad_glUniform3ui64ARB
GLAD_API_CALL PFNGLUNIFORM3UI64VARBPROC glad_glUniform3ui64vARB;
#define glUniform3ui64vARB glad_glUniform3ui64vARB
GLAD_API_CALL PFNGLUNIFORM3UIVPROC glad_glUniform3uiv;
#define glUniform3uiv glad_glUniform3uiv
GLAD_API_CALL PFNGLUNIFORM4DPROC glad_glUniform4d;
#define glUniform4d glad_glUniform4d
GLAD_API_CALL PFNGLUNIFORM4DVPROC glad_glUniform4dv;
#define glUniform4dv glad_glUniform4dv
GLAD_API_CALL PFNGLUNIFORM4FPROC glad_glUniform4f;
#define glUniform4f glad_glUniform4f
GLAD_API_CALL PFNGLUNIFORM4FARBPROC glad_glUniform4fARB;
#define glUniform4fARB glad_glUniform4fARB
GLAD_API_CALL PFNGLUNIFORM4FVPROC glad_glUniform4fv;
#define glUniform4fv glad_glUniform4fv
GLAD_API_CALL PFNGLUNIFORM4FVARBPROC glad_glUniform4fvARB;
#define glUniform4fvARB glad_glUniform4fvARB
GLAD_API_CALL PFNGLUNIFORM4IPROC glad_glUniform4i;
#define glUniform4i glad_glUniform4i
GLAD_API_CALL PFNGLUNIFORM4I64ARBPROC glad_glUniform4i64ARB;
#define glUniform4i64ARB glad_glUniform4i64ARB
GLAD_API_CALL PFNGLUNIFORM4I64VARBPROC glad_glUniform4i64vARB;
#define glUniform4i64vARB glad_glUniform4i64vARB
GLAD_API_CALL PFNGLUNIFORM4IARBPROC glad_glUniform4iARB;
#define glUniform4iARB glad_glUniform4iARB
GLAD_API_CALL PFNGLUNIFORM4IVPROC glad_glUniform4iv;
#define glUniform4iv glad_glUniform4iv
GLAD_API_CALL PFNGLUNIFORM4IVARBPROC glad_glUniform4ivARB;
#define glUniform4ivARB glad_glUniform4ivARB
GLAD_API_CALL PFNGLUNIFORM4UIPROC glad_glUniform4ui;
#define glUniform4ui glad_glUniform4ui
GLAD_API_CALL PFNGLUNIFORM4UI64ARBPROC glad_glUniform4ui64ARB;
#define glUniform4ui64ARB glad_glUniform4ui64ARB
GLAD_API_CALL PFNGLUNIFORM4UI64VARBPROC glad_glUniform4ui64vARB;
#define glUniform4ui64vARB glad_glUniform4ui64vARB
GLAD_API_CALL PFNGLUNIFORM4UIVPROC glad_glUniform4uiv;
#define glUniform4uiv glad_glUniform4uiv
GLAD_API_CALL PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding;
#define glUniformBlockBinding glad_glUniformBlockBinding
GLAD_API_CALL PFNGLUNIFORMHANDLEUI64ARBPROC glad_glUniformHandleui64ARB;
#define glUniformHandleui64ARB glad_glUniformHandleui64ARB
GLAD_API_CALL PFNGLUNIFORMHANDLEUI64VARBPROC glad_glUniformHandleui64vARB;
#define glUniformHandleui64vARB glad_glUniformHandleui64vARB
GLAD_API_CALL PFNGLUNIFORMMATRIX2DVPROC glad_glUniformMatrix2dv;
#define glUniformMatrix2dv glad_glUniformMatrix2dv
GLAD_API_CALL PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv;
#define glUniformMatrix2fv glad_glUniformMatrix2fv
GLAD_API_CALL PFNGLUNIFORMMATRIX2FVARBPROC glad_glUniformMatrix2fvARB;
#define glUniformMatrix2fvARB glad_glUniformMatrix2fvARB
GLAD_API_CALL PFNGLUNIFORMMATRIX2X3DVPROC glad_glUniformMatrix2x3dv;
#define glUniformMatrix2x3dv glad_glUniformMatrix2x3dv
GLAD_API_CALL PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv;
#define glUniformMatrix2x3fv glad_glUniformMatrix2x3fv
GLAD_API_CALL PFNGLUNIFORMMATRIX2X4DVPROC glad_glUniformMatrix2x4dv;
#define glUniformMatrix2x4dv glad_glUniformMatrix2x4dv
GLAD_API_CALL PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv;
#define glUniformMatrix2x4fv glad_glUniformMatrix2x4fv
GLAD_API_CALL PFNGLUNIFORMMATRIX3DVPROC glad_glUniformMatrix3dv;
#define glUniformMatrix3dv glad_glUniformMatrix3dv
GLAD_API_CALL PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv;
#define glUniformMatrix3fv glad_glUniformMatrix3fv
GLAD_API_CALL PFNGLUNIFORMMATRIX3FVARBPROC glad_glUniformMatrix3fvARB;
#define glUniformMatrix3fvARB glad_glUniformMatrix3fvARB
GLAD_API_CALL PFNGLUNIFORMMATRIX3X2DVPROC glad_glUniformMatrix3x2dv;
#define glUniformMatrix3x2dv glad_glUniformMatrix3x2dv
GLAD_API_CALL PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv;
#define glUniformMatrix3x2fv glad_glUniformMatrix3x2fv
GLAD_API_CALL PFNGLUNIFORMMATRIX3X4DVPROC glad_glUniformMatrix3x4dv;
#define glUniformMatrix3x4dv glad_glUniformMatrix3x4dv
GLAD_API_CALL PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv;
#define glUniformMatrix3x4fv glad_glUniformMatrix3x4fv
GLAD_API_CALL PFNGLUNIFORMMATRIX4DVPROC glad_glUniformMatrix4dv;
#define glUniformMatrix4dv glad_glUniformMatrix4dv
GLAD_API_CALL PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
#define glUniformMatrix4fv glad_glUniformMatrix4fv
GLAD_API_CALL PFNGLUNIFORMMATRIX4FVARBPROC glad_glUniformMatrix4fvARB;
#define glUniformMatrix4fvARB glad_glUniformMatrix4fvARB
GLAD_API_CALL PFNGLUNIFORMMATRIX4X2DVPROC glad_glUniformMatrix4x2dv;
#define glUniformMatrix4x2dv glad_glUniformMatrix4x2dv
GLAD_API_CALL PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv;
#define glUniformMatrix4x2fv glad_glUniformMatrix4x2fv
GLAD_API_CALL PFNGLUNIFORMMATRIX4X3DVPROC glad_glUniformMatrix4x3dv;
#define glUniformMatrix4x3dv glad_glUniformMatrix4x3dv
GLAD_API_CALL PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv;
#define glUniformMatrix4x3fv glad_glUniformMatrix4x3fv
GLAD_API_CALL PFNGLUNIFORMSUBROUTINESUIVPROC glad_glUniformSubroutinesuiv;
#define glUniformSubroutinesuiv glad_glUniformSubroutinesuiv
GLAD_API_CALL PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer;
#define glUnmapBuffer glad_glUnmapBuffer
GLAD_API_CALL PFNGLUNMAPBUFFERARBPROC glad_glUnmapBufferARB;
#define glUnmapBufferARB glad_glUnmapBufferARB
GLAD_API_CALL PFNGLUNMAPNAMEDBUFFERPROC glad_glUnmapNamedBuffer;
#define glUnmapNamedBuffer glad_glUnmapNamedBuffer
GLAD_API_CALL PFNGLUSEPROGRAMPROC glad_glUseProgram;
#define glUseProgram glad_glUseProgram
GLAD_API_CALL PFNGLUSEPROGRAMOBJECTARBPROC glad_glUseProgramObjectARB;
#define glUseProgramObjectARB glad_glUseProgramObjectARB
GLAD_API_CALL PFNGLUSEPROGRAMSTAGESPROC glad_glUseProgramStages;
#define glUseProgramStages glad_glUseProgramStages
GLAD_API_CALL PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram;
#define glValidateProgram glad_glValidateProgram
GLAD_API_CALL PFNGLVALIDATEPROGRAMARBPROC glad_glValidateProgramARB;
#define glValidateProgramARB glad_glValidateProgramARB
GLAD_API_CALL PFNGLVALIDATEPROGRAMPIPELINEPROC glad_glValidateProgramPipeline;
#define glValidateProgramPipeline glad_glValidateProgramPipeline
GLAD_API_CALL PFNGLVERTEX2DPROC glad_glVertex2d;
#define glVertex2d glad_glVertex2d
GLAD_API_CALL PFNGLVERTEX2DVPROC glad_glVertex2dv;
#define glVertex2dv glad_glVertex2dv
GLAD_API_CALL PFNGLVERTEX2FPROC glad_glVertex2f;
#define glVertex2f glad_glVertex2f
GLAD_API_CALL PFNGLVERTEX2FVPROC glad_glVertex2fv;
#define glVertex2fv glad_glVertex2fv
GLAD_API_CALL PFNGLVERTEX2IPROC glad_glVertex2i;
#define glVertex2i glad_glVertex2i
GLAD_API_CALL PFNGLVERTEX2IVPROC glad_glVertex2iv;
#define glVertex2iv glad_glVertex2iv
GLAD_API_CALL PFNGLVERTEX2SPROC glad_glVertex2s;
#define glVertex2s glad_glVertex2s
GLAD_API_CALL PFNGLVERTEX2SVPROC glad_glVertex2sv;
#define glVertex2sv glad_glVertex2sv
GLAD_API_CALL PFNGLVERTEX3DPROC glad_glVertex3d;
#define glVertex3d glad_glVertex3d
GLAD_API_CALL PFNGLVERTEX3DVPROC glad_glVertex3dv;
#define glVertex3dv glad_glVertex3dv
GLAD_API_CALL PFNGLVERTEX3FPROC glad_glVertex3f;
#define glVertex3f glad_glVertex3f
GLAD_API_CALL PFNGLVERTEX3FVPROC glad_glVertex3fv;
#define glVertex3fv glad_glVertex3fv
GLAD_API_CALL PFNGLVERTEX3IPROC glad_glVertex3i;
#define glVertex3i glad_glVertex3i
GLAD_API_CALL PFNGLVERTEX3IVPROC glad_glVertex3iv;
#define glVertex3iv glad_glVertex3iv
GLAD_API_CALL PFNGLVERTEX3SPROC glad_glVertex3s;
#define glVertex3s glad_glVertex3s
GLAD_API_CALL PFNGLVERTEX3SVPROC glad_glVertex3sv;
#define glVertex3sv glad_glVertex3sv
GLAD_API_CALL PFNGLVERTEX4DPROC glad_glVertex4d;
#define glVertex4d glad_glVertex4d
GLAD_API_CALL PFNGLVERTEX4DVPROC glad_glVertex4dv;
#define glVertex4dv glad_glVertex4dv
GLAD_API_CALL PFNGLVERTEX4FPROC glad_glVertex4f;
#define glVertex4f glad_glVertex4f
GLAD_API_CALL PFNGLVERTEX4FVPROC glad_glVertex4fv;
#define glVertex4fv glad_glVertex4fv
GLAD_API_CALL PFNGLVERTEX4IPROC glad_glVertex4i;
#define glVertex4i glad_glVertex4i
GLAD_API_CALL PFNGLVERTEX4IVPROC glad_glVertex4iv;
#define glVertex4iv glad_glVertex4iv
GLAD_API_CALL PFNGLVERTEX4SPROC glad_glVertex4s;
#define glVertex4s glad_glVertex4s
GLAD_API_CALL PFNGLVERTEX4SVPROC glad_glVertex4sv;
#define glVertex4sv glad_glVertex4sv
GLAD_API_CALL PFNGLVERTEXARRAYATTRIBBINDINGPROC glad_glVertexArrayAttribBinding;
#define glVertexArrayAttribBinding glad_glVertexArrayAttribBinding
GLAD_API_CALL PFNGLVERTEXARRAYATTRIBFORMATPROC glad_glVertexArrayAttribFormat;
#define glVertexArrayAttribFormat glad_glVertexArrayAttribFormat
GLAD_API_CALL PFNGLVERTEXARRAYATTRIBIFORMATPROC glad_glVertexArrayAttribIFormat;
#define glVertexArrayAttribIFormat glad_glVertexArrayAttribIFormat
GLAD_API_CALL PFNGLVERTEXARRAYATTRIBLFORMATPROC glad_glVertexArrayAttribLFormat;
#define glVertexArrayAttribLFormat glad_glVertexArrayAttribLFormat
GLAD_API_CALL PFNGLVERTEXARRAYBINDINGDIVISORPROC glad_glVertexArrayBindingDivisor;
#define glVertexArrayBindingDivisor glad_glVertexArrayBindingDivisor
GLAD_API_CALL PFNGLVERTEXARRAYELEMENTBUFFERPROC glad_glVertexArrayElementBuffer;
#define glVertexArrayElementBuffer glad_glVertexArrayElementBuffer
GLAD_API_CALL PFNGLVERTEXARRAYVERTEXBUFFERPROC glad_glVertexArrayVertexBuffer;
#define glVertexArrayVertexBuffer glad_glVertexArrayVertexBuffer
GLAD_API_CALL PFNGLVERTEXARRAYVERTEXBUFFERSPROC glad_glVertexArrayVertexBuffers;
#define glVertexArrayVertexBuffers glad_glVertexArrayVertexBuffers
GLAD_API_CALL PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d;
#define glVertexAttrib1d glad_glVertexAttrib1d
GLAD_API_CALL PFNGLVERTEXATTRIB1DARBPROC glad_glVertexAttrib1dARB;
#define glVertexAttrib1dARB glad_glVertexAttrib1dARB
GLAD_API_CALL PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv;
#define glVertexAttrib1dv glad_glVertexAttrib1dv
GLAD_API_CALL PFNGLVERTEXATTRIB1DVARBPROC glad_glVertexAttrib1dvARB;
#define glVertexAttrib1dvARB glad_glVertexAttrib1dvARB
GLAD_API_CALL PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f;
#define glVertexAttrib1f glad_glVertexAttrib1f
GLAD_API_CALL PFNGLVERTEXATTRIB1FARBPROC glad_glVertexAttrib1fARB;
#define glVertexAttrib1fARB glad_glVertexAttrib1fARB
GLAD_API_CALL PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv;
#define glVertexAttrib1fv glad_glVertexAttrib1fv
GLAD_API_CALL PFNGLVERTEXATTRIB1FVARBPROC glad_glVertexAttrib1fvARB;
#define glVertexAttrib1fvARB glad_glVertexAttrib1fvARB
GLAD_API_CALL PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s;
#define glVertexAttrib1s glad_glVertexAttrib1s
GLAD_API_CALL PFNGLVERTEXATTRIB1SARBPROC glad_glVertexAttrib1sARB;
#define glVertexAttrib1sARB glad_glVertexAttrib1sARB
GLAD_API_CALL PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv;
#define glVertexAttrib1sv glad_glVertexAttrib1sv
GLAD_API_CALL PFNGLVERTEXATTRIB1SVARBPROC glad_glVertexAttrib1svARB;
#define glVertexAttrib1svARB glad_glVertexAttrib1svARB
GLAD_API_CALL PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d;
#define glVertexAttrib2d glad_glVertexAttrib2d
GLAD_API_CALL PFNGLVERTEXATTRIB2DARBPROC glad_glVertexAttrib2dARB;
#define glVertexAttrib2dARB glad_glVertexAttrib2dARB
GLAD_API_CALL PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv;
#define glVertexAttrib2dv glad_glVertexAttrib2dv
GLAD_API_CALL PFNGLVERTEXATTRIB2DVARBPROC glad_glVertexAttrib2dvARB;
#define glVertexAttrib2dvARB glad_glVertexAttrib2dvARB
GLAD_API_CALL PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f;
#define glVertexAttrib2f glad_glVertexAttrib2f
GLAD_API_CALL PFNGLVERTEXATTRIB2FARBPROC glad_glVertexAttrib2fARB;
#define glVertexAttrib2fARB glad_glVertexAttrib2fARB
GLAD_API_CALL PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv;
#define glVertexAttrib2fv glad_glVertexAttrib2fv
GLAD_API_CALL PFNGLVERTEXATTRIB2FVARBPROC glad_glVertexAttrib2fvARB;
#define glVertexAttrib2fvARB glad_glVertexAttrib2fvARB
GLAD_API_CALL PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s;
#define glVertexAttrib2s glad_glVertexAttrib2s
GLAD_API_CALL PFNGLVERTEXATTRIB2SARBPROC glad_glVertexAttrib2sARB;
#define glVertexAttrib2sARB glad_glVertexAttrib2sARB
GLAD_API_CALL PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv;
#define glVertexAttrib2sv glad_glVertexAttrib2sv
GLAD_API_CALL PFNGLVERTEXATTRIB2SVARBPROC glad_glVertexAttrib2svARB;
#define glVertexAttrib2svARB glad_glVertexAttrib2svARB
GLAD_API_CALL PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d;
#define glVertexAttrib3d glad_glVertexAttrib3d
GLAD_API_CALL PFNGLVERTEXATTRIB3DARBPROC glad_glVertexAttrib3dARB;
#define glVertexAttrib3dARB glad_glVertexAttrib3dARB
GLAD_API_CALL PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv;
#define glVertexAttrib3dv glad_glVertexAttrib3dv
GLAD_API_CALL PFNGLVERTEXATTRIB3DVARBPROC glad_glVertexAttrib3dvARB;
#define glVertexAttrib3dvARB glad_glVertexAttrib3dvARB
GLAD_API_CALL PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f;
#define glVertexAttrib3f glad_glVertexAttrib3f
GLAD_API_CALL PFNGLVERTEXATTRIB3FARBPROC glad_glVertexAttrib3fARB;
#define glVertexAttrib3fARB glad_glVertexAttrib3fARB
GLAD_API_CALL PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv;
#define glVertexAttrib3fv glad_glVertexAttrib3fv
GLAD_API_CALL PFNGLVERTEXATTRIB3FVARBPROC glad_glVertexAttrib3fvARB;
#define glVertexAttrib3fvARB glad_glVertexAttrib3fvARB
GLAD_API_CALL PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s;
#define glVertexAttrib3s glad_glVertexAttrib3s
GLAD_API_CALL PFNGLVERTEXATTRIB3SARBPROC glad_glVertexAttrib3sARB;
#define glVertexAttrib3sARB glad_glVertexAttrib3sARB
GLAD_API_CALL PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv;
#define glVertexAttrib3sv glad_glVertexAttrib3sv
GLAD_API_CALL PFNGLVERTEXATTRIB3SVARBPROC glad_glVertexAttrib3svARB;
#define glVertexAttrib3svARB glad_glVertexAttrib3svARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv;
#define glVertexAttrib4Nbv glad_glVertexAttrib4Nbv
GLAD_API_CALL PFNGLVERTEXATTRIB4NBVARBPROC glad_glVertexAttrib4NbvARB;
#define glVertexAttrib4NbvARB glad_glVertexAttrib4NbvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv;
#define glVertexAttrib4Niv glad_glVertexAttrib4Niv
GLAD_API_CALL PFNGLVERTEXATTRIB4NIVARBPROC glad_glVertexAttrib4NivARB;
#define glVertexAttrib4NivARB glad_glVertexAttrib4NivARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv;
#define glVertexAttrib4Nsv glad_glVertexAttrib4Nsv
GLAD_API_CALL PFNGLVERTEXATTRIB4NSVARBPROC glad_glVertexAttrib4NsvARB;
#define glVertexAttrib4NsvARB glad_glVertexAttrib4NsvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub;
#define glVertexAttrib4Nub glad_glVertexAttrib4Nub
GLAD_API_CALL PFNGLVERTEXATTRIB4NUBARBPROC glad_glVertexAttrib4NubARB;
#define glVertexAttrib4NubARB glad_glVertexAttrib4NubARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv;
#define glVertexAttrib4Nubv glad_glVertexAttrib4Nubv
GLAD_API_CALL PFNGLVERTEXATTRIB4NUBVARBPROC glad_glVertexAttrib4NubvARB;
#define glVertexAttrib4NubvARB glad_glVertexAttrib4NubvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv;
#define glVertexAttrib4Nuiv glad_glVertexAttrib4Nuiv
GLAD_API_CALL PFNGLVERTEXATTRIB4NUIVARBPROC glad_glVertexAttrib4NuivARB;
#define glVertexAttrib4NuivARB glad_glVertexAttrib4NuivARB
GLAD_API_CALL PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv;
#define glVertexAttrib4Nusv glad_glVertexAttrib4Nusv
GLAD_API_CALL PFNGLVERTEXATTRIB4NUSVARBPROC glad_glVertexAttrib4NusvARB;
#define glVertexAttrib4NusvARB glad_glVertexAttrib4NusvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv;
#define glVertexAttrib4bv glad_glVertexAttrib4bv
GLAD_API_CALL PFNGLVERTEXATTRIB4BVARBPROC glad_glVertexAttrib4bvARB;
#define glVertexAttrib4bvARB glad_glVertexAttrib4bvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d;
#define glVertexAttrib4d glad_glVertexAttrib4d
GLAD_API_CALL PFNGLVERTEXATTRIB4DARBPROC glad_glVertexAttrib4dARB;
#define glVertexAttrib4dARB glad_glVertexAttrib4dARB
GLAD_API_CALL PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv;
#define glVertexAttrib4dv glad_glVertexAttrib4dv
GLAD_API_CALL PFNGLVERTEXATTRIB4DVARBPROC glad_glVertexAttrib4dvARB;
#define glVertexAttrib4dvARB glad_glVertexAttrib4dvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f;
#define glVertexAttrib4f glad_glVertexAttrib4f
GLAD_API_CALL PFNGLVERTEXATTRIB4FARBPROC glad_glVertexAttrib4fARB;
#define glVertexAttrib4fARB glad_glVertexAttrib4fARB
GLAD_API_CALL PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv;
#define glVertexAttrib4fv glad_glVertexAttrib4fv
GLAD_API_CALL PFNGLVERTEXATTRIB4FVARBPROC glad_glVertexAttrib4fvARB;
#define glVertexAttrib4fvARB glad_glVertexAttrib4fvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv;
#define glVertexAttrib4iv glad_glVertexAttrib4iv
GLAD_API_CALL PFNGLVERTEXATTRIB4IVARBPROC glad_glVertexAttrib4ivARB;
#define glVertexAttrib4ivARB glad_glVertexAttrib4ivARB
GLAD_API_CALL PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s;
#define glVertexAttrib4s glad_glVertexAttrib4s
GLAD_API_CALL PFNGLVERTEXATTRIB4SARBPROC glad_glVertexAttrib4sARB;
#define glVertexAttrib4sARB glad_glVertexAttrib4sARB
GLAD_API_CALL PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv;
#define glVertexAttrib4sv glad_glVertexAttrib4sv
GLAD_API_CALL PFNGLVERTEXATTRIB4SVARBPROC glad_glVertexAttrib4svARB;
#define glVertexAttrib4svARB glad_glVertexAttrib4svARB
GLAD_API_CALL PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv;
#define glVertexAttrib4ubv glad_glVertexAttrib4ubv
GLAD_API_CALL PFNGLVERTEXATTRIB4UBVARBPROC glad_glVertexAttrib4ubvARB;
#define glVertexAttrib4ubvARB glad_glVertexAttrib4ubvARB
GLAD_API_CALL PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv;
#define glVertexAttrib4uiv glad_glVertexAttrib4uiv
GLAD_API_CALL PFNGLVERTEXATTRIB4UIVARBPROC glad_glVertexAttrib4uivARB;
#define glVertexAttrib4uivARB glad_glVertexAttrib4uivARB
GLAD_API_CALL PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv;
#define glVertexAttrib4usv glad_glVertexAttrib4usv
GLAD_API_CALL PFNGLVERTEXATTRIB4USVARBPROC glad_glVertexAttrib4usvARB;
#define glVertexAttrib4usvARB glad_glVertexAttrib4usvARB
GLAD_API_CALL PFNGLVERTEXATTRIBBINDINGPROC glad_glVertexAttribBinding;
#define glVertexAttribBinding glad_glVertexAttribBinding
GLAD_API_CALL PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor;
#define glVertexAttribDivisor glad_glVertexAttribDivisor
GLAD_API_CALL PFNGLVERTEXATTRIBDIVISORARBPROC glad_glVertexAttribDivisorARB;
#define glVertexAttribDivisorARB glad_glVertexAttribDivisorARB
GLAD_API_CALL PFNGLVERTEXATTRIBFORMATPROC glad_glVertexAttribFormat;
#define glVertexAttribFormat glad_glVertexAttribFormat
GLAD_API_CALL PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i;
#define glVertexAttribI1i glad_glVertexAttribI1i
GLAD_API_CALL PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv;
#define glVertexAttribI1iv glad_glVertexAttribI1iv
GLAD_API_CALL PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui;
#define glVertexAttribI1ui glad_glVertexAttribI1ui
GLAD_API_CALL PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv;
#define glVertexAttribI1uiv glad_glVertexAttribI1uiv
GLAD_API_CALL PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i;
#define glVertexAttribI2i glad_glVertexAttribI2i
GLAD_API_CALL PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv;
#define glVertexAttribI2iv glad_glVertexAttribI2iv
GLAD_API_CALL PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui;
#define glVertexAttribI2ui glad_glVertexAttribI2ui
GLAD_API_CALL PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv;
#define glVertexAttribI2uiv glad_glVertexAttribI2uiv
GLAD_API_CALL PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i;
#define glVertexAttribI3i glad_glVertexAttribI3i
GLAD_API_CALL PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv;
#define glVertexAttribI3iv glad_glVertexAttribI3iv
GLAD_API_CALL PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui;
#define glVertexAttribI3ui glad_glVertexAttribI3ui
GLAD_API_CALL PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv;
#define glVertexAttribI3uiv glad_glVertexAttribI3uiv
GLAD_API_CALL PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv;
#define glVertexAttribI4bv glad_glVertexAttribI4bv
GLAD_API_CALL PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i;
#define glVertexAttribI4i glad_glVertexAttribI4i
GLAD_API_CALL PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv;
#define glVertexAttribI4iv glad_glVertexAttribI4iv
GLAD_API_CALL PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv;
#define glVertexAttribI4sv glad_glVertexAttribI4sv
GLAD_API_CALL PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv;
#define glVertexAttribI4ubv glad_glVertexAttribI4ubv
GLAD_API_CALL PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui;
#define glVertexAttribI4ui glad_glVertexAttribI4ui
GLAD_API_CALL PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv;
#define glVertexAttribI4uiv glad_glVertexAttribI4uiv
GLAD_API_CALL PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv;
#define glVertexAttribI4usv glad_glVertexAttribI4usv
GLAD_API_CALL PFNGLVERTEXATTRIBIFORMATPROC glad_glVertexAttribIFormat;
#define glVertexAttribIFormat glad_glVertexAttribIFormat
GLAD_API_CALL PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer;
#define glVertexAttribIPointer glad_glVertexAttribIPointer
GLAD_API_CALL PFNGLVERTEXATTRIBL1DPROC glad_glVertexAttribL1d;
#define glVertexAttribL1d glad_glVertexAttribL1d
GLAD_API_CALL PFNGLVERTEXATTRIBL1DVPROC glad_glVertexAttribL1dv;
#define glVertexAttribL1dv glad_glVertexAttribL1dv
GLAD_API_CALL PFNGLVERTEXATTRIBL1UI64ARBPROC glad_glVertexAttribL1ui64ARB;
#define glVertexAttribL1ui64ARB glad_glVertexAttribL1ui64ARB
GLAD_API_CALL PFNGLVERTEXATTRIBL1UI64VARBPROC glad_glVertexAttribL1ui64vARB;
#define glVertexAttribL1ui64vARB glad_glVertexAttribL1ui64vARB
GLAD_API_CALL PFNGLVERTEXATTRIBL2DPROC glad_glVertexAttribL2d;
#define glVertexAttribL2d glad_glVertexAttribL2d
GLAD_API_CALL PFNGLVERTEXATTRIBL2DVPROC glad_glVertexAttribL2dv;
#define glVertexAttribL2dv glad_glVertexAttribL2dv
GLAD_API_CALL PFNGLVERTEXATTRIBL3DPROC glad_glVertexAttribL3d;
#define glVertexAttribL3d glad_glVertexAttribL3d
GLAD_API_CALL PFNGLVERTEXATTRIBL3DVPROC glad_glVertexAttribL3dv;
#define glVertexAttribL3dv glad_glVertexAttribL3dv
GLAD_API_CALL PFNGLVERTEXATTRIBL4DPROC glad_glVertexAttribL4d;
#define glVertexAttribL4d glad_glVertexAttribL4d
GLAD_API_CALL PFNGLVERTEXATTRIBL4DVPROC glad_glVertexAttribL4dv;
#define glVertexAttribL4dv glad_glVertexAttribL4dv
GLAD_API_CALL PFNGLVERTEXATTRIBLFORMATPROC glad_glVertexAttribLFormat;
#define glVertexAttribLFormat glad_glVertexAttribLFormat
GLAD_API_CALL PFNGLVERTEXATTRIBLPOINTERPROC glad_glVertexAttribLPointer;
#define glVertexAttribLPointer glad_glVertexAttribLPointer
GLAD_API_CALL PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui;
#define glVertexAttribP1ui glad_glVertexAttribP1ui
GLAD_API_CALL PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv;
#define glVertexAttribP1uiv glad_glVertexAttribP1uiv
GLAD_API_CALL PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui;
#define glVertexAttribP2ui glad_glVertexAttribP2ui
GLAD_API_CALL PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv;
#define glVertexAttribP2uiv glad_glVertexAttribP2uiv
GLAD_API_CALL PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui;
#define glVertexAttribP3ui glad_glVertexAttribP3ui
GLAD_API_CALL PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv;
#define glVertexAttribP3uiv glad_glVertexAttribP3uiv
GLAD_API_CALL PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui;
#define glVertexAttribP4ui glad_glVertexAttribP4ui
GLAD_API_CALL PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv;
#define glVertexAttribP4uiv glad_glVertexAttribP4uiv
GLAD_API_CALL PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
#define glVertexAttribPointer glad_glVertexAttribPointer
GLAD_API_CALL PFNGLVERTEXATTRIBPOINTERARBPROC glad_glVertexAttribPointerARB;
#define glVertexAttribPointerARB glad_glVertexAttribPointerARB
GLAD_API_CALL PFNGLVERTEXBINDINGDIVISORPROC glad_glVertexBindingDivisor;
#define glVertexBindingDivisor glad_glVertexBindingDivisor
GLAD_API_CALL PFNGLVERTEXBLENDARBPROC glad_glVertexBlendARB;
#define glVertexBlendARB glad_glVertexBlendARB
GLAD_API_CALL PFNGLVERTEXP2UIPROC glad_glVertexP2ui;
#define glVertexP2ui glad_glVertexP2ui
GLAD_API_CALL PFNGLVERTEXP2UIVPROC glad_glVertexP2uiv;
#define glVertexP2uiv glad_glVertexP2uiv
GLAD_API_CALL PFNGLVERTEXP3UIPROC glad_glVertexP3ui;
#define glVertexP3ui glad_glVertexP3ui
GLAD_API_CALL PFNGLVERTEXP3UIVPROC glad_glVertexP3uiv;
#define glVertexP3uiv glad_glVertexP3uiv
GLAD_API_CALL PFNGLVERTEXP4UIPROC glad_glVertexP4ui;
#define glVertexP4ui glad_glVertexP4ui
GLAD_API_CALL PFNGLVERTEXP4UIVPROC glad_glVertexP4uiv;
#define glVertexP4uiv glad_glVertexP4uiv
GLAD_API_CALL PFNGLVERTEXPOINTERPROC glad_glVertexPointer;
#define glVertexPointer glad_glVertexPointer
GLAD_API_CALL PFNGLVIEWPORTPROC glad_glViewport;
#define glViewport glad_glViewport
GLAD_API_CALL PFNGLVIEWPORTARRAYVPROC glad_glViewportArrayv;
#define glViewportArrayv glad_glViewportArrayv
GLAD_API_CALL PFNGLVIEWPORTINDEXEDFPROC glad_glViewportIndexedf;
#define glViewportIndexedf glad_glViewportIndexedf
GLAD_API_CALL PFNGLVIEWPORTINDEXEDFVPROC glad_glViewportIndexedfv;
#define glViewportIndexedfv glad_glViewportIndexedfv
GLAD_API_CALL PFNGLWAITSYNCPROC glad_glWaitSync;
#define glWaitSync glad_glWaitSync
GLAD_API_CALL PFNGLWEIGHTPOINTERARBPROC glad_glWeightPointerARB;
#define glWeightPointerARB glad_glWeightPointerARB
GLAD_API_CALL PFNGLWEIGHTBVARBPROC glad_glWeightbvARB;
#define glWeightbvARB glad_glWeightbvARB
GLAD_API_CALL PFNGLWEIGHTDVARBPROC glad_glWeightdvARB;
#define glWeightdvARB glad_glWeightdvARB
GLAD_API_CALL PFNGLWEIGHTFVARBPROC glad_glWeightfvARB;
#define glWeightfvARB glad_glWeightfvARB
GLAD_API_CALL PFNGLWEIGHTIVARBPROC glad_glWeightivARB;
#define glWeightivARB glad_glWeightivARB
GLAD_API_CALL PFNGLWEIGHTSVARBPROC glad_glWeightsvARB;
#define glWeightsvARB glad_glWeightsvARB
GLAD_API_CALL PFNGLWEIGHTUBVARBPROC glad_glWeightubvARB;
#define glWeightubvARB glad_glWeightubvARB
GLAD_API_CALL PFNGLWEIGHTUIVARBPROC glad_glWeightuivARB;
#define glWeightuivARB glad_glWeightuivARB
GLAD_API_CALL PFNGLWEIGHTUSVARBPROC glad_glWeightusvARB;
#define glWeightusvARB glad_glWeightusvARB
GLAD_API_CALL PFNGLWINDOWPOS2DPROC glad_glWindowPos2d;
#define glWindowPos2d glad_glWindowPos2d
GLAD_API_CALL PFNGLWINDOWPOS2DARBPROC glad_glWindowPos2dARB;
#define glWindowPos2dARB glad_glWindowPos2dARB
GLAD_API_CALL PFNGLWINDOWPOS2DVPROC glad_glWindowPos2dv;
#define glWindowPos2dv glad_glWindowPos2dv
GLAD_API_CALL PFNGLWINDOWPOS2DVARBPROC glad_glWindowPos2dvARB;
#define glWindowPos2dvARB glad_glWindowPos2dvARB
GLAD_API_CALL PFNGLWINDOWPOS2FPROC glad_glWindowPos2f;
#define glWindowPos2f glad_glWindowPos2f
GLAD_API_CALL PFNGLWINDOWPOS2FARBPROC glad_glWindowPos2fARB;
#define glWindowPos2fARB glad_glWindowPos2fARB
GLAD_API_CALL PFNGLWINDOWPOS2FVPROC glad_glWindowPos2fv;
#define glWindowPos2fv glad_glWindowPos2fv
GLAD_API_CALL PFNGLWINDOWPOS2FVARBPROC glad_glWindowPos2fvARB;
#define glWindowPos2fvARB glad_glWindowPos2fvARB
GLAD_API_CALL PFNGLWINDOWPOS2IPROC glad_glWindowPos2i;
#define glWindowPos2i glad_glWindowPos2i
GLAD_API_CALL PFNGLWINDOWPOS2IARBPROC glad_glWindowPos2iARB;
#define glWindowPos2iARB glad_glWindowPos2iARB
GLAD_API_CALL PFNGLWINDOWPOS2IVPROC glad_glWindowPos2iv;
#define glWindowPos2iv glad_glWindowPos2iv
GLAD_API_CALL PFNGLWINDOWPOS2IVARBPROC glad_glWindowPos2ivARB;
#define glWindowPos2ivARB glad_glWindowPos2ivARB
GLAD_API_CALL PFNGLWINDOWPOS2SPROC glad_glWindowPos2s;
#define glWindowPos2s glad_glWindowPos2s
GLAD_API_CALL PFNGLWINDOWPOS2SARBPROC glad_glWindowPos2sARB;
#define glWindowPos2sARB glad_glWindowPos2sARB
GLAD_API_CALL PFNGLWINDOWPOS2SVPROC glad_glWindowPos2sv;
#define glWindowPos2sv glad_glWindowPos2sv
GLAD_API_CALL PFNGLWINDOWPOS2SVARBPROC glad_glWindowPos2svARB;
#define glWindowPos2svARB glad_glWindowPos2svARB
GLAD_API_CALL PFNGLWINDOWPOS3DPROC glad_glWindowPos3d;
#define glWindowPos3d glad_glWindowPos3d
GLAD_API_CALL PFNGLWINDOWPOS3DARBPROC glad_glWindowPos3dARB;
#define glWindowPos3dARB glad_glWindowPos3dARB
GLAD_API_CALL PFNGLWINDOWPOS3DVPROC glad_glWindowPos3dv;
#define glWindowPos3dv glad_glWindowPos3dv
GLAD_API_CALL PFNGLWINDOWPOS3DVARBPROC glad_glWindowPos3dvARB;
#define glWindowPos3dvARB glad_glWindowPos3dvARB
GLAD_API_CALL PFNGLWINDOWPOS3FPROC glad_glWindowPos3f;
#define glWindowPos3f glad_glWindowPos3f
GLAD_API_CALL PFNGLWINDOWPOS3FARBPROC glad_glWindowPos3fARB;
#define glWindowPos3fARB glad_glWindowPos3fARB
GLAD_API_CALL PFNGLWINDOWPOS3FVPROC glad_glWindowPos3fv;
#define glWindowPos3fv glad_glWindowPos3fv
GLAD_API_CALL PFNGLWINDOWPOS3FVARBPROC glad_glWindowPos3fvARB;
#define glWindowPos3fvARB glad_glWindowPos3fvARB
GLAD_API_CALL PFNGLWINDOWPOS3IPROC glad_glWindowPos3i;
#define glWindowPos3i glad_glWindowPos3i
GLAD_API_CALL PFNGLWINDOWPOS3IARBPROC glad_glWindowPos3iARB;
#define glWindowPos3iARB glad_glWindowPos3iARB
GLAD_API_CALL PFNGLWINDOWPOS3IVPROC glad_glWindowPos3iv;
#define glWindowPos3iv glad_glWindowPos3iv
GLAD_API_CALL PFNGLWINDOWPOS3IVARBPROC glad_glWindowPos3ivARB;
#define glWindowPos3ivARB glad_glWindowPos3ivARB
GLAD_API_CALL PFNGLWINDOWPOS3SPROC glad_glWindowPos3s;
#define glWindowPos3s glad_glWindowPos3s
GLAD_API_CALL PFNGLWINDOWPOS3SARBPROC glad_glWindowPos3sARB;
#define glWindowPos3sARB glad_glWindowPos3sARB
GLAD_API_CALL PFNGLWINDOWPOS3SVPROC glad_glWindowPos3sv;
#define glWindowPos3sv glad_glWindowPos3sv
GLAD_API_CALL PFNGLWINDOWPOS3SVARBPROC glad_glWindowPos3svARB;
#define glWindowPos3svARB glad_glWindowPos3svARB





GLAD_API_CALL int gladLoadGLUserPtr( GLADuserptrloadfunc load, void *userptr);
GLAD_API_CALL int gladLoadGL( GLADloadfunc load);



#ifdef __cplusplus
}
#endif
#endif

/* Source */
#ifdef GLAD_GL_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */


int GLAD_GL_VERSION_1_0 = 0;
int GLAD_GL_VERSION_1_1 = 0;
int GLAD_GL_VERSION_1_2 = 0;
int GLAD_GL_VERSION_1_3 = 0;
int GLAD_GL_VERSION_1_4 = 0;
int GLAD_GL_VERSION_1_5 = 0;
int GLAD_GL_VERSION_2_0 = 0;
int GLAD_GL_VERSION_2_1 = 0;
int GLAD_GL_VERSION_3_0 = 0;
int GLAD_GL_VERSION_3_1 = 0;
int GLAD_GL_VERSION_3_2 = 0;
int GLAD_GL_VERSION_3_3 = 0;
int GLAD_GL_ARB_ES2_compatibility = 0;
int GLAD_GL_ARB_ES3_1_compatibility = 0;
int GLAD_GL_ARB_ES3_2_compatibility = 0;
int GLAD_GL_ARB_ES3_compatibility = 0;
int GLAD_GL_ARB_arrays_of_arrays = 0;
int GLAD_GL_ARB_base_instance = 0;
int GLAD_GL_ARB_bindless_texture = 0;
int GLAD_GL_ARB_blend_func_extended = 0;
int GLAD_GL_ARB_buffer_storage = 0;
int GLAD_GL_ARB_cl_event = 0;
int GLAD_GL_ARB_clear_buffer_object = 0;
int GLAD_GL_ARB_clear_texture = 0;
int GLAD_GL_ARB_clip_control = 0;
int GLAD_GL_ARB_color_buffer_float = 0;
int GLAD_GL_ARB_compatibility = 0;
int GLAD_GL_ARB_compressed_texture_pixel_storage = 0;
int GLAD_GL_ARB_compute_shader = 0;
int GLAD_GL_ARB_compute_variable_group_size = 0;
int GLAD_GL_ARB_conditional_render_inverted = 0;
int GLAD_GL_ARB_conservative_depth = 0;
int GLAD_GL_ARB_copy_buffer = 0;
int GLAD_GL_ARB_copy_image = 0;
int GLAD_GL_ARB_cull_distance = 0;
int GLAD_GL_ARB_debug_output = 0;
int GLAD_GL_ARB_depth_buffer_float = 0;
int GLAD_GL_ARB_depth_clamp = 0;
int GLAD_GL_ARB_depth_texture = 0;
int GLAD_GL_ARB_derivative_control = 0;
int GLAD_GL_ARB_direct_state_access = 0;
int GLAD_GL_ARB_draw_buffers = 0;
int GLAD_GL_ARB_draw_buffers_blend = 0;
int GLAD_GL_ARB_draw_elements_base_vertex = 0;
int GLAD_GL_ARB_draw_indirect = 0;
int GLAD_GL_ARB_draw_instanced = 0;
int GLAD_GL_ARB_enhanced_layouts = 0;
int GLAD_GL_ARB_explicit_attrib_location = 0;
int GLAD_GL_ARB_explicit_uniform_location = 0;
int GLAD_GL_ARB_fragment_coord_conventions = 0;
int GLAD_GL_ARB_fragment_layer_viewport = 0;
int GLAD_GL_ARB_fragment_program = 0;
int GLAD_GL_ARB_fragment_program_shadow = 0;
int GLAD_GL_ARB_fragment_shader = 0;
int GLAD_GL_ARB_fragment_shader_interlock = 0;
int GLAD_GL_ARB_framebuffer_no_attachments = 0;
int GLAD_GL_ARB_framebuffer_object = 0;
int GLAD_GL_ARB_framebuffer_sRGB = 0;
int GLAD_GL_ARB_geometry_shader4 = 0;
int GLAD_GL_ARB_get_program_binary = 0;
int GLAD_GL_ARB_get_texture_sub_image = 0;
int GLAD_GL_ARB_gl_spirv = 0;
int GLAD_GL_ARB_gpu_shader5 = 0;
int GLAD_GL_ARB_gpu_shader_fp64 = 0;
int GLAD_GL_ARB_gpu_shader_int64 = 0;
int GLAD_GL_ARB_half_float_pixel = 0;
int GLAD_GL_ARB_half_float_vertex = 0;
int GLAD_GL_ARB_imaging = 0;
int GLAD_GL_ARB_indirect_parameters = 0;
int GLAD_GL_ARB_instanced_arrays = 0;
int GLAD_GL_ARB_internalformat_query = 0;
int GLAD_GL_ARB_internalformat_query2 = 0;
int GLAD_GL_ARB_invalidate_subdata = 0;
int GLAD_GL_ARB_map_buffer_alignment = 0;
int GLAD_GL_ARB_map_buffer_range = 0;
int GLAD_GL_ARB_matrix_palette = 0;
int GLAD_GL_ARB_multi_bind = 0;
int GLAD_GL_ARB_multi_draw_indirect = 0;
int GLAD_GL_ARB_multisample = 0;
int GLAD_GL_ARB_multitexture = 0;
int GLAD_GL_ARB_occlusion_query = 0;
int GLAD_GL_ARB_occlusion_query2 = 0;
int GLAD_GL_ARB_parallel_shader_compile = 0;
int GLAD_GL_ARB_pipeline_statistics_query = 0;
int GLAD_GL_ARB_pixel_buffer_object = 0;
int GLAD_GL_ARB_point_parameters = 0;
int GLAD_GL_ARB_point_sprite = 0;
int GLAD_GL_ARB_polygon_offset_clamp = 0;
int GLAD_GL_ARB_post_depth_coverage = 0;
int GLAD_GL_ARB_program_interface_query = 0;
int GLAD_GL_ARB_provoking_vertex = 0;
int GLAD_GL_ARB_query_buffer_object = 0;
int GLAD_GL_ARB_robust_buffer_access_behavior = 0;
int GLAD_GL_ARB_robustness = 0;
int GLAD_GL_ARB_robustness_isolation = 0;
int GLAD_GL_ARB_sample_locations = 0;
int GLAD_GL_ARB_sample_shading = 0;
int GLAD_GL_ARB_sampler_objects = 0;
int GLAD_GL_ARB_seamless_cube_map = 0;
int GLAD_GL_ARB_seamless_cubemap_per_texture = 0;
int GLAD_GL_ARB_separate_shader_objects = 0;
int GLAD_GL_ARB_shader_atomic_counter_ops = 0;
int GLAD_GL_ARB_shader_atomic_counters = 0;
int GLAD_GL_ARB_shader_ballot = 0;
int GLAD_GL_ARB_shader_bit_encoding = 0;
int GLAD_GL_ARB_shader_clock = 0;
int GLAD_GL_ARB_shader_draw_parameters = 0;
int GLAD_GL_ARB_shader_group_vote = 0;
int GLAD_GL_ARB_shader_image_load_store = 0;
int GLAD_GL_ARB_shader_image_size = 0;
int GLAD_GL_ARB_shader_objects = 0;
int GLAD_GL_ARB_shader_precision = 0;
int GLAD_GL_ARB_shader_stencil_export = 0;
int GLAD_GL_ARB_shader_storage_buffer_object = 0;
int GLAD_GL_ARB_shader_subroutine = 0;
int GLAD_GL_ARB_shader_texture_image_samples = 0;
int GLAD_GL_ARB_shader_texture_lod = 0;
int GLAD_GL_ARB_shader_viewport_layer_array = 0;
int GLAD_GL_ARB_shading_language_100 = 0;
int GLAD_GL_ARB_shading_language_420pack = 0;
int GLAD_GL_ARB_shading_language_include = 0;
int GLAD_GL_ARB_shading_language_packing = 0;
int GLAD_GL_ARB_shadow = 0;
int GLAD_GL_ARB_shadow_ambient = 0;
int GLAD_GL_ARB_sparse_buffer = 0;
int GLAD_GL_ARB_sparse_texture = 0;
int GLAD_GL_ARB_sparse_texture2 = 0;
int GLAD_GL_ARB_sparse_texture_clamp = 0;
int GLAD_GL_ARB_spirv_extensions = 0;
int GLAD_GL_ARB_stencil_texturing = 0;
int GLAD_GL_ARB_sync = 0;
int GLAD_GL_ARB_tessellation_shader = 0;
int GLAD_GL_ARB_texture_barrier = 0;
int GLAD_GL_ARB_texture_border_clamp = 0;
int GLAD_GL_ARB_texture_buffer_object = 0;
int GLAD_GL_ARB_texture_buffer_object_rgb32 = 0;
int GLAD_GL_ARB_texture_buffer_range = 0;
int GLAD_GL_ARB_texture_compression = 0;
int GLAD_GL_ARB_texture_compression_bptc = 0;
int GLAD_GL_ARB_texture_compression_rgtc = 0;
int GLAD_GL_ARB_texture_cube_map = 0;
int GLAD_GL_ARB_texture_cube_map_array = 0;
int GLAD_GL_ARB_texture_env_add = 0;
int GLAD_GL_ARB_texture_env_combine = 0;
int GLAD_GL_ARB_texture_env_crossbar = 0;
int GLAD_GL_ARB_texture_env_dot3 = 0;
int GLAD_GL_ARB_texture_filter_anisotropic = 0;
int GLAD_GL_ARB_texture_filter_minmax = 0;
int GLAD_GL_ARB_texture_float = 0;
int GLAD_GL_ARB_texture_gather = 0;
int GLAD_GL_ARB_texture_mirror_clamp_to_edge = 0;
int GLAD_GL_ARB_texture_mirrored_repeat = 0;
int GLAD_GL_ARB_texture_multisample = 0;
int GLAD_GL_ARB_texture_non_power_of_two = 0;
int GLAD_GL_ARB_texture_query_levels = 0;
int GLAD_GL_ARB_texture_query_lod = 0;
int GLAD_GL_ARB_texture_rectangle = 0;
int GLAD_GL_ARB_texture_rg = 0;
int GLAD_GL_ARB_texture_rgb10_a2ui = 0;
int GLAD_GL_ARB_texture_stencil8 = 0;
int GLAD_GL_ARB_texture_storage = 0;
int GLAD_GL_ARB_texture_storage_multisample = 0;
int GLAD_GL_ARB_texture_swizzle = 0;
int GLAD_GL_ARB_texture_view = 0;
int GLAD_GL_ARB_timer_query = 0;
int GLAD_GL_ARB_transform_feedback2 = 0;
int GLAD_GL_ARB_transform_feedback3 = 0;
int GLAD_GL_ARB_transform_feedback_instanced = 0;
int GLAD_GL_ARB_transform_feedback_overflow_query = 0;
int GLAD_GL_ARB_transpose_matrix = 0;
int GLAD_GL_ARB_uniform_buffer_object = 0;
int GLAD_GL_ARB_vertex_array_bgra = 0;
int GLAD_GL_ARB_vertex_array_object = 0;
int GLAD_GL_ARB_vertex_attrib_64bit = 0;
int GLAD_GL_ARB_vertex_attrib_binding = 0;
int GLAD_GL_ARB_vertex_blend = 0;
int GLAD_GL_ARB_vertex_buffer_object = 0;
int GLAD_GL_ARB_vertex_program = 0;
int GLAD_GL_ARB_vertex_shader = 0;
int GLAD_GL_ARB_vertex_type_10f_11f_11f_rev = 0;
int GLAD_GL_ARB_vertex_type_2_10_10_10_rev = 0;
int GLAD_GL_ARB_viewport_array = 0;
int GLAD_GL_ARB_window_pos = 0;
int GLAD_GL_KHR_blend_equation_advanced = 0;
int GLAD_GL_KHR_blend_equation_advanced_coherent = 0;
int GLAD_GL_KHR_context_flush_control = 0;
int GLAD_GL_KHR_debug = 0;
int GLAD_GL_KHR_no_error = 0;
int GLAD_GL_KHR_parallel_shader_compile = 0;
int GLAD_GL_KHR_robust_buffer_access_behavior = 0;
int GLAD_GL_KHR_robustness = 0;
int GLAD_GL_KHR_shader_subgroup = 0;
int GLAD_GL_KHR_texture_compression_astc_hdr = 0;
int GLAD_GL_KHR_texture_compression_astc_ldr = 0;
int GLAD_GL_KHR_texture_compression_astc_sliced_3d = 0;



PFNGLACCUMPROC glad_glAccum = NULL;
PFNGLACTIVESHADERPROGRAMPROC glad_glActiveShaderProgram = NULL;
PFNGLACTIVETEXTUREPROC glad_glActiveTexture = NULL;
PFNGLACTIVETEXTUREARBPROC glad_glActiveTextureARB = NULL;
PFNGLALPHAFUNCPROC glad_glAlphaFunc = NULL;
PFNGLARETEXTURESRESIDENTPROC glad_glAreTexturesResident = NULL;
PFNGLARRAYELEMENTPROC glad_glArrayElement = NULL;
PFNGLATTACHOBJECTARBPROC glad_glAttachObjectARB = NULL;
PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
PFNGLBEGINPROC glad_glBegin = NULL;
PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender = NULL;
PFNGLBEGINQUERYPROC glad_glBeginQuery = NULL;
PFNGLBEGINQUERYARBPROC glad_glBeginQueryARB = NULL;
PFNGLBEGINQUERYINDEXEDPROC glad_glBeginQueryIndexed = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback = NULL;
PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation = NULL;
PFNGLBINDATTRIBLOCATIONARBPROC glad_glBindAttribLocationARB = NULL;
PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
PFNGLBINDBUFFERARBPROC glad_glBindBufferARB = NULL;
PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange = NULL;
PFNGLBINDBUFFERSBASEPROC glad_glBindBuffersBase = NULL;
PFNGLBINDBUFFERSRANGEPROC glad_glBindBuffersRange = NULL;
PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation = NULL;
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLBINDIMAGETEXTUREPROC glad_glBindImageTexture = NULL;
PFNGLBINDIMAGETEXTURESPROC glad_glBindImageTextures = NULL;
PFNGLBINDPROGRAMARBPROC glad_glBindProgramARB = NULL;
PFNGLBINDPROGRAMPIPELINEPROC glad_glBindProgramPipeline = NULL;
PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
PFNGLBINDSAMPLERPROC glad_glBindSampler = NULL;
PFNGLBINDSAMPLERSPROC glad_glBindSamplers = NULL;
PFNGLBINDTEXTUREPROC glad_glBindTexture = NULL;
PFNGLBINDTEXTUREUNITPROC glad_glBindTextureUnit = NULL;
PFNGLBINDTEXTURESPROC glad_glBindTextures = NULL;
PFNGLBINDTRANSFORMFEEDBACKPROC glad_glBindTransformFeedback = NULL;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
PFNGLBINDVERTEXBUFFERPROC glad_glBindVertexBuffer = NULL;
PFNGLBINDVERTEXBUFFERSPROC glad_glBindVertexBuffers = NULL;
PFNGLBITMAPPROC glad_glBitmap = NULL;
PFNGLBLENDBARRIERPROC glad_glBlendBarrier = NULL;
PFNGLBLENDBARRIERKHRPROC glad_glBlendBarrierKHR = NULL;
PFNGLBLENDCOLORPROC glad_glBlendColor = NULL;
PFNGLBLENDEQUATIONPROC glad_glBlendEquation = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate = NULL;
PFNGLBLENDEQUATIONSEPARATEIPROC glad_glBlendEquationSeparatei = NULL;
PFNGLBLENDEQUATIONSEPARATEIARBPROC glad_glBlendEquationSeparateiARB = NULL;
PFNGLBLENDEQUATIONIPROC glad_glBlendEquationi = NULL;
PFNGLBLENDEQUATIONIARBPROC glad_glBlendEquationiARB = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate = NULL;
PFNGLBLENDFUNCSEPARATEIPROC glad_glBlendFuncSeparatei = NULL;
PFNGLBLENDFUNCSEPARATEIARBPROC glad_glBlendFuncSeparateiARB = NULL;
PFNGLBLENDFUNCIPROC glad_glBlendFunci = NULL;
PFNGLBLENDFUNCIARBPROC glad_glBlendFunciARB = NULL;
PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer = NULL;
PFNGLBLITNAMEDFRAMEBUFFERPROC glad_glBlitNamedFramebuffer = NULL;
PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
PFNGLBUFFERDATAARBPROC glad_glBufferDataARB = NULL;
PFNGLBUFFERPAGECOMMITMENTARBPROC glad_glBufferPageCommitmentARB = NULL;
PFNGLBUFFERSTORAGEPROC glad_glBufferStorage = NULL;
PFNGLBUFFERSUBDATAPROC glad_glBufferSubData = NULL;
PFNGLBUFFERSUBDATAARBPROC glad_glBufferSubDataARB = NULL;
PFNGLCALLLISTPROC glad_glCallList = NULL;
PFNGLCALLLISTSPROC glad_glCallLists = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC glad_glCheckNamedFramebufferStatus = NULL;
PFNGLCLAMPCOLORPROC glad_glClampColor = NULL;
PFNGLCLAMPCOLORARBPROC glad_glClampColorARB = NULL;
PFNGLCLEARPROC glad_glClear = NULL;
PFNGLCLEARACCUMPROC glad_glClearAccum = NULL;
PFNGLCLEARBUFFERDATAPROC glad_glClearBufferData = NULL;
PFNGLCLEARBUFFERSUBDATAPROC glad_glClearBufferSubData = NULL;
PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi = NULL;
PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv = NULL;
PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv = NULL;
PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv = NULL;
PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
PFNGLCLEARDEPTHPROC glad_glClearDepth = NULL;
PFNGLCLEARDEPTHFPROC glad_glClearDepthf = NULL;
PFNGLCLEARINDEXPROC glad_glClearIndex = NULL;
PFNGLCLEARNAMEDBUFFERDATAPROC glad_glClearNamedBufferData = NULL;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC glad_glClearNamedBufferSubData = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERFIPROC glad_glClearNamedFramebufferfi = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERFVPROC glad_glClearNamedFramebufferfv = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERIVPROC glad_glClearNamedFramebufferiv = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC glad_glClearNamedFramebufferuiv = NULL;
PFNGLCLEARSTENCILPROC glad_glClearStencil = NULL;
PFNGLCLEARTEXIMAGEPROC glad_glClearTexImage = NULL;
PFNGLCLEARTEXSUBIMAGEPROC glad_glClearTexSubImage = NULL;
PFNGLCLIENTACTIVETEXTUREPROC glad_glClientActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glad_glClientActiveTextureARB = NULL;
PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync = NULL;
PFNGLCLIPCONTROLPROC glad_glClipControl = NULL;
PFNGLCLIPPLANEPROC glad_glClipPlane = NULL;
PFNGLCOLOR3BPROC glad_glColor3b = NULL;
PFNGLCOLOR3BVPROC glad_glColor3bv = NULL;
PFNGLCOLOR3DPROC glad_glColor3d = NULL;
PFNGLCOLOR3DVPROC glad_glColor3dv = NULL;
PFNGLCOLOR3FPROC glad_glColor3f = NULL;
PFNGLCOLOR3FVPROC glad_glColor3fv = NULL;
PFNGLCOLOR3IPROC glad_glColor3i = NULL;
PFNGLCOLOR3IVPROC glad_glColor3iv = NULL;
PFNGLCOLOR3SPROC glad_glColor3s = NULL;
PFNGLCOLOR3SVPROC glad_glColor3sv = NULL;
PFNGLCOLOR3UBPROC glad_glColor3ub = NULL;
PFNGLCOLOR3UBVPROC glad_glColor3ubv = NULL;
PFNGLCOLOR3UIPROC glad_glColor3ui = NULL;
PFNGLCOLOR3UIVPROC glad_glColor3uiv = NULL;
PFNGLCOLOR3USPROC glad_glColor3us = NULL;
PFNGLCOLOR3USVPROC glad_glColor3usv = NULL;
PFNGLCOLOR4BPROC glad_glColor4b = NULL;
PFNGLCOLOR4BVPROC glad_glColor4bv = NULL;
PFNGLCOLOR4DPROC glad_glColor4d = NULL;
PFNGLCOLOR4DVPROC glad_glColor4dv = NULL;
PFNGLCOLOR4FPROC glad_glColor4f = NULL;
PFNGLCOLOR4FVPROC glad_glColor4fv = NULL;
PFNGLCOLOR4IPROC glad_glColor4i = NULL;
PFNGLCOLOR4IVPROC glad_glColor4iv = NULL;
PFNGLCOLOR4SPROC glad_glColor4s = NULL;
PFNGLCOLOR4SVPROC glad_glColor4sv = NULL;
PFNGLCOLOR4UBPROC glad_glColor4ub = NULL;
PFNGLCOLOR4UBVPROC glad_glColor4ubv = NULL;
PFNGLCOLOR4UIPROC glad_glColor4ui = NULL;
PFNGLCOLOR4UIVPROC glad_glColor4uiv = NULL;
PFNGLCOLOR4USPROC glad_glColor4us = NULL;
PFNGLCOLOR4USVPROC glad_glColor4usv = NULL;
PFNGLCOLORMASKPROC glad_glColorMask = NULL;
PFNGLCOLORMASKIPROC glad_glColorMaski = NULL;
PFNGLCOLORMATERIALPROC glad_glColorMaterial = NULL;
PFNGLCOLORP3UIPROC glad_glColorP3ui = NULL;
PFNGLCOLORP3UIVPROC glad_glColorP3uiv = NULL;
PFNGLCOLORP4UIPROC glad_glColorP4ui = NULL;
PFNGLCOLORP4UIVPROC glad_glColorP4uiv = NULL;
PFNGLCOLORPOINTERPROC glad_glColorPointer = NULL;
PFNGLCOLORSUBTABLEPROC glad_glColorSubTable = NULL;
PFNGLCOLORTABLEPROC glad_glColorTable = NULL;
PFNGLCOLORTABLEPARAMETERFVPROC glad_glColorTableParameterfv = NULL;
PFNGLCOLORTABLEPARAMETERIVPROC glad_glColorTableParameteriv = NULL;
PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
PFNGLCOMPILESHADERARBPROC glad_glCompileShaderARB = NULL;
PFNGLCOMPILESHADERINCLUDEARBPROC glad_glCompileShaderIncludeARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glad_glCompressedTexImage1DARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glad_glCompressedTexImage2DARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glad_glCompressedTexImage3DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glad_glCompressedTexSubImage1DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glad_glCompressedTexSubImage2DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glad_glCompressedTexSubImage3DARB = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC glad_glCompressedTextureSubImage1D = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC glad_glCompressedTextureSubImage2D = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC glad_glCompressedTextureSubImage3D = NULL;
PFNGLCONVOLUTIONFILTER1DPROC glad_glConvolutionFilter1D = NULL;
PFNGLCONVOLUTIONFILTER2DPROC glad_glConvolutionFilter2D = NULL;
PFNGLCONVOLUTIONPARAMETERFPROC glad_glConvolutionParameterf = NULL;
PFNGLCONVOLUTIONPARAMETERFVPROC glad_glConvolutionParameterfv = NULL;
PFNGLCONVOLUTIONPARAMETERIPROC glad_glConvolutionParameteri = NULL;
PFNGLCONVOLUTIONPARAMETERIVPROC glad_glConvolutionParameteriv = NULL;
PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData = NULL;
PFNGLCOPYCOLORSUBTABLEPROC glad_glCopyColorSubTable = NULL;
PFNGLCOPYCOLORTABLEPROC glad_glCopyColorTable = NULL;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glad_glCopyConvolutionFilter1D = NULL;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glad_glCopyConvolutionFilter2D = NULL;
PFNGLCOPYIMAGESUBDATAPROC glad_glCopyImageSubData = NULL;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC glad_glCopyNamedBufferSubData = NULL;
PFNGLCOPYPIXELSPROC glad_glCopyPixels = NULL;
PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D = NULL;
PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D = NULL;
PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D = NULL;
PFNGLCOPYTEXTURESUBIMAGE1DPROC glad_glCopyTextureSubImage1D = NULL;
PFNGLCOPYTEXTURESUBIMAGE2DPROC glad_glCopyTextureSubImage2D = NULL;
PFNGLCOPYTEXTURESUBIMAGE3DPROC glad_glCopyTextureSubImage3D = NULL;
PFNGLCREATEBUFFERSPROC glad_glCreateBuffers = NULL;
PFNGLCREATEFRAMEBUFFERSPROC glad_glCreateFramebuffers = NULL;
PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glad_glCreateProgramObjectARB = NULL;
PFNGLCREATEPROGRAMPIPELINESPROC glad_glCreateProgramPipelines = NULL;
PFNGLCREATEQUERIESPROC glad_glCreateQueries = NULL;
PFNGLCREATERENDERBUFFERSPROC glad_glCreateRenderbuffers = NULL;
PFNGLCREATESAMPLERSPROC glad_glCreateSamplers = NULL;
PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
PFNGLCREATESHADEROBJECTARBPROC glad_glCreateShaderObjectARB = NULL;
PFNGLCREATESHADERPROGRAMVPROC glad_glCreateShaderProgramv = NULL;
PFNGLCREATESYNCFROMCLEVENTARBPROC glad_glCreateSyncFromCLeventARB = NULL;
PFNGLCREATETEXTURESPROC glad_glCreateTextures = NULL;
PFNGLCREATETRANSFORMFEEDBACKSPROC glad_glCreateTransformFeedbacks = NULL;
PFNGLCREATEVERTEXARRAYSPROC glad_glCreateVertexArrays = NULL;
PFNGLCULLFACEPROC glad_glCullFace = NULL;
PFNGLCURRENTPALETTEMATRIXARBPROC glad_glCurrentPaletteMatrixARB = NULL;
PFNGLDEBUGMESSAGECALLBACKPROC glad_glDebugMessageCallback = NULL;
PFNGLDEBUGMESSAGECALLBACKARBPROC glad_glDebugMessageCallbackARB = NULL;
PFNGLDEBUGMESSAGECONTROLPROC glad_glDebugMessageControl = NULL;
PFNGLDEBUGMESSAGECONTROLARBPROC glad_glDebugMessageControlARB = NULL;
PFNGLDEBUGMESSAGEINSERTPROC glad_glDebugMessageInsert = NULL;
PFNGLDEBUGMESSAGEINSERTARBPROC glad_glDebugMessageInsertARB = NULL;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;
PFNGLDELETEBUFFERSARBPROC glad_glDeleteBuffersARB = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLDELETELISTSPROC glad_glDeleteLists = NULL;
PFNGLDELETENAMEDSTRINGARBPROC glad_glDeleteNamedStringARB = NULL;
PFNGLDELETEOBJECTARBPROC glad_glDeleteObjectARB = NULL;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
PFNGLDELETEPROGRAMPIPELINESPROC glad_glDeleteProgramPipelines = NULL;
PFNGLDELETEPROGRAMSARBPROC glad_glDeleteProgramsARB = NULL;
PFNGLDELETEQUERIESPROC glad_glDeleteQueries = NULL;
PFNGLDELETEQUERIESARBPROC glad_glDeleteQueriesARB = NULL;
PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;
PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers = NULL;
PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
PFNGLDELETESYNCPROC glad_glDeleteSync = NULL;
PFNGLDELETETEXTURESPROC glad_glDeleteTextures = NULL;
PFNGLDELETETRANSFORMFEEDBACKSPROC glad_glDeleteTransformFeedbacks = NULL;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
PFNGLDEPTHMASKPROC glad_glDepthMask = NULL;
PFNGLDEPTHRANGEPROC glad_glDepthRange = NULL;
PFNGLDEPTHRANGEARRAYDVNVPROC glad_glDepthRangeArraydvNV = NULL;
PFNGLDEPTHRANGEARRAYVPROC glad_glDepthRangeArrayv = NULL;
PFNGLDEPTHRANGEINDEXEDPROC glad_glDepthRangeIndexed = NULL;
PFNGLDEPTHRANGEINDEXEDDNVPROC glad_glDepthRangeIndexeddNV = NULL;
PFNGLDEPTHRANGEFPROC glad_glDepthRangef = NULL;
PFNGLDETACHOBJECTARBPROC glad_glDetachObjectARB = NULL;
PFNGLDETACHSHADERPROC glad_glDetachShader = NULL;
PFNGLDISABLEPROC glad_glDisable = NULL;
PFNGLDISABLECLIENTSTATEPROC glad_glDisableClientState = NULL;
PFNGLDISABLEVERTEXARRAYATTRIBPROC glad_glDisableVertexArrayAttrib = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glad_glDisableVertexAttribArrayARB = NULL;
PFNGLDISABLEIPROC glad_glDisablei = NULL;
PFNGLDISPATCHCOMPUTEPROC glad_glDispatchCompute = NULL;
PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC glad_glDispatchComputeGroupSizeARB = NULL;
PFNGLDISPATCHCOMPUTEINDIRECTPROC glad_glDispatchComputeIndirect = NULL;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
PFNGLDRAWARRAYSINDIRECTPROC glad_glDrawArraysIndirect = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced = NULL;
PFNGLDRAWARRAYSINSTANCEDARBPROC glad_glDrawArraysInstancedARB = NULL;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glad_glDrawArraysInstancedBaseInstance = NULL;
PFNGLDRAWBUFFERPROC glad_glDrawBuffer = NULL;
PFNGLDRAWBUFFERSPROC glad_glDrawBuffers = NULL;
PFNGLDRAWBUFFERSARBPROC glad_glDrawBuffersARB = NULL;
PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;
PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINDIRECTPROC glad_glDrawElementsIndirect = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDARBPROC glad_glDrawElementsInstancedARB = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glad_glDrawElementsInstancedBaseInstance = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glad_glDrawElementsInstancedBaseVertexBaseInstance = NULL;
PFNGLDRAWPIXELSPROC glad_glDrawPixels = NULL;
PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex = NULL;
PFNGLDRAWTRANSFORMFEEDBACKPROC glad_glDrawTransformFeedback = NULL;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glad_glDrawTransformFeedbackInstanced = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glad_glDrawTransformFeedbackStream = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glad_glDrawTransformFeedbackStreamInstanced = NULL;
PFNGLEDGEFLAGPROC glad_glEdgeFlag = NULL;
PFNGLEDGEFLAGPOINTERPROC glad_glEdgeFlagPointer = NULL;
PFNGLEDGEFLAGVPROC glad_glEdgeFlagv = NULL;
PFNGLENABLEPROC glad_glEnable = NULL;
PFNGLENABLECLIENTSTATEPROC glad_glEnableClientState = NULL;
PFNGLENABLEVERTEXARRAYATTRIBPROC glad_glEnableVertexArrayAttrib = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glad_glEnableVertexAttribArrayARB = NULL;
PFNGLENABLEIPROC glad_glEnablei = NULL;
PFNGLENDPROC glad_glEnd = NULL;
PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender = NULL;
PFNGLENDLISTPROC glad_glEndList = NULL;
PFNGLENDQUERYPROC glad_glEndQuery = NULL;
PFNGLENDQUERYARBPROC glad_glEndQueryARB = NULL;
PFNGLENDQUERYINDEXEDPROC glad_glEndQueryIndexed = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback = NULL;
PFNGLEVALCOORD1DPROC glad_glEvalCoord1d = NULL;
PFNGLEVALCOORD1DVPROC glad_glEvalCoord1dv = NULL;
PFNGLEVALCOORD1FPROC glad_glEvalCoord1f = NULL;
PFNGLEVALCOORD1FVPROC glad_glEvalCoord1fv = NULL;
PFNGLEVALCOORD2DPROC glad_glEvalCoord2d = NULL;
PFNGLEVALCOORD2DVPROC glad_glEvalCoord2dv = NULL;
PFNGLEVALCOORD2FPROC glad_glEvalCoord2f = NULL;
PFNGLEVALCOORD2FVPROC glad_glEvalCoord2fv = NULL;
PFNGLEVALMESH1PROC glad_glEvalMesh1 = NULL;
PFNGLEVALMESH2PROC glad_glEvalMesh2 = NULL;
PFNGLEVALPOINT1PROC glad_glEvalPoint1 = NULL;
PFNGLEVALPOINT2PROC glad_glEvalPoint2 = NULL;
PFNGLEVALUATEDEPTHVALUESARBPROC glad_glEvaluateDepthValuesARB = NULL;
PFNGLFEEDBACKBUFFERPROC glad_glFeedbackBuffer = NULL;
PFNGLFENCESYNCPROC glad_glFenceSync = NULL;
PFNGLFINISHPROC glad_glFinish = NULL;
PFNGLFLUSHPROC glad_glFlush = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange = NULL;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glad_glFlushMappedNamedBufferRange = NULL;
PFNGLFOGCOORDPOINTERPROC glad_glFogCoordPointer = NULL;
PFNGLFOGCOORDDPROC glad_glFogCoordd = NULL;
PFNGLFOGCOORDDVPROC glad_glFogCoorddv = NULL;
PFNGLFOGCOORDFPROC glad_glFogCoordf = NULL;
PFNGLFOGCOORDFVPROC glad_glFogCoordfv = NULL;
PFNGLFOGFPROC glad_glFogf = NULL;
PFNGLFOGFVPROC glad_glFogfv = NULL;
PFNGLFOGIPROC glad_glFogi = NULL;
PFNGLFOGIVPROC glad_glFogiv = NULL;
PFNGLFRAMEBUFFERPARAMETERIPROC glad_glFramebufferParameteri = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERSAMPLELOCATIONSFVARBPROC glad_glFramebufferSampleLocationsfvARB = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERTEXTUREARBPROC glad_glFramebufferTextureARB = NULL;
PFNGLFRAMEBUFFERTEXTUREFACEARBPROC glad_glFramebufferTextureFaceARB = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERARBPROC glad_glFramebufferTextureLayerARB = NULL;
PFNGLFRONTFACEPROC glad_glFrontFace = NULL;
PFNGLFRUSTUMPROC glad_glFrustum = NULL;
PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
PFNGLGENBUFFERSARBPROC glad_glGenBuffersARB = NULL;
PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLGENLISTSPROC glad_glGenLists = NULL;
PFNGLGENPROGRAMPIPELINESPROC glad_glGenProgramPipelines = NULL;
PFNGLGENPROGRAMSARBPROC glad_glGenProgramsARB = NULL;
PFNGLGENQUERIESPROC glad_glGenQueries = NULL;
PFNGLGENQUERIESARBPROC glad_glGenQueriesARB = NULL;
PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
PFNGLGENSAMPLERSPROC glad_glGenSamplers = NULL;
PFNGLGENTEXTURESPROC glad_glGenTextures = NULL;
PFNGLGENTRANSFORMFEEDBACKSPROC glad_glGenTransformFeedbacks = NULL;
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap = NULL;
PFNGLGENERATETEXTUREMIPMAPPROC glad_glGenerateTextureMipmap = NULL;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glad_glGetActiveAtomicCounterBufferiv = NULL;
PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib = NULL;
PFNGLGETACTIVEATTRIBARBPROC glad_glGetActiveAttribARB = NULL;
PFNGLGETACTIVESUBROUTINENAMEPROC glad_glGetActiveSubroutineName = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glad_glGetActiveSubroutineUniformName = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glad_glGetActiveSubroutineUniformiv = NULL;
PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform = NULL;
PFNGLGETACTIVEUNIFORMARBPROC glad_glGetActiveUniformARB = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv = NULL;
PFNGLGETATTACHEDOBJECTSARBPROC glad_glGetAttachedObjectsARB = NULL;
PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation = NULL;
PFNGLGETATTRIBLOCATIONARBPROC glad_glGetAttribLocationARB = NULL;
PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v = NULL;
PFNGLGETBOOLEANVPROC glad_glGetBooleanv = NULL;
PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v = NULL;
PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC glad_glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv = NULL;
PFNGLGETBUFFERPOINTERVARBPROC glad_glGetBufferPointervARB = NULL;
PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData = NULL;
PFNGLGETBUFFERSUBDATAARBPROC glad_glGetBufferSubDataARB = NULL;
PFNGLGETCLIPPLANEPROC glad_glGetClipPlane = NULL;
PFNGLGETCOLORTABLEPROC glad_glGetColorTable = NULL;
PFNGLGETCOLORTABLEPARAMETERFVPROC glad_glGetColorTableParameterfv = NULL;
PFNGLGETCOLORTABLEPARAMETERIVPROC glad_glGetColorTableParameteriv = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glad_glGetCompressedTexImageARB = NULL;
PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC glad_glGetCompressedTextureImage = NULL;
PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC glad_glGetCompressedTextureSubImage = NULL;
PFNGLGETCONVOLUTIONFILTERPROC glad_glGetConvolutionFilter = NULL;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glad_glGetConvolutionParameterfv = NULL;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glad_glGetConvolutionParameteriv = NULL;
PFNGLGETDEBUGMESSAGELOGPROC glad_glGetDebugMessageLog = NULL;
PFNGLGETDEBUGMESSAGELOGARBPROC glad_glGetDebugMessageLogARB = NULL;
PFNGLGETDOUBLEI_VPROC glad_glGetDoublei_v = NULL;
PFNGLGETDOUBLEVPROC glad_glGetDoublev = NULL;
PFNGLGETERRORPROC glad_glGetError = NULL;
PFNGLGETFLOATI_VPROC glad_glGetFloati_v = NULL;
PFNGLGETFLOATVPROC glad_glGetFloatv = NULL;
PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex = NULL;
PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC glad_glGetFramebufferParameteriv = NULL;
PFNGLGETGRAPHICSRESETSTATUSPROC glad_glGetGraphicsResetStatus = NULL;
PFNGLGETGRAPHICSRESETSTATUSARBPROC glad_glGetGraphicsResetStatusARB = NULL;
PFNGLGETHANDLEARBPROC glad_glGetHandleARB = NULL;
PFNGLGETHISTOGRAMPROC glad_glGetHistogram = NULL;
PFNGLGETHISTOGRAMPARAMETERFVPROC glad_glGetHistogramParameterfv = NULL;
PFNGLGETHISTOGRAMPARAMETERIVPROC glad_glGetHistogramParameteriv = NULL;
PFNGLGETIMAGEHANDLEARBPROC glad_glGetImageHandleARB = NULL;
PFNGLGETINFOLOGARBPROC glad_glGetInfoLogARB = NULL;
PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v = NULL;
PFNGLGETINTEGER64VPROC glad_glGetInteger64v = NULL;
PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v = NULL;
PFNGLGETINTEGERVPROC glad_glGetIntegerv = NULL;
PFNGLGETINTERNALFORMATI64VPROC glad_glGetInternalformati64v = NULL;
PFNGLGETINTERNALFORMATIVPROC glad_glGetInternalformativ = NULL;
PFNGLGETLIGHTFVPROC glad_glGetLightfv = NULL;
PFNGLGETLIGHTIVPROC glad_glGetLightiv = NULL;
PFNGLGETMAPDVPROC glad_glGetMapdv = NULL;
PFNGLGETMAPFVPROC glad_glGetMapfv = NULL;
PFNGLGETMAPIVPROC glad_glGetMapiv = NULL;
PFNGLGETMATERIALFVPROC glad_glGetMaterialfv = NULL;
PFNGLGETMATERIALIVPROC glad_glGetMaterialiv = NULL;
PFNGLGETMINMAXPROC glad_glGetMinmax = NULL;
PFNGLGETMINMAXPARAMETERFVPROC glad_glGetMinmaxParameterfv = NULL;
PFNGLGETMINMAXPARAMETERIVPROC glad_glGetMinmaxParameteriv = NULL;
PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv = NULL;
PFNGLGETNAMEDBUFFERPARAMETERI64VPROC glad_glGetNamedBufferParameteri64v = NULL;
PFNGLGETNAMEDBUFFERPARAMETERIVPROC glad_glGetNamedBufferParameteriv = NULL;
PFNGLGETNAMEDBUFFERPOINTERVPROC glad_glGetNamedBufferPointerv = NULL;
PFNGLGETNAMEDBUFFERSUBDATAPROC glad_glGetNamedBufferSubData = NULL;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetNamedFramebufferAttachmentParameteriv = NULL;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC glad_glGetNamedFramebufferParameteriv = NULL;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC glad_glGetNamedRenderbufferParameteriv = NULL;
PFNGLGETNAMEDSTRINGARBPROC glad_glGetNamedStringARB = NULL;
PFNGLGETNAMEDSTRINGIVARBPROC glad_glGetNamedStringivARB = NULL;
PFNGLGETOBJECTLABELPROC glad_glGetObjectLabel = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC glad_glGetObjectParameterfvARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glad_glGetObjectParameterivARB = NULL;
PFNGLGETOBJECTPTRLABELPROC glad_glGetObjectPtrLabel = NULL;
PFNGLGETPIXELMAPFVPROC glad_glGetPixelMapfv = NULL;
PFNGLGETPIXELMAPUIVPROC glad_glGetPixelMapuiv = NULL;
PFNGLGETPIXELMAPUSVPROC glad_glGetPixelMapusv = NULL;
PFNGLGETPOINTERVPROC glad_glGetPointerv = NULL;
PFNGLGETPOLYGONSTIPPLEPROC glad_glGetPolygonStipple = NULL;
PFNGLGETPROGRAMBINARYPROC glad_glGetProgramBinary = NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC glad_glGetProgramEnvParameterdvARB = NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC glad_glGetProgramEnvParameterfvARB = NULL;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMINTERFACEIVPROC glad_glGetProgramInterfaceiv = NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glad_glGetProgramLocalParameterdvARB = NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glad_glGetProgramLocalParameterfvARB = NULL;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glad_glGetProgramPipelineInfoLog = NULL;
PFNGLGETPROGRAMPIPELINEIVPROC glad_glGetProgramPipelineiv = NULL;
PFNGLGETPROGRAMRESOURCEINDEXPROC glad_glGetProgramResourceIndex = NULL;
PFNGLGETPROGRAMRESOURCELOCATIONPROC glad_glGetProgramResourceLocation = NULL;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glad_glGetProgramResourceLocationIndex = NULL;
PFNGLGETPROGRAMRESOURCENAMEPROC glad_glGetProgramResourceName = NULL;
PFNGLGETPROGRAMRESOURCEIVPROC glad_glGetProgramResourceiv = NULL;
PFNGLGETPROGRAMSTAGEIVPROC glad_glGetProgramStageiv = NULL;
PFNGLGETPROGRAMSTRINGARBPROC glad_glGetProgramStringARB = NULL;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
PFNGLGETPROGRAMIVARBPROC glad_glGetProgramivARB = NULL;
PFNGLGETQUERYBUFFEROBJECTI64VPROC glad_glGetQueryBufferObjecti64v = NULL;
PFNGLGETQUERYBUFFEROBJECTIVPROC glad_glGetQueryBufferObjectiv = NULL;
PFNGLGETQUERYBUFFEROBJECTUI64VPROC glad_glGetQueryBufferObjectui64v = NULL;
PFNGLGETQUERYBUFFEROBJECTUIVPROC glad_glGetQueryBufferObjectuiv = NULL;
PFNGLGETQUERYINDEXEDIVPROC glad_glGetQueryIndexediv = NULL;
PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTIVARBPROC glad_glGetQueryObjectivARB = NULL;
PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v = NULL;
PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv = NULL;
PFNGLGETQUERYOBJECTUIVARBPROC glad_glGetQueryObjectuivARB = NULL;
PFNGLGETQUERYIVPROC glad_glGetQueryiv = NULL;
PFNGLGETQUERYIVARBPROC glad_glGetQueryivARB = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv = NULL;
PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv = NULL;
PFNGLGETSEPARABLEFILTERPROC glad_glGetSeparableFilter = NULL;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;
PFNGLGETSHADERPRECISIONFORMATPROC glad_glGetShaderPrecisionFormat = NULL;
PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource = NULL;
PFNGLGETSHADERSOURCEARBPROC glad_glGetShaderSourceARB = NULL;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
PFNGLGETSTRINGPROC glad_glGetString = NULL;
PFNGLGETSTRINGIPROC glad_glGetStringi = NULL;
PFNGLGETSUBROUTINEINDEXPROC glad_glGetSubroutineIndex = NULL;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glad_glGetSubroutineUniformLocation = NULL;
PFNGLGETSYNCIVPROC glad_glGetSynciv = NULL;
PFNGLGETTEXENVFVPROC glad_glGetTexEnvfv = NULL;
PFNGLGETTEXENVIVPROC glad_glGetTexEnviv = NULL;
PFNGLGETTEXGENDVPROC glad_glGetTexGendv = NULL;
PFNGLGETTEXGENFVPROC glad_glGetTexGenfv = NULL;
PFNGLGETTEXGENIVPROC glad_glGetTexGeniv = NULL;
PFNGLGETTEXIMAGEPROC glad_glGetTexImage = NULL;
PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv = NULL;
PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv = NULL;
PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv = NULL;
PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv = NULL;
PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv = NULL;
PFNGLGETTEXTUREHANDLEARBPROC glad_glGetTextureHandleARB = NULL;
PFNGLGETTEXTUREIMAGEPROC glad_glGetTextureImage = NULL;
PFNGLGETTEXTURELEVELPARAMETERFVPROC glad_glGetTextureLevelParameterfv = NULL;
PFNGLGETTEXTURELEVELPARAMETERIVPROC glad_glGetTextureLevelParameteriv = NULL;
PFNGLGETTEXTUREPARAMETERIIVPROC glad_glGetTextureParameterIiv = NULL;
PFNGLGETTEXTUREPARAMETERIUIVPROC glad_glGetTextureParameterIuiv = NULL;
PFNGLGETTEXTUREPARAMETERFVPROC glad_glGetTextureParameterfv = NULL;
PFNGLGETTEXTUREPARAMETERIVPROC glad_glGetTextureParameteriv = NULL;
PFNGLGETTEXTURESAMPLERHANDLEARBPROC glad_glGetTextureSamplerHandleARB = NULL;
PFNGLGETTEXTURESUBIMAGEPROC glad_glGetTextureSubImage = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying = NULL;
PFNGLGETTRANSFORMFEEDBACKI64_VPROC glad_glGetTransformFeedbacki64_v = NULL;
PFNGLGETTRANSFORMFEEDBACKI_VPROC glad_glGetTransformFeedbacki_v = NULL;
PFNGLGETTRANSFORMFEEDBACKIVPROC glad_glGetTransformFeedbackiv = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex = NULL;
PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices = NULL;
PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glad_glGetUniformLocationARB = NULL;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glad_glGetUniformSubroutineuiv = NULL;
PFNGLGETUNIFORMDVPROC glad_glGetUniformdv = NULL;
PFNGLGETUNIFORMFVPROC glad_glGetUniformfv = NULL;
PFNGLGETUNIFORMFVARBPROC glad_glGetUniformfvARB = NULL;
PFNGLGETUNIFORMI64VARBPROC glad_glGetUniformi64vARB = NULL;
PFNGLGETUNIFORMIVPROC glad_glGetUniformiv = NULL;
PFNGLGETUNIFORMIVARBPROC glad_glGetUniformivARB = NULL;
PFNGLGETUNIFORMUI64VARBPROC glad_glGetUniformui64vARB = NULL;
PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv = NULL;
PFNGLGETVERTEXARRAYINDEXED64IVPROC glad_glGetVertexArrayIndexed64iv = NULL;
PFNGLGETVERTEXARRAYINDEXEDIVPROC glad_glGetVertexArrayIndexediv = NULL;
PFNGLGETVERTEXARRAYIVPROC glad_glGetVertexArrayiv = NULL;
PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv = NULL;
PFNGLGETVERTEXATTRIBLDVPROC glad_glGetVertexAttribLdv = NULL;
PFNGLGETVERTEXATTRIBLUI64VARBPROC glad_glGetVertexAttribLui64vARB = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glad_glGetVertexAttribPointervARB = NULL;
PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBDVARBPROC glad_glGetVertexAttribdvARB = NULL;
PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBFVARBPROC glad_glGetVertexAttribfvARB = NULL;
PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv = NULL;
PFNGLGETVERTEXATTRIBIVARBPROC glad_glGetVertexAttribivARB = NULL;
PFNGLGETNCOLORTABLEARBPROC glad_glGetnColorTableARB = NULL;
PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC glad_glGetnCompressedTexImageARB = NULL;
PFNGLGETNCONVOLUTIONFILTERARBPROC glad_glGetnConvolutionFilterARB = NULL;
PFNGLGETNHISTOGRAMARBPROC glad_glGetnHistogramARB = NULL;
PFNGLGETNMAPDVARBPROC glad_glGetnMapdvARB = NULL;
PFNGLGETNMAPFVARBPROC glad_glGetnMapfvARB = NULL;
PFNGLGETNMAPIVARBPROC glad_glGetnMapivARB = NULL;
PFNGLGETNMINMAXARBPROC glad_glGetnMinmaxARB = NULL;
PFNGLGETNPIXELMAPFVARBPROC glad_glGetnPixelMapfvARB = NULL;
PFNGLGETNPIXELMAPUIVARBPROC glad_glGetnPixelMapuivARB = NULL;
PFNGLGETNPIXELMAPUSVARBPROC glad_glGetnPixelMapusvARB = NULL;
PFNGLGETNPOLYGONSTIPPLEARBPROC glad_glGetnPolygonStippleARB = NULL;
PFNGLGETNSEPARABLEFILTERARBPROC glad_glGetnSeparableFilterARB = NULL;
PFNGLGETNTEXIMAGEARBPROC glad_glGetnTexImageARB = NULL;
PFNGLGETNUNIFORMDVARBPROC glad_glGetnUniformdvARB = NULL;
PFNGLGETNUNIFORMFVPROC glad_glGetnUniformfv = NULL;
PFNGLGETNUNIFORMFVARBPROC glad_glGetnUniformfvARB = NULL;
PFNGLGETNUNIFORMI64VARBPROC glad_glGetnUniformi64vARB = NULL;
PFNGLGETNUNIFORMIVPROC glad_glGetnUniformiv = NULL;
PFNGLGETNUNIFORMIVARBPROC glad_glGetnUniformivARB = NULL;
PFNGLGETNUNIFORMUI64VARBPROC glad_glGetnUniformui64vARB = NULL;
PFNGLGETNUNIFORMUIVPROC glad_glGetnUniformuiv = NULL;
PFNGLGETNUNIFORMUIVARBPROC glad_glGetnUniformuivARB = NULL;
PFNGLHINTPROC glad_glHint = NULL;
PFNGLHISTOGRAMPROC glad_glHistogram = NULL;
PFNGLINDEXMASKPROC glad_glIndexMask = NULL;
PFNGLINDEXPOINTERPROC glad_glIndexPointer = NULL;
PFNGLINDEXDPROC glad_glIndexd = NULL;
PFNGLINDEXDVPROC glad_glIndexdv = NULL;
PFNGLINDEXFPROC glad_glIndexf = NULL;
PFNGLINDEXFVPROC glad_glIndexfv = NULL;
PFNGLINDEXIPROC glad_glIndexi = NULL;
PFNGLINDEXIVPROC glad_glIndexiv = NULL;
PFNGLINDEXSPROC glad_glIndexs = NULL;
PFNGLINDEXSVPROC glad_glIndexsv = NULL;
PFNGLINDEXUBPROC glad_glIndexub = NULL;
PFNGLINDEXUBVPROC glad_glIndexubv = NULL;
PFNGLINITNAMESPROC glad_glInitNames = NULL;
PFNGLINTERLEAVEDARRAYSPROC glad_glInterleavedArrays = NULL;
PFNGLINVALIDATEBUFFERDATAPROC glad_glInvalidateBufferData = NULL;
PFNGLINVALIDATEBUFFERSUBDATAPROC glad_glInvalidateBufferSubData = NULL;
PFNGLINVALIDATEFRAMEBUFFERPROC glad_glInvalidateFramebuffer = NULL;
PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC glad_glInvalidateNamedFramebufferData = NULL;
PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC glad_glInvalidateNamedFramebufferSubData = NULL;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glad_glInvalidateSubFramebuffer = NULL;
PFNGLINVALIDATETEXIMAGEPROC glad_glInvalidateTexImage = NULL;
PFNGLINVALIDATETEXSUBIMAGEPROC glad_glInvalidateTexSubImage = NULL;
PFNGLISBUFFERPROC glad_glIsBuffer = NULL;
PFNGLISBUFFERARBPROC glad_glIsBufferARB = NULL;
PFNGLISENABLEDPROC glad_glIsEnabled = NULL;
PFNGLISENABLEDIPROC glad_glIsEnabledi = NULL;
PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer = NULL;
PFNGLISIMAGEHANDLERESIDENTARBPROC glad_glIsImageHandleResidentARB = NULL;
PFNGLISLISTPROC glad_glIsList = NULL;
PFNGLISNAMEDSTRINGARBPROC glad_glIsNamedStringARB = NULL;
PFNGLISPROGRAMPROC glad_glIsProgram = NULL;
PFNGLISPROGRAMARBPROC glad_glIsProgramARB = NULL;
PFNGLISPROGRAMPIPELINEPROC glad_glIsProgramPipeline = NULL;
PFNGLISQUERYPROC glad_glIsQuery = NULL;
PFNGLISQUERYARBPROC glad_glIsQueryARB = NULL;
PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer = NULL;
PFNGLISSAMPLERPROC glad_glIsSampler = NULL;
PFNGLISSHADERPROC glad_glIsShader = NULL;
PFNGLISSYNCPROC glad_glIsSync = NULL;
PFNGLISTEXTUREPROC glad_glIsTexture = NULL;
PFNGLISTEXTUREHANDLERESIDENTARBPROC glad_glIsTextureHandleResidentARB = NULL;
PFNGLISTRANSFORMFEEDBACKPROC glad_glIsTransformFeedback = NULL;
PFNGLISVERTEXARRAYPROC glad_glIsVertexArray = NULL;
PFNGLLIGHTMODELFPROC glad_glLightModelf = NULL;
PFNGLLIGHTMODELFVPROC glad_glLightModelfv = NULL;
PFNGLLIGHTMODELIPROC glad_glLightModeli = NULL;
PFNGLLIGHTMODELIVPROC glad_glLightModeliv = NULL;
PFNGLLIGHTFPROC glad_glLightf = NULL;
PFNGLLIGHTFVPROC glad_glLightfv = NULL;
PFNGLLIGHTIPROC glad_glLighti = NULL;
PFNGLLIGHTIVPROC glad_glLightiv = NULL;
PFNGLLINESTIPPLEPROC glad_glLineStipple = NULL;
PFNGLLINEWIDTHPROC glad_glLineWidth = NULL;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
PFNGLLINKPROGRAMARBPROC glad_glLinkProgramARB = NULL;
PFNGLLISTBASEPROC glad_glListBase = NULL;
PFNGLLOADIDENTITYPROC glad_glLoadIdentity = NULL;
PFNGLLOADMATRIXDPROC glad_glLoadMatrixd = NULL;
PFNGLLOADMATRIXFPROC glad_glLoadMatrixf = NULL;
PFNGLLOADNAMEPROC glad_glLoadName = NULL;
PFNGLLOADTRANSPOSEMATRIXDPROC glad_glLoadTransposeMatrixd = NULL;
PFNGLLOADTRANSPOSEMATRIXDARBPROC glad_glLoadTransposeMatrixdARB = NULL;
PFNGLLOADTRANSPOSEMATRIXFPROC glad_glLoadTransposeMatrixf = NULL;
PFNGLLOADTRANSPOSEMATRIXFARBPROC glad_glLoadTransposeMatrixfARB = NULL;
PFNGLLOGICOPPROC glad_glLogicOp = NULL;
PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC glad_glMakeImageHandleNonResidentARB = NULL;
PFNGLMAKEIMAGEHANDLERESIDENTARBPROC glad_glMakeImageHandleResidentARB = NULL;
PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC glad_glMakeTextureHandleNonResidentARB = NULL;
PFNGLMAKETEXTUREHANDLERESIDENTARBPROC glad_glMakeTextureHandleResidentARB = NULL;
PFNGLMAP1DPROC glad_glMap1d = NULL;
PFNGLMAP1FPROC glad_glMap1f = NULL;
PFNGLMAP2DPROC glad_glMap2d = NULL;
PFNGLMAP2FPROC glad_glMap2f = NULL;
PFNGLMAPBUFFERPROC glad_glMapBuffer = NULL;
PFNGLMAPBUFFERARBPROC glad_glMapBufferARB = NULL;
PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange = NULL;
PFNGLMAPGRID1DPROC glad_glMapGrid1d = NULL;
PFNGLMAPGRID1FPROC glad_glMapGrid1f = NULL;
PFNGLMAPGRID2DPROC glad_glMapGrid2d = NULL;
PFNGLMAPGRID2FPROC glad_glMapGrid2f = NULL;
PFNGLMAPNAMEDBUFFERPROC glad_glMapNamedBuffer = NULL;
PFNGLMAPNAMEDBUFFERRANGEPROC glad_glMapNamedBufferRange = NULL;
PFNGLMATERIALFPROC glad_glMaterialf = NULL;
PFNGLMATERIALFVPROC glad_glMaterialfv = NULL;
PFNGLMATERIALIPROC glad_glMateriali = NULL;
PFNGLMATERIALIVPROC glad_glMaterialiv = NULL;
PFNGLMATRIXINDEXPOINTERARBPROC glad_glMatrixIndexPointerARB = NULL;
PFNGLMATRIXINDEXUBVARBPROC glad_glMatrixIndexubvARB = NULL;
PFNGLMATRIXINDEXUIVARBPROC glad_glMatrixIndexuivARB = NULL;
PFNGLMATRIXINDEXUSVARBPROC glad_glMatrixIndexusvARB = NULL;
PFNGLMATRIXMODEPROC glad_glMatrixMode = NULL;
PFNGLMAXSHADERCOMPILERTHREADSARBPROC glad_glMaxShaderCompilerThreadsARB = NULL;
PFNGLMAXSHADERCOMPILERTHREADSKHRPROC glad_glMaxShaderCompilerThreadsKHR = NULL;
PFNGLMEMORYBARRIERPROC glad_glMemoryBarrier = NULL;
PFNGLMEMORYBARRIERBYREGIONPROC glad_glMemoryBarrierByRegion = NULL;
PFNGLMINSAMPLESHADINGPROC glad_glMinSampleShading = NULL;
PFNGLMINSAMPLESHADINGARBPROC glad_glMinSampleShadingARB = NULL;
PFNGLMINMAXPROC glad_glMinmax = NULL;
PFNGLMULTMATRIXDPROC glad_glMultMatrixd = NULL;
PFNGLMULTMATRIXFPROC glad_glMultMatrixf = NULL;
PFNGLMULTTRANSPOSEMATRIXDPROC glad_glMultTransposeMatrixd = NULL;
PFNGLMULTTRANSPOSEMATRIXDARBPROC glad_glMultTransposeMatrixdARB = NULL;
PFNGLMULTTRANSPOSEMATRIXFPROC glad_glMultTransposeMatrixf = NULL;
PFNGLMULTTRANSPOSEMATRIXFARBPROC glad_glMultTransposeMatrixfARB = NULL;
PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays = NULL;
PFNGLMULTIDRAWARRAYSINDIRECTPROC glad_glMultiDrawArraysIndirect = NULL;
PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC glad_glMultiDrawArraysIndirectCount = NULL;
PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC glad_glMultiDrawArraysIndirectCountARB = NULL;
PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex = NULL;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC glad_glMultiDrawElementsIndirect = NULL;
PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC glad_glMultiDrawElementsIndirectCount = NULL;
PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC glad_glMultiDrawElementsIndirectCountARB = NULL;
PFNGLMULTITEXCOORD1DPROC glad_glMultiTexCoord1d = NULL;
PFNGLMULTITEXCOORD1DARBPROC glad_glMultiTexCoord1dARB = NULL;
PFNGLMULTITEXCOORD1DVPROC glad_glMultiTexCoord1dv = NULL;
PFNGLMULTITEXCOORD1DVARBPROC glad_glMultiTexCoord1dvARB = NULL;
PFNGLMULTITEXCOORD1FPROC glad_glMultiTexCoord1f = NULL;
PFNGLMULTITEXCOORD1FARBPROC glad_glMultiTexCoord1fARB = NULL;
PFNGLMULTITEXCOORD1FVPROC glad_glMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD1FVARBPROC glad_glMultiTexCoord1fvARB = NULL;
PFNGLMULTITEXCOORD1IPROC glad_glMultiTexCoord1i = NULL;
PFNGLMULTITEXCOORD1IARBPROC glad_glMultiTexCoord1iARB = NULL;
PFNGLMULTITEXCOORD1IVPROC glad_glMultiTexCoord1iv = NULL;
PFNGLMULTITEXCOORD1IVARBPROC glad_glMultiTexCoord1ivARB = NULL;
PFNGLMULTITEXCOORD1SPROC glad_glMultiTexCoord1s = NULL;
PFNGLMULTITEXCOORD1SARBPROC glad_glMultiTexCoord1sARB = NULL;
PFNGLMULTITEXCOORD1SVPROC glad_glMultiTexCoord1sv = NULL;
PFNGLMULTITEXCOORD1SVARBPROC glad_glMultiTexCoord1svARB = NULL;
PFNGLMULTITEXCOORD2DPROC glad_glMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD2DARBPROC glad_glMultiTexCoord2dARB = NULL;
PFNGLMULTITEXCOORD2DVPROC glad_glMultiTexCoord2dv = NULL;
PFNGLMULTITEXCOORD2DVARBPROC glad_glMultiTexCoord2dvARB = NULL;
PFNGLMULTITEXCOORD2FPROC glad_glMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FARBPROC glad_glMultiTexCoord2fARB = NULL;
PFNGLMULTITEXCOORD2FVPROC glad_glMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2FVARBPROC glad_glMultiTexCoord2fvARB = NULL;
PFNGLMULTITEXCOORD2IPROC glad_glMultiTexCoord2i = NULL;
PFNGLMULTITEXCOORD2IARBPROC glad_glMultiTexCoord2iARB = NULL;
PFNGLMULTITEXCOORD2IVPROC glad_glMultiTexCoord2iv = NULL;
PFNGLMULTITEXCOORD2IVARBPROC glad_glMultiTexCoord2ivARB = NULL;
PFNGLMULTITEXCOORD2SPROC glad_glMultiTexCoord2s = NULL;
PFNGLMULTITEXCOORD2SARBPROC glad_glMultiTexCoord2sARB = NULL;
PFNGLMULTITEXCOORD2SVPROC glad_glMultiTexCoord2sv = NULL;
PFNGLMULTITEXCOORD2SVARBPROC glad_glMultiTexCoord2svARB = NULL;
PFNGLMULTITEXCOORD3DPROC glad_glMultiTexCoord3d = NULL;
PFNGLMULTITEXCOORD3DARBPROC glad_glMultiTexCoord3dARB = NULL;
PFNGLMULTITEXCOORD3DVPROC glad_glMultiTexCoord3dv = NULL;
PFNGLMULTITEXCOORD3DVARBPROC glad_glMultiTexCoord3dvARB = NULL;
PFNGLMULTITEXCOORD3FPROC glad_glMultiTexCoord3f = NULL;
PFNGLMULTITEXCOORD3FARBPROC glad_glMultiTexCoord3fARB = NULL;
PFNGLMULTITEXCOORD3FVPROC glad_glMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD3FVARBPROC glad_glMultiTexCoord3fvARB = NULL;
PFNGLMULTITEXCOORD3IPROC glad_glMultiTexCoord3i = NULL;
PFNGLMULTITEXCOORD3IARBPROC glad_glMultiTexCoord3iARB = NULL;
PFNGLMULTITEXCOORD3IVPROC glad_glMultiTexCoord3iv = NULL;
PFNGLMULTITEXCOORD3IVARBPROC glad_glMultiTexCoord3ivARB = NULL;
PFNGLMULTITEXCOORD3SPROC glad_glMultiTexCoord3s = NULL;
PFNGLMULTITEXCOORD3SARBPROC glad_glMultiTexCoord3sARB = NULL;
PFNGLMULTITEXCOORD3SVPROC glad_glMultiTexCoord3sv = NULL;
PFNGLMULTITEXCOORD3SVARBPROC glad_glMultiTexCoord3svARB = NULL;
PFNGLMULTITEXCOORD4DPROC glad_glMultiTexCoord4d = NULL;
PFNGLMULTITEXCOORD4DARBPROC glad_glMultiTexCoord4dARB = NULL;
PFNGLMULTITEXCOORD4DVPROC glad_glMultiTexCoord4dv = NULL;
PFNGLMULTITEXCOORD4DVARBPROC glad_glMultiTexCoord4dvARB = NULL;
PFNGLMULTITEXCOORD4FPROC glad_glMultiTexCoord4f = NULL;
PFNGLMULTITEXCOORD4FARBPROC glad_glMultiTexCoord4fARB = NULL;
PFNGLMULTITEXCOORD4FVPROC glad_glMultiTexCoord4fv = NULL;
PFNGLMULTITEXCOORD4FVARBPROC glad_glMultiTexCoord4fvARB = NULL;
PFNGLMULTITEXCOORD4IPROC glad_glMultiTexCoord4i = NULL;
PFNGLMULTITEXCOORD4IARBPROC glad_glMultiTexCoord4iARB = NULL;
PFNGLMULTITEXCOORD4IVPROC glad_glMultiTexCoord4iv = NULL;
PFNGLMULTITEXCOORD4IVARBPROC glad_glMultiTexCoord4ivARB = NULL;
PFNGLMULTITEXCOORD4SPROC glad_glMultiTexCoord4s = NULL;
PFNGLMULTITEXCOORD4SARBPROC glad_glMultiTexCoord4sARB = NULL;
PFNGLMULTITEXCOORD4SVPROC glad_glMultiTexCoord4sv = NULL;
PFNGLMULTITEXCOORD4SVARBPROC glad_glMultiTexCoord4svARB = NULL;
PFNGLMULTITEXCOORDP1UIPROC glad_glMultiTexCoordP1ui = NULL;
PFNGLMULTITEXCOORDP1UIVPROC glad_glMultiTexCoordP1uiv = NULL;
PFNGLMULTITEXCOORDP2UIPROC glad_glMultiTexCoordP2ui = NULL;
PFNGLMULTITEXCOORDP2UIVPROC glad_glMultiTexCoordP2uiv = NULL;
PFNGLMULTITEXCOORDP3UIPROC glad_glMultiTexCoordP3ui = NULL;
PFNGLMULTITEXCOORDP3UIVPROC glad_glMultiTexCoordP3uiv = NULL;
PFNGLMULTITEXCOORDP4UIPROC glad_glMultiTexCoordP4ui = NULL;
PFNGLMULTITEXCOORDP4UIVPROC glad_glMultiTexCoordP4uiv = NULL;
PFNGLNAMEDBUFFERDATAPROC glad_glNamedBufferData = NULL;
PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC glad_glNamedBufferPageCommitmentARB = NULL;
PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC glad_glNamedBufferPageCommitmentEXT = NULL;
PFNGLNAMEDBUFFERSTORAGEPROC glad_glNamedBufferStorage = NULL;
PFNGLNAMEDBUFFERSUBDATAPROC glad_glNamedBufferSubData = NULL;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC glad_glNamedFramebufferDrawBuffer = NULL;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC glad_glNamedFramebufferDrawBuffers = NULL;
PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC glad_glNamedFramebufferParameteri = NULL;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC glad_glNamedFramebufferReadBuffer = NULL;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC glad_glNamedFramebufferRenderbuffer = NULL;
PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVARBPROC glad_glNamedFramebufferSampleLocationsfvARB = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glad_glNamedFramebufferTexture = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC glad_glNamedFramebufferTextureLayer = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEPROC glad_glNamedRenderbufferStorage = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glNamedRenderbufferStorageMultisample = NULL;
PFNGLNAMEDSTRINGARBPROC glad_glNamedStringARB = NULL;
PFNGLNEWLISTPROC glad_glNewList = NULL;
PFNGLNORMAL3BPROC glad_glNormal3b = NULL;
PFNGLNORMAL3BVPROC glad_glNormal3bv = NULL;
PFNGLNORMAL3DPROC glad_glNormal3d = NULL;
PFNGLNORMAL3DVPROC glad_glNormal3dv = NULL;
PFNGLNORMAL3FPROC glad_glNormal3f = NULL;
PFNGLNORMAL3FVPROC glad_glNormal3fv = NULL;
PFNGLNORMAL3IPROC glad_glNormal3i = NULL;
PFNGLNORMAL3IVPROC glad_glNormal3iv = NULL;
PFNGLNORMAL3SPROC glad_glNormal3s = NULL;
PFNGLNORMAL3SVPROC glad_glNormal3sv = NULL;
PFNGLNORMALP3UIPROC glad_glNormalP3ui = NULL;
PFNGLNORMALP3UIVPROC glad_glNormalP3uiv = NULL;
PFNGLNORMALPOINTERPROC glad_glNormalPointer = NULL;
PFNGLOBJECTLABELPROC glad_glObjectLabel = NULL;
PFNGLOBJECTPTRLABELPROC glad_glObjectPtrLabel = NULL;
PFNGLORTHOPROC glad_glOrtho = NULL;
PFNGLPASSTHROUGHPROC glad_glPassThrough = NULL;
PFNGLPATCHPARAMETERFVPROC glad_glPatchParameterfv = NULL;
PFNGLPATCHPARAMETERIPROC glad_glPatchParameteri = NULL;
PFNGLPAUSETRANSFORMFEEDBACKPROC glad_glPauseTransformFeedback = NULL;
PFNGLPIXELMAPFVPROC glad_glPixelMapfv = NULL;
PFNGLPIXELMAPUIVPROC glad_glPixelMapuiv = NULL;
PFNGLPIXELMAPUSVPROC glad_glPixelMapusv = NULL;
PFNGLPIXELSTOREFPROC glad_glPixelStoref = NULL;
PFNGLPIXELSTOREIPROC glad_glPixelStorei = NULL;
PFNGLPIXELTRANSFERFPROC glad_glPixelTransferf = NULL;
PFNGLPIXELTRANSFERIPROC glad_glPixelTransferi = NULL;
PFNGLPIXELZOOMPROC glad_glPixelZoom = NULL;
PFNGLPOINTPARAMETERFPROC glad_glPointParameterf = NULL;
PFNGLPOINTPARAMETERFARBPROC glad_glPointParameterfARB = NULL;
PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv = NULL;
PFNGLPOINTPARAMETERFVARBPROC glad_glPointParameterfvARB = NULL;
PFNGLPOINTPARAMETERIPROC glad_glPointParameteri = NULL;
PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv = NULL;
PFNGLPOINTSIZEPROC glad_glPointSize = NULL;
PFNGLPOLYGONMODEPROC glad_glPolygonMode = NULL;
PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset = NULL;
PFNGLPOLYGONOFFSETCLAMPPROC glad_glPolygonOffsetClamp = NULL;
PFNGLPOLYGONSTIPPLEPROC glad_glPolygonStipple = NULL;
PFNGLPOPATTRIBPROC glad_glPopAttrib = NULL;
PFNGLPOPCLIENTATTRIBPROC glad_glPopClientAttrib = NULL;
PFNGLPOPDEBUGGROUPPROC glad_glPopDebugGroup = NULL;
PFNGLPOPMATRIXPROC glad_glPopMatrix = NULL;
PFNGLPOPNAMEPROC glad_glPopName = NULL;
PFNGLPRIMITIVEBOUNDINGBOXPROC glad_glPrimitiveBoundingBox = NULL;
PFNGLPRIMITIVEBOUNDINGBOXARBPROC glad_glPrimitiveBoundingBoxARB = NULL;
PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex = NULL;
PFNGLPRIORITIZETEXTURESPROC glad_glPrioritizeTextures = NULL;
PFNGLPROGRAMBINARYPROC glad_glProgramBinary = NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC glad_glProgramEnvParameter4dARB = NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC glad_glProgramEnvParameter4dvARB = NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC glad_glProgramEnvParameter4fARB = NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC glad_glProgramEnvParameter4fvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC glad_glProgramLocalParameter4dARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glad_glProgramLocalParameter4dvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glad_glProgramLocalParameter4fARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glad_glProgramLocalParameter4fvARB = NULL;
PFNGLPROGRAMPARAMETERIPROC glad_glProgramParameteri = NULL;
PFNGLPROGRAMPARAMETERIARBPROC glad_glProgramParameteriARB = NULL;
PFNGLPROGRAMSTRINGARBPROC glad_glProgramStringARB = NULL;
PFNGLPROGRAMUNIFORM1DPROC glad_glProgramUniform1d = NULL;
PFNGLPROGRAMUNIFORM1DVPROC glad_glProgramUniform1dv = NULL;
PFNGLPROGRAMUNIFORM1FPROC glad_glProgramUniform1f = NULL;
PFNGLPROGRAMUNIFORM1FVPROC glad_glProgramUniform1fv = NULL;
PFNGLPROGRAMUNIFORM1IPROC glad_glProgramUniform1i = NULL;
PFNGLPROGRAMUNIFORM1I64ARBPROC glad_glProgramUniform1i64ARB = NULL;
PFNGLPROGRAMUNIFORM1I64VARBPROC glad_glProgramUniform1i64vARB = NULL;
PFNGLPROGRAMUNIFORM1IVPROC glad_glProgramUniform1iv = NULL;
PFNGLPROGRAMUNIFORM1UIPROC glad_glProgramUniform1ui = NULL;
PFNGLPROGRAMUNIFORM1UI64ARBPROC glad_glProgramUniform1ui64ARB = NULL;
PFNGLPROGRAMUNIFORM1UI64VARBPROC glad_glProgramUniform1ui64vARB = NULL;
PFNGLPROGRAMUNIFORM1UIVPROC glad_glProgramUniform1uiv = NULL;
PFNGLPROGRAMUNIFORM2DPROC glad_glProgramUniform2d = NULL;
PFNGLPROGRAMUNIFORM2DVPROC glad_glProgramUniform2dv = NULL;
PFNGLPROGRAMUNIFORM2FPROC glad_glProgramUniform2f = NULL;
PFNGLPROGRAMUNIFORM2FVPROC glad_glProgramUniform2fv = NULL;
PFNGLPROGRAMUNIFORM2IPROC glad_glProgramUniform2i = NULL;
PFNGLPROGRAMUNIFORM2I64ARBPROC glad_glProgramUniform2i64ARB = NULL;
PFNGLPROGRAMUNIFORM2I64VARBPROC glad_glProgramUniform2i64vARB = NULL;
PFNGLPROGRAMUNIFORM2IVPROC glad_glProgramUniform2iv = NULL;
PFNGLPROGRAMUNIFORM2UIPROC glad_glProgramUniform2ui = NULL;
PFNGLPROGRAMUNIFORM2UI64ARBPROC glad_glProgramUniform2ui64ARB = NULL;
PFNGLPROGRAMUNIFORM2UI64VARBPROC glad_glProgramUniform2ui64vARB = NULL;
PFNGLPROGRAMUNIFORM2UIVPROC glad_glProgramUniform2uiv = NULL;
PFNGLPROGRAMUNIFORM3DPROC glad_glProgramUniform3d = NULL;
PFNGLPROGRAMUNIFORM3DVPROC glad_glProgramUniform3dv = NULL;
PFNGLPROGRAMUNIFORM3FPROC glad_glProgramUniform3f = NULL;
PFNGLPROGRAMUNIFORM3FVPROC glad_glProgramUniform3fv = NULL;
PFNGLPROGRAMUNIFORM3IPROC glad_glProgramUniform3i = NULL;
PFNGLPROGRAMUNIFORM3I64ARBPROC glad_glProgramUniform3i64ARB = NULL;
PFNGLPROGRAMUNIFORM3I64VARBPROC glad_glProgramUniform3i64vARB = NULL;
PFNGLPROGRAMUNIFORM3IVPROC glad_glProgramUniform3iv = NULL;
PFNGLPROGRAMUNIFORM3UIPROC glad_glProgramUniform3ui = NULL;
PFNGLPROGRAMUNIFORM3UI64ARBPROC glad_glProgramUniform3ui64ARB = NULL;
PFNGLPROGRAMUNIFORM3UI64VARBPROC glad_glProgramUniform3ui64vARB = NULL;
PFNGLPROGRAMUNIFORM3UIVPROC glad_glProgramUniform3uiv = NULL;
PFNGLPROGRAMUNIFORM4DPROC glad_glProgramUniform4d = NULL;
PFNGLPROGRAMUNIFORM4DVPROC glad_glProgramUniform4dv = NULL;
PFNGLPROGRAMUNIFORM4FPROC glad_glProgramUniform4f = NULL;
PFNGLPROGRAMUNIFORM4FVPROC glad_glProgramUniform4fv = NULL;
PFNGLPROGRAMUNIFORM4IPROC glad_glProgramUniform4i = NULL;
PFNGLPROGRAMUNIFORM4I64ARBPROC glad_glProgramUniform4i64ARB = NULL;
PFNGLPROGRAMUNIFORM4I64VARBPROC glad_glProgramUniform4i64vARB = NULL;
PFNGLPROGRAMUNIFORM4IVPROC glad_glProgramUniform4iv = NULL;
PFNGLPROGRAMUNIFORM4UIPROC glad_glProgramUniform4ui = NULL;
PFNGLPROGRAMUNIFORM4UI64ARBPROC glad_glProgramUniform4ui64ARB = NULL;
PFNGLPROGRAMUNIFORM4UI64VARBPROC glad_glProgramUniform4ui64vARB = NULL;
PFNGLPROGRAMUNIFORM4UIVPROC glad_glProgramUniform4uiv = NULL;
PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC glad_glProgramUniformHandleui64ARB = NULL;
PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC glad_glProgramUniformHandleui64vARB = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glad_glProgramUniformMatrix2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glad_glProgramUniformMatrix2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glad_glProgramUniformMatrix2x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glad_glProgramUniformMatrix2x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glad_glProgramUniformMatrix2x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glad_glProgramUniformMatrix2x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glad_glProgramUniformMatrix3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glad_glProgramUniformMatrix3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glad_glProgramUniformMatrix3x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glad_glProgramUniformMatrix3x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glad_glProgramUniformMatrix3x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glad_glProgramUniformMatrix3x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glad_glProgramUniformMatrix4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glad_glProgramUniformMatrix4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glad_glProgramUniformMatrix4x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glad_glProgramUniformMatrix4x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glad_glProgramUniformMatrix4x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glad_glProgramUniformMatrix4x3fv = NULL;
PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex = NULL;
PFNGLPUSHATTRIBPROC glad_glPushAttrib = NULL;
PFNGLPUSHCLIENTATTRIBPROC glad_glPushClientAttrib = NULL;
PFNGLPUSHDEBUGGROUPPROC glad_glPushDebugGroup = NULL;
PFNGLPUSHMATRIXPROC glad_glPushMatrix = NULL;
PFNGLPUSHNAMEPROC glad_glPushName = NULL;
PFNGLQUERYCOUNTERPROC glad_glQueryCounter = NULL;
PFNGLRASTERPOS2DPROC glad_glRasterPos2d = NULL;
PFNGLRASTERPOS2DVPROC glad_glRasterPos2dv = NULL;
PFNGLRASTERPOS2FPROC glad_glRasterPos2f = NULL;
PFNGLRASTERPOS2FVPROC glad_glRasterPos2fv = NULL;
PFNGLRASTERPOS2IPROC glad_glRasterPos2i = NULL;
PFNGLRASTERPOS2IVPROC glad_glRasterPos2iv = NULL;
PFNGLRASTERPOS2SPROC glad_glRasterPos2s = NULL;
PFNGLRASTERPOS2SVPROC glad_glRasterPos2sv = NULL;
PFNGLRASTERPOS3DPROC glad_glRasterPos3d = NULL;
PFNGLRASTERPOS3DVPROC glad_glRasterPos3dv = NULL;
PFNGLRASTERPOS3FPROC glad_glRasterPos3f = NULL;
PFNGLRASTERPOS3FVPROC glad_glRasterPos3fv = NULL;
PFNGLRASTERPOS3IPROC glad_glRasterPos3i = NULL;
PFNGLRASTERPOS3IVPROC glad_glRasterPos3iv = NULL;
PFNGLRASTERPOS3SPROC glad_glRasterPos3s = NULL;
PFNGLRASTERPOS3SVPROC glad_glRasterPos3sv = NULL;
PFNGLRASTERPOS4DPROC glad_glRasterPos4d = NULL;
PFNGLRASTERPOS4DVPROC glad_glRasterPos4dv = NULL;
PFNGLRASTERPOS4FPROC glad_glRasterPos4f = NULL;
PFNGLRASTERPOS4FVPROC glad_glRasterPos4fv = NULL;
PFNGLRASTERPOS4IPROC glad_glRasterPos4i = NULL;
PFNGLRASTERPOS4IVPROC glad_glRasterPos4iv = NULL;
PFNGLRASTERPOS4SPROC glad_glRasterPos4s = NULL;
PFNGLRASTERPOS4SVPROC glad_glRasterPos4sv = NULL;
PFNGLREADBUFFERPROC glad_glReadBuffer = NULL;
PFNGLREADPIXELSPROC glad_glReadPixels = NULL;
PFNGLREADNPIXELSPROC glad_glReadnPixels = NULL;
PFNGLREADNPIXELSARBPROC glad_glReadnPixelsARB = NULL;
PFNGLRECTDPROC glad_glRectd = NULL;
PFNGLRECTDVPROC glad_glRectdv = NULL;
PFNGLRECTFPROC glad_glRectf = NULL;
PFNGLRECTFVPROC glad_glRectfv = NULL;
PFNGLRECTIPROC glad_glRecti = NULL;
PFNGLRECTIVPROC glad_glRectiv = NULL;
PFNGLRECTSPROC glad_glRects = NULL;
PFNGLRECTSVPROC glad_glRectsv = NULL;
PFNGLRELEASESHADERCOMPILERPROC glad_glReleaseShaderCompiler = NULL;
PFNGLRENDERMODEPROC glad_glRenderMode = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample = NULL;
PFNGLRESETHISTOGRAMPROC glad_glResetHistogram = NULL;
PFNGLRESETMINMAXPROC glad_glResetMinmax = NULL;
PFNGLRESUMETRANSFORMFEEDBACKPROC glad_glResumeTransformFeedback = NULL;
PFNGLROTATEDPROC glad_glRotated = NULL;
PFNGLROTATEFPROC glad_glRotatef = NULL;
PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage = NULL;
PFNGLSAMPLECOVERAGEARBPROC glad_glSampleCoverageARB = NULL;
PFNGLSAMPLEMASKIPROC glad_glSampleMaski = NULL;
PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv = NULL;
PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv = NULL;
PFNGLSCALEDPROC glad_glScaled = NULL;
PFNGLSCALEFPROC glad_glScalef = NULL;
PFNGLSCISSORPROC glad_glScissor = NULL;
PFNGLSCISSORARRAYVPROC glad_glScissorArrayv = NULL;
PFNGLSCISSORINDEXEDPROC glad_glScissorIndexed = NULL;
PFNGLSCISSORINDEXEDVPROC glad_glScissorIndexedv = NULL;
PFNGLSECONDARYCOLOR3BPROC glad_glSecondaryColor3b = NULL;
PFNGLSECONDARYCOLOR3BVPROC glad_glSecondaryColor3bv = NULL;
PFNGLSECONDARYCOLOR3DPROC glad_glSecondaryColor3d = NULL;
PFNGLSECONDARYCOLOR3DVPROC glad_glSecondaryColor3dv = NULL;
PFNGLSECONDARYCOLOR3FPROC glad_glSecondaryColor3f = NULL;
PFNGLSECONDARYCOLOR3FVPROC glad_glSecondaryColor3fv = NULL;
PFNGLSECONDARYCOLOR3IPROC glad_glSecondaryColor3i = NULL;
PFNGLSECONDARYCOLOR3IVPROC glad_glSecondaryColor3iv = NULL;
PFNGLSECONDARYCOLOR3SPROC glad_glSecondaryColor3s = NULL;
PFNGLSECONDARYCOLOR3SVPROC glad_glSecondaryColor3sv = NULL;
PFNGLSECONDARYCOLOR3UBPROC glad_glSecondaryColor3ub = NULL;
PFNGLSECONDARYCOLOR3UBVPROC glad_glSecondaryColor3ubv = NULL;
PFNGLSECONDARYCOLOR3UIPROC glad_glSecondaryColor3ui = NULL;
PFNGLSECONDARYCOLOR3UIVPROC glad_glSecondaryColor3uiv = NULL;
PFNGLSECONDARYCOLOR3USPROC glad_glSecondaryColor3us = NULL;
PFNGLSECONDARYCOLOR3USVPROC glad_glSecondaryColor3usv = NULL;
PFNGLSECONDARYCOLORP3UIPROC glad_glSecondaryColorP3ui = NULL;
PFNGLSECONDARYCOLORP3UIVPROC glad_glSecondaryColorP3uiv = NULL;
PFNGLSECONDARYCOLORPOINTERPROC glad_glSecondaryColorPointer = NULL;
PFNGLSELECTBUFFERPROC glad_glSelectBuffer = NULL;
PFNGLSEPARABLEFILTER2DPROC glad_glSeparableFilter2D = NULL;
PFNGLSHADEMODELPROC glad_glShadeModel = NULL;
PFNGLSHADERBINARYPROC glad_glShaderBinary = NULL;
PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
PFNGLSHADERSOURCEARBPROC glad_glShaderSourceARB = NULL;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC glad_glShaderStorageBlockBinding = NULL;
PFNGLSPECIALIZESHADERPROC glad_glSpecializeShader = NULL;
PFNGLSPECIALIZESHADERARBPROC glad_glSpecializeShaderARB = NULL;
PFNGLSTENCILFUNCPROC glad_glStencilFunc = NULL;
PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate = NULL;
PFNGLSTENCILMASKPROC glad_glStencilMask = NULL;
PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate = NULL;
PFNGLSTENCILOPPROC glad_glStencilOp = NULL;
PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate = NULL;
PFNGLTEXBUFFERPROC glad_glTexBuffer = NULL;
PFNGLTEXBUFFERARBPROC glad_glTexBufferARB = NULL;
PFNGLTEXBUFFERRANGEPROC glad_glTexBufferRange = NULL;
PFNGLTEXCOORD1DPROC glad_glTexCoord1d = NULL;
PFNGLTEXCOORD1DVPROC glad_glTexCoord1dv = NULL;
PFNGLTEXCOORD1FPROC glad_glTexCoord1f = NULL;
PFNGLTEXCOORD1FVPROC glad_glTexCoord1fv = NULL;
PFNGLTEXCOORD1IPROC glad_glTexCoord1i = NULL;
PFNGLTEXCOORD1IVPROC glad_glTexCoord1iv = NULL;
PFNGLTEXCOORD1SPROC glad_glTexCoord1s = NULL;
PFNGLTEXCOORD1SVPROC glad_glTexCoord1sv = NULL;
PFNGLTEXCOORD2DPROC glad_glTexCoord2d = NULL;
PFNGLTEXCOORD2DVPROC glad_glTexCoord2dv = NULL;
PFNGLTEXCOORD2FPROC glad_glTexCoord2f = NULL;
PFNGLTEXCOORD2FVPROC glad_glTexCoord2fv = NULL;
PFNGLTEXCOORD2IPROC glad_glTexCoord2i = NULL;
PFNGLTEXCOORD2IVPROC glad_glTexCoord2iv = NULL;
PFNGLTEXCOORD2SPROC glad_glTexCoord2s = NULL;
PFNGLTEXCOORD2SVPROC glad_glTexCoord2sv = NULL;
PFNGLTEXCOORD3DPROC glad_glTexCoord3d = NULL;
PFNGLTEXCOORD3DVPROC glad_glTexCoord3dv = NULL;
PFNGLTEXCOORD3FPROC glad_glTexCoord3f = NULL;
PFNGLTEXCOORD3FVPROC glad_glTexCoord3fv = NULL;
PFNGLTEXCOORD3IPROC glad_glTexCoord3i = NULL;
PFNGLTEXCOORD3IVPROC glad_glTexCoord3iv = NULL;
PFNGLTEXCOORD3SPROC glad_glTexCoord3s = NULL;
PFNGLTEXCOORD3SVPROC glad_glTexCoord3sv = NULL;
PFNGLTEXCOORD4DPROC glad_glTexCoord4d = NULL;
PFNGLTEXCOORD4DVPROC glad_glTexCoord4dv = NULL;
PFNGLTEXCOORD4FPROC glad_glTexCoord4f = NULL;
PFNGLTEXCOORD4FVPROC glad_glTexCoord4fv = NULL;
PFNGLTEXCOORD4IPROC glad_glTexCoord4i = NULL;
PFNGLTEXCOORD4IVPROC glad_glTexCoord4iv = NULL;
PFNGLTEXCOORD4SPROC glad_glTexCoord4s = NULL;
PFNGLTEXCOORD4SVPROC glad_glTexCoord4sv = NULL;
PFNGLTEXCOORDP1UIPROC glad_glTexCoordP1ui = NULL;
PFNGLTEXCOORDP1UIVPROC glad_glTexCoordP1uiv = NULL;
PFNGLTEXCOORDP2UIPROC glad_glTexCoordP2ui = NULL;
PFNGLTEXCOORDP2UIVPROC glad_glTexCoordP2uiv = NULL;
PFNGLTEXCOORDP3UIPROC glad_glTexCoordP3ui = NULL;
PFNGLTEXCOORDP3UIVPROC glad_glTexCoordP3uiv = NULL;
PFNGLTEXCOORDP4UIPROC glad_glTexCoordP4ui = NULL;
PFNGLTEXCOORDP4UIVPROC glad_glTexCoordP4uiv = NULL;
PFNGLTEXCOORDPOINTERPROC glad_glTexCoordPointer = NULL;
PFNGLTEXENVFPROC glad_glTexEnvf = NULL;
PFNGLTEXENVFVPROC glad_glTexEnvfv = NULL;
PFNGLTEXENVIPROC glad_glTexEnvi = NULL;
PFNGLTEXENVIVPROC glad_glTexEnviv = NULL;
PFNGLTEXGENDPROC glad_glTexGend = NULL;
PFNGLTEXGENDVPROC glad_glTexGendv = NULL;
PFNGLTEXGENFPROC glad_glTexGenf = NULL;
PFNGLTEXGENFVPROC glad_glTexGenfv = NULL;
PFNGLTEXGENIPROC glad_glTexGeni = NULL;
PFNGLTEXGENIVPROC glad_glTexGeniv = NULL;
PFNGLTEXIMAGE1DPROC glad_glTexImage1D = NULL;
PFNGLTEXIMAGE2DPROC glad_glTexImage2D = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DPROC glad_glTexImage3D = NULL;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample = NULL;
PFNGLTEXPAGECOMMITMENTARBPROC glad_glTexPageCommitmentARB = NULL;
PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv = NULL;
PFNGLTEXPARAMETERFPROC glad_glTexParameterf = NULL;
PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv = NULL;
PFNGLTEXPARAMETERIPROC glad_glTexParameteri = NULL;
PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv = NULL;
PFNGLTEXSTORAGE1DPROC glad_glTexStorage1D = NULL;
PFNGLTEXSTORAGE2DPROC glad_glTexStorage2D = NULL;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC glad_glTexStorage2DMultisample = NULL;
PFNGLTEXSTORAGE3DPROC glad_glTexStorage3D = NULL;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC glad_glTexStorage3DMultisample = NULL;
PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D = NULL;
PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D = NULL;
PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D = NULL;
PFNGLTEXTUREBARRIERPROC glad_glTextureBarrier = NULL;
PFNGLTEXTUREBUFFERPROC glad_glTextureBuffer = NULL;
PFNGLTEXTUREBUFFERRANGEPROC glad_glTextureBufferRange = NULL;
PFNGLTEXTUREPARAMETERIIVPROC glad_glTextureParameterIiv = NULL;
PFNGLTEXTUREPARAMETERIUIVPROC glad_glTextureParameterIuiv = NULL;
PFNGLTEXTUREPARAMETERFPROC glad_glTextureParameterf = NULL;
PFNGLTEXTUREPARAMETERFVPROC glad_glTextureParameterfv = NULL;
PFNGLTEXTUREPARAMETERIPROC glad_glTextureParameteri = NULL;
PFNGLTEXTUREPARAMETERIVPROC glad_glTextureParameteriv = NULL;
PFNGLTEXTURESTORAGE1DPROC glad_glTextureStorage1D = NULL;
PFNGLTEXTURESTORAGE2DPROC glad_glTextureStorage2D = NULL;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC glad_glTextureStorage2DMultisample = NULL;
PFNGLTEXTURESTORAGE3DPROC glad_glTextureStorage3D = NULL;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC glad_glTextureStorage3DMultisample = NULL;
PFNGLTEXTURESUBIMAGE1DPROC glad_glTextureSubImage1D = NULL;
PFNGLTEXTURESUBIMAGE2DPROC glad_glTextureSubImage2D = NULL;
PFNGLTEXTURESUBIMAGE3DPROC glad_glTextureSubImage3D = NULL;
PFNGLTEXTUREVIEWPROC glad_glTextureView = NULL;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glad_glTransformFeedbackBufferBase = NULL;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glad_glTransformFeedbackBufferRange = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings = NULL;
PFNGLTRANSLATEDPROC glad_glTranslated = NULL;
PFNGLTRANSLATEFPROC glad_glTranslatef = NULL;
PFNGLUNIFORM1DPROC glad_glUniform1d = NULL;
PFNGLUNIFORM1DVPROC glad_glUniform1dv = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM1FARBPROC glad_glUniform1fARB = NULL;
PFNGLUNIFORM1FVPROC glad_glUniform1fv = NULL;
PFNGLUNIFORM1FVARBPROC glad_glUniform1fvARB = NULL;
PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
PFNGLUNIFORM1I64ARBPROC glad_glUniform1i64ARB = NULL;
PFNGLUNIFORM1I64VARBPROC glad_glUniform1i64vARB = NULL;
PFNGLUNIFORM1IARBPROC glad_glUniform1iARB = NULL;
PFNGLUNIFORM1IVPROC glad_glUniform1iv = NULL;
PFNGLUNIFORM1IVARBPROC glad_glUniform1ivARB = NULL;
PFNGLUNIFORM1UIPROC glad_glUniform1ui = NULL;
PFNGLUNIFORM1UI64ARBPROC glad_glUniform1ui64ARB = NULL;
PFNGLUNIFORM1UI64VARBPROC glad_glUniform1ui64vARB = NULL;
PFNGLUNIFORM1UIVPROC glad_glUniform1uiv = NULL;
PFNGLUNIFORM2DPROC glad_glUniform2d = NULL;
PFNGLUNIFORM2DVPROC glad_glUniform2dv = NULL;
PFNGLUNIFORM2FPROC glad_glUniform2f = NULL;
PFNGLUNIFORM2FARBPROC glad_glUniform2fARB = NULL;
PFNGLUNIFORM2FVPROC glad_glUniform2fv = NULL;
PFNGLUNIFORM2FVARBPROC glad_glUniform2fvARB = NULL;
PFNGLUNIFORM2IPROC glad_glUniform2i = NULL;
PFNGLUNIFORM2I64ARBPROC glad_glUniform2i64ARB = NULL;
PFNGLUNIFORM2I64VARBPROC glad_glUniform2i64vARB = NULL;
PFNGLUNIFORM2IARBPROC glad_glUniform2iARB = NULL;
PFNGLUNIFORM2IVPROC glad_glUniform2iv = NULL;
PFNGLUNIFORM2IVARBPROC glad_glUniform2ivARB = NULL;
PFNGLUNIFORM2UIPROC glad_glUniform2ui = NULL;
PFNGLUNIFORM2UI64ARBPROC glad_glUniform2ui64ARB = NULL;
PFNGLUNIFORM2UI64VARBPROC glad_glUniform2ui64vARB = NULL;
PFNGLUNIFORM2UIVPROC glad_glUniform2uiv = NULL;
PFNGLUNIFORM3DPROC glad_glUniform3d = NULL;
PFNGLUNIFORM3DVPROC glad_glUniform3dv = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM3FARBPROC glad_glUniform3fARB = NULL;
PFNGLUNIFORM3FVPROC glad_glUniform3fv = NULL;
PFNGLUNIFORM3FVARBPROC glad_glUniform3fvARB = NULL;
PFNGLUNIFORM3IPROC glad_glUniform3i = NULL;
PFNGLUNIFORM3I64ARBPROC glad_glUniform3i64ARB = NULL;
PFNGLUNIFORM3I64VARBPROC glad_glUniform3i64vARB = NULL;
PFNGLUNIFORM3IARBPROC glad_glUniform3iARB = NULL;
PFNGLUNIFORM3IVPROC glad_glUniform3iv = NULL;
PFNGLUNIFORM3IVARBPROC glad_glUniform3ivARB = NULL;
PFNGLUNIFORM3UIPROC glad_glUniform3ui = NULL;
PFNGLUNIFORM3UI64ARBPROC glad_glUniform3ui64ARB = NULL;
PFNGLUNIFORM3UI64VARBPROC glad_glUniform3ui64vARB = NULL;
PFNGLUNIFORM3UIVPROC glad_glUniform3uiv = NULL;
PFNGLUNIFORM4DPROC glad_glUniform4d = NULL;
PFNGLUNIFORM4DVPROC glad_glUniform4dv = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
PFNGLUNIFORM4FARBPROC glad_glUniform4fARB = NULL;
PFNGLUNIFORM4FVPROC glad_glUniform4fv = NULL;
PFNGLUNIFORM4FVARBPROC glad_glUniform4fvARB = NULL;
PFNGLUNIFORM4IPROC glad_glUniform4i = NULL;
PFNGLUNIFORM4I64ARBPROC glad_glUniform4i64ARB = NULL;
PFNGLUNIFORM4I64VARBPROC glad_glUniform4i64vARB = NULL;
PFNGLUNIFORM4IARBPROC glad_glUniform4iARB = NULL;
PFNGLUNIFORM4IVPROC glad_glUniform4iv = NULL;
PFNGLUNIFORM4IVARBPROC glad_glUniform4ivARB = NULL;
PFNGLUNIFORM4UIPROC glad_glUniform4ui = NULL;
PFNGLUNIFORM4UI64ARBPROC glad_glUniform4ui64ARB = NULL;
PFNGLUNIFORM4UI64VARBPROC glad_glUniform4ui64vARB = NULL;
PFNGLUNIFORM4UIVPROC glad_glUniform4uiv = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding = NULL;
PFNGLUNIFORMHANDLEUI64ARBPROC glad_glUniformHandleui64ARB = NULL;
PFNGLUNIFORMHANDLEUI64VARBPROC glad_glUniformHandleui64vARB = NULL;
PFNGLUNIFORMMATRIX2DVPROC glad_glUniformMatrix2dv = NULL;
PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC glad_glUniformMatrix2fvARB = NULL;
PFNGLUNIFORMMATRIX2X3DVPROC glad_glUniformMatrix2x3dv = NULL;
PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX2X4DVPROC glad_glUniformMatrix2x4dv = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX3DVPROC glad_glUniformMatrix3dv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC glad_glUniformMatrix3fvARB = NULL;
PFNGLUNIFORMMATRIX3X2DVPROC glad_glUniformMatrix3x2dv = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX3X4DVPROC glad_glUniformMatrix3x4dv = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4DVPROC glad_glUniformMatrix4dv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC glad_glUniformMatrix4fvARB = NULL;
PFNGLUNIFORMMATRIX4X2DVPROC glad_glUniformMatrix4x2dv = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX4X3DVPROC glad_glUniformMatrix4x3dv = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv = NULL;
PFNGLUNIFORMSUBROUTINESUIVPROC glad_glUniformSubroutinesuiv = NULL;
PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer = NULL;
PFNGLUNMAPBUFFERARBPROC glad_glUnmapBufferARB = NULL;
PFNGLUNMAPNAMEDBUFFERPROC glad_glUnmapNamedBuffer = NULL;
PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glad_glUseProgramObjectARB = NULL;
PFNGLUSEPROGRAMSTAGESPROC glad_glUseProgramStages = NULL;
PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram = NULL;
PFNGLVALIDATEPROGRAMARBPROC glad_glValidateProgramARB = NULL;
PFNGLVALIDATEPROGRAMPIPELINEPROC glad_glValidateProgramPipeline = NULL;
PFNGLVERTEX2DPROC glad_glVertex2d = NULL;
PFNGLVERTEX2DVPROC glad_glVertex2dv = NULL;
PFNGLVERTEX2FPROC glad_glVertex2f = NULL;
PFNGLVERTEX2FVPROC glad_glVertex2fv = NULL;
PFNGLVERTEX2IPROC glad_glVertex2i = NULL;
PFNGLVERTEX2IVPROC glad_glVertex2iv = NULL;
PFNGLVERTEX2SPROC glad_glVertex2s = NULL;
PFNGLVERTEX2SVPROC glad_glVertex2sv = NULL;
PFNGLVERTEX3DPROC glad_glVertex3d = NULL;
PFNGLVERTEX3DVPROC glad_glVertex3dv = NULL;
PFNGLVERTEX3FPROC glad_glVertex3f = NULL;
PFNGLVERTEX3FVPROC glad_glVertex3fv = NULL;
PFNGLVERTEX3IPROC glad_glVertex3i = NULL;
PFNGLVERTEX3IVPROC glad_glVertex3iv = NULL;
PFNGLVERTEX3SPROC glad_glVertex3s = NULL;
PFNGLVERTEX3SVPROC glad_glVertex3sv = NULL;
PFNGLVERTEX4DPROC glad_glVertex4d = NULL;
PFNGLVERTEX4DVPROC glad_glVertex4dv = NULL;
PFNGLVERTEX4FPROC glad_glVertex4f = NULL;
PFNGLVERTEX4FVPROC glad_glVertex4fv = NULL;
PFNGLVERTEX4IPROC glad_glVertex4i = NULL;
PFNGLVERTEX4IVPROC glad_glVertex4iv = NULL;
PFNGLVERTEX4SPROC glad_glVertex4s = NULL;
PFNGLVERTEX4SVPROC glad_glVertex4sv = NULL;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glad_glVertexArrayAttribBinding = NULL;
PFNGLVERTEXARRAYATTRIBFORMATPROC glad_glVertexArrayAttribFormat = NULL;
PFNGLVERTEXARRAYATTRIBIFORMATPROC glad_glVertexArrayAttribIFormat = NULL;
PFNGLVERTEXARRAYATTRIBLFORMATPROC glad_glVertexArrayAttribLFormat = NULL;
PFNGLVERTEXARRAYBINDINGDIVISORPROC glad_glVertexArrayBindingDivisor = NULL;
PFNGLVERTEXARRAYELEMENTBUFFERPROC glad_glVertexArrayElementBuffer = NULL;
PFNGLVERTEXARRAYVERTEXBUFFERPROC glad_glVertexArrayVertexBuffer = NULL;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC glad_glVertexArrayVertexBuffers = NULL;
PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DARBPROC glad_glVertexAttrib1dARB = NULL;
PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1DVARBPROC glad_glVertexAttrib1dvARB = NULL;
PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FARBPROC glad_glVertexAttrib1fARB = NULL;
PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1FVARBPROC glad_glVertexAttrib1fvARB = NULL;
PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SARBPROC glad_glVertexAttrib1sARB = NULL;
PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB1SVARBPROC glad_glVertexAttrib1svARB = NULL;
PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DARBPROC glad_glVertexAttrib2dARB = NULL;
PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2DVARBPROC glad_glVertexAttrib2dvARB = NULL;
PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FARBPROC glad_glVertexAttrib2fARB = NULL;
PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2FVARBPROC glad_glVertexAttrib2fvARB = NULL;
PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SARBPROC glad_glVertexAttrib2sARB = NULL;
PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB2SVARBPROC glad_glVertexAttrib2svARB = NULL;
PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DARBPROC glad_glVertexAttrib3dARB = NULL;
PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3DVARBPROC glad_glVertexAttrib3dvARB = NULL;
PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FARBPROC glad_glVertexAttrib3fARB = NULL;
PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3FVARBPROC glad_glVertexAttrib3fvARB = NULL;
PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SARBPROC glad_glVertexAttrib3sARB = NULL;
PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB3SVARBPROC glad_glVertexAttrib3svARB = NULL;
PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NBVARBPROC glad_glVertexAttrib4NbvARB = NULL;
PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NIVARBPROC glad_glVertexAttrib4NivARB = NULL;
PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NSVARBPROC glad_glVertexAttrib4NsvARB = NULL;
PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBARBPROC glad_glVertexAttrib4NubARB = NULL;
PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC glad_glVertexAttrib4NubvARB = NULL;
PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC glad_glVertexAttrib4NuivARB = NULL;
PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC glad_glVertexAttrib4NusvARB = NULL;
PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4BVARBPROC glad_glVertexAttrib4bvARB = NULL;
PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DARBPROC glad_glVertexAttrib4dARB = NULL;
PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4DVARBPROC glad_glVertexAttrib4dvARB = NULL;
PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FARBPROC glad_glVertexAttrib4fARB = NULL;
PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4FVARBPROC glad_glVertexAttrib4fvARB = NULL;
PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4IVARBPROC glad_glVertexAttrib4ivARB = NULL;
PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SARBPROC glad_glVertexAttrib4sARB = NULL;
PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4SVARBPROC glad_glVertexAttrib4svARB = NULL;
PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UBVARBPROC glad_glVertexAttrib4ubvARB = NULL;
PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4UIVARBPROC glad_glVertexAttrib4uivARB = NULL;
PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIB4USVARBPROC glad_glVertexAttrib4usvARB = NULL;
PFNGLVERTEXATTRIBBINDINGPROC glad_glVertexAttribBinding = NULL;
PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor = NULL;
PFNGLVERTEXATTRIBDIVISORARBPROC glad_glVertexAttribDivisorARB = NULL;
PFNGLVERTEXATTRIBFORMATPROC glad_glVertexAttribFormat = NULL;
PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv = NULL;
PFNGLVERTEXATTRIBIFORMATPROC glad_glVertexAttribIFormat = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer = NULL;
PFNGLVERTEXATTRIBL1DPROC glad_glVertexAttribL1d = NULL;
PFNGLVERTEXATTRIBL1DVPROC glad_glVertexAttribL1dv = NULL;
PFNGLVERTEXATTRIBL1UI64ARBPROC glad_glVertexAttribL1ui64ARB = NULL;
PFNGLVERTEXATTRIBL1UI64VARBPROC glad_glVertexAttribL1ui64vARB = NULL;
PFNGLVERTEXATTRIBL2DPROC glad_glVertexAttribL2d = NULL;
PFNGLVERTEXATTRIBL2DVPROC glad_glVertexAttribL2dv = NULL;
PFNGLVERTEXATTRIBL3DPROC glad_glVertexAttribL3d = NULL;
PFNGLVERTEXATTRIBL3DVPROC glad_glVertexAttribL3dv = NULL;
PFNGLVERTEXATTRIBL4DPROC glad_glVertexAttribL4d = NULL;
PFNGLVERTEXATTRIBL4DVPROC glad_glVertexAttribL4dv = NULL;
PFNGLVERTEXATTRIBLFORMATPROC glad_glVertexAttribLFormat = NULL;
PFNGLVERTEXATTRIBLPOINTERPROC glad_glVertexAttribLPointer = NULL;
PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC glad_glVertexAttribPointerARB = NULL;
PFNGLVERTEXBINDINGDIVISORPROC glad_glVertexBindingDivisor = NULL;
PFNGLVERTEXBLENDARBPROC glad_glVertexBlendARB = NULL;
PFNGLVERTEXP2UIPROC glad_glVertexP2ui = NULL;
PFNGLVERTEXP2UIVPROC glad_glVertexP2uiv = NULL;
PFNGLVERTEXP3UIPROC glad_glVertexP3ui = NULL;
PFNGLVERTEXP3UIVPROC glad_glVertexP3uiv = NULL;
PFNGLVERTEXP4UIPROC glad_glVertexP4ui = NULL;
PFNGLVERTEXP4UIVPROC glad_glVertexP4uiv = NULL;
PFNGLVERTEXPOINTERPROC glad_glVertexPointer = NULL;
PFNGLVIEWPORTPROC glad_glViewport = NULL;
PFNGLVIEWPORTARRAYVPROC glad_glViewportArrayv = NULL;
PFNGLVIEWPORTINDEXEDFPROC glad_glViewportIndexedf = NULL;
PFNGLVIEWPORTINDEXEDFVPROC glad_glViewportIndexedfv = NULL;
PFNGLWAITSYNCPROC glad_glWaitSync = NULL;
PFNGLWEIGHTPOINTERARBPROC glad_glWeightPointerARB = NULL;
PFNGLWEIGHTBVARBPROC glad_glWeightbvARB = NULL;
PFNGLWEIGHTDVARBPROC glad_glWeightdvARB = NULL;
PFNGLWEIGHTFVARBPROC glad_glWeightfvARB = NULL;
PFNGLWEIGHTIVARBPROC glad_glWeightivARB = NULL;
PFNGLWEIGHTSVARBPROC glad_glWeightsvARB = NULL;
PFNGLWEIGHTUBVARBPROC glad_glWeightubvARB = NULL;
PFNGLWEIGHTUIVARBPROC glad_glWeightuivARB = NULL;
PFNGLWEIGHTUSVARBPROC glad_glWeightusvARB = NULL;
PFNGLWINDOWPOS2DPROC glad_glWindowPos2d = NULL;
PFNGLWINDOWPOS2DARBPROC glad_glWindowPos2dARB = NULL;
PFNGLWINDOWPOS2DVPROC glad_glWindowPos2dv = NULL;
PFNGLWINDOWPOS2DVARBPROC glad_glWindowPos2dvARB = NULL;
PFNGLWINDOWPOS2FPROC glad_glWindowPos2f = NULL;
PFNGLWINDOWPOS2FARBPROC glad_glWindowPos2fARB = NULL;
PFNGLWINDOWPOS2FVPROC glad_glWindowPos2fv = NULL;
PFNGLWINDOWPOS2FVARBPROC glad_glWindowPos2fvARB = NULL;
PFNGLWINDOWPOS2IPROC glad_glWindowPos2i = NULL;
PFNGLWINDOWPOS2IARBPROC glad_glWindowPos2iARB = NULL;
PFNGLWINDOWPOS2IVPROC glad_glWindowPos2iv = NULL;
PFNGLWINDOWPOS2IVARBPROC glad_glWindowPos2ivARB = NULL;
PFNGLWINDOWPOS2SPROC glad_glWindowPos2s = NULL;
PFNGLWINDOWPOS2SARBPROC glad_glWindowPos2sARB = NULL;
PFNGLWINDOWPOS2SVPROC glad_glWindowPos2sv = NULL;
PFNGLWINDOWPOS2SVARBPROC glad_glWindowPos2svARB = NULL;
PFNGLWINDOWPOS3DPROC glad_glWindowPos3d = NULL;
PFNGLWINDOWPOS3DARBPROC glad_glWindowPos3dARB = NULL;
PFNGLWINDOWPOS3DVPROC glad_glWindowPos3dv = NULL;
PFNGLWINDOWPOS3DVARBPROC glad_glWindowPos3dvARB = NULL;
PFNGLWINDOWPOS3FPROC glad_glWindowPos3f = NULL;
PFNGLWINDOWPOS3FARBPROC glad_glWindowPos3fARB = NULL;
PFNGLWINDOWPOS3FVPROC glad_glWindowPos3fv = NULL;
PFNGLWINDOWPOS3FVARBPROC glad_glWindowPos3fvARB = NULL;
PFNGLWINDOWPOS3IPROC glad_glWindowPos3i = NULL;
PFNGLWINDOWPOS3IARBPROC glad_glWindowPos3iARB = NULL;
PFNGLWINDOWPOS3IVPROC glad_glWindowPos3iv = NULL;
PFNGLWINDOWPOS3IVARBPROC glad_glWindowPos3ivARB = NULL;
PFNGLWINDOWPOS3SPROC glad_glWindowPos3s = NULL;
PFNGLWINDOWPOS3SARBPROC glad_glWindowPos3sARB = NULL;
PFNGLWINDOWPOS3SVPROC glad_glWindowPos3sv = NULL;
PFNGLWINDOWPOS3SVARBPROC glad_glWindowPos3svARB = NULL;


static void glad_gl_load_GL_VERSION_1_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_0) return;
    glad_glAccum = (PFNGLACCUMPROC) load(userptr, "glAccum");
    glad_glAlphaFunc = (PFNGLALPHAFUNCPROC) load(userptr, "glAlphaFunc");
    glad_glBegin = (PFNGLBEGINPROC) load(userptr, "glBegin");
    glad_glBitmap = (PFNGLBITMAPPROC) load(userptr, "glBitmap");
    glad_glBlendFunc = (PFNGLBLENDFUNCPROC) load(userptr, "glBlendFunc");
    glad_glCallList = (PFNGLCALLLISTPROC) load(userptr, "glCallList");
    glad_glCallLists = (PFNGLCALLLISTSPROC) load(userptr, "glCallLists");
    glad_glClear = (PFNGLCLEARPROC) load(userptr, "glClear");
    glad_glClearAccum = (PFNGLCLEARACCUMPROC) load(userptr, "glClearAccum");
    glad_glClearColor = (PFNGLCLEARCOLORPROC) load(userptr, "glClearColor");
    glad_glClearDepth = (PFNGLCLEARDEPTHPROC) load(userptr, "glClearDepth");
    glad_glClearIndex = (PFNGLCLEARINDEXPROC) load(userptr, "glClearIndex");
    glad_glClearStencil = (PFNGLCLEARSTENCILPROC) load(userptr, "glClearStencil");
    glad_glClipPlane = (PFNGLCLIPPLANEPROC) load(userptr, "glClipPlane");
    glad_glColor3b = (PFNGLCOLOR3BPROC) load(userptr, "glColor3b");
    glad_glColor3bv = (PFNGLCOLOR3BVPROC) load(userptr, "glColor3bv");
    glad_glColor3d = (PFNGLCOLOR3DPROC) load(userptr, "glColor3d");
    glad_glColor3dv = (PFNGLCOLOR3DVPROC) load(userptr, "glColor3dv");
    glad_glColor3f = (PFNGLCOLOR3FPROC) load(userptr, "glColor3f");
    glad_glColor3fv = (PFNGLCOLOR3FVPROC) load(userptr, "glColor3fv");
    glad_glColor3i = (PFNGLCOLOR3IPROC) load(userptr, "glColor3i");
    glad_glColor3iv = (PFNGLCOLOR3IVPROC) load(userptr, "glColor3iv");
    glad_glColor3s = (PFNGLCOLOR3SPROC) load(userptr, "glColor3s");
    glad_glColor3sv = (PFNGLCOLOR3SVPROC) load(userptr, "glColor3sv");
    glad_glColor3ub = (PFNGLCOLOR3UBPROC) load(userptr, "glColor3ub");
    glad_glColor3ubv = (PFNGLCOLOR3UBVPROC) load(userptr, "glColor3ubv");
    glad_glColor3ui = (PFNGLCOLOR3UIPROC) load(userptr, "glColor3ui");
    glad_glColor3uiv = (PFNGLCOLOR3UIVPROC) load(userptr, "glColor3uiv");
    glad_glColor3us = (PFNGLCOLOR3USPROC) load(userptr, "glColor3us");
    glad_glColor3usv = (PFNGLCOLOR3USVPROC) load(userptr, "glColor3usv");
    glad_glColor4b = (PFNGLCOLOR4BPROC) load(userptr, "glColor4b");
    glad_glColor4bv = (PFNGLCOLOR4BVPROC) load(userptr, "glColor4bv");
    glad_glColor4d = (PFNGLCOLOR4DPROC) load(userptr, "glColor4d");
    glad_glColor4dv = (PFNGLCOLOR4DVPROC) load(userptr, "glColor4dv");
    glad_glColor4f = (PFNGLCOLOR4FPROC) load(userptr, "glColor4f");
    glad_glColor4fv = (PFNGLCOLOR4FVPROC) load(userptr, "glColor4fv");
    glad_glColor4i = (PFNGLCOLOR4IPROC) load(userptr, "glColor4i");
    glad_glColor4iv = (PFNGLCOLOR4IVPROC) load(userptr, "glColor4iv");
    glad_glColor4s = (PFNGLCOLOR4SPROC) load(userptr, "glColor4s");
    glad_glColor4sv = (PFNGLCOLOR4SVPROC) load(userptr, "glColor4sv");
    glad_glColor4ub = (PFNGLCOLOR4UBPROC) load(userptr, "glColor4ub");
    glad_glColor4ubv = (PFNGLCOLOR4UBVPROC) load(userptr, "glColor4ubv");
    glad_glColor4ui = (PFNGLCOLOR4UIPROC) load(userptr, "glColor4ui");
    glad_glColor4uiv = (PFNGLCOLOR4UIVPROC) load(userptr, "glColor4uiv");
    glad_glColor4us = (PFNGLCOLOR4USPROC) load(userptr, "glColor4us");
    glad_glColor4usv = (PFNGLCOLOR4USVPROC) load(userptr, "glColor4usv");
    glad_glColorMask = (PFNGLCOLORMASKPROC) load(userptr, "glColorMask");
    glad_glColorMaterial = (PFNGLCOLORMATERIALPROC) load(userptr, "glColorMaterial");
    glad_glCopyPixels = (PFNGLCOPYPIXELSPROC) load(userptr, "glCopyPixels");
    glad_glCullFace = (PFNGLCULLFACEPROC) load(userptr, "glCullFace");
    glad_glDeleteLists = (PFNGLDELETELISTSPROC) load(userptr, "glDeleteLists");
    glad_glDepthFunc = (PFNGLDEPTHFUNCPROC) load(userptr, "glDepthFunc");
    glad_glDepthMask = (PFNGLDEPTHMASKPROC) load(userptr, "glDepthMask");
    glad_glDepthRange = (PFNGLDEPTHRANGEPROC) load(userptr, "glDepthRange");
    glad_glDisable = (PFNGLDISABLEPROC) load(userptr, "glDisable");
    glad_glDrawBuffer = (PFNGLDRAWBUFFERPROC) load(userptr, "glDrawBuffer");
    glad_glDrawPixels = (PFNGLDRAWPIXELSPROC) load(userptr, "glDrawPixels");
    glad_glEdgeFlag = (PFNGLEDGEFLAGPROC) load(userptr, "glEdgeFlag");
    glad_glEdgeFlagv = (PFNGLEDGEFLAGVPROC) load(userptr, "glEdgeFlagv");
    glad_glEnable = (PFNGLENABLEPROC) load(userptr, "glEnable");
    glad_glEnd = (PFNGLENDPROC) load(userptr, "glEnd");
    glad_glEndList = (PFNGLENDLISTPROC) load(userptr, "glEndList");
    glad_glEvalCoord1d = (PFNGLEVALCOORD1DPROC) load(userptr, "glEvalCoord1d");
    glad_glEvalCoord1dv = (PFNGLEVALCOORD1DVPROC) load(userptr, "glEvalCoord1dv");
    glad_glEvalCoord1f = (PFNGLEVALCOORD1FPROC) load(userptr, "glEvalCoord1f");
    glad_glEvalCoord1fv = (PFNGLEVALCOORD1FVPROC) load(userptr, "glEvalCoord1fv");
    glad_glEvalCoord2d = (PFNGLEVALCOORD2DPROC) load(userptr, "glEvalCoord2d");
    glad_glEvalCoord2dv = (PFNGLEVALCOORD2DVPROC) load(userptr, "glEvalCoord2dv");
    glad_glEvalCoord2f = (PFNGLEVALCOORD2FPROC) load(userptr, "glEvalCoord2f");
    glad_glEvalCoord2fv = (PFNGLEVALCOORD2FVPROC) load(userptr, "glEvalCoord2fv");
    glad_glEvalMesh1 = (PFNGLEVALMESH1PROC) load(userptr, "glEvalMesh1");
    glad_glEvalMesh2 = (PFNGLEVALMESH2PROC) load(userptr, "glEvalMesh2");
    glad_glEvalPoint1 = (PFNGLEVALPOINT1PROC) load(userptr, "glEvalPoint1");
    glad_glEvalPoint2 = (PFNGLEVALPOINT2PROC) load(userptr, "glEvalPoint2");
    glad_glFeedbackBuffer = (PFNGLFEEDBACKBUFFERPROC) load(userptr, "glFeedbackBuffer");
    glad_glFinish = (PFNGLFINISHPROC) load(userptr, "glFinish");
    glad_glFlush = (PFNGLFLUSHPROC) load(userptr, "glFlush");
    glad_glFogf = (PFNGLFOGFPROC) load(userptr, "glFogf");
    glad_glFogfv = (PFNGLFOGFVPROC) load(userptr, "glFogfv");
    glad_glFogi = (PFNGLFOGIPROC) load(userptr, "glFogi");
    glad_glFogiv = (PFNGLFOGIVPROC) load(userptr, "glFogiv");
    glad_glFrontFace = (PFNGLFRONTFACEPROC) load(userptr, "glFrontFace");
    glad_glFrustum = (PFNGLFRUSTUMPROC) load(userptr, "glFrustum");
    glad_glGenLists = (PFNGLGENLISTSPROC) load(userptr, "glGenLists");
    glad_glGetBooleanv = (PFNGLGETBOOLEANVPROC) load(userptr, "glGetBooleanv");
    glad_glGetClipPlane = (PFNGLGETCLIPPLANEPROC) load(userptr, "glGetClipPlane");
    glad_glGetDoublev = (PFNGLGETDOUBLEVPROC) load(userptr, "glGetDoublev");
    glad_glGetError = (PFNGLGETERRORPROC) load(userptr, "glGetError");
    glad_glGetFloatv = (PFNGLGETFLOATVPROC) load(userptr, "glGetFloatv");
    glad_glGetIntegerv = (PFNGLGETINTEGERVPROC) load(userptr, "glGetIntegerv");
    glad_glGetLightfv = (PFNGLGETLIGHTFVPROC) load(userptr, "glGetLightfv");
    glad_glGetLightiv = (PFNGLGETLIGHTIVPROC) load(userptr, "glGetLightiv");
    glad_glGetMapdv = (PFNGLGETMAPDVPROC) load(userptr, "glGetMapdv");
    glad_glGetMapfv = (PFNGLGETMAPFVPROC) load(userptr, "glGetMapfv");
    glad_glGetMapiv = (PFNGLGETMAPIVPROC) load(userptr, "glGetMapiv");
    glad_glGetMaterialfv = (PFNGLGETMATERIALFVPROC) load(userptr, "glGetMaterialfv");
    glad_glGetMaterialiv = (PFNGLGETMATERIALIVPROC) load(userptr, "glGetMaterialiv");
    glad_glGetPixelMapfv = (PFNGLGETPIXELMAPFVPROC) load(userptr, "glGetPixelMapfv");
    glad_glGetPixelMapuiv = (PFNGLGETPIXELMAPUIVPROC) load(userptr, "glGetPixelMapuiv");
    glad_glGetPixelMapusv = (PFNGLGETPIXELMAPUSVPROC) load(userptr, "glGetPixelMapusv");
    glad_glGetPolygonStipple = (PFNGLGETPOLYGONSTIPPLEPROC) load(userptr, "glGetPolygonStipple");
    glad_glGetString = (PFNGLGETSTRINGPROC) load(userptr, "glGetString");
    glad_glGetTexEnvfv = (PFNGLGETTEXENVFVPROC) load(userptr, "glGetTexEnvfv");
    glad_glGetTexEnviv = (PFNGLGETTEXENVIVPROC) load(userptr, "glGetTexEnviv");
    glad_glGetTexGendv = (PFNGLGETTEXGENDVPROC) load(userptr, "glGetTexGendv");
    glad_glGetTexGenfv = (PFNGLGETTEXGENFVPROC) load(userptr, "glGetTexGenfv");
    glad_glGetTexGeniv = (PFNGLGETTEXGENIVPROC) load(userptr, "glGetTexGeniv");
    glad_glGetTexImage = (PFNGLGETTEXIMAGEPROC) load(userptr, "glGetTexImage");
    glad_glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC) load(userptr, "glGetTexLevelParameterfv");
    glad_glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC) load(userptr, "glGetTexLevelParameteriv");
    glad_glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC) load(userptr, "glGetTexParameterfv");
    glad_glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC) load(userptr, "glGetTexParameteriv");
    glad_glHint = (PFNGLHINTPROC) load(userptr, "glHint");
    glad_glIndexMask = (PFNGLINDEXMASKPROC) load(userptr, "glIndexMask");
    glad_glIndexd = (PFNGLINDEXDPROC) load(userptr, "glIndexd");
    glad_glIndexdv = (PFNGLINDEXDVPROC) load(userptr, "glIndexdv");
    glad_glIndexf = (PFNGLINDEXFPROC) load(userptr, "glIndexf");
    glad_glIndexfv = (PFNGLINDEXFVPROC) load(userptr, "glIndexfv");
    glad_glIndexi = (PFNGLINDEXIPROC) load(userptr, "glIndexi");
    glad_glIndexiv = (PFNGLINDEXIVPROC) load(userptr, "glIndexiv");
    glad_glIndexs = (PFNGLINDEXSPROC) load(userptr, "glIndexs");
    glad_glIndexsv = (PFNGLINDEXSVPROC) load(userptr, "glIndexsv");
    glad_glInitNames = (PFNGLINITNAMESPROC) load(userptr, "glInitNames");
    glad_glIsEnabled = (PFNGLISENABLEDPROC) load(userptr, "glIsEnabled");
    glad_glIsList = (PFNGLISLISTPROC) load(userptr, "glIsList");
    glad_glLightModelf = (PFNGLLIGHTMODELFPROC) load(userptr, "glLightModelf");
    glad_glLightModelfv = (PFNGLLIGHTMODELFVPROC) load(userptr, "glLightModelfv");
    glad_glLightModeli = (PFNGLLIGHTMODELIPROC) load(userptr, "glLightModeli");
    glad_glLightModeliv = (PFNGLLIGHTMODELIVPROC) load(userptr, "glLightModeliv");
    glad_glLightf = (PFNGLLIGHTFPROC) load(userptr, "glLightf");
    glad_glLightfv = (PFNGLLIGHTFVPROC) load(userptr, "glLightfv");
    glad_glLighti = (PFNGLLIGHTIPROC) load(userptr, "glLighti");
    glad_glLightiv = (PFNGLLIGHTIVPROC) load(userptr, "glLightiv");
    glad_glLineStipple = (PFNGLLINESTIPPLEPROC) load(userptr, "glLineStipple");
    glad_glLineWidth = (PFNGLLINEWIDTHPROC) load(userptr, "glLineWidth");
    glad_glListBase = (PFNGLLISTBASEPROC) load(userptr, "glListBase");
    glad_glLoadIdentity = (PFNGLLOADIDENTITYPROC) load(userptr, "glLoadIdentity");
    glad_glLoadMatrixd = (PFNGLLOADMATRIXDPROC) load(userptr, "glLoadMatrixd");
    glad_glLoadMatrixf = (PFNGLLOADMATRIXFPROC) load(userptr, "glLoadMatrixf");
    glad_glLoadName = (PFNGLLOADNAMEPROC) load(userptr, "glLoadName");
    glad_glLogicOp = (PFNGLLOGICOPPROC) load(userptr, "glLogicOp");
    glad_glMap1d = (PFNGLMAP1DPROC) load(userptr, "glMap1d");
    glad_glMap1f = (PFNGLMAP1FPROC) load(userptr, "glMap1f");
    glad_glMap2d = (PFNGLMAP2DPROC) load(userptr, "glMap2d");
    glad_glMap2f = (PFNGLMAP2FPROC) load(userptr, "glMap2f");
    glad_glMapGrid1d = (PFNGLMAPGRID1DPROC) load(userptr, "glMapGrid1d");
    glad_glMapGrid1f = (PFNGLMAPGRID1FPROC) load(userptr, "glMapGrid1f");
    glad_glMapGrid2d = (PFNGLMAPGRID2DPROC) load(userptr, "glMapGrid2d");
    glad_glMapGrid2f = (PFNGLMAPGRID2FPROC) load(userptr, "glMapGrid2f");
    glad_glMaterialf = (PFNGLMATERIALFPROC) load(userptr, "glMaterialf");
    glad_glMaterialfv = (PFNGLMATERIALFVPROC) load(userptr, "glMaterialfv");
    glad_glMateriali = (PFNGLMATERIALIPROC) load(userptr, "glMateriali");
    glad_glMaterialiv = (PFNGLMATERIALIVPROC) load(userptr, "glMaterialiv");
    glad_glMatrixMode = (PFNGLMATRIXMODEPROC) load(userptr, "glMatrixMode");
    glad_glMultMatrixd = (PFNGLMULTMATRIXDPROC) load(userptr, "glMultMatrixd");
    glad_glMultMatrixf = (PFNGLMULTMATRIXFPROC) load(userptr, "glMultMatrixf");
    glad_glNewList = (PFNGLNEWLISTPROC) load(userptr, "glNewList");
    glad_glNormal3b = (PFNGLNORMAL3BPROC) load(userptr, "glNormal3b");
    glad_glNormal3bv = (PFNGLNORMAL3BVPROC) load(userptr, "glNormal3bv");
    glad_glNormal3d = (PFNGLNORMAL3DPROC) load(userptr, "glNormal3d");
    glad_glNormal3dv = (PFNGLNORMAL3DVPROC) load(userptr, "glNormal3dv");
    glad_glNormal3f = (PFNGLNORMAL3FPROC) load(userptr, "glNormal3f");
    glad_glNormal3fv = (PFNGLNORMAL3FVPROC) load(userptr, "glNormal3fv");
    glad_glNormal3i = (PFNGLNORMAL3IPROC) load(userptr, "glNormal3i");
    glad_glNormal3iv = (PFNGLNORMAL3IVPROC) load(userptr, "glNormal3iv");
    glad_glNormal3s = (PFNGLNORMAL3SPROC) load(userptr, "glNormal3s");
    glad_glNormal3sv = (PFNGLNORMAL3SVPROC) load(userptr, "glNormal3sv");
    glad_glOrtho = (PFNGLORTHOPROC) load(userptr, "glOrtho");
    glad_glPassThrough = (PFNGLPASSTHROUGHPROC) load(userptr, "glPassThrough");
    glad_glPixelMapfv = (PFNGLPIXELMAPFVPROC) load(userptr, "glPixelMapfv");
    glad_glPixelMapuiv = (PFNGLPIXELMAPUIVPROC) load(userptr, "glPixelMapuiv");
    glad_glPixelMapusv = (PFNGLPIXELMAPUSVPROC) load(userptr, "glPixelMapusv");
    glad_glPixelStoref = (PFNGLPIXELSTOREFPROC) load(userptr, "glPixelStoref");
    glad_glPixelStorei = (PFNGLPIXELSTOREIPROC) load(userptr, "glPixelStorei");
    glad_glPixelTransferf = (PFNGLPIXELTRANSFERFPROC) load(userptr, "glPixelTransferf");
    glad_glPixelTransferi = (PFNGLPIXELTRANSFERIPROC) load(userptr, "glPixelTransferi");
    glad_glPixelZoom = (PFNGLPIXELZOOMPROC) load(userptr, "glPixelZoom");
    glad_glPointSize = (PFNGLPOINTSIZEPROC) load(userptr, "glPointSize");
    glad_glPolygonMode = (PFNGLPOLYGONMODEPROC) load(userptr, "glPolygonMode");
    glad_glPolygonStipple = (PFNGLPOLYGONSTIPPLEPROC) load(userptr, "glPolygonStipple");
    glad_glPopAttrib = (PFNGLPOPATTRIBPROC) load(userptr, "glPopAttrib");
    glad_glPopMatrix = (PFNGLPOPMATRIXPROC) load(userptr, "glPopMatrix");
    glad_glPopName = (PFNGLPOPNAMEPROC) load(userptr, "glPopName");
    glad_glPushAttrib = (PFNGLPUSHATTRIBPROC) load(userptr, "glPushAttrib");
    glad_glPushMatrix = (PFNGLPUSHMATRIXPROC) load(userptr, "glPushMatrix");
    glad_glPushName = (PFNGLPUSHNAMEPROC) load(userptr, "glPushName");
    glad_glRasterPos2d = (PFNGLRASTERPOS2DPROC) load(userptr, "glRasterPos2d");
    glad_glRasterPos2dv = (PFNGLRASTERPOS2DVPROC) load(userptr, "glRasterPos2dv");
    glad_glRasterPos2f = (PFNGLRASTERPOS2FPROC) load(userptr, "glRasterPos2f");
    glad_glRasterPos2fv = (PFNGLRASTERPOS2FVPROC) load(userptr, "glRasterPos2fv");
    glad_glRasterPos2i = (PFNGLRASTERPOS2IPROC) load(userptr, "glRasterPos2i");
    glad_glRasterPos2iv = (PFNGLRASTERPOS2IVPROC) load(userptr, "glRasterPos2iv");
    glad_glRasterPos2s = (PFNGLRASTERPOS2SPROC) load(userptr, "glRasterPos2s");
    glad_glRasterPos2sv = (PFNGLRASTERPOS2SVPROC) load(userptr, "glRasterPos2sv");
    glad_glRasterPos3d = (PFNGLRASTERPOS3DPROC) load(userptr, "glRasterPos3d");
    glad_glRasterPos3dv = (PFNGLRASTERPOS3DVPROC) load(userptr, "glRasterPos3dv");
    glad_glRasterPos3f = (PFNGLRASTERPOS3FPROC) load(userptr, "glRasterPos3f");
    glad_glRasterPos3fv = (PFNGLRASTERPOS3FVPROC) load(userptr, "glRasterPos3fv");
    glad_glRasterPos3i = (PFNGLRASTERPOS3IPROC) load(userptr, "glRasterPos3i");
    glad_glRasterPos3iv = (PFNGLRASTERPOS3IVPROC) load(userptr, "glRasterPos3iv");
    glad_glRasterPos3s = (PFNGLRASTERPOS3SPROC) load(userptr, "glRasterPos3s");
    glad_glRasterPos3sv = (PFNGLRASTERPOS3SVPROC) load(userptr, "glRasterPos3sv");
    glad_glRasterPos4d = (PFNGLRASTERPOS4DPROC) load(userptr, "glRasterPos4d");
    glad_glRasterPos4dv = (PFNGLRASTERPOS4DVPROC) load(userptr, "glRasterPos4dv");
    glad_glRasterPos4f = (PFNGLRASTERPOS4FPROC) load(userptr, "glRasterPos4f");
    glad_glRasterPos4fv = (PFNGLRASTERPOS4FVPROC) load(userptr, "glRasterPos4fv");
    glad_glRasterPos4i = (PFNGLRASTERPOS4IPROC) load(userptr, "glRasterPos4i");
    glad_glRasterPos4iv = (PFNGLRASTERPOS4IVPROC) load(userptr, "glRasterPos4iv");
    glad_glRasterPos4s = (PFNGLRASTERPOS4SPROC) load(userptr, "glRasterPos4s");
    glad_glRasterPos4sv = (PFNGLRASTERPOS4SVPROC) load(userptr, "glRasterPos4sv");
    glad_glReadBuffer = (PFNGLREADBUFFERPROC) load(userptr, "glReadBuffer");
    glad_glReadPixels = (PFNGLREADPIXELSPROC) load(userptr, "glReadPixels");
    glad_glRectd = (PFNGLRECTDPROC) load(userptr, "glRectd");
    glad_glRectdv = (PFNGLRECTDVPROC) load(userptr, "glRectdv");
    glad_glRectf = (PFNGLRECTFPROC) load(userptr, "glRectf");
    glad_glRectfv = (PFNGLRECTFVPROC) load(userptr, "glRectfv");
    glad_glRecti = (PFNGLRECTIPROC) load(userptr, "glRecti");
    glad_glRectiv = (PFNGLRECTIVPROC) load(userptr, "glRectiv");
    glad_glRects = (PFNGLRECTSPROC) load(userptr, "glRects");
    glad_glRectsv = (PFNGLRECTSVPROC) load(userptr, "glRectsv");
    glad_glRenderMode = (PFNGLRENDERMODEPROC) load(userptr, "glRenderMode");
    glad_glRotated = (PFNGLROTATEDPROC) load(userptr, "glRotated");
    glad_glRotatef = (PFNGLROTATEFPROC) load(userptr, "glRotatef");
    glad_glScaled = (PFNGLSCALEDPROC) load(userptr, "glScaled");
    glad_glScalef = (PFNGLSCALEFPROC) load(userptr, "glScalef");
    glad_glScissor = (PFNGLSCISSORPROC) load(userptr, "glScissor");
    glad_glSelectBuffer = (PFNGLSELECTBUFFERPROC) load(userptr, "glSelectBuffer");
    glad_glShadeModel = (PFNGLSHADEMODELPROC) load(userptr, "glShadeModel");
    glad_glStencilFunc = (PFNGLSTENCILFUNCPROC) load(userptr, "glStencilFunc");
    glad_glStencilMask = (PFNGLSTENCILMASKPROC) load(userptr, "glStencilMask");
    glad_glStencilOp = (PFNGLSTENCILOPPROC) load(userptr, "glStencilOp");
    glad_glTexCoord1d = (PFNGLTEXCOORD1DPROC) load(userptr, "glTexCoord1d");
    glad_glTexCoord1dv = (PFNGLTEXCOORD1DVPROC) load(userptr, "glTexCoord1dv");
    glad_glTexCoord1f = (PFNGLTEXCOORD1FPROC) load(userptr, "glTexCoord1f");
    glad_glTexCoord1fv = (PFNGLTEXCOORD1FVPROC) load(userptr, "glTexCoord1fv");
    glad_glTexCoord1i = (PFNGLTEXCOORD1IPROC) load(userptr, "glTexCoord1i");
    glad_glTexCoord1iv = (PFNGLTEXCOORD1IVPROC) load(userptr, "glTexCoord1iv");
    glad_glTexCoord1s = (PFNGLTEXCOORD1SPROC) load(userptr, "glTexCoord1s");
    glad_glTexCoord1sv = (PFNGLTEXCOORD1SVPROC) load(userptr, "glTexCoord1sv");
    glad_glTexCoord2d = (PFNGLTEXCOORD2DPROC) load(userptr, "glTexCoord2d");
    glad_glTexCoord2dv = (PFNGLTEXCOORD2DVPROC) load(userptr, "glTexCoord2dv");
    glad_glTexCoord2f = (PFNGLTEXCOORD2FPROC) load(userptr, "glTexCoord2f");
    glad_glTexCoord2fv = (PFNGLTEXCOORD2FVPROC) load(userptr, "glTexCoord2fv");
    glad_glTexCoord2i = (PFNGLTEXCOORD2IPROC) load(userptr, "glTexCoord2i");
    glad_glTexCoord2iv = (PFNGLTEXCOORD2IVPROC) load(userptr, "glTexCoord2iv");
    glad_glTexCoord2s = (PFNGLTEXCOORD2SPROC) load(userptr, "glTexCoord2s");
    glad_glTexCoord2sv = (PFNGLTEXCOORD2SVPROC) load(userptr, "glTexCoord2sv");
    glad_glTexCoord3d = (PFNGLTEXCOORD3DPROC) load(userptr, "glTexCoord3d");
    glad_glTexCoord3dv = (PFNGLTEXCOORD3DVPROC) load(userptr, "glTexCoord3dv");
    glad_glTexCoord3f = (PFNGLTEXCOORD3FPROC) load(userptr, "glTexCoord3f");
    glad_glTexCoord3fv = (PFNGLTEXCOORD3FVPROC) load(userptr, "glTexCoord3fv");
    glad_glTexCoord3i = (PFNGLTEXCOORD3IPROC) load(userptr, "glTexCoord3i");
    glad_glTexCoord3iv = (PFNGLTEXCOORD3IVPROC) load(userptr, "glTexCoord3iv");
    glad_glTexCoord3s = (PFNGLTEXCOORD3SPROC) load(userptr, "glTexCoord3s");
    glad_glTexCoord3sv = (PFNGLTEXCOORD3SVPROC) load(userptr, "glTexCoord3sv");
    glad_glTexCoord4d = (PFNGLTEXCOORD4DPROC) load(userptr, "glTexCoord4d");
    glad_glTexCoord4dv = (PFNGLTEXCOORD4DVPROC) load(userptr, "glTexCoord4dv");
    glad_glTexCoord4f = (PFNGLTEXCOORD4FPROC) load(userptr, "glTexCoord4f");
    glad_glTexCoord4fv = (PFNGLTEXCOORD4FVPROC) load(userptr, "glTexCoord4fv");
    glad_glTexCoord4i = (PFNGLTEXCOORD4IPROC) load(userptr, "glTexCoord4i");
    glad_glTexCoord4iv = (PFNGLTEXCOORD4IVPROC) load(userptr, "glTexCoord4iv");
    glad_glTexCoord4s = (PFNGLTEXCOORD4SPROC) load(userptr, "glTexCoord4s");
    glad_glTexCoord4sv = (PFNGLTEXCOORD4SVPROC) load(userptr, "glTexCoord4sv");
    glad_glTexEnvf = (PFNGLTEXENVFPROC) load(userptr, "glTexEnvf");
    glad_glTexEnvfv = (PFNGLTEXENVFVPROC) load(userptr, "glTexEnvfv");
    glad_glTexEnvi = (PFNGLTEXENVIPROC) load(userptr, "glTexEnvi");
    glad_glTexEnviv = (PFNGLTEXENVIVPROC) load(userptr, "glTexEnviv");
    glad_glTexGend = (PFNGLTEXGENDPROC) load(userptr, "glTexGend");
    glad_glTexGendv = (PFNGLTEXGENDVPROC) load(userptr, "glTexGendv");
    glad_glTexGenf = (PFNGLTEXGENFPROC) load(userptr, "glTexGenf");
    glad_glTexGenfv = (PFNGLTEXGENFVPROC) load(userptr, "glTexGenfv");
    glad_glTexGeni = (PFNGLTEXGENIPROC) load(userptr, "glTexGeni");
    glad_glTexGeniv = (PFNGLTEXGENIVPROC) load(userptr, "glTexGeniv");
    glad_glTexImage1D = (PFNGLTEXIMAGE1DPROC) load(userptr, "glTexImage1D");
    glad_glTexImage2D = (PFNGLTEXIMAGE2DPROC) load(userptr, "glTexImage2D");
    glad_glTexParameterf = (PFNGLTEXPARAMETERFPROC) load(userptr, "glTexParameterf");
    glad_glTexParameterfv = (PFNGLTEXPARAMETERFVPROC) load(userptr, "glTexParameterfv");
    glad_glTexParameteri = (PFNGLTEXPARAMETERIPROC) load(userptr, "glTexParameteri");
    glad_glTexParameteriv = (PFNGLTEXPARAMETERIVPROC) load(userptr, "glTexParameteriv");
    glad_glTranslated = (PFNGLTRANSLATEDPROC) load(userptr, "glTranslated");
    glad_glTranslatef = (PFNGLTRANSLATEFPROC) load(userptr, "glTranslatef");
    glad_glVertex2d = (PFNGLVERTEX2DPROC) load(userptr, "glVertex2d");
    glad_glVertex2dv = (PFNGLVERTEX2DVPROC) load(userptr, "glVertex2dv");
    glad_glVertex2f = (PFNGLVERTEX2FPROC) load(userptr, "glVertex2f");
    glad_glVertex2fv = (PFNGLVERTEX2FVPROC) load(userptr, "glVertex2fv");
    glad_glVertex2i = (PFNGLVERTEX2IPROC) load(userptr, "glVertex2i");
    glad_glVertex2iv = (PFNGLVERTEX2IVPROC) load(userptr, "glVertex2iv");
    glad_glVertex2s = (PFNGLVERTEX2SPROC) load(userptr, "glVertex2s");
    glad_glVertex2sv = (PFNGLVERTEX2SVPROC) load(userptr, "glVertex2sv");
    glad_glVertex3d = (PFNGLVERTEX3DPROC) load(userptr, "glVertex3d");
    glad_glVertex3dv = (PFNGLVERTEX3DVPROC) load(userptr, "glVertex3dv");
    glad_glVertex3f = (PFNGLVERTEX3FPROC) load(userptr, "glVertex3f");
    glad_glVertex3fv = (PFNGLVERTEX3FVPROC) load(userptr, "glVertex3fv");
    glad_glVertex3i = (PFNGLVERTEX3IPROC) load(userptr, "glVertex3i");
    glad_glVertex3iv = (PFNGLVERTEX3IVPROC) load(userptr, "glVertex3iv");
    glad_glVertex3s = (PFNGLVERTEX3SPROC) load(userptr, "glVertex3s");
    glad_glVertex3sv = (PFNGLVERTEX3SVPROC) load(userptr, "glVertex3sv");
    glad_glVertex4d = (PFNGLVERTEX4DPROC) load(userptr, "glVertex4d");
    glad_glVertex4dv = (PFNGLVERTEX4DVPROC) load(userptr, "glVertex4dv");
    glad_glVertex4f = (PFNGLVERTEX4FPROC) load(userptr, "glVertex4f");
    glad_glVertex4fv = (PFNGLVERTEX4FVPROC) load(userptr, "glVertex4fv");
    glad_glVertex4i = (PFNGLVERTEX4IPROC) load(userptr, "glVertex4i");
    glad_glVertex4iv = (PFNGLVERTEX4IVPROC) load(userptr, "glVertex4iv");
    glad_glVertex4s = (PFNGLVERTEX4SPROC) load(userptr, "glVertex4s");
    glad_glVertex4sv = (PFNGLVERTEX4SVPROC) load(userptr, "glVertex4sv");
    glad_glViewport = (PFNGLVIEWPORTPROC) load(userptr, "glViewport");
}
static void glad_gl_load_GL_VERSION_1_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_1) return;
    glad_glAreTexturesResident = (PFNGLARETEXTURESRESIDENTPROC) load(userptr, "glAreTexturesResident");
    glad_glArrayElement = (PFNGLARRAYELEMENTPROC) load(userptr, "glArrayElement");
    glad_glBindTexture = (PFNGLBINDTEXTUREPROC) load(userptr, "glBindTexture");
    glad_glColorPointer = (PFNGLCOLORPOINTERPROC) load(userptr, "glColorPointer");
    glad_glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC) load(userptr, "glCopyTexImage1D");
    glad_glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC) load(userptr, "glCopyTexImage2D");
    glad_glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC) load(userptr, "glCopyTexSubImage1D");
    glad_glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC) load(userptr, "glCopyTexSubImage2D");
    glad_glDeleteTextures = (PFNGLDELETETEXTURESPROC) load(userptr, "glDeleteTextures");
    glad_glDisableClientState = (PFNGLDISABLECLIENTSTATEPROC) load(userptr, "glDisableClientState");
    glad_glDrawArrays = (PFNGLDRAWARRAYSPROC) load(userptr, "glDrawArrays");
    glad_glDrawElements = (PFNGLDRAWELEMENTSPROC) load(userptr, "glDrawElements");
    glad_glEdgeFlagPointer = (PFNGLEDGEFLAGPOINTERPROC) load(userptr, "glEdgeFlagPointer");
    glad_glEnableClientState = (PFNGLENABLECLIENTSTATEPROC) load(userptr, "glEnableClientState");
    glad_glGenTextures = (PFNGLGENTEXTURESPROC) load(userptr, "glGenTextures");
    glad_glGetPointerv = (PFNGLGETPOINTERVPROC) load(userptr, "glGetPointerv");
    glad_glIndexPointer = (PFNGLINDEXPOINTERPROC) load(userptr, "glIndexPointer");
    glad_glIndexub = (PFNGLINDEXUBPROC) load(userptr, "glIndexub");
    glad_glIndexubv = (PFNGLINDEXUBVPROC) load(userptr, "glIndexubv");
    glad_glInterleavedArrays = (PFNGLINTERLEAVEDARRAYSPROC) load(userptr, "glInterleavedArrays");
    glad_glIsTexture = (PFNGLISTEXTUREPROC) load(userptr, "glIsTexture");
    glad_glNormalPointer = (PFNGLNORMALPOINTERPROC) load(userptr, "glNormalPointer");
    glad_glPolygonOffset = (PFNGLPOLYGONOFFSETPROC) load(userptr, "glPolygonOffset");
    glad_glPopClientAttrib = (PFNGLPOPCLIENTATTRIBPROC) load(userptr, "glPopClientAttrib");
    glad_glPrioritizeTextures = (PFNGLPRIORITIZETEXTURESPROC) load(userptr, "glPrioritizeTextures");
    glad_glPushClientAttrib = (PFNGLPUSHCLIENTATTRIBPROC) load(userptr, "glPushClientAttrib");
    glad_glTexCoordPointer = (PFNGLTEXCOORDPOINTERPROC) load(userptr, "glTexCoordPointer");
    glad_glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC) load(userptr, "glTexSubImage1D");
    glad_glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC) load(userptr, "glTexSubImage2D");
    glad_glVertexPointer = (PFNGLVERTEXPOINTERPROC) load(userptr, "glVertexPointer");
}
static void glad_gl_load_GL_VERSION_1_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_2) return;
    glad_glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) load(userptr, "glCopyTexSubImage3D");
    glad_glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) load(userptr, "glDrawRangeElements");
    glad_glTexImage3D = (PFNGLTEXIMAGE3DPROC) load(userptr, "glTexImage3D");
    glad_glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) load(userptr, "glTexSubImage3D");
}
static void glad_gl_load_GL_VERSION_1_3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_3) return;
    glad_glActiveTexture = (PFNGLACTIVETEXTUREPROC) load(userptr, "glActiveTexture");
    glad_glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) load(userptr, "glClientActiveTexture");
    glad_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) load(userptr, "glCompressedTexImage1D");
    glad_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) load(userptr, "glCompressedTexImage2D");
    glad_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) load(userptr, "glCompressedTexImage3D");
    glad_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) load(userptr, "glCompressedTexSubImage1D");
    glad_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) load(userptr, "glCompressedTexSubImage2D");
    glad_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) load(userptr, "glCompressedTexSubImage3D");
    glad_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) load(userptr, "glGetCompressedTexImage");
    glad_glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC) load(userptr, "glLoadTransposeMatrixd");
    glad_glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC) load(userptr, "glLoadTransposeMatrixf");
    glad_glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC) load(userptr, "glMultTransposeMatrixd");
    glad_glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC) load(userptr, "glMultTransposeMatrixf");
    glad_glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC) load(userptr, "glMultiTexCoord1d");
    glad_glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC) load(userptr, "glMultiTexCoord1dv");
    glad_glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC) load(userptr, "glMultiTexCoord1f");
    glad_glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC) load(userptr, "glMultiTexCoord1fv");
    glad_glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC) load(userptr, "glMultiTexCoord1i");
    glad_glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC) load(userptr, "glMultiTexCoord1iv");
    glad_glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC) load(userptr, "glMultiTexCoord1s");
    glad_glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC) load(userptr, "glMultiTexCoord1sv");
    glad_glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC) load(userptr, "glMultiTexCoord2d");
    glad_glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC) load(userptr, "glMultiTexCoord2dv");
    glad_glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC) load(userptr, "glMultiTexCoord2f");
    glad_glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC) load(userptr, "glMultiTexCoord2fv");
    glad_glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC) load(userptr, "glMultiTexCoord2i");
    glad_glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC) load(userptr, "glMultiTexCoord2iv");
    glad_glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC) load(userptr, "glMultiTexCoord2s");
    glad_glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC) load(userptr, "glMultiTexCoord2sv");
    glad_glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC) load(userptr, "glMultiTexCoord3d");
    glad_glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC) load(userptr, "glMultiTexCoord3dv");
    glad_glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC) load(userptr, "glMultiTexCoord3f");
    glad_glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC) load(userptr, "glMultiTexCoord3fv");
    glad_glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC) load(userptr, "glMultiTexCoord3i");
    glad_glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC) load(userptr, "glMultiTexCoord3iv");
    glad_glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC) load(userptr, "glMultiTexCoord3s");
    glad_glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC) load(userptr, "glMultiTexCoord3sv");
    glad_glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC) load(userptr, "glMultiTexCoord4d");
    glad_glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC) load(userptr, "glMultiTexCoord4dv");
    glad_glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC) load(userptr, "glMultiTexCoord4f");
    glad_glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC) load(userptr, "glMultiTexCoord4fv");
    glad_glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC) load(userptr, "glMultiTexCoord4i");
    glad_glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC) load(userptr, "glMultiTexCoord4iv");
    glad_glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC) load(userptr, "glMultiTexCoord4s");
    glad_glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC) load(userptr, "glMultiTexCoord4sv");
    glad_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) load(userptr, "glSampleCoverage");
}
static void glad_gl_load_GL_VERSION_1_4( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_4) return;
    glad_glBlendColor = (PFNGLBLENDCOLORPROC) load(userptr, "glBlendColor");
    glad_glBlendEquation = (PFNGLBLENDEQUATIONPROC) load(userptr, "glBlendEquation");
    glad_glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) load(userptr, "glBlendFuncSeparate");
    glad_glFogCoordPointer = (PFNGLFOGCOORDPOINTERPROC) load(userptr, "glFogCoordPointer");
    glad_glFogCoordd = (PFNGLFOGCOORDDPROC) load(userptr, "glFogCoordd");
    glad_glFogCoorddv = (PFNGLFOGCOORDDVPROC) load(userptr, "glFogCoorddv");
    glad_glFogCoordf = (PFNGLFOGCOORDFPROC) load(userptr, "glFogCoordf");
    glad_glFogCoordfv = (PFNGLFOGCOORDFVPROC) load(userptr, "glFogCoordfv");
    glad_glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) load(userptr, "glMultiDrawArrays");
    glad_glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) load(userptr, "glMultiDrawElements");
    glad_glPointParameterf = (PFNGLPOINTPARAMETERFPROC) load(userptr, "glPointParameterf");
    glad_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) load(userptr, "glPointParameterfv");
    glad_glPointParameteri = (PFNGLPOINTPARAMETERIPROC) load(userptr, "glPointParameteri");
    glad_glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) load(userptr, "glPointParameteriv");
    glad_glSecondaryColor3b = (PFNGLSECONDARYCOLOR3BPROC) load(userptr, "glSecondaryColor3b");
    glad_glSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BVPROC) load(userptr, "glSecondaryColor3bv");
    glad_glSecondaryColor3d = (PFNGLSECONDARYCOLOR3DPROC) load(userptr, "glSecondaryColor3d");
    glad_glSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DVPROC) load(userptr, "glSecondaryColor3dv");
    glad_glSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC) load(userptr, "glSecondaryColor3f");
    glad_glSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FVPROC) load(userptr, "glSecondaryColor3fv");
    glad_glSecondaryColor3i = (PFNGLSECONDARYCOLOR3IPROC) load(userptr, "glSecondaryColor3i");
    glad_glSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IVPROC) load(userptr, "glSecondaryColor3iv");
    glad_glSecondaryColor3s = (PFNGLSECONDARYCOLOR3SPROC) load(userptr, "glSecondaryColor3s");
    glad_glSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SVPROC) load(userptr, "glSecondaryColor3sv");
    glad_glSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UBPROC) load(userptr, "glSecondaryColor3ub");
    glad_glSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBVPROC) load(userptr, "glSecondaryColor3ubv");
    glad_glSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UIPROC) load(userptr, "glSecondaryColor3ui");
    glad_glSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIVPROC) load(userptr, "glSecondaryColor3uiv");
    glad_glSecondaryColor3us = (PFNGLSECONDARYCOLOR3USPROC) load(userptr, "glSecondaryColor3us");
    glad_glSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USVPROC) load(userptr, "glSecondaryColor3usv");
    glad_glSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTERPROC) load(userptr, "glSecondaryColorPointer");
    glad_glWindowPos2d = (PFNGLWINDOWPOS2DPROC) load(userptr, "glWindowPos2d");
    glad_glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC) load(userptr, "glWindowPos2dv");
    glad_glWindowPos2f = (PFNGLWINDOWPOS2FPROC) load(userptr, "glWindowPos2f");
    glad_glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC) load(userptr, "glWindowPos2fv");
    glad_glWindowPos2i = (PFNGLWINDOWPOS2IPROC) load(userptr, "glWindowPos2i");
    glad_glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC) load(userptr, "glWindowPos2iv");
    glad_glWindowPos2s = (PFNGLWINDOWPOS2SPROC) load(userptr, "glWindowPos2s");
    glad_glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC) load(userptr, "glWindowPos2sv");
    glad_glWindowPos3d = (PFNGLWINDOWPOS3DPROC) load(userptr, "glWindowPos3d");
    glad_glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC) load(userptr, "glWindowPos3dv");
    glad_glWindowPos3f = (PFNGLWINDOWPOS3FPROC) load(userptr, "glWindowPos3f");
    glad_glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC) load(userptr, "glWindowPos3fv");
    glad_glWindowPos3i = (PFNGLWINDOWPOS3IPROC) load(userptr, "glWindowPos3i");
    glad_glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC) load(userptr, "glWindowPos3iv");
    glad_glWindowPos3s = (PFNGLWINDOWPOS3SPROC) load(userptr, "glWindowPos3s");
    glad_glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC) load(userptr, "glWindowPos3sv");
}
static void glad_gl_load_GL_VERSION_1_5( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_5) return;
    glad_glBeginQuery = (PFNGLBEGINQUERYPROC) load(userptr, "glBeginQuery");
    glad_glBindBuffer = (PFNGLBINDBUFFERPROC) load(userptr, "glBindBuffer");
    glad_glBufferData = (PFNGLBUFFERDATAPROC) load(userptr, "glBufferData");
    glad_glBufferSubData = (PFNGLBUFFERSUBDATAPROC) load(userptr, "glBufferSubData");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) load(userptr, "glDeleteBuffers");
    glad_glDeleteQueries = (PFNGLDELETEQUERIESPROC) load(userptr, "glDeleteQueries");
    glad_glEndQuery = (PFNGLENDQUERYPROC) load(userptr, "glEndQuery");
    glad_glGenBuffers = (PFNGLGENBUFFERSPROC) load(userptr, "glGenBuffers");
    glad_glGenQueries = (PFNGLGENQUERIESPROC) load(userptr, "glGenQueries");
    glad_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) load(userptr, "glGetBufferParameteriv");
    glad_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) load(userptr, "glGetBufferPointerv");
    glad_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) load(userptr, "glGetBufferSubData");
    glad_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) load(userptr, "glGetQueryObjectiv");
    glad_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) load(userptr, "glGetQueryObjectuiv");
    glad_glGetQueryiv = (PFNGLGETQUERYIVPROC) load(userptr, "glGetQueryiv");
    glad_glIsBuffer = (PFNGLISBUFFERPROC) load(userptr, "glIsBuffer");
    glad_glIsQuery = (PFNGLISQUERYPROC) load(userptr, "glIsQuery");
    glad_glMapBuffer = (PFNGLMAPBUFFERPROC) load(userptr, "glMapBuffer");
    glad_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) load(userptr, "glUnmapBuffer");
}
static void glad_gl_load_GL_VERSION_2_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_2_0) return;
    glad_glAttachShader = (PFNGLATTACHSHADERPROC) load(userptr, "glAttachShader");
    glad_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) load(userptr, "glBindAttribLocation");
    glad_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) load(userptr, "glBlendEquationSeparate");
    glad_glCompileShader = (PFNGLCOMPILESHADERPROC) load(userptr, "glCompileShader");
    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC) load(userptr, "glCreateProgram");
    glad_glCreateShader = (PFNGLCREATESHADERPROC) load(userptr, "glCreateShader");
    glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC) load(userptr, "glDeleteProgram");
    glad_glDeleteShader = (PFNGLDELETESHADERPROC) load(userptr, "glDeleteShader");
    glad_glDetachShader = (PFNGLDETACHSHADERPROC) load(userptr, "glDetachShader");
    glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) load(userptr, "glDisableVertexAttribArray");
    glad_glDrawBuffers = (PFNGLDRAWBUFFERSPROC) load(userptr, "glDrawBuffers");
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load(userptr, "glEnableVertexAttribArray");
    glad_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) load(userptr, "glGetActiveAttrib");
    glad_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) load(userptr, "glGetActiveUniform");
    glad_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) load(userptr, "glGetAttachedShaders");
    glad_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) load(userptr, "glGetAttribLocation");
    glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) load(userptr, "glGetProgramInfoLog");
    glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC) load(userptr, "glGetProgramiv");
    glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) load(userptr, "glGetShaderInfoLog");
    glad_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) load(userptr, "glGetShaderSource");
    glad_glGetShaderiv = (PFNGLGETSHADERIVPROC) load(userptr, "glGetShaderiv");
    glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) load(userptr, "glGetUniformLocation");
    glad_glGetUniformfv = (PFNGLGETUNIFORMFVPROC) load(userptr, "glGetUniformfv");
    glad_glGetUniformiv = (PFNGLGETUNIFORMIVPROC) load(userptr, "glGetUniformiv");
    glad_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) load(userptr, "glGetVertexAttribPointerv");
    glad_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) load(userptr, "glGetVertexAttribdv");
    glad_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) load(userptr, "glGetVertexAttribfv");
    glad_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) load(userptr, "glGetVertexAttribiv");
    glad_glIsProgram = (PFNGLISPROGRAMPROC) load(userptr, "glIsProgram");
    glad_glIsShader = (PFNGLISSHADERPROC) load(userptr, "glIsShader");
    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC) load(userptr, "glLinkProgram");
    glad_glShaderSource = (PFNGLSHADERSOURCEPROC) load(userptr, "glShaderSource");
    glad_glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) load(userptr, "glStencilFuncSeparate");
    glad_glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) load(userptr, "glStencilMaskSeparate");
    glad_glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) load(userptr, "glStencilOpSeparate");
    glad_glUniform1f = (PFNGLUNIFORM1FPROC) load(userptr, "glUniform1f");
    glad_glUniform1fv = (PFNGLUNIFORM1FVPROC) load(userptr, "glUniform1fv");
    glad_glUniform1i = (PFNGLUNIFORM1IPROC) load(userptr, "glUniform1i");
    glad_glUniform1iv = (PFNGLUNIFORM1IVPROC) load(userptr, "glUniform1iv");
    glad_glUniform2f = (PFNGLUNIFORM2FPROC) load(userptr, "glUniform2f");
    glad_glUniform2fv = (PFNGLUNIFORM2FVPROC) load(userptr, "glUniform2fv");
    glad_glUniform2i = (PFNGLUNIFORM2IPROC) load(userptr, "glUniform2i");
    glad_glUniform2iv = (PFNGLUNIFORM2IVPROC) load(userptr, "glUniform2iv");
    glad_glUniform3f = (PFNGLUNIFORM3FPROC) load(userptr, "glUniform3f");
    glad_glUniform3fv = (PFNGLUNIFORM3FVPROC) load(userptr, "glUniform3fv");
    glad_glUniform3i = (PFNGLUNIFORM3IPROC) load(userptr, "glUniform3i");
    glad_glUniform3iv = (PFNGLUNIFORM3IVPROC) load(userptr, "glUniform3iv");
    glad_glUniform4f = (PFNGLUNIFORM4FPROC) load(userptr, "glUniform4f");
    glad_glUniform4fv = (PFNGLUNIFORM4FVPROC) load(userptr, "glUniform4fv");
    glad_glUniform4i = (PFNGLUNIFORM4IPROC) load(userptr, "glUniform4i");
    glad_glUniform4iv = (PFNGLUNIFORM4IVPROC) load(userptr, "glUniform4iv");
    glad_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) load(userptr, "glUniformMatrix2fv");
    glad_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) load(userptr, "glUniformMatrix3fv");
    glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) load(userptr, "glUniformMatrix4fv");
    glad_glUseProgram = (PFNGLUSEPROGRAMPROC) load(userptr, "glUseProgram");
    glad_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) load(userptr, "glValidateProgram");
    glad_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) load(userptr, "glVertexAttrib1d");
    glad_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) load(userptr, "glVertexAttrib1dv");
    glad_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) load(userptr, "glVertexAttrib1f");
    glad_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) load(userptr, "glVertexAttrib1fv");
    glad_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) load(userptr, "glVertexAttrib1s");
    glad_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) load(userptr, "glVertexAttrib1sv");
    glad_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) load(userptr, "glVertexAttrib2d");
    glad_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) load(userptr, "glVertexAttrib2dv");
    glad_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) load(userptr, "glVertexAttrib2f");
    glad_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) load(userptr, "glVertexAttrib2fv");
    glad_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) load(userptr, "glVertexAttrib2s");
    glad_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) load(userptr, "glVertexAttrib2sv");
    glad_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) load(userptr, "glVertexAttrib3d");
    glad_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) load(userptr, "glVertexAttrib3dv");
    glad_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) load(userptr, "glVertexAttrib3f");
    glad_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) load(userptr, "glVertexAttrib3fv");
    glad_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) load(userptr, "glVertexAttrib3s");
    glad_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) load(userptr, "glVertexAttrib3sv");
    glad_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) load(userptr, "glVertexAttrib4Nbv");
    glad_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) load(userptr, "glVertexAttrib4Niv");
    glad_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) load(userptr, "glVertexAttrib4Nsv");
    glad_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) load(userptr, "glVertexAttrib4Nub");
    glad_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) load(userptr, "glVertexAttrib4Nubv");
    glad_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) load(userptr, "glVertexAttrib4Nuiv");
    glad_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) load(userptr, "glVertexAttrib4Nusv");
    glad_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) load(userptr, "glVertexAttrib4bv");
    glad_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) load(userptr, "glVertexAttrib4d");
    glad_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) load(userptr, "glVertexAttrib4dv");
    glad_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) load(userptr, "glVertexAttrib4f");
    glad_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) load(userptr, "glVertexAttrib4fv");
    glad_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) load(userptr, "glVertexAttrib4iv");
    glad_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) load(userptr, "glVertexAttrib4s");
    glad_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) load(userptr, "glVertexAttrib4sv");
    glad_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) load(userptr, "glVertexAttrib4ubv");
    glad_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) load(userptr, "glVertexAttrib4uiv");
    glad_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) load(userptr, "glVertexAttrib4usv");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) load(userptr, "glVertexAttribPointer");
}
static void glad_gl_load_GL_VERSION_2_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_2_1) return;
    glad_glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) load(userptr, "glUniformMatrix2x3fv");
    glad_glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) load(userptr, "glUniformMatrix2x4fv");
    glad_glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) load(userptr, "glUniformMatrix3x2fv");
    glad_glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) load(userptr, "glUniformMatrix3x4fv");
    glad_glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) load(userptr, "glUniformMatrix4x2fv");
    glad_glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) load(userptr, "glUniformMatrix4x3fv");
}
static void glad_gl_load_GL_VERSION_3_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_0) return;
    glad_glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC) load(userptr, "glBeginConditionalRender");
    glad_glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC) load(userptr, "glBeginTransformFeedback");
    glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load(userptr, "glBindBufferBase");
    glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load(userptr, "glBindBufferRange");
    glad_glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) load(userptr, "glBindFragDataLocation");
    glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) load(userptr, "glBindFramebuffer");
    glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) load(userptr, "glBindRenderbuffer");
    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) load(userptr, "glBindVertexArray");
    glad_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) load(userptr, "glBlitFramebuffer");
    glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) load(userptr, "glCheckFramebufferStatus");
    glad_glClampColor = (PFNGLCLAMPCOLORPROC) load(userptr, "glClampColor");
    glad_glClearBufferfi = (PFNGLCLEARBUFFERFIPROC) load(userptr, "glClearBufferfi");
    glad_glClearBufferfv = (PFNGLCLEARBUFFERFVPROC) load(userptr, "glClearBufferfv");
    glad_glClearBufferiv = (PFNGLCLEARBUFFERIVPROC) load(userptr, "glClearBufferiv");
    glad_glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC) load(userptr, "glClearBufferuiv");
    glad_glColorMaski = (PFNGLCOLORMASKIPROC) load(userptr, "glColorMaski");
    glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) load(userptr, "glDeleteFramebuffers");
    glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) load(userptr, "glDeleteRenderbuffers");
    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) load(userptr, "glDeleteVertexArrays");
    glad_glDisablei = (PFNGLDISABLEIPROC) load(userptr, "glDisablei");
    glad_glEnablei = (PFNGLENABLEIPROC) load(userptr, "glEnablei");
    glad_glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC) load(userptr, "glEndConditionalRender");
    glad_glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC) load(userptr, "glEndTransformFeedback");
    glad_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) load(userptr, "glFlushMappedBufferRange");
    glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) load(userptr, "glFramebufferRenderbuffer");
    glad_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) load(userptr, "glFramebufferTexture1D");
    glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) load(userptr, "glFramebufferTexture2D");
    glad_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) load(userptr, "glFramebufferTexture3D");
    glad_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) load(userptr, "glFramebufferTextureLayer");
    glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) load(userptr, "glGenFramebuffers");
    glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) load(userptr, "glGenRenderbuffers");
    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) load(userptr, "glGenVertexArrays");
    glad_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) load(userptr, "glGenerateMipmap");
    glad_glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC) load(userptr, "glGetBooleani_v");
    glad_glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) load(userptr, "glGetFragDataLocation");
    glad_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) load(userptr, "glGetFramebufferAttachmentParameteriv");
    glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load(userptr, "glGetIntegeri_v");
    glad_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) load(userptr, "glGetRenderbufferParameteriv");
    glad_glGetStringi = (PFNGLGETSTRINGIPROC) load(userptr, "glGetStringi");
    glad_glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC) load(userptr, "glGetTexParameterIiv");
    glad_glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC) load(userptr, "glGetTexParameterIuiv");
    glad_glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) load(userptr, "glGetTransformFeedbackVarying");
    glad_glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC) load(userptr, "glGetUniformuiv");
    glad_glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC) load(userptr, "glGetVertexAttribIiv");
    glad_glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC) load(userptr, "glGetVertexAttribIuiv");
    glad_glIsEnabledi = (PFNGLISENABLEDIPROC) load(userptr, "glIsEnabledi");
    glad_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) load(userptr, "glIsFramebuffer");
    glad_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) load(userptr, "glIsRenderbuffer");
    glad_glIsVertexArray = (PFNGLISVERTEXARRAYPROC) load(userptr, "glIsVertexArray");
    glad_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) load(userptr, "glMapBufferRange");
    glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) load(userptr, "glRenderbufferStorage");
    glad_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) load(userptr, "glRenderbufferStorageMultisample");
    glad_glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC) load(userptr, "glTexParameterIiv");
    glad_glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC) load(userptr, "glTexParameterIuiv");
    glad_glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC) load(userptr, "glTransformFeedbackVaryings");
    glad_glUniform1ui = (PFNGLUNIFORM1UIPROC) load(userptr, "glUniform1ui");
    glad_glUniform1uiv = (PFNGLUNIFORM1UIVPROC) load(userptr, "glUniform1uiv");
    glad_glUniform2ui = (PFNGLUNIFORM2UIPROC) load(userptr, "glUniform2ui");
    glad_glUniform2uiv = (PFNGLUNIFORM2UIVPROC) load(userptr, "glUniform2uiv");
    glad_glUniform3ui = (PFNGLUNIFORM3UIPROC) load(userptr, "glUniform3ui");
    glad_glUniform3uiv = (PFNGLUNIFORM3UIVPROC) load(userptr, "glUniform3uiv");
    glad_glUniform4ui = (PFNGLUNIFORM4UIPROC) load(userptr, "glUniform4ui");
    glad_glUniform4uiv = (PFNGLUNIFORM4UIVPROC) load(userptr, "glUniform4uiv");
    glad_glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC) load(userptr, "glVertexAttribI1i");
    glad_glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC) load(userptr, "glVertexAttribI1iv");
    glad_glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC) load(userptr, "glVertexAttribI1ui");
    glad_glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC) load(userptr, "glVertexAttribI1uiv");
    glad_glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC) load(userptr, "glVertexAttribI2i");
    glad_glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC) load(userptr, "glVertexAttribI2iv");
    glad_glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC) load(userptr, "glVertexAttribI2ui");
    glad_glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC) load(userptr, "glVertexAttribI2uiv");
    glad_glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC) load(userptr, "glVertexAttribI3i");
    glad_glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC) load(userptr, "glVertexAttribI3iv");
    glad_glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC) load(userptr, "glVertexAttribI3ui");
    glad_glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC) load(userptr, "glVertexAttribI3uiv");
    glad_glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC) load(userptr, "glVertexAttribI4bv");
    glad_glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC) load(userptr, "glVertexAttribI4i");
    glad_glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC) load(userptr, "glVertexAttribI4iv");
    glad_glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC) load(userptr, "glVertexAttribI4sv");
    glad_glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC) load(userptr, "glVertexAttribI4ubv");
    glad_glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC) load(userptr, "glVertexAttribI4ui");
    glad_glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC) load(userptr, "glVertexAttribI4uiv");
    glad_glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC) load(userptr, "glVertexAttribI4usv");
    glad_glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) load(userptr, "glVertexAttribIPointer");
}
static void glad_gl_load_GL_VERSION_3_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_1) return;
    glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load(userptr, "glBindBufferBase");
    glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load(userptr, "glBindBufferRange");
    glad_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) load(userptr, "glCopyBufferSubData");
    glad_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) load(userptr, "glDrawArraysInstanced");
    glad_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) load(userptr, "glDrawElementsInstanced");
    glad_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) load(userptr, "glGetActiveUniformBlockName");
    glad_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) load(userptr, "glGetActiveUniformBlockiv");
    glad_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) load(userptr, "glGetActiveUniformName");
    glad_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) load(userptr, "glGetActiveUniformsiv");
    glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load(userptr, "glGetIntegeri_v");
    glad_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) load(userptr, "glGetUniformBlockIndex");
    glad_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) load(userptr, "glGetUniformIndices");
    glad_glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC) load(userptr, "glPrimitiveRestartIndex");
    glad_glTexBuffer = (PFNGLTEXBUFFERPROC) load(userptr, "glTexBuffer");
    glad_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) load(userptr, "glUniformBlockBinding");
}
static void glad_gl_load_GL_VERSION_3_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_2) return;
    glad_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) load(userptr, "glClientWaitSync");
    glad_glDeleteSync = (PFNGLDELETESYNCPROC) load(userptr, "glDeleteSync");
    glad_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) load(userptr, "glDrawElementsBaseVertex");
    glad_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) load(userptr, "glDrawElementsInstancedBaseVertex");
    glad_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) load(userptr, "glDrawRangeElementsBaseVertex");
    glad_glFenceSync = (PFNGLFENCESYNCPROC) load(userptr, "glFenceSync");
    glad_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) load(userptr, "glFramebufferTexture");
    glad_glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC) load(userptr, "glGetBufferParameteri64v");
    glad_glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC) load(userptr, "glGetInteger64i_v");
    glad_glGetInteger64v = (PFNGLGETINTEGER64VPROC) load(userptr, "glGetInteger64v");
    glad_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) load(userptr, "glGetMultisamplefv");
    glad_glGetSynciv = (PFNGLGETSYNCIVPROC) load(userptr, "glGetSynciv");
    glad_glIsSync = (PFNGLISSYNCPROC) load(userptr, "glIsSync");
    glad_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) load(userptr, "glMultiDrawElementsBaseVertex");
    glad_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) load(userptr, "glProvokingVertex");
    glad_glSampleMaski = (PFNGLSAMPLEMASKIPROC) load(userptr, "glSampleMaski");
    glad_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) load(userptr, "glTexImage2DMultisample");
    glad_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) load(userptr, "glTexImage3DMultisample");
    glad_glWaitSync = (PFNGLWAITSYNCPROC) load(userptr, "glWaitSync");
}
static void glad_gl_load_GL_VERSION_3_3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_3) return;
    glad_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) load(userptr, "glBindFragDataLocationIndexed");
    glad_glBindSampler = (PFNGLBINDSAMPLERPROC) load(userptr, "glBindSampler");
    glad_glColorP3ui = (PFNGLCOLORP3UIPROC) load(userptr, "glColorP3ui");
    glad_glColorP3uiv = (PFNGLCOLORP3UIVPROC) load(userptr, "glColorP3uiv");
    glad_glColorP4ui = (PFNGLCOLORP4UIPROC) load(userptr, "glColorP4ui");
    glad_glColorP4uiv = (PFNGLCOLORP4UIVPROC) load(userptr, "glColorP4uiv");
    glad_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) load(userptr, "glDeleteSamplers");
    glad_glGenSamplers = (PFNGLGENSAMPLERSPROC) load(userptr, "glGenSamplers");
    glad_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC) load(userptr, "glGetFragDataIndex");
    glad_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) load(userptr, "glGetQueryObjecti64v");
    glad_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC) load(userptr, "glGetQueryObjectui64v");
    glad_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) load(userptr, "glGetSamplerParameterIiv");
    glad_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) load(userptr, "glGetSamplerParameterIuiv");
    glad_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) load(userptr, "glGetSamplerParameterfv");
    glad_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) load(userptr, "glGetSamplerParameteriv");
    glad_glIsSampler = (PFNGLISSAMPLERPROC) load(userptr, "glIsSampler");
    glad_glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC) load(userptr, "glMultiTexCoordP1ui");
    glad_glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC) load(userptr, "glMultiTexCoordP1uiv");
    glad_glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC) load(userptr, "glMultiTexCoordP2ui");
    glad_glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC) load(userptr, "glMultiTexCoordP2uiv");
    glad_glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC) load(userptr, "glMultiTexCoordP3ui");
    glad_glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC) load(userptr, "glMultiTexCoordP3uiv");
    glad_glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC) load(userptr, "glMultiTexCoordP4ui");
    glad_glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC) load(userptr, "glMultiTexCoordP4uiv");
    glad_glNormalP3ui = (PFNGLNORMALP3UIPROC) load(userptr, "glNormalP3ui");
    glad_glNormalP3uiv = (PFNGLNORMALP3UIVPROC) load(userptr, "glNormalP3uiv");
    glad_glQueryCounter = (PFNGLQUERYCOUNTERPROC) load(userptr, "glQueryCounter");
    glad_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) load(userptr, "glSamplerParameterIiv");
    glad_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) load(userptr, "glSamplerParameterIuiv");
    glad_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) load(userptr, "glSamplerParameterf");
    glad_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) load(userptr, "glSamplerParameterfv");
    glad_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) load(userptr, "glSamplerParameteri");
    glad_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) load(userptr, "glSamplerParameteriv");
    glad_glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC) load(userptr, "glSecondaryColorP3ui");
    glad_glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC) load(userptr, "glSecondaryColorP3uiv");
    glad_glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC) load(userptr, "glTexCoordP1ui");
    glad_glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC) load(userptr, "glTexCoordP1uiv");
    glad_glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC) load(userptr, "glTexCoordP2ui");
    glad_glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC) load(userptr, "glTexCoordP2uiv");
    glad_glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC) load(userptr, "glTexCoordP3ui");
    glad_glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC) load(userptr, "glTexCoordP3uiv");
    glad_glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC) load(userptr, "glTexCoordP4ui");
    glad_glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC) load(userptr, "glTexCoordP4uiv");
    glad_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC) load(userptr, "glVertexAttribDivisor");
    glad_glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC) load(userptr, "glVertexAttribP1ui");
    glad_glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC) load(userptr, "glVertexAttribP1uiv");
    glad_glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC) load(userptr, "glVertexAttribP2ui");
    glad_glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC) load(userptr, "glVertexAttribP2uiv");
    glad_glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC) load(userptr, "glVertexAttribP3ui");
    glad_glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC) load(userptr, "glVertexAttribP3uiv");
    glad_glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC) load(userptr, "glVertexAttribP4ui");
    glad_glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC) load(userptr, "glVertexAttribP4uiv");
    glad_glVertexP2ui = (PFNGLVERTEXP2UIPROC) load(userptr, "glVertexP2ui");
    glad_glVertexP2uiv = (PFNGLVERTEXP2UIVPROC) load(userptr, "glVertexP2uiv");
    glad_glVertexP3ui = (PFNGLVERTEXP3UIPROC) load(userptr, "glVertexP3ui");
    glad_glVertexP3uiv = (PFNGLVERTEXP3UIVPROC) load(userptr, "glVertexP3uiv");
    glad_glVertexP4ui = (PFNGLVERTEXP4UIPROC) load(userptr, "glVertexP4ui");
    glad_glVertexP4uiv = (PFNGLVERTEXP4UIVPROC) load(userptr, "glVertexP4uiv");
}
static void glad_gl_load_GL_ARB_ES2_compatibility( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_ES2_compatibility) return;
    glad_glClearDepthf = (PFNGLCLEARDEPTHFPROC) load(userptr, "glClearDepthf");
    glad_glDepthRangef = (PFNGLDEPTHRANGEFPROC) load(userptr, "glDepthRangef");
    glad_glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC) load(userptr, "glGetShaderPrecisionFormat");
    glad_glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC) load(userptr, "glReleaseShaderCompiler");
    glad_glShaderBinary = (PFNGLSHADERBINARYPROC) load(userptr, "glShaderBinary");
}
static void glad_gl_load_GL_ARB_ES3_1_compatibility( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_ES3_1_compatibility) return;
    glad_glMemoryBarrierByRegion = (PFNGLMEMORYBARRIERBYREGIONPROC) load(userptr, "glMemoryBarrierByRegion");
}
static void glad_gl_load_GL_ARB_ES3_2_compatibility( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_ES3_2_compatibility) return;
    glad_glPrimitiveBoundingBox = (PFNGLPRIMITIVEBOUNDINGBOXPROC) load(userptr, "glPrimitiveBoundingBox");
    glad_glPrimitiveBoundingBoxARB = (PFNGLPRIMITIVEBOUNDINGBOXARBPROC) load(userptr, "glPrimitiveBoundingBoxARB");
}
static void glad_gl_load_GL_ARB_base_instance( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_base_instance) return;
    glad_glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) load(userptr, "glDrawArraysInstancedBaseInstance");
    glad_glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) load(userptr, "glDrawElementsInstancedBaseInstance");
    glad_glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) load(userptr, "glDrawElementsInstancedBaseVertexBaseInstance");
}
static void glad_gl_load_GL_ARB_bindless_texture( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_bindless_texture) return;
    glad_glGetImageHandleARB = (PFNGLGETIMAGEHANDLEARBPROC) load(userptr, "glGetImageHandleARB");
    glad_glGetTextureHandleARB = (PFNGLGETTEXTUREHANDLEARBPROC) load(userptr, "glGetTextureHandleARB");
    glad_glGetTextureSamplerHandleARB = (PFNGLGETTEXTURESAMPLERHANDLEARBPROC) load(userptr, "glGetTextureSamplerHandleARB");
    glad_glGetVertexAttribLui64vARB = (PFNGLGETVERTEXATTRIBLUI64VARBPROC) load(userptr, "glGetVertexAttribLui64vARB");
    glad_glIsImageHandleResidentARB = (PFNGLISIMAGEHANDLERESIDENTARBPROC) load(userptr, "glIsImageHandleResidentARB");
    glad_glIsTextureHandleResidentARB = (PFNGLISTEXTUREHANDLERESIDENTARBPROC) load(userptr, "glIsTextureHandleResidentARB");
    glad_glMakeImageHandleNonResidentARB = (PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC) load(userptr, "glMakeImageHandleNonResidentARB");
    glad_glMakeImageHandleResidentARB = (PFNGLMAKEIMAGEHANDLERESIDENTARBPROC) load(userptr, "glMakeImageHandleResidentARB");
    glad_glMakeTextureHandleNonResidentARB = (PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC) load(userptr, "glMakeTextureHandleNonResidentARB");
    glad_glMakeTextureHandleResidentARB = (PFNGLMAKETEXTUREHANDLERESIDENTARBPROC) load(userptr, "glMakeTextureHandleResidentARB");
    glad_glProgramUniformHandleui64ARB = (PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC) load(userptr, "glProgramUniformHandleui64ARB");
    glad_glProgramUniformHandleui64vARB = (PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC) load(userptr, "glProgramUniformHandleui64vARB");
    glad_glUniformHandleui64ARB = (PFNGLUNIFORMHANDLEUI64ARBPROC) load(userptr, "glUniformHandleui64ARB");
    glad_glUniformHandleui64vARB = (PFNGLUNIFORMHANDLEUI64VARBPROC) load(userptr, "glUniformHandleui64vARB");
    glad_glVertexAttribL1ui64ARB = (PFNGLVERTEXATTRIBL1UI64ARBPROC) load(userptr, "glVertexAttribL1ui64ARB");
    glad_glVertexAttribL1ui64vARB = (PFNGLVERTEXATTRIBL1UI64VARBPROC) load(userptr, "glVertexAttribL1ui64vARB");
}
static void glad_gl_load_GL_ARB_blend_func_extended( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_blend_func_extended) return;
    glad_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) load(userptr, "glBindFragDataLocationIndexed");
    glad_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC) load(userptr, "glGetFragDataIndex");
}
static void glad_gl_load_GL_ARB_buffer_storage( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_buffer_storage) return;
    glad_glBufferStorage = (PFNGLBUFFERSTORAGEPROC) load(userptr, "glBufferStorage");
}
static void glad_gl_load_GL_ARB_cl_event( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_cl_event) return;
    glad_glCreateSyncFromCLeventARB = (PFNGLCREATESYNCFROMCLEVENTARBPROC) load(userptr, "glCreateSyncFromCLeventARB");
}
static void glad_gl_load_GL_ARB_clear_buffer_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_clear_buffer_object) return;
    glad_glClearBufferData = (PFNGLCLEARBUFFERDATAPROC) load(userptr, "glClearBufferData");
    glad_glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC) load(userptr, "glClearBufferSubData");
}
static void glad_gl_load_GL_ARB_clear_texture( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_clear_texture) return;
    glad_glClearTexImage = (PFNGLCLEARTEXIMAGEPROC) load(userptr, "glClearTexImage");
    glad_glClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC) load(userptr, "glClearTexSubImage");
}
static void glad_gl_load_GL_ARB_clip_control( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_clip_control) return;
    glad_glClipControl = (PFNGLCLIPCONTROLPROC) load(userptr, "glClipControl");
}
static void glad_gl_load_GL_ARB_color_buffer_float( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_color_buffer_float) return;
    glad_glClampColor = (PFNGLCLAMPCOLORPROC) load(userptr, "glClampColor");
    glad_glClampColorARB = (PFNGLCLAMPCOLORARBPROC) load(userptr, "glClampColorARB");
}
static void glad_gl_load_GL_ARB_compute_shader( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_compute_shader) return;
    glad_glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC) load(userptr, "glDispatchCompute");
    glad_glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC) load(userptr, "glDispatchComputeIndirect");
}
static void glad_gl_load_GL_ARB_compute_variable_group_size( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_compute_variable_group_size) return;
    glad_glDispatchComputeGroupSizeARB = (PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC) load(userptr, "glDispatchComputeGroupSizeARB");
}
static void glad_gl_load_GL_ARB_copy_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_copy_buffer) return;
    glad_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) load(userptr, "glCopyBufferSubData");
}
static void glad_gl_load_GL_ARB_copy_image( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_copy_image) return;
    glad_glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC) load(userptr, "glCopyImageSubData");
}
static void glad_gl_load_GL_ARB_debug_output( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_debug_output) return;
    glad_glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC) load(userptr, "glDebugMessageCallback");
    glad_glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC) load(userptr, "glDebugMessageCallbackARB");
    glad_glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC) load(userptr, "glDebugMessageControl");
    glad_glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC) load(userptr, "glDebugMessageControlARB");
    glad_glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC) load(userptr, "glDebugMessageInsert");
    glad_glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC) load(userptr, "glDebugMessageInsertARB");
    glad_glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC) load(userptr, "glGetDebugMessageLog");
    glad_glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC) load(userptr, "glGetDebugMessageLogARB");
}
static void glad_gl_load_GL_ARB_direct_state_access( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_direct_state_access) return;
    glad_glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC) load(userptr, "glBindTextureUnit");
    glad_glBlitNamedFramebuffer = (PFNGLBLITNAMEDFRAMEBUFFERPROC) load(userptr, "glBlitNamedFramebuffer");
    glad_glCheckNamedFramebufferStatus = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC) load(userptr, "glCheckNamedFramebufferStatus");
    glad_glClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC) load(userptr, "glClearNamedBufferData");
    glad_glClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC) load(userptr, "glClearNamedBufferSubData");
    glad_glClearNamedFramebufferfi = (PFNGLCLEARNAMEDFRAMEBUFFERFIPROC) load(userptr, "glClearNamedFramebufferfi");
    glad_glClearNamedFramebufferfv = (PFNGLCLEARNAMEDFRAMEBUFFERFVPROC) load(userptr, "glClearNamedFramebufferfv");
    glad_glClearNamedFramebufferiv = (PFNGLCLEARNAMEDFRAMEBUFFERIVPROC) load(userptr, "glClearNamedFramebufferiv");
    glad_glClearNamedFramebufferuiv = (PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC) load(userptr, "glClearNamedFramebufferuiv");
    glad_glCompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC) load(userptr, "glCompressedTextureSubImage1D");
    glad_glCompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC) load(userptr, "glCompressedTextureSubImage2D");
    glad_glCompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC) load(userptr, "glCompressedTextureSubImage3D");
    glad_glCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC) load(userptr, "glCopyNamedBufferSubData");
    glad_glCopyTextureSubImage1D = (PFNGLCOPYTEXTURESUBIMAGE1DPROC) load(userptr, "glCopyTextureSubImage1D");
    glad_glCopyTextureSubImage2D = (PFNGLCOPYTEXTURESUBIMAGE2DPROC) load(userptr, "glCopyTextureSubImage2D");
    glad_glCopyTextureSubImage3D = (PFNGLCOPYTEXTURESUBIMAGE3DPROC) load(userptr, "glCopyTextureSubImage3D");
    glad_glCreateBuffers = (PFNGLCREATEBUFFERSPROC) load(userptr, "glCreateBuffers");
    glad_glCreateFramebuffers = (PFNGLCREATEFRAMEBUFFERSPROC) load(userptr, "glCreateFramebuffers");
    glad_glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC) load(userptr, "glCreateProgramPipelines");
    glad_glCreateQueries = (PFNGLCREATEQUERIESPROC) load(userptr, "glCreateQueries");
    glad_glCreateRenderbuffers = (PFNGLCREATERENDERBUFFERSPROC) load(userptr, "glCreateRenderbuffers");
    glad_glCreateSamplers = (PFNGLCREATESAMPLERSPROC) load(userptr, "glCreateSamplers");
    glad_glCreateTextures = (PFNGLCREATETEXTURESPROC) load(userptr, "glCreateTextures");
    glad_glCreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKSPROC) load(userptr, "glCreateTransformFeedbacks");
    glad_glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC) load(userptr, "glCreateVertexArrays");
    glad_glDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC) load(userptr, "glDisableVertexArrayAttrib");
    glad_glEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC) load(userptr, "glEnableVertexArrayAttrib");
    glad_glFlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC) load(userptr, "glFlushMappedNamedBufferRange");
    glad_glGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC) load(userptr, "glGenerateTextureMipmap");
    glad_glGetCompressedTextureImage = (PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC) load(userptr, "glGetCompressedTextureImage");
    glad_glGetNamedBufferParameteri64v = (PFNGLGETNAMEDBUFFERPARAMETERI64VPROC) load(userptr, "glGetNamedBufferParameteri64v");
    glad_glGetNamedBufferParameteriv = (PFNGLGETNAMEDBUFFERPARAMETERIVPROC) load(userptr, "glGetNamedBufferParameteriv");
    glad_glGetNamedBufferPointerv = (PFNGLGETNAMEDBUFFERPOINTERVPROC) load(userptr, "glGetNamedBufferPointerv");
    glad_glGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC) load(userptr, "glGetNamedBufferSubData");
    glad_glGetNamedFramebufferAttachmentParameteriv = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC) load(userptr, "glGetNamedFramebufferAttachmentParameteriv");
    glad_glGetNamedFramebufferParameteriv = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC) load(userptr, "glGetNamedFramebufferParameteriv");
    glad_glGetNamedRenderbufferParameteriv = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC) load(userptr, "glGetNamedRenderbufferParameteriv");
    glad_glGetQueryBufferObjecti64v = (PFNGLGETQUERYBUFFEROBJECTI64VPROC) load(userptr, "glGetQueryBufferObjecti64v");
    glad_glGetQueryBufferObjectiv = (PFNGLGETQUERYBUFFEROBJECTIVPROC) load(userptr, "glGetQueryBufferObjectiv");
    glad_glGetQueryBufferObjectui64v = (PFNGLGETQUERYBUFFEROBJECTUI64VPROC) load(userptr, "glGetQueryBufferObjectui64v");
    glad_glGetQueryBufferObjectuiv = (PFNGLGETQUERYBUFFEROBJECTUIVPROC) load(userptr, "glGetQueryBufferObjectuiv");
    glad_glGetTextureImage = (PFNGLGETTEXTUREIMAGEPROC) load(userptr, "glGetTextureImage");
    glad_glGetTextureLevelParameterfv = (PFNGLGETTEXTURELEVELPARAMETERFVPROC) load(userptr, "glGetTextureLevelParameterfv");
    glad_glGetTextureLevelParameteriv = (PFNGLGETTEXTURELEVELPARAMETERIVPROC) load(userptr, "glGetTextureLevelParameteriv");
    glad_glGetTextureParameterIiv = (PFNGLGETTEXTUREPARAMETERIIVPROC) load(userptr, "glGetTextureParameterIiv");
    glad_glGetTextureParameterIuiv = (PFNGLGETTEXTUREPARAMETERIUIVPROC) load(userptr, "glGetTextureParameterIuiv");
    glad_glGetTextureParameterfv = (PFNGLGETTEXTUREPARAMETERFVPROC) load(userptr, "glGetTextureParameterfv");
    glad_glGetTextureParameteriv = (PFNGLGETTEXTUREPARAMETERIVPROC) load(userptr, "glGetTextureParameteriv");
    glad_glGetTransformFeedbacki64_v = (PFNGLGETTRANSFORMFEEDBACKI64_VPROC) load(userptr, "glGetTransformFeedbacki64_v");
    glad_glGetTransformFeedbacki_v = (PFNGLGETTRANSFORMFEEDBACKI_VPROC) load(userptr, "glGetTransformFeedbacki_v");
    glad_glGetTransformFeedbackiv = (PFNGLGETTRANSFORMFEEDBACKIVPROC) load(userptr, "glGetTransformFeedbackiv");
    glad_glGetVertexArrayIndexed64iv = (PFNGLGETVERTEXARRAYINDEXED64IVPROC) load(userptr, "glGetVertexArrayIndexed64iv");
    glad_glGetVertexArrayIndexediv = (PFNGLGETVERTEXARRAYINDEXEDIVPROC) load(userptr, "glGetVertexArrayIndexediv");
    glad_glGetVertexArrayiv = (PFNGLGETVERTEXARRAYIVPROC) load(userptr, "glGetVertexArrayiv");
    glad_glInvalidateNamedFramebufferData = (PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC) load(userptr, "glInvalidateNamedFramebufferData");
    glad_glInvalidateNamedFramebufferSubData = (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC) load(userptr, "glInvalidateNamedFramebufferSubData");
    glad_glMapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC) load(userptr, "glMapNamedBuffer");
    glad_glMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC) load(userptr, "glMapNamedBufferRange");
    glad_glNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC) load(userptr, "glNamedBufferData");
    glad_glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC) load(userptr, "glNamedBufferStorage");
    glad_glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC) load(userptr, "glNamedBufferSubData");
    glad_glNamedFramebufferDrawBuffer = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC) load(userptr, "glNamedFramebufferDrawBuffer");
    glad_glNamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC) load(userptr, "glNamedFramebufferDrawBuffers");
    glad_glNamedFramebufferParameteri = (PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC) load(userptr, "glNamedFramebufferParameteri");
    glad_glNamedFramebufferReadBuffer = (PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC) load(userptr, "glNamedFramebufferReadBuffer");
    glad_glNamedFramebufferRenderbuffer = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC) load(userptr, "glNamedFramebufferRenderbuffer");
    glad_glNamedFramebufferTexture = (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC) load(userptr, "glNamedFramebufferTexture");
    glad_glNamedFramebufferTextureLayer = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC) load(userptr, "glNamedFramebufferTextureLayer");
    glad_glNamedRenderbufferStorage = (PFNGLNAMEDRENDERBUFFERSTORAGEPROC) load(userptr, "glNamedRenderbufferStorage");
    glad_glNamedRenderbufferStorageMultisample = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC) load(userptr, "glNamedRenderbufferStorageMultisample");
    glad_glTextureBuffer = (PFNGLTEXTUREBUFFERPROC) load(userptr, "glTextureBuffer");
    glad_glTextureBufferRange = (PFNGLTEXTUREBUFFERRANGEPROC) load(userptr, "glTextureBufferRange");
    glad_glTextureParameterIiv = (PFNGLTEXTUREPARAMETERIIVPROC) load(userptr, "glTextureParameterIiv");
    glad_glTextureParameterIuiv = (PFNGLTEXTUREPARAMETERIUIVPROC) load(userptr, "glTextureParameterIuiv");
    glad_glTextureParameterf = (PFNGLTEXTUREPARAMETERFPROC) load(userptr, "glTextureParameterf");
    glad_glTextureParameterfv = (PFNGLTEXTUREPARAMETERFVPROC) load(userptr, "glTextureParameterfv");
    glad_glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC) load(userptr, "glTextureParameteri");
    glad_glTextureParameteriv = (PFNGLTEXTUREPARAMETERIVPROC) load(userptr, "glTextureParameteriv");
    glad_glTextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC) load(userptr, "glTextureStorage1D");
    glad_glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC) load(userptr, "glTextureStorage2D");
    glad_glTextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC) load(userptr, "glTextureStorage2DMultisample");
    glad_glTextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC) load(userptr, "glTextureStorage3D");
    glad_glTextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC) load(userptr, "glTextureStorage3DMultisample");
    glad_glTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC) load(userptr, "glTextureSubImage1D");
    glad_glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC) load(userptr, "glTextureSubImage2D");
    glad_glTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC) load(userptr, "glTextureSubImage3D");
    glad_glTransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC) load(userptr, "glTransformFeedbackBufferBase");
    glad_glTransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC) load(userptr, "glTransformFeedbackBufferRange");
    glad_glUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC) load(userptr, "glUnmapNamedBuffer");
    glad_glVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC) load(userptr, "glVertexArrayAttribBinding");
    glad_glVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC) load(userptr, "glVertexArrayAttribFormat");
    glad_glVertexArrayAttribIFormat = (PFNGLVERTEXARRAYATTRIBIFORMATPROC) load(userptr, "glVertexArrayAttribIFormat");
    glad_glVertexArrayAttribLFormat = (PFNGLVERTEXARRAYATTRIBLFORMATPROC) load(userptr, "glVertexArrayAttribLFormat");
    glad_glVertexArrayBindingDivisor = (PFNGLVERTEXARRAYBINDINGDIVISORPROC) load(userptr, "glVertexArrayBindingDivisor");
    glad_glVertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFERPROC) load(userptr, "glVertexArrayElementBuffer");
    glad_glVertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFERPROC) load(userptr, "glVertexArrayVertexBuffer");
    glad_glVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC) load(userptr, "glVertexArrayVertexBuffers");
}
static void glad_gl_load_GL_ARB_draw_buffers( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_draw_buffers) return;
    glad_glDrawBuffers = (PFNGLDRAWBUFFERSPROC) load(userptr, "glDrawBuffers");
    glad_glDrawBuffersARB = (PFNGLDRAWBUFFERSARBPROC) load(userptr, "glDrawBuffersARB");
}
static void glad_gl_load_GL_ARB_draw_buffers_blend( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_draw_buffers_blend) return;
    glad_glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC) load(userptr, "glBlendEquationSeparatei");
    glad_glBlendEquationSeparateiARB = (PFNGLBLENDEQUATIONSEPARATEIARBPROC) load(userptr, "glBlendEquationSeparateiARB");
    glad_glBlendEquationi = (PFNGLBLENDEQUATIONIPROC) load(userptr, "glBlendEquationi");
    glad_glBlendEquationiARB = (PFNGLBLENDEQUATIONIARBPROC) load(userptr, "glBlendEquationiARB");
    glad_glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC) load(userptr, "glBlendFuncSeparatei");
    glad_glBlendFuncSeparateiARB = (PFNGLBLENDFUNCSEPARATEIARBPROC) load(userptr, "glBlendFuncSeparateiARB");
    glad_glBlendFunci = (PFNGLBLENDFUNCIPROC) load(userptr, "glBlendFunci");
    glad_glBlendFunciARB = (PFNGLBLENDFUNCIARBPROC) load(userptr, "glBlendFunciARB");
}
static void glad_gl_load_GL_ARB_draw_elements_base_vertex( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_draw_elements_base_vertex) return;
    glad_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) load(userptr, "glDrawElementsBaseVertex");
    glad_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) load(userptr, "glDrawElementsInstancedBaseVertex");
    glad_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) load(userptr, "glDrawRangeElementsBaseVertex");
    glad_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) load(userptr, "glMultiDrawElementsBaseVertex");
}
static void glad_gl_load_GL_ARB_draw_indirect( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_draw_indirect) return;
    glad_glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC) load(userptr, "glDrawArraysIndirect");
    glad_glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC) load(userptr, "glDrawElementsIndirect");
}
static void glad_gl_load_GL_ARB_draw_instanced( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_draw_instanced) return;
    glad_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) load(userptr, "glDrawArraysInstanced");
    glad_glDrawArraysInstancedARB = (PFNGLDRAWARRAYSINSTANCEDARBPROC) load(userptr, "glDrawArraysInstancedARB");
    glad_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) load(userptr, "glDrawElementsInstanced");
    glad_glDrawElementsInstancedARB = (PFNGLDRAWELEMENTSINSTANCEDARBPROC) load(userptr, "glDrawElementsInstancedARB");
}
static void glad_gl_load_GL_ARB_fragment_program( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_fragment_program) return;
    glad_glBindProgramARB = (PFNGLBINDPROGRAMARBPROC) load(userptr, "glBindProgramARB");
    glad_glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC) load(userptr, "glDeleteProgramsARB");
    glad_glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC) load(userptr, "glGenProgramsARB");
    glad_glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC) load(userptr, "glGetProgramEnvParameterdvARB");
    glad_glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC) load(userptr, "glGetProgramEnvParameterfvARB");
    glad_glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) load(userptr, "glGetProgramLocalParameterdvARB");
    glad_glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) load(userptr, "glGetProgramLocalParameterfvARB");
    glad_glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARBPROC) load(userptr, "glGetProgramStringARB");
    glad_glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC) load(userptr, "glGetProgramivARB");
    glad_glIsProgramARB = (PFNGLISPROGRAMARBPROC) load(userptr, "glIsProgramARB");
    glad_glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARBPROC) load(userptr, "glProgramEnvParameter4dARB");
    glad_glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARBPROC) load(userptr, "glProgramEnvParameter4dvARB");
    glad_glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARBPROC) load(userptr, "glProgramEnvParameter4fARB");
    glad_glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARBPROC) load(userptr, "glProgramEnvParameter4fvARB");
    glad_glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC) load(userptr, "glProgramLocalParameter4dARB");
    glad_glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) load(userptr, "glProgramLocalParameter4dvARB");
    glad_glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC) load(userptr, "glProgramLocalParameter4fARB");
    glad_glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) load(userptr, "glProgramLocalParameter4fvARB");
    glad_glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC) load(userptr, "glProgramStringARB");
}
static void glad_gl_load_GL_ARB_framebuffer_no_attachments( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_framebuffer_no_attachments) return;
    glad_glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC) load(userptr, "glFramebufferParameteri");
    glad_glGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC) load(userptr, "glGetFramebufferParameteriv");
}
static void glad_gl_load_GL_ARB_framebuffer_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_framebuffer_object) return;
    glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) load(userptr, "glBindFramebuffer");
    glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) load(userptr, "glBindRenderbuffer");
    glad_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) load(userptr, "glBlitFramebuffer");
    glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) load(userptr, "glCheckFramebufferStatus");
    glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) load(userptr, "glDeleteFramebuffers");
    glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) load(userptr, "glDeleteRenderbuffers");
    glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) load(userptr, "glFramebufferRenderbuffer");
    glad_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) load(userptr, "glFramebufferTexture1D");
    glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) load(userptr, "glFramebufferTexture2D");
    glad_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) load(userptr, "glFramebufferTexture3D");
    glad_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) load(userptr, "glFramebufferTextureLayer");
    glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) load(userptr, "glGenFramebuffers");
    glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) load(userptr, "glGenRenderbuffers");
    glad_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) load(userptr, "glGenerateMipmap");
    glad_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) load(userptr, "glGetFramebufferAttachmentParameteriv");
    glad_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) load(userptr, "glGetRenderbufferParameteriv");
    glad_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) load(userptr, "glIsFramebuffer");
    glad_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) load(userptr, "glIsRenderbuffer");
    glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) load(userptr, "glRenderbufferStorage");
    glad_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) load(userptr, "glRenderbufferStorageMultisample");
}
static void glad_gl_load_GL_ARB_geometry_shader4( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_geometry_shader4) return;
    glad_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) load(userptr, "glFramebufferTexture");
    glad_glFramebufferTextureARB = (PFNGLFRAMEBUFFERTEXTUREARBPROC) load(userptr, "glFramebufferTextureARB");
    glad_glFramebufferTextureFaceARB = (PFNGLFRAMEBUFFERTEXTUREFACEARBPROC) load(userptr, "glFramebufferTextureFaceARB");
    glad_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) load(userptr, "glFramebufferTextureLayer");
    glad_glFramebufferTextureLayerARB = (PFNGLFRAMEBUFFERTEXTURELAYERARBPROC) load(userptr, "glFramebufferTextureLayerARB");
    glad_glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC) load(userptr, "glProgramParameteri");
    glad_glProgramParameteriARB = (PFNGLPROGRAMPARAMETERIARBPROC) load(userptr, "glProgramParameteriARB");
}
static void glad_gl_load_GL_ARB_get_program_binary( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_get_program_binary) return;
    glad_glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC) load(userptr, "glGetProgramBinary");
    glad_glProgramBinary = (PFNGLPROGRAMBINARYPROC) load(userptr, "glProgramBinary");
    glad_glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC) load(userptr, "glProgramParameteri");
}
static void glad_gl_load_GL_ARB_get_texture_sub_image( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_get_texture_sub_image) return;
    glad_glGetCompressedTextureSubImage = (PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC) load(userptr, "glGetCompressedTextureSubImage");
    glad_glGetTextureSubImage = (PFNGLGETTEXTURESUBIMAGEPROC) load(userptr, "glGetTextureSubImage");
}
static void glad_gl_load_GL_ARB_gl_spirv( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_gl_spirv) return;
    glad_glSpecializeShader = (PFNGLSPECIALIZESHADERPROC) load(userptr, "glSpecializeShader");
    glad_glSpecializeShaderARB = (PFNGLSPECIALIZESHADERARBPROC) load(userptr, "glSpecializeShaderARB");
}
static void glad_gl_load_GL_ARB_gpu_shader_fp64( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_gpu_shader_fp64) return;
    glad_glGetUniformdv = (PFNGLGETUNIFORMDVPROC) load(userptr, "glGetUniformdv");
    glad_glUniform1d = (PFNGLUNIFORM1DPROC) load(userptr, "glUniform1d");
    glad_glUniform1dv = (PFNGLUNIFORM1DVPROC) load(userptr, "glUniform1dv");
    glad_glUniform2d = (PFNGLUNIFORM2DPROC) load(userptr, "glUniform2d");
    glad_glUniform2dv = (PFNGLUNIFORM2DVPROC) load(userptr, "glUniform2dv");
    glad_glUniform3d = (PFNGLUNIFORM3DPROC) load(userptr, "glUniform3d");
    glad_glUniform3dv = (PFNGLUNIFORM3DVPROC) load(userptr, "glUniform3dv");
    glad_glUniform4d = (PFNGLUNIFORM4DPROC) load(userptr, "glUniform4d");
    glad_glUniform4dv = (PFNGLUNIFORM4DVPROC) load(userptr, "glUniform4dv");
    glad_glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC) load(userptr, "glUniformMatrix2dv");
    glad_glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC) load(userptr, "glUniformMatrix2x3dv");
    glad_glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC) load(userptr, "glUniformMatrix2x4dv");
    glad_glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC) load(userptr, "glUniformMatrix3dv");
    glad_glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC) load(userptr, "glUniformMatrix3x2dv");
    glad_glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC) load(userptr, "glUniformMatrix3x4dv");
    glad_glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC) load(userptr, "glUniformMatrix4dv");
    glad_glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC) load(userptr, "glUniformMatrix4x2dv");
    glad_glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC) load(userptr, "glUniformMatrix4x3dv");
}
static void glad_gl_load_GL_ARB_gpu_shader_int64( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_gpu_shader_int64) return;
    glad_glGetUniformi64vARB = (PFNGLGETUNIFORMI64VARBPROC) load(userptr, "glGetUniformi64vARB");
    glad_glGetUniformui64vARB = (PFNGLGETUNIFORMUI64VARBPROC) load(userptr, "glGetUniformui64vARB");
    glad_glGetnUniformi64vARB = (PFNGLGETNUNIFORMI64VARBPROC) load(userptr, "glGetnUniformi64vARB");
    glad_glGetnUniformui64vARB = (PFNGLGETNUNIFORMUI64VARBPROC) load(userptr, "glGetnUniformui64vARB");
    glad_glProgramUniform1i64ARB = (PFNGLPROGRAMUNIFORM1I64ARBPROC) load(userptr, "glProgramUniform1i64ARB");
    glad_glProgramUniform1i64vARB = (PFNGLPROGRAMUNIFORM1I64VARBPROC) load(userptr, "glProgramUniform1i64vARB");
    glad_glProgramUniform1ui64ARB = (PFNGLPROGRAMUNIFORM1UI64ARBPROC) load(userptr, "glProgramUniform1ui64ARB");
    glad_glProgramUniform1ui64vARB = (PFNGLPROGRAMUNIFORM1UI64VARBPROC) load(userptr, "glProgramUniform1ui64vARB");
    glad_glProgramUniform2i64ARB = (PFNGLPROGRAMUNIFORM2I64ARBPROC) load(userptr, "glProgramUniform2i64ARB");
    glad_glProgramUniform2i64vARB = (PFNGLPROGRAMUNIFORM2I64VARBPROC) load(userptr, "glProgramUniform2i64vARB");
    glad_glProgramUniform2ui64ARB = (PFNGLPROGRAMUNIFORM2UI64ARBPROC) load(userptr, "glProgramUniform2ui64ARB");
    glad_glProgramUniform2ui64vARB = (PFNGLPROGRAMUNIFORM2UI64VARBPROC) load(userptr, "glProgramUniform2ui64vARB");
    glad_glProgramUniform3i64ARB = (PFNGLPROGRAMUNIFORM3I64ARBPROC) load(userptr, "glProgramUniform3i64ARB");
    glad_glProgramUniform3i64vARB = (PFNGLPROGRAMUNIFORM3I64VARBPROC) load(userptr, "glProgramUniform3i64vARB");
    glad_glProgramUniform3ui64ARB = (PFNGLPROGRAMUNIFORM3UI64ARBPROC) load(userptr, "glProgramUniform3ui64ARB");
    glad_glProgramUniform3ui64vARB = (PFNGLPROGRAMUNIFORM3UI64VARBPROC) load(userptr, "glProgramUniform3ui64vARB");
    glad_glProgramUniform4i64ARB = (PFNGLPROGRAMUNIFORM4I64ARBPROC) load(userptr, "glProgramUniform4i64ARB");
    glad_glProgramUniform4i64vARB = (PFNGLPROGRAMUNIFORM4I64VARBPROC) load(userptr, "glProgramUniform4i64vARB");
    glad_glProgramUniform4ui64ARB = (PFNGLPROGRAMUNIFORM4UI64ARBPROC) load(userptr, "glProgramUniform4ui64ARB");
    glad_glProgramUniform4ui64vARB = (PFNGLPROGRAMUNIFORM4UI64VARBPROC) load(userptr, "glProgramUniform4ui64vARB");
    glad_glUniform1i64ARB = (PFNGLUNIFORM1I64ARBPROC) load(userptr, "glUniform1i64ARB");
    glad_glUniform1i64vARB = (PFNGLUNIFORM1I64VARBPROC) load(userptr, "glUniform1i64vARB");
    glad_glUniform1ui64ARB = (PFNGLUNIFORM1UI64ARBPROC) load(userptr, "glUniform1ui64ARB");
    glad_glUniform1ui64vARB = (PFNGLUNIFORM1UI64VARBPROC) load(userptr, "glUniform1ui64vARB");
    glad_glUniform2i64ARB = (PFNGLUNIFORM2I64ARBPROC) load(userptr, "glUniform2i64ARB");
    glad_glUniform2i64vARB = (PFNGLUNIFORM2I64VARBPROC) load(userptr, "glUniform2i64vARB");
    glad_glUniform2ui64ARB = (PFNGLUNIFORM2UI64ARBPROC) load(userptr, "glUniform2ui64ARB");
    glad_glUniform2ui64vARB = (PFNGLUNIFORM2UI64VARBPROC) load(userptr, "glUniform2ui64vARB");
    glad_glUniform3i64ARB = (PFNGLUNIFORM3I64ARBPROC) load(userptr, "glUniform3i64ARB");
    glad_glUniform3i64vARB = (PFNGLUNIFORM3I64VARBPROC) load(userptr, "glUniform3i64vARB");
    glad_glUniform3ui64ARB = (PFNGLUNIFORM3UI64ARBPROC) load(userptr, "glUniform3ui64ARB");
    glad_glUniform3ui64vARB = (PFNGLUNIFORM3UI64VARBPROC) load(userptr, "glUniform3ui64vARB");
    glad_glUniform4i64ARB = (PFNGLUNIFORM4I64ARBPROC) load(userptr, "glUniform4i64ARB");
    glad_glUniform4i64vARB = (PFNGLUNIFORM4I64VARBPROC) load(userptr, "glUniform4i64vARB");
    glad_glUniform4ui64ARB = (PFNGLUNIFORM4UI64ARBPROC) load(userptr, "glUniform4ui64ARB");
    glad_glUniform4ui64vARB = (PFNGLUNIFORM4UI64VARBPROC) load(userptr, "glUniform4ui64vARB");
}
static void glad_gl_load_GL_ARB_imaging( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_imaging) return;
    glad_glBlendColor = (PFNGLBLENDCOLORPROC) load(userptr, "glBlendColor");
    glad_glBlendEquation = (PFNGLBLENDEQUATIONPROC) load(userptr, "glBlendEquation");
    glad_glColorSubTable = (PFNGLCOLORSUBTABLEPROC) load(userptr, "glColorSubTable");
    glad_glColorTable = (PFNGLCOLORTABLEPROC) load(userptr, "glColorTable");
    glad_glColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFVPROC) load(userptr, "glColorTableParameterfv");
    glad_glColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIVPROC) load(userptr, "glColorTableParameteriv");
    glad_glConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1DPROC) load(userptr, "glConvolutionFilter1D");
    glad_glConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2DPROC) load(userptr, "glConvolutionFilter2D");
    glad_glConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERFPROC) load(userptr, "glConvolutionParameterf");
    glad_glConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFVPROC) load(userptr, "glConvolutionParameterfv");
    glad_glConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERIPROC) load(userptr, "glConvolutionParameteri");
    glad_glConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIVPROC) load(userptr, "glConvolutionParameteriv");
    glad_glCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLEPROC) load(userptr, "glCopyColorSubTable");
    glad_glCopyColorTable = (PFNGLCOPYCOLORTABLEPROC) load(userptr, "glCopyColorTable");
    glad_glCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1DPROC) load(userptr, "glCopyConvolutionFilter1D");
    glad_glCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2DPROC) load(userptr, "glCopyConvolutionFilter2D");
    glad_glGetColorTable = (PFNGLGETCOLORTABLEPROC) load(userptr, "glGetColorTable");
    glad_glGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFVPROC) load(userptr, "glGetColorTableParameterfv");
    glad_glGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIVPROC) load(userptr, "glGetColorTableParameteriv");
    glad_glGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTERPROC) load(userptr, "glGetConvolutionFilter");
    glad_glGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFVPROC) load(userptr, "glGetConvolutionParameterfv");
    glad_glGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIVPROC) load(userptr, "glGetConvolutionParameteriv");
    glad_glGetHistogram = (PFNGLGETHISTOGRAMPROC) load(userptr, "glGetHistogram");
    glad_glGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFVPROC) load(userptr, "glGetHistogramParameterfv");
    glad_glGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIVPROC) load(userptr, "glGetHistogramParameteriv");
    glad_glGetMinmax = (PFNGLGETMINMAXPROC) load(userptr, "glGetMinmax");
    glad_glGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFVPROC) load(userptr, "glGetMinmaxParameterfv");
    glad_glGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIVPROC) load(userptr, "glGetMinmaxParameteriv");
    glad_glGetSeparableFilter = (PFNGLGETSEPARABLEFILTERPROC) load(userptr, "glGetSeparableFilter");
    glad_glHistogram = (PFNGLHISTOGRAMPROC) load(userptr, "glHistogram");
    glad_glMinmax = (PFNGLMINMAXPROC) load(userptr, "glMinmax");
    glad_glResetHistogram = (PFNGLRESETHISTOGRAMPROC) load(userptr, "glResetHistogram");
    glad_glResetMinmax = (PFNGLRESETMINMAXPROC) load(userptr, "glResetMinmax");
    glad_glSeparableFilter2D = (PFNGLSEPARABLEFILTER2DPROC) load(userptr, "glSeparableFilter2D");
}
static void glad_gl_load_GL_ARB_indirect_parameters( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_indirect_parameters) return;
    glad_glMultiDrawArraysIndirectCount = (PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC) load(userptr, "glMultiDrawArraysIndirectCount");
    glad_glMultiDrawArraysIndirectCountARB = (PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC) load(userptr, "glMultiDrawArraysIndirectCountARB");
    glad_glMultiDrawElementsIndirectCount = (PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC) load(userptr, "glMultiDrawElementsIndirectCount");
    glad_glMultiDrawElementsIndirectCountARB = (PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC) load(userptr, "glMultiDrawElementsIndirectCountARB");
}
static void glad_gl_load_GL_ARB_instanced_arrays( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_instanced_arrays) return;
    glad_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC) load(userptr, "glVertexAttribDivisor");
    glad_glVertexAttribDivisorARB = (PFNGLVERTEXATTRIBDIVISORARBPROC) load(userptr, "glVertexAttribDivisorARB");
}
static void glad_gl_load_GL_ARB_internalformat_query( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_internalformat_query) return;
    glad_glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC) load(userptr, "glGetInternalformativ");
}
static void glad_gl_load_GL_ARB_internalformat_query2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_internalformat_query2) return;
    glad_glGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC) load(userptr, "glGetInternalformati64v");
}
static void glad_gl_load_GL_ARB_invalidate_subdata( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_invalidate_subdata) return;
    glad_glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC) load(userptr, "glInvalidateBufferData");
    glad_glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC) load(userptr, "glInvalidateBufferSubData");
    glad_glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC) load(userptr, "glInvalidateFramebuffer");
    glad_glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC) load(userptr, "glInvalidateSubFramebuffer");
    glad_glInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC) load(userptr, "glInvalidateTexImage");
    glad_glInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC) load(userptr, "glInvalidateTexSubImage");
}
static void glad_gl_load_GL_ARB_map_buffer_range( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_map_buffer_range) return;
    glad_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) load(userptr, "glFlushMappedBufferRange");
    glad_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) load(userptr, "glMapBufferRange");
}
static void glad_gl_load_GL_ARB_matrix_palette( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_matrix_palette) return;
    glad_glCurrentPaletteMatrixARB = (PFNGLCURRENTPALETTEMATRIXARBPROC) load(userptr, "glCurrentPaletteMatrixARB");
    glad_glMatrixIndexPointerARB = (PFNGLMATRIXINDEXPOINTERARBPROC) load(userptr, "glMatrixIndexPointerARB");
    glad_glMatrixIndexubvARB = (PFNGLMATRIXINDEXUBVARBPROC) load(userptr, "glMatrixIndexubvARB");
    glad_glMatrixIndexuivARB = (PFNGLMATRIXINDEXUIVARBPROC) load(userptr, "glMatrixIndexuivARB");
    glad_glMatrixIndexusvARB = (PFNGLMATRIXINDEXUSVARBPROC) load(userptr, "glMatrixIndexusvARB");
}
static void glad_gl_load_GL_ARB_multi_bind( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_multi_bind) return;
    glad_glBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC) load(userptr, "glBindBuffersBase");
    glad_glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC) load(userptr, "glBindBuffersRange");
    glad_glBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC) load(userptr, "glBindImageTextures");
    glad_glBindSamplers = (PFNGLBINDSAMPLERSPROC) load(userptr, "glBindSamplers");
    glad_glBindTextures = (PFNGLBINDTEXTURESPROC) load(userptr, "glBindTextures");
    glad_glBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC) load(userptr, "glBindVertexBuffers");
}
static void glad_gl_load_GL_ARB_multi_draw_indirect( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_multi_draw_indirect) return;
    glad_glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC) load(userptr, "glMultiDrawArraysIndirect");
    glad_glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC) load(userptr, "glMultiDrawElementsIndirect");
}
static void glad_gl_load_GL_ARB_multisample( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_multisample) return;
    glad_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) load(userptr, "glSampleCoverage");
    glad_glSampleCoverageARB = (PFNGLSAMPLECOVERAGEARBPROC) load(userptr, "glSampleCoverageARB");
}
static void glad_gl_load_GL_ARB_multitexture( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_multitexture) return;
    glad_glActiveTexture = (PFNGLACTIVETEXTUREPROC) load(userptr, "glActiveTexture");
    glad_glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) load(userptr, "glActiveTextureARB");
    glad_glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) load(userptr, "glClientActiveTexture");
    glad_glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) load(userptr, "glClientActiveTextureARB");
    glad_glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC) load(userptr, "glMultiTexCoord1d");
    glad_glMultiTexCoord1dARB = (PFNGLMULTITEXCOORD1DARBPROC) load(userptr, "glMultiTexCoord1dARB");
    glad_glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC) load(userptr, "glMultiTexCoord1dv");
    glad_glMultiTexCoord1dvARB = (PFNGLMULTITEXCOORD1DVARBPROC) load(userptr, "glMultiTexCoord1dvARB");
    glad_glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC) load(userptr, "glMultiTexCoord1f");
    glad_glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) load(userptr, "glMultiTexCoord1fARB");
    glad_glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC) load(userptr, "glMultiTexCoord1fv");
    glad_glMultiTexCoord1fvARB = (PFNGLMULTITEXCOORD1FVARBPROC) load(userptr, "glMultiTexCoord1fvARB");
    glad_glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC) load(userptr, "glMultiTexCoord1i");
    glad_glMultiTexCoord1iARB = (PFNGLMULTITEXCOORD1IARBPROC) load(userptr, "glMultiTexCoord1iARB");
    glad_glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC) load(userptr, "glMultiTexCoord1iv");
    glad_glMultiTexCoord1ivARB = (PFNGLMULTITEXCOORD1IVARBPROC) load(userptr, "glMultiTexCoord1ivARB");
    glad_glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC) load(userptr, "glMultiTexCoord1s");
    glad_glMultiTexCoord1sARB = (PFNGLMULTITEXCOORD1SARBPROC) load(userptr, "glMultiTexCoord1sARB");
    glad_glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC) load(userptr, "glMultiTexCoord1sv");
    glad_glMultiTexCoord1svARB = (PFNGLMULTITEXCOORD1SVARBPROC) load(userptr, "glMultiTexCoord1svARB");
    glad_glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC) load(userptr, "glMultiTexCoord2d");
    glad_glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC) load(userptr, "glMultiTexCoord2dARB");
    glad_glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC) load(userptr, "glMultiTexCoord2dv");
    glad_glMultiTexCoord2dvARB = (PFNGLMULTITEXCOORD2DVARBPROC) load(userptr, "glMultiTexCoord2dvARB");
    glad_glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC) load(userptr, "glMultiTexCoord2f");
    glad_glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) load(userptr, "glMultiTexCoord2fARB");
    glad_glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC) load(userptr, "glMultiTexCoord2fv");
    glad_glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC) load(userptr, "glMultiTexCoord2fvARB");
    glad_glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC) load(userptr, "glMultiTexCoord2i");
    glad_glMultiTexCoord2iARB = (PFNGLMULTITEXCOORD2IARBPROC) load(userptr, "glMultiTexCoord2iARB");
    glad_glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC) load(userptr, "glMultiTexCoord2iv");
    glad_glMultiTexCoord2ivARB = (PFNGLMULTITEXCOORD2IVARBPROC) load(userptr, "glMultiTexCoord2ivARB");
    glad_glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC) load(userptr, "glMultiTexCoord2s");
    glad_glMultiTexCoord2sARB = (PFNGLMULTITEXCOORD2SARBPROC) load(userptr, "glMultiTexCoord2sARB");
    glad_glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC) load(userptr, "glMultiTexCoord2sv");
    glad_glMultiTexCoord2svARB = (PFNGLMULTITEXCOORD2SVARBPROC) load(userptr, "glMultiTexCoord2svARB");
    glad_glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC) load(userptr, "glMultiTexCoord3d");
    glad_glMultiTexCoord3dARB = (PFNGLMULTITEXCOORD3DARBPROC) load(userptr, "glMultiTexCoord3dARB");
    glad_glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC) load(userptr, "glMultiTexCoord3dv");
    glad_glMultiTexCoord3dvARB = (PFNGLMULTITEXCOORD3DVARBPROC) load(userptr, "glMultiTexCoord3dvARB");
    glad_glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC) load(userptr, "glMultiTexCoord3f");
    glad_glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) load(userptr, "glMultiTexCoord3fARB");
    glad_glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC) load(userptr, "glMultiTexCoord3fv");
    glad_glMultiTexCoord3fvARB = (PFNGLMULTITEXCOORD3FVARBPROC) load(userptr, "glMultiTexCoord3fvARB");
    glad_glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC) load(userptr, "glMultiTexCoord3i");
    glad_glMultiTexCoord3iARB = (PFNGLMULTITEXCOORD3IARBPROC) load(userptr, "glMultiTexCoord3iARB");
    glad_glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC) load(userptr, "glMultiTexCoord3iv");
    glad_glMultiTexCoord3ivARB = (PFNGLMULTITEXCOORD3IVARBPROC) load(userptr, "glMultiTexCoord3ivARB");
    glad_glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC) load(userptr, "glMultiTexCoord3s");
    glad_glMultiTexCoord3sARB = (PFNGLMULTITEXCOORD3SARBPROC) load(userptr, "glMultiTexCoord3sARB");
    glad_glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC) load(userptr, "glMultiTexCoord3sv");
    glad_glMultiTexCoord3svARB = (PFNGLMULTITEXCOORD3SVARBPROC) load(userptr, "glMultiTexCoord3svARB");
    glad_glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC) load(userptr, "glMultiTexCoord4d");
    glad_glMultiTexCoord4dARB = (PFNGLMULTITEXCOORD4DARBPROC) load(userptr, "glMultiTexCoord4dARB");
    glad_glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC) load(userptr, "glMultiTexCoord4dv");
    glad_glMultiTexCoord4dvARB = (PFNGLMULTITEXCOORD4DVARBPROC) load(userptr, "glMultiTexCoord4dvARB");
    glad_glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC) load(userptr, "glMultiTexCoord4f");
    glad_glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) load(userptr, "glMultiTexCoord4fARB");
    glad_glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC) load(userptr, "glMultiTexCoord4fv");
    glad_glMultiTexCoord4fvARB = (PFNGLMULTITEXCOORD4FVARBPROC) load(userptr, "glMultiTexCoord4fvARB");
    glad_glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC) load(userptr, "glMultiTexCoord4i");
    glad_glMultiTexCoord4iARB = (PFNGLMULTITEXCOORD4IARBPROC) load(userptr, "glMultiTexCoord4iARB");
    glad_glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC) load(userptr, "glMultiTexCoord4iv");
    glad_glMultiTexCoord4ivARB = (PFNGLMULTITEXCOORD4IVARBPROC) load(userptr, "glMultiTexCoord4ivARB");
    glad_glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC) load(userptr, "glMultiTexCoord4s");
    glad_glMultiTexCoord4sARB = (PFNGLMULTITEXCOORD4SARBPROC) load(userptr, "glMultiTexCoord4sARB");
    glad_glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC) load(userptr, "glMultiTexCoord4sv");
    glad_glMultiTexCoord4svARB = (PFNGLMULTITEXCOORD4SVARBPROC) load(userptr, "glMultiTexCoord4svARB");
}
static void glad_gl_load_GL_ARB_occlusion_query( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_occlusion_query) return;
    glad_glBeginQuery = (PFNGLBEGINQUERYPROC) load(userptr, "glBeginQuery");
    glad_glBeginQueryARB = (PFNGLBEGINQUERYARBPROC) load(userptr, "glBeginQueryARB");
    glad_glDeleteQueries = (PFNGLDELETEQUERIESPROC) load(userptr, "glDeleteQueries");
    glad_glDeleteQueriesARB = (PFNGLDELETEQUERIESARBPROC) load(userptr, "glDeleteQueriesARB");
    glad_glEndQuery = (PFNGLENDQUERYPROC) load(userptr, "glEndQuery");
    glad_glEndQueryARB = (PFNGLENDQUERYARBPROC) load(userptr, "glEndQueryARB");
    glad_glGenQueries = (PFNGLGENQUERIESPROC) load(userptr, "glGenQueries");
    glad_glGenQueriesARB = (PFNGLGENQUERIESARBPROC) load(userptr, "glGenQueriesARB");
    glad_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) load(userptr, "glGetQueryObjectiv");
    glad_glGetQueryObjectivARB = (PFNGLGETQUERYOBJECTIVARBPROC) load(userptr, "glGetQueryObjectivARB");
    glad_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) load(userptr, "glGetQueryObjectuiv");
    glad_glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC) load(userptr, "glGetQueryObjectuivARB");
    glad_glGetQueryiv = (PFNGLGETQUERYIVPROC) load(userptr, "glGetQueryiv");
    glad_glGetQueryivARB = (PFNGLGETQUERYIVARBPROC) load(userptr, "glGetQueryivARB");
    glad_glIsQuery = (PFNGLISQUERYPROC) load(userptr, "glIsQuery");
    glad_glIsQueryARB = (PFNGLISQUERYARBPROC) load(userptr, "glIsQueryARB");
}
static void glad_gl_load_GL_ARB_parallel_shader_compile( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_parallel_shader_compile) return;
    glad_glMaxShaderCompilerThreadsARB = (PFNGLMAXSHADERCOMPILERTHREADSARBPROC) load(userptr, "glMaxShaderCompilerThreadsARB");
    glad_glMaxShaderCompilerThreadsKHR = (PFNGLMAXSHADERCOMPILERTHREADSKHRPROC) load(userptr, "glMaxShaderCompilerThreadsKHR");
}
static void glad_gl_load_GL_ARB_point_parameters( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_point_parameters) return;
    glad_glPointParameterf = (PFNGLPOINTPARAMETERFPROC) load(userptr, "glPointParameterf");
    glad_glPointParameterfARB = (PFNGLPOINTPARAMETERFARBPROC) load(userptr, "glPointParameterfARB");
    glad_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) load(userptr, "glPointParameterfv");
    glad_glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC) load(userptr, "glPointParameterfvARB");
}
static void glad_gl_load_GL_ARB_polygon_offset_clamp( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_polygon_offset_clamp) return;
    glad_glPolygonOffsetClamp = (PFNGLPOLYGONOFFSETCLAMPPROC) load(userptr, "glPolygonOffsetClamp");
}
static void glad_gl_load_GL_ARB_program_interface_query( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_program_interface_query) return;
    glad_glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC) load(userptr, "glGetProgramInterfaceiv");
    glad_glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC) load(userptr, "glGetProgramResourceIndex");
    glad_glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC) load(userptr, "glGetProgramResourceLocation");
    glad_glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) load(userptr, "glGetProgramResourceLocationIndex");
    glad_glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC) load(userptr, "glGetProgramResourceName");
    glad_glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC) load(userptr, "glGetProgramResourceiv");
}
static void glad_gl_load_GL_ARB_provoking_vertex( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_provoking_vertex) return;
    glad_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) load(userptr, "glProvokingVertex");
}
static void glad_gl_load_GL_ARB_robustness( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_robustness) return;
    glad_glGetGraphicsResetStatusARB = (PFNGLGETGRAPHICSRESETSTATUSARBPROC) load(userptr, "glGetGraphicsResetStatusARB");
    glad_glGetnColorTableARB = (PFNGLGETNCOLORTABLEARBPROC) load(userptr, "glGetnColorTableARB");
    glad_glGetnCompressedTexImageARB = (PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) load(userptr, "glGetnCompressedTexImageARB");
    glad_glGetnConvolutionFilterARB = (PFNGLGETNCONVOLUTIONFILTERARBPROC) load(userptr, "glGetnConvolutionFilterARB");
    glad_glGetnHistogramARB = (PFNGLGETNHISTOGRAMARBPROC) load(userptr, "glGetnHistogramARB");
    glad_glGetnMapdvARB = (PFNGLGETNMAPDVARBPROC) load(userptr, "glGetnMapdvARB");
    glad_glGetnMapfvARB = (PFNGLGETNMAPFVARBPROC) load(userptr, "glGetnMapfvARB");
    glad_glGetnMapivARB = (PFNGLGETNMAPIVARBPROC) load(userptr, "glGetnMapivARB");
    glad_glGetnMinmaxARB = (PFNGLGETNMINMAXARBPROC) load(userptr, "glGetnMinmaxARB");
    glad_glGetnPixelMapfvARB = (PFNGLGETNPIXELMAPFVARBPROC) load(userptr, "glGetnPixelMapfvARB");
    glad_glGetnPixelMapuivARB = (PFNGLGETNPIXELMAPUIVARBPROC) load(userptr, "glGetnPixelMapuivARB");
    glad_glGetnPixelMapusvARB = (PFNGLGETNPIXELMAPUSVARBPROC) load(userptr, "glGetnPixelMapusvARB");
    glad_glGetnPolygonStippleARB = (PFNGLGETNPOLYGONSTIPPLEARBPROC) load(userptr, "glGetnPolygonStippleARB");
    glad_glGetnSeparableFilterARB = (PFNGLGETNSEPARABLEFILTERARBPROC) load(userptr, "glGetnSeparableFilterARB");
    glad_glGetnTexImageARB = (PFNGLGETNTEXIMAGEARBPROC) load(userptr, "glGetnTexImageARB");
    glad_glGetnUniformdvARB = (PFNGLGETNUNIFORMDVARBPROC) load(userptr, "glGetnUniformdvARB");
    glad_glGetnUniformfvARB = (PFNGLGETNUNIFORMFVARBPROC) load(userptr, "glGetnUniformfvARB");
    glad_glGetnUniformivARB = (PFNGLGETNUNIFORMIVARBPROC) load(userptr, "glGetnUniformivARB");
    glad_glGetnUniformuivARB = (PFNGLGETNUNIFORMUIVARBPROC) load(userptr, "glGetnUniformuivARB");
    glad_glReadnPixels = (PFNGLREADNPIXELSPROC) load(userptr, "glReadnPixels");
    glad_glReadnPixelsARB = (PFNGLREADNPIXELSARBPROC) load(userptr, "glReadnPixelsARB");
}
static void glad_gl_load_GL_ARB_sample_locations( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_sample_locations) return;
    glad_glEvaluateDepthValuesARB = (PFNGLEVALUATEDEPTHVALUESARBPROC) load(userptr, "glEvaluateDepthValuesARB");
    glad_glFramebufferSampleLocationsfvARB = (PFNGLFRAMEBUFFERSAMPLELOCATIONSFVARBPROC) load(userptr, "glFramebufferSampleLocationsfvARB");
    glad_glNamedFramebufferSampleLocationsfvARB = (PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVARBPROC) load(userptr, "glNamedFramebufferSampleLocationsfvARB");
}
static void glad_gl_load_GL_ARB_sample_shading( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_sample_shading) return;
    glad_glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC) load(userptr, "glMinSampleShading");
    glad_glMinSampleShadingARB = (PFNGLMINSAMPLESHADINGARBPROC) load(userptr, "glMinSampleShadingARB");
}
static void glad_gl_load_GL_ARB_sampler_objects( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_sampler_objects) return;
    glad_glBindSampler = (PFNGLBINDSAMPLERPROC) load(userptr, "glBindSampler");
    glad_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) load(userptr, "glDeleteSamplers");
    glad_glGenSamplers = (PFNGLGENSAMPLERSPROC) load(userptr, "glGenSamplers");
    glad_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) load(userptr, "glGetSamplerParameterIiv");
    glad_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) load(userptr, "glGetSamplerParameterIuiv");
    glad_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) load(userptr, "glGetSamplerParameterfv");
    glad_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) load(userptr, "glGetSamplerParameteriv");
    glad_glIsSampler = (PFNGLISSAMPLERPROC) load(userptr, "glIsSampler");
    glad_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) load(userptr, "glSamplerParameterIiv");
    glad_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) load(userptr, "glSamplerParameterIuiv");
    glad_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) load(userptr, "glSamplerParameterf");
    glad_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) load(userptr, "glSamplerParameterfv");
    glad_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) load(userptr, "glSamplerParameteri");
    glad_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) load(userptr, "glSamplerParameteriv");
}
static void glad_gl_load_GL_ARB_separate_shader_objects( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_separate_shader_objects) return;
    glad_glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC) load(userptr, "glActiveShaderProgram");
    glad_glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC) load(userptr, "glBindProgramPipeline");
    glad_glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC) load(userptr, "glCreateShaderProgramv");
    glad_glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC) load(userptr, "glDeleteProgramPipelines");
    glad_glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC) load(userptr, "glGenProgramPipelines");
    glad_glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC) load(userptr, "glGetProgramPipelineInfoLog");
    glad_glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC) load(userptr, "glGetProgramPipelineiv");
    glad_glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC) load(userptr, "glIsProgramPipeline");
    glad_glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC) load(userptr, "glProgramParameteri");
    glad_glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC) load(userptr, "glProgramUniform1d");
    glad_glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC) load(userptr, "glProgramUniform1dv");
    glad_glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC) load(userptr, "glProgramUniform1f");
    glad_glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC) load(userptr, "glProgramUniform1fv");
    glad_glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC) load(userptr, "glProgramUniform1i");
    glad_glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC) load(userptr, "glProgramUniform1iv");
    glad_glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC) load(userptr, "glProgramUniform1ui");
    glad_glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC) load(userptr, "glProgramUniform1uiv");
    glad_glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC) load(userptr, "glProgramUniform2d");
    glad_glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC) load(userptr, "glProgramUniform2dv");
    glad_glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC) load(userptr, "glProgramUniform2f");
    glad_glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC) load(userptr, "glProgramUniform2fv");
    glad_glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC) load(userptr, "glProgramUniform2i");
    glad_glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC) load(userptr, "glProgramUniform2iv");
    glad_glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC) load(userptr, "glProgramUniform2ui");
    glad_glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC) load(userptr, "glProgramUniform2uiv");
    glad_glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC) load(userptr, "glProgramUniform3d");
    glad_glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC) load(userptr, "glProgramUniform3dv");
    glad_glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC) load(userptr, "glProgramUniform3f");
    glad_glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC) load(userptr, "glProgramUniform3fv");
    glad_glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC) load(userptr, "glProgramUniform3i");
    glad_glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC) load(userptr, "glProgramUniform3iv");
    glad_glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC) load(userptr, "glProgramUniform3ui");
    glad_glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC) load(userptr, "glProgramUniform3uiv");
    glad_glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC) load(userptr, "glProgramUniform4d");
    glad_glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC) load(userptr, "glProgramUniform4dv");
    glad_glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC) load(userptr, "glProgramUniform4f");
    glad_glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC) load(userptr, "glProgramUniform4fv");
    glad_glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC) load(userptr, "glProgramUniform4i");
    glad_glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC) load(userptr, "glProgramUniform4iv");
    glad_glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC) load(userptr, "glProgramUniform4ui");
    glad_glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC) load(userptr, "glProgramUniform4uiv");
    glad_glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC) load(userptr, "glProgramUniformMatrix2dv");
    glad_glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC) load(userptr, "glProgramUniformMatrix2fv");
    glad_glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) load(userptr, "glProgramUniformMatrix2x3dv");
    glad_glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) load(userptr, "glProgramUniformMatrix2x3fv");
    glad_glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) load(userptr, "glProgramUniformMatrix2x4dv");
    glad_glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) load(userptr, "glProgramUniformMatrix2x4fv");
    glad_glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC) load(userptr, "glProgramUniformMatrix3dv");
    glad_glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC) load(userptr, "glProgramUniformMatrix3fv");
    glad_glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) load(userptr, "glProgramUniformMatrix3x2dv");
    glad_glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) load(userptr, "glProgramUniformMatrix3x2fv");
    glad_glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) load(userptr, "glProgramUniformMatrix3x4dv");
    glad_glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) load(userptr, "glProgramUniformMatrix3x4fv");
    glad_glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC) load(userptr, "glProgramUniformMatrix4dv");
    glad_glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC) load(userptr, "glProgramUniformMatrix4fv");
    glad_glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) load(userptr, "glProgramUniformMatrix4x2dv");
    glad_glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) load(userptr, "glProgramUniformMatrix4x2fv");
    glad_glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) load(userptr, "glProgramUniformMatrix4x3dv");
    glad_glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) load(userptr, "glProgramUniformMatrix4x3fv");
    glad_glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC) load(userptr, "glUseProgramStages");
    glad_glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC) load(userptr, "glValidateProgramPipeline");
}
static void glad_gl_load_GL_ARB_shader_atomic_counters( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_shader_atomic_counters) return;
    glad_glGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) load(userptr, "glGetActiveAtomicCounterBufferiv");
}
static void glad_gl_load_GL_ARB_shader_image_load_store( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_shader_image_load_store) return;
    glad_glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC) load(userptr, "glBindImageTexture");
    glad_glMemoryBarrier = (PFNGLMEMORYBARRIERPROC) load(userptr, "glMemoryBarrier");
}
static void glad_gl_load_GL_ARB_shader_objects( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_shader_objects) return;
    glad_glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) load(userptr, "glAttachObjectARB");
    glad_glAttachShader = (PFNGLATTACHSHADERPROC) load(userptr, "glAttachShader");
    glad_glCompileShader = (PFNGLCOMPILESHADERPROC) load(userptr, "glCompileShader");
    glad_glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) load(userptr, "glCompileShaderARB");
    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC) load(userptr, "glCreateProgram");
    glad_glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) load(userptr, "glCreateProgramObjectARB");
    glad_glCreateShader = (PFNGLCREATESHADERPROC) load(userptr, "glCreateShader");
    glad_glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) load(userptr, "glCreateShaderObjectARB");
    glad_glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) load(userptr, "glDeleteObjectARB");
    glad_glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC) load(userptr, "glDetachObjectARB");
    glad_glDetachShader = (PFNGLDETACHSHADERPROC) load(userptr, "glDetachShader");
    glad_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) load(userptr, "glGetActiveUniform");
    glad_glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC) load(userptr, "glGetActiveUniformARB");
    glad_glGetAttachedObjectsARB = (PFNGLGETATTACHEDOBJECTSARBPROC) load(userptr, "glGetAttachedObjectsARB");
    glad_glGetHandleARB = (PFNGLGETHANDLEARBPROC) load(userptr, "glGetHandleARB");
    glad_glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) load(userptr, "glGetInfoLogARB");
    glad_glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC) load(userptr, "glGetObjectParameterfvARB");
    glad_glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) load(userptr, "glGetObjectParameterivARB");
    glad_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) load(userptr, "glGetShaderSource");
    glad_glGetShaderSourceARB = (PFNGLGETSHADERSOURCEARBPROC) load(userptr, "glGetShaderSourceARB");
    glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) load(userptr, "glGetUniformLocation");
    glad_glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) load(userptr, "glGetUniformLocationARB");
    glad_glGetUniformfv = (PFNGLGETUNIFORMFVPROC) load(userptr, "glGetUniformfv");
    glad_glGetUniformfvARB = (PFNGLGETUNIFORMFVARBPROC) load(userptr, "glGetUniformfvARB");
    glad_glGetUniformiv = (PFNGLGETUNIFORMIVPROC) load(userptr, "glGetUniformiv");
    glad_glGetUniformivARB = (PFNGLGETUNIFORMIVARBPROC) load(userptr, "glGetUniformivARB");
    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC) load(userptr, "glLinkProgram");
    glad_glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) load(userptr, "glLinkProgramARB");
    glad_glShaderSource = (PFNGLSHADERSOURCEPROC) load(userptr, "glShaderSource");
    glad_glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) load(userptr, "glShaderSourceARB");
    glad_glUniform1f = (PFNGLUNIFORM1FPROC) load(userptr, "glUniform1f");
    glad_glUniform1fARB = (PFNGLUNIFORM1FARBPROC) load(userptr, "glUniform1fARB");
    glad_glUniform1fv = (PFNGLUNIFORM1FVPROC) load(userptr, "glUniform1fv");
    glad_glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC) load(userptr, "glUniform1fvARB");
    glad_glUniform1i = (PFNGLUNIFORM1IPROC) load(userptr, "glUniform1i");
    glad_glUniform1iARB = (PFNGLUNIFORM1IARBPROC) load(userptr, "glUniform1iARB");
    glad_glUniform1iv = (PFNGLUNIFORM1IVPROC) load(userptr, "glUniform1iv");
    glad_glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC) load(userptr, "glUniform1ivARB");
    glad_glUniform2f = (PFNGLUNIFORM2FPROC) load(userptr, "glUniform2f");
    glad_glUniform2fARB = (PFNGLUNIFORM2FARBPROC) load(userptr, "glUniform2fARB");
    glad_glUniform2fv = (PFNGLUNIFORM2FVPROC) load(userptr, "glUniform2fv");
    glad_glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) load(userptr, "glUniform2fvARB");
    glad_glUniform2i = (PFNGLUNIFORM2IPROC) load(userptr, "glUniform2i");
    glad_glUniform2iARB = (PFNGLUNIFORM2IARBPROC) load(userptr, "glUniform2iARB");
    glad_glUniform2iv = (PFNGLUNIFORM2IVPROC) load(userptr, "glUniform2iv");
    glad_glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC) load(userptr, "glUniform2ivARB");
    glad_glUniform3f = (PFNGLUNIFORM3FPROC) load(userptr, "glUniform3f");
    glad_glUniform3fARB = (PFNGLUNIFORM3FARBPROC) load(userptr, "glUniform3fARB");
    glad_glUniform3fv = (PFNGLUNIFORM3FVPROC) load(userptr, "glUniform3fv");
    glad_glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC) load(userptr, "glUniform3fvARB");
    glad_glUniform3i = (PFNGLUNIFORM3IPROC) load(userptr, "glUniform3i");
    glad_glUniform3iARB = (PFNGLUNIFORM3IARBPROC) load(userptr, "glUniform3iARB");
    glad_glUniform3iv = (PFNGLUNIFORM3IVPROC) load(userptr, "glUniform3iv");
    glad_glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC) load(userptr, "glUniform3ivARB");
    glad_glUniform4f = (PFNGLUNIFORM4FPROC) load(userptr, "glUniform4f");
    glad_glUniform4fARB = (PFNGLUNIFORM4FARBPROC) load(userptr, "glUniform4fARB");
    glad_glUniform4fv = (PFNGLUNIFORM4FVPROC) load(userptr, "glUniform4fv");
    glad_glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) load(userptr, "glUniform4fvARB");
    glad_glUniform4i = (PFNGLUNIFORM4IPROC) load(userptr, "glUniform4i");
    glad_glUniform4iARB = (PFNGLUNIFORM4IARBPROC) load(userptr, "glUniform4iARB");
    glad_glUniform4iv = (PFNGLUNIFORM4IVPROC) load(userptr, "glUniform4iv");
    glad_glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC) load(userptr, "glUniform4ivARB");
    glad_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) load(userptr, "glUniformMatrix2fv");
    glad_glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC) load(userptr, "glUniformMatrix2fvARB");
    glad_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) load(userptr, "glUniformMatrix3fv");
    glad_glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC) load(userptr, "glUniformMatrix3fvARB");
    glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) load(userptr, "glUniformMatrix4fv");
    glad_glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) load(userptr, "glUniformMatrix4fvARB");
    glad_glUseProgram = (PFNGLUSEPROGRAMPROC) load(userptr, "glUseProgram");
    glad_glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) load(userptr, "glUseProgramObjectARB");
    glad_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) load(userptr, "glValidateProgram");
    glad_glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC) load(userptr, "glValidateProgramARB");
}
static void glad_gl_load_GL_ARB_shader_storage_buffer_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_shader_storage_buffer_object) return;
    glad_glShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC) load(userptr, "glShaderStorageBlockBinding");
}
static void glad_gl_load_GL_ARB_shader_subroutine( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_shader_subroutine) return;
    glad_glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC) load(userptr, "glGetActiveSubroutineName");
    glad_glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) load(userptr, "glGetActiveSubroutineUniformName");
    glad_glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) load(userptr, "glGetActiveSubroutineUniformiv");
    glad_glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC) load(userptr, "glGetProgramStageiv");
    glad_glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC) load(userptr, "glGetSubroutineIndex");
    glad_glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) load(userptr, "glGetSubroutineUniformLocation");
    glad_glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC) load(userptr, "glGetUniformSubroutineuiv");
    glad_glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC) load(userptr, "glUniformSubroutinesuiv");
}
static void glad_gl_load_GL_ARB_shading_language_include( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_shading_language_include) return;
    glad_glCompileShaderIncludeARB = (PFNGLCOMPILESHADERINCLUDEARBPROC) load(userptr, "glCompileShaderIncludeARB");
    glad_glDeleteNamedStringARB = (PFNGLDELETENAMEDSTRINGARBPROC) load(userptr, "glDeleteNamedStringARB");
    glad_glGetNamedStringARB = (PFNGLGETNAMEDSTRINGARBPROC) load(userptr, "glGetNamedStringARB");
    glad_glGetNamedStringivARB = (PFNGLGETNAMEDSTRINGIVARBPROC) load(userptr, "glGetNamedStringivARB");
    glad_glIsNamedStringARB = (PFNGLISNAMEDSTRINGARBPROC) load(userptr, "glIsNamedStringARB");
    glad_glNamedStringARB = (PFNGLNAMEDSTRINGARBPROC) load(userptr, "glNamedStringARB");
}
static void glad_gl_load_GL_ARB_sparse_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_sparse_buffer) return;
    glad_glBufferPageCommitmentARB = (PFNGLBUFFERPAGECOMMITMENTARBPROC) load(userptr, "glBufferPageCommitmentARB");
    glad_glNamedBufferPageCommitmentARB = (PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC) load(userptr, "glNamedBufferPageCommitmentARB");
    glad_glNamedBufferPageCommitmentEXT = (PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC) load(userptr, "glNamedBufferPageCommitmentEXT");
}
static void glad_gl_load_GL_ARB_sparse_texture( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_sparse_texture) return;
    glad_glTexPageCommitmentARB = (PFNGLTEXPAGECOMMITMENTARBPROC) load(userptr, "glTexPageCommitmentARB");
}
static void glad_gl_load_GL_ARB_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_sync) return;
    glad_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) load(userptr, "glClientWaitSync");
    glad_glDeleteSync = (PFNGLDELETESYNCPROC) load(userptr, "glDeleteSync");
    glad_glFenceSync = (PFNGLFENCESYNCPROC) load(userptr, "glFenceSync");
    glad_glGetInteger64v = (PFNGLGETINTEGER64VPROC) load(userptr, "glGetInteger64v");
    glad_glGetSynciv = (PFNGLGETSYNCIVPROC) load(userptr, "glGetSynciv");
    glad_glIsSync = (PFNGLISSYNCPROC) load(userptr, "glIsSync");
    glad_glWaitSync = (PFNGLWAITSYNCPROC) load(userptr, "glWaitSync");
}
static void glad_gl_load_GL_ARB_tessellation_shader( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_tessellation_shader) return;
    glad_glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC) load(userptr, "glPatchParameterfv");
    glad_glPatchParameteri = (PFNGLPATCHPARAMETERIPROC) load(userptr, "glPatchParameteri");
}
static void glad_gl_load_GL_ARB_texture_barrier( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_barrier) return;
    glad_glTextureBarrier = (PFNGLTEXTUREBARRIERPROC) load(userptr, "glTextureBarrier");
}
static void glad_gl_load_GL_ARB_texture_buffer_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_buffer_object) return;
    glad_glTexBuffer = (PFNGLTEXBUFFERPROC) load(userptr, "glTexBuffer");
    glad_glTexBufferARB = (PFNGLTEXBUFFERARBPROC) load(userptr, "glTexBufferARB");
}
static void glad_gl_load_GL_ARB_texture_buffer_range( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_buffer_range) return;
    glad_glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC) load(userptr, "glTexBufferRange");
}
static void glad_gl_load_GL_ARB_texture_compression( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_compression) return;
    glad_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) load(userptr, "glCompressedTexImage1D");
    glad_glCompressedTexImage1DARB = (PFNGLCOMPRESSEDTEXIMAGE1DARBPROC) load(userptr, "glCompressedTexImage1DARB");
    glad_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) load(userptr, "glCompressedTexImage2D");
    glad_glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) load(userptr, "glCompressedTexImage2DARB");
    glad_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) load(userptr, "glCompressedTexImage3D");
    glad_glCompressedTexImage3DARB = (PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) load(userptr, "glCompressedTexImage3DARB");
    glad_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) load(userptr, "glCompressedTexSubImage1D");
    glad_glCompressedTexSubImage1DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC) load(userptr, "glCompressedTexSubImage1DARB");
    glad_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) load(userptr, "glCompressedTexSubImage2D");
    glad_glCompressedTexSubImage2DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) load(userptr, "glCompressedTexSubImage2DARB");
    glad_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) load(userptr, "glCompressedTexSubImage3D");
    glad_glCompressedTexSubImage3DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC) load(userptr, "glCompressedTexSubImage3DARB");
    glad_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) load(userptr, "glGetCompressedTexImage");
    glad_glGetCompressedTexImageARB = (PFNGLGETCOMPRESSEDTEXIMAGEARBPROC) load(userptr, "glGetCompressedTexImageARB");
}
static void glad_gl_load_GL_ARB_texture_multisample( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_multisample) return;
    glad_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) load(userptr, "glGetMultisamplefv");
    glad_glSampleMaski = (PFNGLSAMPLEMASKIPROC) load(userptr, "glSampleMaski");
    glad_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) load(userptr, "glTexImage2DMultisample");
    glad_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) load(userptr, "glTexImage3DMultisample");
}
static void glad_gl_load_GL_ARB_texture_storage( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_storage) return;
    glad_glTexStorage1D = (PFNGLTEXSTORAGE1DPROC) load(userptr, "glTexStorage1D");
    glad_glTexStorage2D = (PFNGLTEXSTORAGE2DPROC) load(userptr, "glTexStorage2D");
    glad_glTexStorage3D = (PFNGLTEXSTORAGE3DPROC) load(userptr, "glTexStorage3D");
}
static void glad_gl_load_GL_ARB_texture_storage_multisample( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_storage_multisample) return;
    glad_glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC) load(userptr, "glTexStorage2DMultisample");
    glad_glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC) load(userptr, "glTexStorage3DMultisample");
}
static void glad_gl_load_GL_ARB_texture_view( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_texture_view) return;
    glad_glTextureView = (PFNGLTEXTUREVIEWPROC) load(userptr, "glTextureView");
}
static void glad_gl_load_GL_ARB_timer_query( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_timer_query) return;
    glad_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) load(userptr, "glGetQueryObjecti64v");
    glad_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC) load(userptr, "glGetQueryObjectui64v");
    glad_glQueryCounter = (PFNGLQUERYCOUNTERPROC) load(userptr, "glQueryCounter");
}
static void glad_gl_load_GL_ARB_transform_feedback2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_transform_feedback2) return;
    glad_glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC) load(userptr, "glBindTransformFeedback");
    glad_glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC) load(userptr, "glDeleteTransformFeedbacks");
    glad_glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC) load(userptr, "glDrawTransformFeedback");
    glad_glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC) load(userptr, "glGenTransformFeedbacks");
    glad_glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC) load(userptr, "glIsTransformFeedback");
    glad_glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC) load(userptr, "glPauseTransformFeedback");
    glad_glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC) load(userptr, "glResumeTransformFeedback");
}
static void glad_gl_load_GL_ARB_transform_feedback3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_transform_feedback3) return;
    glad_glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC) load(userptr, "glBeginQueryIndexed");
    glad_glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) load(userptr, "glDrawTransformFeedbackStream");
    glad_glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC) load(userptr, "glEndQueryIndexed");
    glad_glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC) load(userptr, "glGetQueryIndexediv");
}
static void glad_gl_load_GL_ARB_transform_feedback_instanced( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_transform_feedback_instanced) return;
    glad_glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) load(userptr, "glDrawTransformFeedbackInstanced");
    glad_glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) load(userptr, "glDrawTransformFeedbackStreamInstanced");
}
static void glad_gl_load_GL_ARB_transpose_matrix( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_transpose_matrix) return;
    glad_glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC) load(userptr, "glLoadTransposeMatrixd");
    glad_glLoadTransposeMatrixdARB = (PFNGLLOADTRANSPOSEMATRIXDARBPROC) load(userptr, "glLoadTransposeMatrixdARB");
    glad_glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC) load(userptr, "glLoadTransposeMatrixf");
    glad_glLoadTransposeMatrixfARB = (PFNGLLOADTRANSPOSEMATRIXFARBPROC) load(userptr, "glLoadTransposeMatrixfARB");
    glad_glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC) load(userptr, "glMultTransposeMatrixd");
    glad_glMultTransposeMatrixdARB = (PFNGLMULTTRANSPOSEMATRIXDARBPROC) load(userptr, "glMultTransposeMatrixdARB");
    glad_glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC) load(userptr, "glMultTransposeMatrixf");
    glad_glMultTransposeMatrixfARB = (PFNGLMULTTRANSPOSEMATRIXFARBPROC) load(userptr, "glMultTransposeMatrixfARB");
}
static void glad_gl_load_GL_ARB_uniform_buffer_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_uniform_buffer_object) return;
    glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load(userptr, "glBindBufferBase");
    glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load(userptr, "glBindBufferRange");
    glad_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) load(userptr, "glGetActiveUniformBlockName");
    glad_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) load(userptr, "glGetActiveUniformBlockiv");
    glad_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) load(userptr, "glGetActiveUniformName");
    glad_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) load(userptr, "glGetActiveUniformsiv");
    glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load(userptr, "glGetIntegeri_v");
    glad_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) load(userptr, "glGetUniformBlockIndex");
    glad_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) load(userptr, "glGetUniformIndices");
    glad_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) load(userptr, "glUniformBlockBinding");
}
static void glad_gl_load_GL_ARB_vertex_array_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_array_object) return;
    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) load(userptr, "glBindVertexArray");
    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) load(userptr, "glDeleteVertexArrays");
    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) load(userptr, "glGenVertexArrays");
    glad_glIsVertexArray = (PFNGLISVERTEXARRAYPROC) load(userptr, "glIsVertexArray");
}
static void glad_gl_load_GL_ARB_vertex_attrib_64bit( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_attrib_64bit) return;
    glad_glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC) load(userptr, "glGetVertexAttribLdv");
    glad_glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC) load(userptr, "glVertexAttribL1d");
    glad_glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC) load(userptr, "glVertexAttribL1dv");
    glad_glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC) load(userptr, "glVertexAttribL2d");
    glad_glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC) load(userptr, "glVertexAttribL2dv");
    glad_glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC) load(userptr, "glVertexAttribL3d");
    glad_glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC) load(userptr, "glVertexAttribL3dv");
    glad_glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC) load(userptr, "glVertexAttribL4d");
    glad_glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC) load(userptr, "glVertexAttribL4dv");
    glad_glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC) load(userptr, "glVertexAttribLPointer");
}
static void glad_gl_load_GL_ARB_vertex_attrib_binding( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_attrib_binding) return;
    glad_glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC) load(userptr, "glBindVertexBuffer");
    glad_glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC) load(userptr, "glVertexAttribBinding");
    glad_glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC) load(userptr, "glVertexAttribFormat");
    glad_glVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC) load(userptr, "glVertexAttribIFormat");
    glad_glVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC) load(userptr, "glVertexAttribLFormat");
    glad_glVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC) load(userptr, "glVertexBindingDivisor");
}
static void glad_gl_load_GL_ARB_vertex_blend( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_blend) return;
    glad_glVertexBlendARB = (PFNGLVERTEXBLENDARBPROC) load(userptr, "glVertexBlendARB");
    glad_glWeightPointerARB = (PFNGLWEIGHTPOINTERARBPROC) load(userptr, "glWeightPointerARB");
    glad_glWeightbvARB = (PFNGLWEIGHTBVARBPROC) load(userptr, "glWeightbvARB");
    glad_glWeightdvARB = (PFNGLWEIGHTDVARBPROC) load(userptr, "glWeightdvARB");
    glad_glWeightfvARB = (PFNGLWEIGHTFVARBPROC) load(userptr, "glWeightfvARB");
    glad_glWeightivARB = (PFNGLWEIGHTIVARBPROC) load(userptr, "glWeightivARB");
    glad_glWeightsvARB = (PFNGLWEIGHTSVARBPROC) load(userptr, "glWeightsvARB");
    glad_glWeightubvARB = (PFNGLWEIGHTUBVARBPROC) load(userptr, "glWeightubvARB");
    glad_glWeightuivARB = (PFNGLWEIGHTUIVARBPROC) load(userptr, "glWeightuivARB");
    glad_glWeightusvARB = (PFNGLWEIGHTUSVARBPROC) load(userptr, "glWeightusvARB");
}
static void glad_gl_load_GL_ARB_vertex_buffer_object( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_buffer_object) return;
    glad_glBindBuffer = (PFNGLBINDBUFFERPROC) load(userptr, "glBindBuffer");
    glad_glBindBufferARB = (PFNGLBINDBUFFERARBPROC) load(userptr, "glBindBufferARB");
    glad_glBufferData = (PFNGLBUFFERDATAPROC) load(userptr, "glBufferData");
    glad_glBufferDataARB = (PFNGLBUFFERDATAARBPROC) load(userptr, "glBufferDataARB");
    glad_glBufferSubData = (PFNGLBUFFERSUBDATAPROC) load(userptr, "glBufferSubData");
    glad_glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) load(userptr, "glBufferSubDataARB");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) load(userptr, "glDeleteBuffers");
    glad_glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) load(userptr, "glDeleteBuffersARB");
    glad_glGenBuffers = (PFNGLGENBUFFERSPROC) load(userptr, "glGenBuffers");
    glad_glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) load(userptr, "glGenBuffersARB");
    glad_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) load(userptr, "glGetBufferParameteriv");
    glad_glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC) load(userptr, "glGetBufferParameterivARB");
    glad_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) load(userptr, "glGetBufferPointerv");
    glad_glGetBufferPointervARB = (PFNGLGETBUFFERPOINTERVARBPROC) load(userptr, "glGetBufferPointervARB");
    glad_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) load(userptr, "glGetBufferSubData");
    glad_glGetBufferSubDataARB = (PFNGLGETBUFFERSUBDATAARBPROC) load(userptr, "glGetBufferSubDataARB");
    glad_glIsBuffer = (PFNGLISBUFFERPROC) load(userptr, "glIsBuffer");
    glad_glIsBufferARB = (PFNGLISBUFFERARBPROC) load(userptr, "glIsBufferARB");
    glad_glMapBuffer = (PFNGLMAPBUFFERPROC) load(userptr, "glMapBuffer");
    glad_glMapBufferARB = (PFNGLMAPBUFFERARBPROC) load(userptr, "glMapBufferARB");
    glad_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) load(userptr, "glUnmapBuffer");
    glad_glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) load(userptr, "glUnmapBufferARB");
}
static void glad_gl_load_GL_ARB_vertex_program( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_program) return;
    glad_glBindProgramARB = (PFNGLBINDPROGRAMARBPROC) load(userptr, "glBindProgramARB");
    glad_glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC) load(userptr, "glDeleteProgramsARB");
    glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) load(userptr, "glDisableVertexAttribArray");
    glad_glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) load(userptr, "glDisableVertexAttribArrayARB");
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load(userptr, "glEnableVertexAttribArray");
    glad_glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC) load(userptr, "glEnableVertexAttribArrayARB");
    glad_glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC) load(userptr, "glGenProgramsARB");
    glad_glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC) load(userptr, "glGetProgramEnvParameterdvARB");
    glad_glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC) load(userptr, "glGetProgramEnvParameterfvARB");
    glad_glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) load(userptr, "glGetProgramLocalParameterdvARB");
    glad_glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) load(userptr, "glGetProgramLocalParameterfvARB");
    glad_glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARBPROC) load(userptr, "glGetProgramStringARB");
    glad_glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC) load(userptr, "glGetProgramivARB");
    glad_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) load(userptr, "glGetVertexAttribPointerv");
    glad_glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC) load(userptr, "glGetVertexAttribPointervARB");
    glad_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) load(userptr, "glGetVertexAttribdv");
    glad_glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARBPROC) load(userptr, "glGetVertexAttribdvARB");
    glad_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) load(userptr, "glGetVertexAttribfv");
    glad_glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARBPROC) load(userptr, "glGetVertexAttribfvARB");
    glad_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) load(userptr, "glGetVertexAttribiv");
    glad_glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARBPROC) load(userptr, "glGetVertexAttribivARB");
    glad_glIsProgramARB = (PFNGLISPROGRAMARBPROC) load(userptr, "glIsProgramARB");
    glad_glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARBPROC) load(userptr, "glProgramEnvParameter4dARB");
    glad_glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARBPROC) load(userptr, "glProgramEnvParameter4dvARB");
    glad_glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARBPROC) load(userptr, "glProgramEnvParameter4fARB");
    glad_glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARBPROC) load(userptr, "glProgramEnvParameter4fvARB");
    glad_glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC) load(userptr, "glProgramLocalParameter4dARB");
    glad_glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) load(userptr, "glProgramLocalParameter4dvARB");
    glad_glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC) load(userptr, "glProgramLocalParameter4fARB");
    glad_glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) load(userptr, "glProgramLocalParameter4fvARB");
    glad_glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC) load(userptr, "glProgramStringARB");
    glad_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) load(userptr, "glVertexAttrib1d");
    glad_glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARBPROC) load(userptr, "glVertexAttrib1dARB");
    glad_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) load(userptr, "glVertexAttrib1dv");
    glad_glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARBPROC) load(userptr, "glVertexAttrib1dvARB");
    glad_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) load(userptr, "glVertexAttrib1f");
    glad_glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC) load(userptr, "glVertexAttrib1fARB");
    glad_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) load(userptr, "glVertexAttrib1fv");
    glad_glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARBPROC) load(userptr, "glVertexAttrib1fvARB");
    glad_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) load(userptr, "glVertexAttrib1s");
    glad_glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARBPROC) load(userptr, "glVertexAttrib1sARB");
    glad_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) load(userptr, "glVertexAttrib1sv");
    glad_glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARBPROC) load(userptr, "glVertexAttrib1svARB");
    glad_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) load(userptr, "glVertexAttrib2d");
    glad_glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARBPROC) load(userptr, "glVertexAttrib2dARB");
    glad_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) load(userptr, "glVertexAttrib2dv");
    glad_glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARBPROC) load(userptr, "glVertexAttrib2dvARB");
    glad_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) load(userptr, "glVertexAttrib2f");
    glad_glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARBPROC) load(userptr, "glVertexAttrib2fARB");
    glad_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) load(userptr, "glVertexAttrib2fv");
    glad_glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARBPROC) load(userptr, "glVertexAttrib2fvARB");
    glad_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) load(userptr, "glVertexAttrib2s");
    glad_glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARBPROC) load(userptr, "glVertexAttrib2sARB");
    glad_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) load(userptr, "glVertexAttrib2sv");
    glad_glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARBPROC) load(userptr, "glVertexAttrib2svARB");
    glad_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) load(userptr, "glVertexAttrib3d");
    glad_glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARBPROC) load(userptr, "glVertexAttrib3dARB");
    glad_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) load(userptr, "glVertexAttrib3dv");
    glad_glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARBPROC) load(userptr, "glVertexAttrib3dvARB");
    glad_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) load(userptr, "glVertexAttrib3f");
    glad_glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC) load(userptr, "glVertexAttrib3fARB");
    glad_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) load(userptr, "glVertexAttrib3fv");
    glad_glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC) load(userptr, "glVertexAttrib3fvARB");
    glad_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) load(userptr, "glVertexAttrib3s");
    glad_glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARBPROC) load(userptr, "glVertexAttrib3sARB");
    glad_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) load(userptr, "glVertexAttrib3sv");
    glad_glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARBPROC) load(userptr, "glVertexAttrib3svARB");
    glad_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) load(userptr, "glVertexAttrib4Nbv");
    glad_glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARBPROC) load(userptr, "glVertexAttrib4NbvARB");
    glad_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) load(userptr, "glVertexAttrib4Niv");
    glad_glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARBPROC) load(userptr, "glVertexAttrib4NivARB");
    glad_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) load(userptr, "glVertexAttrib4Nsv");
    glad_glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARBPROC) load(userptr, "glVertexAttrib4NsvARB");
    glad_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) load(userptr, "glVertexAttrib4Nub");
    glad_glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARBPROC) load(userptr, "glVertexAttrib4NubARB");
    glad_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) load(userptr, "glVertexAttrib4Nubv");
    glad_glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARBPROC) load(userptr, "glVertexAttrib4NubvARB");
    glad_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) load(userptr, "glVertexAttrib4Nuiv");
    glad_glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARBPROC) load(userptr, "glVertexAttrib4NuivARB");
    glad_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) load(userptr, "glVertexAttrib4Nusv");
    glad_glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARBPROC) load(userptr, "glVertexAttrib4NusvARB");
    glad_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) load(userptr, "glVertexAttrib4bv");
    glad_glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARBPROC) load(userptr, "glVertexAttrib4bvARB");
    glad_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) load(userptr, "glVertexAttrib4d");
    glad_glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARBPROC) load(userptr, "glVertexAttrib4dARB");
    glad_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) load(userptr, "glVertexAttrib4dv");
    glad_glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARBPROC) load(userptr, "glVertexAttrib4dvARB");
    glad_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) load(userptr, "glVertexAttrib4f");
    glad_glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARBPROC) load(userptr, "glVertexAttrib4fARB");
    glad_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) load(userptr, "glVertexAttrib4fv");
    glad_glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARBPROC) load(userptr, "glVertexAttrib4fvARB");
    glad_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) load(userptr, "glVertexAttrib4iv");
    glad_glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARBPROC) load(userptr, "glVertexAttrib4ivARB");
    glad_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) load(userptr, "glVertexAttrib4s");
    glad_glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARBPROC) load(userptr, "glVertexAttrib4sARB");
    glad_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) load(userptr, "glVertexAttrib4sv");
    glad_glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARBPROC) load(userptr, "glVertexAttrib4svARB");
    glad_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) load(userptr, "glVertexAttrib4ubv");
    glad_glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARBPROC) load(userptr, "glVertexAttrib4ubvARB");
    glad_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) load(userptr, "glVertexAttrib4uiv");
    glad_glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARBPROC) load(userptr, "glVertexAttrib4uivARB");
    glad_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) load(userptr, "glVertexAttrib4usv");
    glad_glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARBPROC) load(userptr, "glVertexAttrib4usvARB");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) load(userptr, "glVertexAttribPointer");
    glad_glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC) load(userptr, "glVertexAttribPointerARB");
}
static void glad_gl_load_GL_ARB_vertex_shader( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_shader) return;
    glad_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) load(userptr, "glBindAttribLocation");
    glad_glBindAttribLocationARB = (PFNGLBINDATTRIBLOCATIONARBPROC) load(userptr, "glBindAttribLocationARB");
    glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) load(userptr, "glDisableVertexAttribArray");
    glad_glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) load(userptr, "glDisableVertexAttribArrayARB");
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load(userptr, "glEnableVertexAttribArray");
    glad_glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC) load(userptr, "glEnableVertexAttribArrayARB");
    glad_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) load(userptr, "glGetActiveAttrib");
    glad_glGetActiveAttribARB = (PFNGLGETACTIVEATTRIBARBPROC) load(userptr, "glGetActiveAttribARB");
    glad_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) load(userptr, "glGetAttribLocation");
    glad_glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC) load(userptr, "glGetAttribLocationARB");
    glad_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) load(userptr, "glGetVertexAttribPointerv");
    glad_glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC) load(userptr, "glGetVertexAttribPointervARB");
    glad_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) load(userptr, "glGetVertexAttribdv");
    glad_glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARBPROC) load(userptr, "glGetVertexAttribdvARB");
    glad_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) load(userptr, "glGetVertexAttribfv");
    glad_glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARBPROC) load(userptr, "glGetVertexAttribfvARB");
    glad_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) load(userptr, "glGetVertexAttribiv");
    glad_glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARBPROC) load(userptr, "glGetVertexAttribivARB");
    glad_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) load(userptr, "glVertexAttrib1d");
    glad_glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARBPROC) load(userptr, "glVertexAttrib1dARB");
    glad_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) load(userptr, "glVertexAttrib1dv");
    glad_glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARBPROC) load(userptr, "glVertexAttrib1dvARB");
    glad_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) load(userptr, "glVertexAttrib1f");
    glad_glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC) load(userptr, "glVertexAttrib1fARB");
    glad_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) load(userptr, "glVertexAttrib1fv");
    glad_glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARBPROC) load(userptr, "glVertexAttrib1fvARB");
    glad_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) load(userptr, "glVertexAttrib1s");
    glad_glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARBPROC) load(userptr, "glVertexAttrib1sARB");
    glad_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) load(userptr, "glVertexAttrib1sv");
    glad_glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARBPROC) load(userptr, "glVertexAttrib1svARB");
    glad_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) load(userptr, "glVertexAttrib2d");
    glad_glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARBPROC) load(userptr, "glVertexAttrib2dARB");
    glad_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) load(userptr, "glVertexAttrib2dv");
    glad_glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARBPROC) load(userptr, "glVertexAttrib2dvARB");
    glad_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) load(userptr, "glVertexAttrib2f");
    glad_glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARBPROC) load(userptr, "glVertexAttrib2fARB");
    glad_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) load(userptr, "glVertexAttrib2fv");
    glad_glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARBPROC) load(userptr, "glVertexAttrib2fvARB");
    glad_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) load(userptr, "glVertexAttrib2s");
    glad_glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARBPROC) load(userptr, "glVertexAttrib2sARB");
    glad_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) load(userptr, "glVertexAttrib2sv");
    glad_glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARBPROC) load(userptr, "glVertexAttrib2svARB");
    glad_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) load(userptr, "glVertexAttrib3d");
    glad_glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARBPROC) load(userptr, "glVertexAttrib3dARB");
    glad_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) load(userptr, "glVertexAttrib3dv");
    glad_glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARBPROC) load(userptr, "glVertexAttrib3dvARB");
    glad_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) load(userptr, "glVertexAttrib3f");
    glad_glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC) load(userptr, "glVertexAttrib3fARB");
    glad_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) load(userptr, "glVertexAttrib3fv");
    glad_glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC) load(userptr, "glVertexAttrib3fvARB");
    glad_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) load(userptr, "glVertexAttrib3s");
    glad_glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARBPROC) load(userptr, "glVertexAttrib3sARB");
    glad_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) load(userptr, "glVertexAttrib3sv");
    glad_glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARBPROC) load(userptr, "glVertexAttrib3svARB");
    glad_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) load(userptr, "glVertexAttrib4Nbv");
    glad_glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARBPROC) load(userptr, "glVertexAttrib4NbvARB");
    glad_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) load(userptr, "glVertexAttrib4Niv");
    glad_glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARBPROC) load(userptr, "glVertexAttrib4NivARB");
    glad_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) load(userptr, "glVertexAttrib4Nsv");
    glad_glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARBPROC) load(userptr, "glVertexAttrib4NsvARB");
    glad_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) load(userptr, "glVertexAttrib4Nub");
    glad_glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARBPROC) load(userptr, "glVertexAttrib4NubARB");
    glad_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) load(userptr, "glVertexAttrib4Nubv");
    glad_glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARBPROC) load(userptr, "glVertexAttrib4NubvARB");
    glad_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) load(userptr, "glVertexAttrib4Nuiv");
    glad_glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARBPROC) load(userptr, "glVertexAttrib4NuivARB");
    glad_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) load(userptr, "glVertexAttrib4Nusv");
    glad_glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARBPROC) load(userptr, "glVertexAttrib4NusvARB");
    glad_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) load(userptr, "glVertexAttrib4bv");
    glad_glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARBPROC) load(userptr, "glVertexAttrib4bvARB");
    glad_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) load(userptr, "glVertexAttrib4d");
    glad_glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARBPROC) load(userptr, "glVertexAttrib4dARB");
    glad_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) load(userptr, "glVertexAttrib4dv");
    glad_glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARBPROC) load(userptr, "glVertexAttrib4dvARB");
    glad_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) load(userptr, "glVertexAttrib4f");
    glad_glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARBPROC) load(userptr, "glVertexAttrib4fARB");
    glad_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) load(userptr, "glVertexAttrib4fv");
    glad_glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARBPROC) load(userptr, "glVertexAttrib4fvARB");
    glad_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) load(userptr, "glVertexAttrib4iv");
    glad_glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARBPROC) load(userptr, "glVertexAttrib4ivARB");
    glad_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) load(userptr, "glVertexAttrib4s");
    glad_glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARBPROC) load(userptr, "glVertexAttrib4sARB");
    glad_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) load(userptr, "glVertexAttrib4sv");
    glad_glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARBPROC) load(userptr, "glVertexAttrib4svARB");
    glad_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) load(userptr, "glVertexAttrib4ubv");
    glad_glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARBPROC) load(userptr, "glVertexAttrib4ubvARB");
    glad_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) load(userptr, "glVertexAttrib4uiv");
    glad_glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARBPROC) load(userptr, "glVertexAttrib4uivARB");
    glad_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) load(userptr, "glVertexAttrib4usv");
    glad_glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARBPROC) load(userptr, "glVertexAttrib4usvARB");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) load(userptr, "glVertexAttribPointer");
    glad_glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC) load(userptr, "glVertexAttribPointerARB");
}
static void glad_gl_load_GL_ARB_vertex_type_2_10_10_10_rev( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_vertex_type_2_10_10_10_rev) return;
    glad_glColorP3ui = (PFNGLCOLORP3UIPROC) load(userptr, "glColorP3ui");
    glad_glColorP3uiv = (PFNGLCOLORP3UIVPROC) load(userptr, "glColorP3uiv");
    glad_glColorP4ui = (PFNGLCOLORP4UIPROC) load(userptr, "glColorP4ui");
    glad_glColorP4uiv = (PFNGLCOLORP4UIVPROC) load(userptr, "glColorP4uiv");
    glad_glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC) load(userptr, "glMultiTexCoordP1ui");
    glad_glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC) load(userptr, "glMultiTexCoordP1uiv");
    glad_glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC) load(userptr, "glMultiTexCoordP2ui");
    glad_glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC) load(userptr, "glMultiTexCoordP2uiv");
    glad_glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC) load(userptr, "glMultiTexCoordP3ui");
    glad_glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC) load(userptr, "glMultiTexCoordP3uiv");
    glad_glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC) load(userptr, "glMultiTexCoordP4ui");
    glad_glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC) load(userptr, "glMultiTexCoordP4uiv");
    glad_glNormalP3ui = (PFNGLNORMALP3UIPROC) load(userptr, "glNormalP3ui");
    glad_glNormalP3uiv = (PFNGLNORMALP3UIVPROC) load(userptr, "glNormalP3uiv");
    glad_glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC) load(userptr, "glSecondaryColorP3ui");
    glad_glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC) load(userptr, "glSecondaryColorP3uiv");
    glad_glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC) load(userptr, "glTexCoordP1ui");
    glad_glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC) load(userptr, "glTexCoordP1uiv");
    glad_glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC) load(userptr, "glTexCoordP2ui");
    glad_glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC) load(userptr, "glTexCoordP2uiv");
    glad_glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC) load(userptr, "glTexCoordP3ui");
    glad_glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC) load(userptr, "glTexCoordP3uiv");
    glad_glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC) load(userptr, "glTexCoordP4ui");
    glad_glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC) load(userptr, "glTexCoordP4uiv");
    glad_glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC) load(userptr, "glVertexAttribP1ui");
    glad_glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC) load(userptr, "glVertexAttribP1uiv");
    glad_glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC) load(userptr, "glVertexAttribP2ui");
    glad_glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC) load(userptr, "glVertexAttribP2uiv");
    glad_glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC) load(userptr, "glVertexAttribP3ui");
    glad_glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC) load(userptr, "glVertexAttribP3uiv");
    glad_glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC) load(userptr, "glVertexAttribP4ui");
    glad_glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC) load(userptr, "glVertexAttribP4uiv");
    glad_glVertexP2ui = (PFNGLVERTEXP2UIPROC) load(userptr, "glVertexP2ui");
    glad_glVertexP2uiv = (PFNGLVERTEXP2UIVPROC) load(userptr, "glVertexP2uiv");
    glad_glVertexP3ui = (PFNGLVERTEXP3UIPROC) load(userptr, "glVertexP3ui");
    glad_glVertexP3uiv = (PFNGLVERTEXP3UIVPROC) load(userptr, "glVertexP3uiv");
    glad_glVertexP4ui = (PFNGLVERTEXP4UIPROC) load(userptr, "glVertexP4ui");
    glad_glVertexP4uiv = (PFNGLVERTEXP4UIVPROC) load(userptr, "glVertexP4uiv");
}
static void glad_gl_load_GL_ARB_viewport_array( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_viewport_array) return;
    glad_glDepthRangeArraydvNV = (PFNGLDEPTHRANGEARRAYDVNVPROC) load(userptr, "glDepthRangeArraydvNV");
    glad_glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC) load(userptr, "glDepthRangeArrayv");
    glad_glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC) load(userptr, "glDepthRangeIndexed");
    glad_glDepthRangeIndexeddNV = (PFNGLDEPTHRANGEINDEXEDDNVPROC) load(userptr, "glDepthRangeIndexeddNV");
    glad_glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC) load(userptr, "glGetDoublei_v");
    glad_glGetFloati_v = (PFNGLGETFLOATI_VPROC) load(userptr, "glGetFloati_v");
    glad_glScissorArrayv = (PFNGLSCISSORARRAYVPROC) load(userptr, "glScissorArrayv");
    glad_glScissorIndexed = (PFNGLSCISSORINDEXEDPROC) load(userptr, "glScissorIndexed");
    glad_glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC) load(userptr, "glScissorIndexedv");
    glad_glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC) load(userptr, "glViewportArrayv");
    glad_glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC) load(userptr, "glViewportIndexedf");
    glad_glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC) load(userptr, "glViewportIndexedfv");
}
static void glad_gl_load_GL_ARB_window_pos( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_ARB_window_pos) return;
    glad_glWindowPos2d = (PFNGLWINDOWPOS2DPROC) load(userptr, "glWindowPos2d");
    glad_glWindowPos2dARB = (PFNGLWINDOWPOS2DARBPROC) load(userptr, "glWindowPos2dARB");
    glad_glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC) load(userptr, "glWindowPos2dv");
    glad_glWindowPos2dvARB = (PFNGLWINDOWPOS2DVARBPROC) load(userptr, "glWindowPos2dvARB");
    glad_glWindowPos2f = (PFNGLWINDOWPOS2FPROC) load(userptr, "glWindowPos2f");
    glad_glWindowPos2fARB = (PFNGLWINDOWPOS2FARBPROC) load(userptr, "glWindowPos2fARB");
    glad_glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC) load(userptr, "glWindowPos2fv");
    glad_glWindowPos2fvARB = (PFNGLWINDOWPOS2FVARBPROC) load(userptr, "glWindowPos2fvARB");
    glad_glWindowPos2i = (PFNGLWINDOWPOS2IPROC) load(userptr, "glWindowPos2i");
    glad_glWindowPos2iARB = (PFNGLWINDOWPOS2IARBPROC) load(userptr, "glWindowPos2iARB");
    glad_glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC) load(userptr, "glWindowPos2iv");
    glad_glWindowPos2ivARB = (PFNGLWINDOWPOS2IVARBPROC) load(userptr, "glWindowPos2ivARB");
    glad_glWindowPos2s = (PFNGLWINDOWPOS2SPROC) load(userptr, "glWindowPos2s");
    glad_glWindowPos2sARB = (PFNGLWINDOWPOS2SARBPROC) load(userptr, "glWindowPos2sARB");
    glad_glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC) load(userptr, "glWindowPos2sv");
    glad_glWindowPos2svARB = (PFNGLWINDOWPOS2SVARBPROC) load(userptr, "glWindowPos2svARB");
    glad_glWindowPos3d = (PFNGLWINDOWPOS3DPROC) load(userptr, "glWindowPos3d");
    glad_glWindowPos3dARB = (PFNGLWINDOWPOS3DARBPROC) load(userptr, "glWindowPos3dARB");
    glad_glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC) load(userptr, "glWindowPos3dv");
    glad_glWindowPos3dvARB = (PFNGLWINDOWPOS3DVARBPROC) load(userptr, "glWindowPos3dvARB");
    glad_glWindowPos3f = (PFNGLWINDOWPOS3FPROC) load(userptr, "glWindowPos3f");
    glad_glWindowPos3fARB = (PFNGLWINDOWPOS3FARBPROC) load(userptr, "glWindowPos3fARB");
    glad_glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC) load(userptr, "glWindowPos3fv");
    glad_glWindowPos3fvARB = (PFNGLWINDOWPOS3FVARBPROC) load(userptr, "glWindowPos3fvARB");
    glad_glWindowPos3i = (PFNGLWINDOWPOS3IPROC) load(userptr, "glWindowPos3i");
    glad_glWindowPos3iARB = (PFNGLWINDOWPOS3IARBPROC) load(userptr, "glWindowPos3iARB");
    glad_glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC) load(userptr, "glWindowPos3iv");
    glad_glWindowPos3ivARB = (PFNGLWINDOWPOS3IVARBPROC) load(userptr, "glWindowPos3ivARB");
    glad_glWindowPos3s = (PFNGLWINDOWPOS3SPROC) load(userptr, "glWindowPos3s");
    glad_glWindowPos3sARB = (PFNGLWINDOWPOS3SARBPROC) load(userptr, "glWindowPos3sARB");
    glad_glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC) load(userptr, "glWindowPos3sv");
    glad_glWindowPos3svARB = (PFNGLWINDOWPOS3SVARBPROC) load(userptr, "glWindowPos3svARB");
}
static void glad_gl_load_GL_KHR_blend_equation_advanced( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_KHR_blend_equation_advanced) return;
    glad_glBlendBarrier = (PFNGLBLENDBARRIERPROC) load(userptr, "glBlendBarrier");
    glad_glBlendBarrierKHR = (PFNGLBLENDBARRIERKHRPROC) load(userptr, "glBlendBarrierKHR");
}
static void glad_gl_load_GL_KHR_debug( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_KHR_debug) return;
    glad_glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC) load(userptr, "glDebugMessageCallback");
    glad_glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC) load(userptr, "glDebugMessageControl");
    glad_glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC) load(userptr, "glDebugMessageInsert");
    glad_glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC) load(userptr, "glGetDebugMessageLog");
    glad_glGetObjectLabel = (PFNGLGETOBJECTLABELPROC) load(userptr, "glGetObjectLabel");
    glad_glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC) load(userptr, "glGetObjectPtrLabel");
    glad_glGetPointerv = (PFNGLGETPOINTERVPROC) load(userptr, "glGetPointerv");
    glad_glObjectLabel = (PFNGLOBJECTLABELPROC) load(userptr, "glObjectLabel");
    glad_glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC) load(userptr, "glObjectPtrLabel");
    glad_glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC) load(userptr, "glPopDebugGroup");
    glad_glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC) load(userptr, "glPushDebugGroup");
}
static void glad_gl_load_GL_KHR_parallel_shader_compile( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_KHR_parallel_shader_compile) return;
    glad_glMaxShaderCompilerThreadsKHR = (PFNGLMAXSHADERCOMPILERTHREADSKHRPROC) load(userptr, "glMaxShaderCompilerThreadsKHR");
}
static void glad_gl_load_GL_KHR_robustness( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_KHR_robustness) return;
    glad_glGetGraphicsResetStatus = (PFNGLGETGRAPHICSRESETSTATUSPROC) load(userptr, "glGetGraphicsResetStatus");
    glad_glGetnUniformfv = (PFNGLGETNUNIFORMFVPROC) load(userptr, "glGetnUniformfv");
    glad_glGetnUniformiv = (PFNGLGETNUNIFORMIVPROC) load(userptr, "glGetnUniformiv");
    glad_glGetnUniformuiv = (PFNGLGETNUNIFORMUIVPROC) load(userptr, "glGetnUniformuiv");
    glad_glReadnPixels = (PFNGLREADNPIXELSPROC) load(userptr, "glReadnPixels");
}



#if defined(GL_ES_VERSION_3_0) || defined(GL_VERSION_3_0)
#define GLAD_GL_IS_SOME_NEW_VERSION 1
#else
#define GLAD_GL_IS_SOME_NEW_VERSION 0
#endif

static int glad_gl_get_extensions( int version, const char **out_exts, unsigned int *out_num_exts_i, char ***out_exts_i) {
#if GLAD_GL_IS_SOME_NEW_VERSION
    if(GLAD_VERSION_MAJOR(version) < 3) {
#else
    (void) version;
    (void) out_num_exts_i;
    (void) out_exts_i;
#endif
        if (glad_glGetString == NULL) {
            return 0;
        }
        *out_exts = (const char *)glad_glGetString(GL_EXTENSIONS);
#if GLAD_GL_IS_SOME_NEW_VERSION
    } else {
        unsigned int index = 0;
        unsigned int num_exts_i = 0;
        char **exts_i = NULL;
        if (glad_glGetStringi == NULL || glad_glGetIntegerv == NULL) {
            return 0;
        }
        glad_glGetIntegerv(GL_NUM_EXTENSIONS, (int*) &num_exts_i);
        if (num_exts_i > 0) {
            exts_i = (char **) malloc(num_exts_i * (sizeof *exts_i));
        }
        if (exts_i == NULL) {
            return 0;
        }
        for(index = 0; index < num_exts_i; index++) {
            const char *gl_str_tmp = (const char*) glad_glGetStringi(GL_EXTENSIONS, index);
            size_t len = strlen(gl_str_tmp) + 1;

            char *local_str = (char*) malloc(len * sizeof(char));
            if(local_str != NULL) {
                memcpy(local_str, gl_str_tmp, len * sizeof(char));
            }

            exts_i[index] = local_str;
        }

        *out_num_exts_i = num_exts_i;
        *out_exts_i = exts_i;
    }
#endif
    return 1;
}
static void glad_gl_free_extensions(char **exts_i, unsigned int num_exts_i) {
    if (exts_i != NULL) {
        unsigned int index;
        for(index = 0; index < num_exts_i; index++) {
            free((void *) (exts_i[index]));
        }
        free((void *)exts_i);
        exts_i = NULL;
    }
}
static int glad_gl_has_extension(int version, const char *exts, unsigned int num_exts_i, char **exts_i, const char *ext) {
    if(GLAD_VERSION_MAJOR(version) < 3 || !GLAD_GL_IS_SOME_NEW_VERSION) {
        const char *extensions;
        const char *loc;
        const char *terminator;
        extensions = exts;
        if(extensions == NULL || ext == NULL) {
            return 0;
        }
        while(1) {
            loc = strstr(extensions, ext);
            if(loc == NULL) {
                return 0;
            }
            terminator = loc + strlen(ext);
            if((loc == extensions || *(loc - 1) == ' ') &&
                (*terminator == ' ' || *terminator == '\0')) {
                return 1;
            }
            extensions = terminator;
        }
    } else {
        unsigned int index;
        for(index = 0; index < num_exts_i; index++) {
            const char *e = exts_i[index];
            if(strcmp(e, ext) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

static GLADapiproc glad_gl_get_proc_from_userptr(void *userptr, const char* name) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int glad_gl_find_extensions_gl( int version) {
    const char *exts = NULL;
    unsigned int num_exts_i = 0;
    char **exts_i = NULL;
    if (!glad_gl_get_extensions(version, &exts, &num_exts_i, &exts_i)) return 0;

    GLAD_GL_ARB_ES2_compatibility = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_ES2_compatibility");
    GLAD_GL_ARB_ES3_1_compatibility = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_ES3_1_compatibility");
    GLAD_GL_ARB_ES3_2_compatibility = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_ES3_2_compatibility");
    GLAD_GL_ARB_ES3_compatibility = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_ES3_compatibility");
    GLAD_GL_ARB_arrays_of_arrays = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_arrays_of_arrays");
    GLAD_GL_ARB_base_instance = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_base_instance");
    GLAD_GL_ARB_bindless_texture = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_bindless_texture");
    GLAD_GL_ARB_blend_func_extended = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_blend_func_extended");
    GLAD_GL_ARB_buffer_storage = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_buffer_storage");
    GLAD_GL_ARB_cl_event = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_cl_event");
    GLAD_GL_ARB_clear_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_clear_buffer_object");
    GLAD_GL_ARB_clear_texture = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_clear_texture");
    GLAD_GL_ARB_clip_control = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_clip_control");
    GLAD_GL_ARB_color_buffer_float = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_color_buffer_float");
    GLAD_GL_ARB_compatibility = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_compatibility");
    GLAD_GL_ARB_compressed_texture_pixel_storage = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_compressed_texture_pixel_storage");
    GLAD_GL_ARB_compute_shader = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_compute_shader");
    GLAD_GL_ARB_compute_variable_group_size = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_compute_variable_group_size");
    GLAD_GL_ARB_conditional_render_inverted = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_conditional_render_inverted");
    GLAD_GL_ARB_conservative_depth = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_conservative_depth");
    GLAD_GL_ARB_copy_buffer = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_copy_buffer");
    GLAD_GL_ARB_copy_image = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_copy_image");
    GLAD_GL_ARB_cull_distance = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_cull_distance");
    GLAD_GL_ARB_debug_output = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_debug_output");
    GLAD_GL_ARB_depth_buffer_float = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_depth_buffer_float");
    GLAD_GL_ARB_depth_clamp = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_depth_clamp");
    GLAD_GL_ARB_depth_texture = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_depth_texture");
    GLAD_GL_ARB_derivative_control = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_derivative_control");
    GLAD_GL_ARB_direct_state_access = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_direct_state_access");
    GLAD_GL_ARB_draw_buffers = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_draw_buffers");
    GLAD_GL_ARB_draw_buffers_blend = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_draw_buffers_blend");
    GLAD_GL_ARB_draw_elements_base_vertex = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_draw_elements_base_vertex");
    GLAD_GL_ARB_draw_indirect = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_draw_indirect");
    GLAD_GL_ARB_draw_instanced = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_draw_instanced");
    GLAD_GL_ARB_enhanced_layouts = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_enhanced_layouts");
    GLAD_GL_ARB_explicit_attrib_location = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_explicit_attrib_location");
    GLAD_GL_ARB_explicit_uniform_location = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_explicit_uniform_location");
    GLAD_GL_ARB_fragment_coord_conventions = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_fragment_coord_conventions");
    GLAD_GL_ARB_fragment_layer_viewport = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_fragment_layer_viewport");
    GLAD_GL_ARB_fragment_program = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_fragment_program");
    GLAD_GL_ARB_fragment_program_shadow = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_fragment_program_shadow");
    GLAD_GL_ARB_fragment_shader = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_fragment_shader");
    GLAD_GL_ARB_fragment_shader_interlock = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_fragment_shader_interlock");
    GLAD_GL_ARB_framebuffer_no_attachments = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_framebuffer_no_attachments");
    GLAD_GL_ARB_framebuffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_framebuffer_object");
    GLAD_GL_ARB_framebuffer_sRGB = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_framebuffer_sRGB");
    GLAD_GL_ARB_geometry_shader4 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_geometry_shader4");
    GLAD_GL_ARB_get_program_binary = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_get_program_binary");
    GLAD_GL_ARB_get_texture_sub_image = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_get_texture_sub_image");
    GLAD_GL_ARB_gl_spirv = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_gl_spirv");
    GLAD_GL_ARB_gpu_shader5 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_gpu_shader5");
    GLAD_GL_ARB_gpu_shader_fp64 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_gpu_shader_fp64");
    GLAD_GL_ARB_gpu_shader_int64 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_gpu_shader_int64");
    GLAD_GL_ARB_half_float_pixel = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_half_float_pixel");
    GLAD_GL_ARB_half_float_vertex = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_half_float_vertex");
    GLAD_GL_ARB_imaging = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_imaging");
    GLAD_GL_ARB_indirect_parameters = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_indirect_parameters");
    GLAD_GL_ARB_instanced_arrays = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_instanced_arrays");
    GLAD_GL_ARB_internalformat_query = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_internalformat_query");
    GLAD_GL_ARB_internalformat_query2 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_internalformat_query2");
    GLAD_GL_ARB_invalidate_subdata = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_invalidate_subdata");
    GLAD_GL_ARB_map_buffer_alignment = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_map_buffer_alignment");
    GLAD_GL_ARB_map_buffer_range = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_map_buffer_range");
    GLAD_GL_ARB_matrix_palette = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_matrix_palette");
    GLAD_GL_ARB_multi_bind = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_multi_bind");
    GLAD_GL_ARB_multi_draw_indirect = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_multi_draw_indirect");
    GLAD_GL_ARB_multisample = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_multisample");
    GLAD_GL_ARB_multitexture = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_multitexture");
    GLAD_GL_ARB_occlusion_query = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_occlusion_query");
    GLAD_GL_ARB_occlusion_query2 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_occlusion_query2");
    GLAD_GL_ARB_parallel_shader_compile = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_parallel_shader_compile");
    GLAD_GL_ARB_pipeline_statistics_query = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_pipeline_statistics_query");
    GLAD_GL_ARB_pixel_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_pixel_buffer_object");
    GLAD_GL_ARB_point_parameters = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_point_parameters");
    GLAD_GL_ARB_point_sprite = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_point_sprite");
    GLAD_GL_ARB_polygon_offset_clamp = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_polygon_offset_clamp");
    GLAD_GL_ARB_post_depth_coverage = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_post_depth_coverage");
    GLAD_GL_ARB_program_interface_query = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_program_interface_query");
    GLAD_GL_ARB_provoking_vertex = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_provoking_vertex");
    GLAD_GL_ARB_query_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_query_buffer_object");
    GLAD_GL_ARB_robust_buffer_access_behavior = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_robust_buffer_access_behavior");
    GLAD_GL_ARB_robustness = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_robustness");
    GLAD_GL_ARB_robustness_isolation = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_robustness_isolation");
    GLAD_GL_ARB_sample_locations = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sample_locations");
    GLAD_GL_ARB_sample_shading = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sample_shading");
    GLAD_GL_ARB_sampler_objects = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sampler_objects");
    GLAD_GL_ARB_seamless_cube_map = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_seamless_cube_map");
    GLAD_GL_ARB_seamless_cubemap_per_texture = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_seamless_cubemap_per_texture");
    GLAD_GL_ARB_separate_shader_objects = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_separate_shader_objects");
    GLAD_GL_ARB_shader_atomic_counter_ops = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_atomic_counter_ops");
    GLAD_GL_ARB_shader_atomic_counters = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_atomic_counters");
    GLAD_GL_ARB_shader_ballot = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_ballot");
    GLAD_GL_ARB_shader_bit_encoding = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_bit_encoding");
    GLAD_GL_ARB_shader_clock = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_clock");
    GLAD_GL_ARB_shader_draw_parameters = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_draw_parameters");
    GLAD_GL_ARB_shader_group_vote = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_group_vote");
    GLAD_GL_ARB_shader_image_load_store = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_image_load_store");
    GLAD_GL_ARB_shader_image_size = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_image_size");
    GLAD_GL_ARB_shader_objects = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_objects");
    GLAD_GL_ARB_shader_precision = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_precision");
    GLAD_GL_ARB_shader_stencil_export = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_stencil_export");
    GLAD_GL_ARB_shader_storage_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_storage_buffer_object");
    GLAD_GL_ARB_shader_subroutine = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_subroutine");
    GLAD_GL_ARB_shader_texture_image_samples = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_texture_image_samples");
    GLAD_GL_ARB_shader_texture_lod = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_texture_lod");
    GLAD_GL_ARB_shader_viewport_layer_array = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shader_viewport_layer_array");
    GLAD_GL_ARB_shading_language_100 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shading_language_100");
    GLAD_GL_ARB_shading_language_420pack = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shading_language_420pack");
    GLAD_GL_ARB_shading_language_include = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shading_language_include");
    GLAD_GL_ARB_shading_language_packing = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shading_language_packing");
    GLAD_GL_ARB_shadow = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shadow");
    GLAD_GL_ARB_shadow_ambient = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_shadow_ambient");
    GLAD_GL_ARB_sparse_buffer = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sparse_buffer");
    GLAD_GL_ARB_sparse_texture = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sparse_texture");
    GLAD_GL_ARB_sparse_texture2 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sparse_texture2");
    GLAD_GL_ARB_sparse_texture_clamp = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sparse_texture_clamp");
    GLAD_GL_ARB_spirv_extensions = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_spirv_extensions");
    GLAD_GL_ARB_stencil_texturing = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_stencil_texturing");
    GLAD_GL_ARB_sync = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_sync");
    GLAD_GL_ARB_tessellation_shader = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_tessellation_shader");
    GLAD_GL_ARB_texture_barrier = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_barrier");
    GLAD_GL_ARB_texture_border_clamp = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_border_clamp");
    GLAD_GL_ARB_texture_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_buffer_object");
    GLAD_GL_ARB_texture_buffer_object_rgb32 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_buffer_object_rgb32");
    GLAD_GL_ARB_texture_buffer_range = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_buffer_range");
    GLAD_GL_ARB_texture_compression = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_compression");
    GLAD_GL_ARB_texture_compression_bptc = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_compression_bptc");
    GLAD_GL_ARB_texture_compression_rgtc = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_compression_rgtc");
    GLAD_GL_ARB_texture_cube_map = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_cube_map");
    GLAD_GL_ARB_texture_cube_map_array = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_cube_map_array");
    GLAD_GL_ARB_texture_env_add = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_env_add");
    GLAD_GL_ARB_texture_env_combine = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_env_combine");
    GLAD_GL_ARB_texture_env_crossbar = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_env_crossbar");
    GLAD_GL_ARB_texture_env_dot3 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_env_dot3");
    GLAD_GL_ARB_texture_filter_anisotropic = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_filter_anisotropic");
    GLAD_GL_ARB_texture_filter_minmax = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_filter_minmax");
    GLAD_GL_ARB_texture_float = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_float");
    GLAD_GL_ARB_texture_gather = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_gather");
    GLAD_GL_ARB_texture_mirror_clamp_to_edge = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_mirror_clamp_to_edge");
    GLAD_GL_ARB_texture_mirrored_repeat = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_mirrored_repeat");
    GLAD_GL_ARB_texture_multisample = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_multisample");
    GLAD_GL_ARB_texture_non_power_of_two = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_non_power_of_two");
    GLAD_GL_ARB_texture_query_levels = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_query_levels");
    GLAD_GL_ARB_texture_query_lod = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_query_lod");
    GLAD_GL_ARB_texture_rectangle = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_rectangle");
    GLAD_GL_ARB_texture_rg = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_rg");
    GLAD_GL_ARB_texture_rgb10_a2ui = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_rgb10_a2ui");
    GLAD_GL_ARB_texture_stencil8 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_stencil8");
    GLAD_GL_ARB_texture_storage = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_storage");
    GLAD_GL_ARB_texture_storage_multisample = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_storage_multisample");
    GLAD_GL_ARB_texture_swizzle = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_swizzle");
    GLAD_GL_ARB_texture_view = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_texture_view");
    GLAD_GL_ARB_timer_query = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_timer_query");
    GLAD_GL_ARB_transform_feedback2 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_transform_feedback2");
    GLAD_GL_ARB_transform_feedback3 = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_transform_feedback3");
    GLAD_GL_ARB_transform_feedback_instanced = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_transform_feedback_instanced");
    GLAD_GL_ARB_transform_feedback_overflow_query = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_transform_feedback_overflow_query");
    GLAD_GL_ARB_transpose_matrix = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_transpose_matrix");
    GLAD_GL_ARB_uniform_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_uniform_buffer_object");
    GLAD_GL_ARB_vertex_array_bgra = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_array_bgra");
    GLAD_GL_ARB_vertex_array_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_array_object");
    GLAD_GL_ARB_vertex_attrib_64bit = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_attrib_64bit");
    GLAD_GL_ARB_vertex_attrib_binding = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_attrib_binding");
    GLAD_GL_ARB_vertex_blend = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_blend");
    GLAD_GL_ARB_vertex_buffer_object = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_buffer_object");
    GLAD_GL_ARB_vertex_program = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_program");
    GLAD_GL_ARB_vertex_shader = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_shader");
    GLAD_GL_ARB_vertex_type_10f_11f_11f_rev = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_type_10f_11f_11f_rev");
    GLAD_GL_ARB_vertex_type_2_10_10_10_rev = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_vertex_type_2_10_10_10_rev");
    GLAD_GL_ARB_viewport_array = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_viewport_array");
    GLAD_GL_ARB_window_pos = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_ARB_window_pos");
    GLAD_GL_KHR_blend_equation_advanced = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_blend_equation_advanced");
    GLAD_GL_KHR_blend_equation_advanced_coherent = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_blend_equation_advanced_coherent");
    GLAD_GL_KHR_context_flush_control = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_context_flush_control");
    GLAD_GL_KHR_debug = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_debug");
    GLAD_GL_KHR_no_error = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_no_error");
    GLAD_GL_KHR_parallel_shader_compile = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_parallel_shader_compile");
    GLAD_GL_KHR_robust_buffer_access_behavior = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_robust_buffer_access_behavior");
    GLAD_GL_KHR_robustness = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_robustness");
    GLAD_GL_KHR_shader_subgroup = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_shader_subgroup");
    GLAD_GL_KHR_texture_compression_astc_hdr = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_texture_compression_astc_hdr");
    GLAD_GL_KHR_texture_compression_astc_ldr = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_texture_compression_astc_ldr");
    GLAD_GL_KHR_texture_compression_astc_sliced_3d = glad_gl_has_extension(version, exts, num_exts_i, exts_i, "GL_KHR_texture_compression_astc_sliced_3d");

    glad_gl_free_extensions(exts_i, num_exts_i);

    return 1;
}

static int glad_gl_find_core_gl(void) {
    int i, major, minor;
    const char* version;
    const char* prefixes[] = {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES ",
        NULL
    };
    version = (const char*) glad_glGetString(GL_VERSION);
    if (!version) return 0;
    for (i = 0;  prefixes[i];  i++) {
        const size_t length = strlen(prefixes[i]);
        if (strncmp(version, prefixes[i], length) == 0) {
            version += length;
            break;
        }
    }

    GLAD_IMPL_UTIL_SSCANF(version, "%d.%d", &major, &minor);

    GLAD_GL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    GLAD_GL_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
    GLAD_GL_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
    GLAD_GL_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
    GLAD_GL_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
    GLAD_GL_VERSION_1_5 = (major == 1 && minor >= 5) || major > 1;
    GLAD_GL_VERSION_2_0 = (major == 2 && minor >= 0) || major > 2;
    GLAD_GL_VERSION_2_1 = (major == 2 && minor >= 1) || major > 2;
    GLAD_GL_VERSION_3_0 = (major == 3 && minor >= 0) || major > 3;
    GLAD_GL_VERSION_3_1 = (major == 3 && minor >= 1) || major > 3;
    GLAD_GL_VERSION_3_2 = (major == 3 && minor >= 2) || major > 3;
    GLAD_GL_VERSION_3_3 = (major == 3 && minor >= 3) || major > 3;

    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadGLUserPtr( GLADuserptrloadfunc load, void *userptr) {
    int version;

    glad_glGetString = (PFNGLGETSTRINGPROC) load(userptr, "glGetString");
    if(glad_glGetString == NULL) return 0;
    if(glad_glGetString(GL_VERSION) == NULL) return 0;
    version = glad_gl_find_core_gl();

    glad_gl_load_GL_VERSION_1_0(load, userptr);
    glad_gl_load_GL_VERSION_1_1(load, userptr);
    glad_gl_load_GL_VERSION_1_2(load, userptr);
    glad_gl_load_GL_VERSION_1_3(load, userptr);
    glad_gl_load_GL_VERSION_1_4(load, userptr);
    glad_gl_load_GL_VERSION_1_5(load, userptr);
    glad_gl_load_GL_VERSION_2_0(load, userptr);
    glad_gl_load_GL_VERSION_2_1(load, userptr);
    glad_gl_load_GL_VERSION_3_0(load, userptr);
    glad_gl_load_GL_VERSION_3_1(load, userptr);
    glad_gl_load_GL_VERSION_3_2(load, userptr);
    glad_gl_load_GL_VERSION_3_3(load, userptr);

    if (!glad_gl_find_extensions_gl(version)) return 0;
    glad_gl_load_GL_ARB_ES2_compatibility(load, userptr);
    glad_gl_load_GL_ARB_ES3_1_compatibility(load, userptr);
    glad_gl_load_GL_ARB_ES3_2_compatibility(load, userptr);
    glad_gl_load_GL_ARB_base_instance(load, userptr);
    glad_gl_load_GL_ARB_bindless_texture(load, userptr);
    glad_gl_load_GL_ARB_blend_func_extended(load, userptr);
    glad_gl_load_GL_ARB_buffer_storage(load, userptr);
    glad_gl_load_GL_ARB_cl_event(load, userptr);
    glad_gl_load_GL_ARB_clear_buffer_object(load, userptr);
    glad_gl_load_GL_ARB_clear_texture(load, userptr);
    glad_gl_load_GL_ARB_clip_control(load, userptr);
    glad_gl_load_GL_ARB_color_buffer_float(load, userptr);
    glad_gl_load_GL_ARB_compute_shader(load, userptr);
    glad_gl_load_GL_ARB_compute_variable_group_size(load, userptr);
    glad_gl_load_GL_ARB_copy_buffer(load, userptr);
    glad_gl_load_GL_ARB_copy_image(load, userptr);
    glad_gl_load_GL_ARB_debug_output(load, userptr);
    glad_gl_load_GL_ARB_direct_state_access(load, userptr);
    glad_gl_load_GL_ARB_draw_buffers(load, userptr);
    glad_gl_load_GL_ARB_draw_buffers_blend(load, userptr);
    glad_gl_load_GL_ARB_draw_elements_base_vertex(load, userptr);
    glad_gl_load_GL_ARB_draw_indirect(load, userptr);
    glad_gl_load_GL_ARB_draw_instanced(load, userptr);
    glad_gl_load_GL_ARB_fragment_program(load, userptr);
    glad_gl_load_GL_ARB_framebuffer_no_attachments(load, userptr);
    glad_gl_load_GL_ARB_framebuffer_object(load, userptr);
    glad_gl_load_GL_ARB_geometry_shader4(load, userptr);
    glad_gl_load_GL_ARB_get_program_binary(load, userptr);
    glad_gl_load_GL_ARB_get_texture_sub_image(load, userptr);
    glad_gl_load_GL_ARB_gl_spirv(load, userptr);
    glad_gl_load_GL_ARB_gpu_shader_fp64(load, userptr);
    glad_gl_load_GL_ARB_gpu_shader_int64(load, userptr);
    glad_gl_load_GL_ARB_imaging(load, userptr);
    glad_gl_load_GL_ARB_indirect_parameters(load, userptr);
    glad_gl_load_GL_ARB_instanced_arrays(load, userptr);
    glad_gl_load_GL_ARB_internalformat_query(load, userptr);
    glad_gl_load_GL_ARB_internalformat_query2(load, userptr);
    glad_gl_load_GL_ARB_invalidate_subdata(load, userptr);
    glad_gl_load_GL_ARB_map_buffer_range(load, userptr);
    glad_gl_load_GL_ARB_matrix_palette(load, userptr);
    glad_gl_load_GL_ARB_multi_bind(load, userptr);
    glad_gl_load_GL_ARB_multi_draw_indirect(load, userptr);
    glad_gl_load_GL_ARB_multisample(load, userptr);
    glad_gl_load_GL_ARB_multitexture(load, userptr);
    glad_gl_load_GL_ARB_occlusion_query(load, userptr);
    glad_gl_load_GL_ARB_parallel_shader_compile(load, userptr);
    glad_gl_load_GL_ARB_point_parameters(load, userptr);
    glad_gl_load_GL_ARB_polygon_offset_clamp(load, userptr);
    glad_gl_load_GL_ARB_program_interface_query(load, userptr);
    glad_gl_load_GL_ARB_provoking_vertex(load, userptr);
    glad_gl_load_GL_ARB_robustness(load, userptr);
    glad_gl_load_GL_ARB_sample_locations(load, userptr);
    glad_gl_load_GL_ARB_sample_shading(load, userptr);
    glad_gl_load_GL_ARB_sampler_objects(load, userptr);
    glad_gl_load_GL_ARB_separate_shader_objects(load, userptr);
    glad_gl_load_GL_ARB_shader_atomic_counters(load, userptr);
    glad_gl_load_GL_ARB_shader_image_load_store(load, userptr);
    glad_gl_load_GL_ARB_shader_objects(load, userptr);
    glad_gl_load_GL_ARB_shader_storage_buffer_object(load, userptr);
    glad_gl_load_GL_ARB_shader_subroutine(load, userptr);
    glad_gl_load_GL_ARB_shading_language_include(load, userptr);
    glad_gl_load_GL_ARB_sparse_buffer(load, userptr);
    glad_gl_load_GL_ARB_sparse_texture(load, userptr);
    glad_gl_load_GL_ARB_sync(load, userptr);
    glad_gl_load_GL_ARB_tessellation_shader(load, userptr);
    glad_gl_load_GL_ARB_texture_barrier(load, userptr);
    glad_gl_load_GL_ARB_texture_buffer_object(load, userptr);
    glad_gl_load_GL_ARB_texture_buffer_range(load, userptr);
    glad_gl_load_GL_ARB_texture_compression(load, userptr);
    glad_gl_load_GL_ARB_texture_multisample(load, userptr);
    glad_gl_load_GL_ARB_texture_storage(load, userptr);
    glad_gl_load_GL_ARB_texture_storage_multisample(load, userptr);
    glad_gl_load_GL_ARB_texture_view(load, userptr);
    glad_gl_load_GL_ARB_timer_query(load, userptr);
    glad_gl_load_GL_ARB_transform_feedback2(load, userptr);
    glad_gl_load_GL_ARB_transform_feedback3(load, userptr);
    glad_gl_load_GL_ARB_transform_feedback_instanced(load, userptr);
    glad_gl_load_GL_ARB_transpose_matrix(load, userptr);
    glad_gl_load_GL_ARB_uniform_buffer_object(load, userptr);
    glad_gl_load_GL_ARB_vertex_array_object(load, userptr);
    glad_gl_load_GL_ARB_vertex_attrib_64bit(load, userptr);
    glad_gl_load_GL_ARB_vertex_attrib_binding(load, userptr);
    glad_gl_load_GL_ARB_vertex_blend(load, userptr);
    glad_gl_load_GL_ARB_vertex_buffer_object(load, userptr);
    glad_gl_load_GL_ARB_vertex_program(load, userptr);
    glad_gl_load_GL_ARB_vertex_shader(load, userptr);
    glad_gl_load_GL_ARB_vertex_type_2_10_10_10_rev(load, userptr);
    glad_gl_load_GL_ARB_viewport_array(load, userptr);
    glad_gl_load_GL_ARB_window_pos(load, userptr);
    glad_gl_load_GL_KHR_blend_equation_advanced(load, userptr);
    glad_gl_load_GL_KHR_debug(load, userptr);
    glad_gl_load_GL_KHR_parallel_shader_compile(load, userptr);
    glad_gl_load_GL_KHR_robustness(load, userptr);



    return version;
}


int gladLoadGL( GLADloadfunc load) {
    return gladLoadGLUserPtr( glad_gl_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}






#endif /* GLAD_GL_IMPLEMENTATION */

#line 0
