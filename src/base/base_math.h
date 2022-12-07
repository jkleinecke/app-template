#ifndef BASE_MATH_H
#define BASE_MATH_H

#define MATH_USE_SSE 1

#if MATH_USE_SSE
# include <immintrin.h>
// <mmintrin.h>  MMX
// <xmmintrin.h> SSE
// <emmintrin.h> SSE2
// <pmmintrin.h> SSE3
// <tmmintrin.h> SSSE3
// <smmintrin.h> SSE4.1
// <nmmintrin.h> SSE4.2
// <ammintrin.h> SSE4A
// <wmmintrin.h> AES
// <immintrin.h> AVX, AVX2, FMA <-- Some refs say to use this one...
// <zmmintrin.h> AVX512
#endif

union V2I
{
    struct { S32 x, y; };
    struct { S32 width, height; };
    S32 e[2];

    inline S32 &operator[](const int &index) { return e[index]; }
};

union V2F
{
    struct { F32 x, y; };
    struct { F32 u, v; };
    struct { F32 width, height; };
    F32 e[2];

    inline F32 &operator[](const int &index) { return e[index]; }
};

union V3F
{
    struct { F32 x, y, z; };
    struct { F32 u, v, w; };
    struct { F32 r, g, b; };
    struct { F32 width, height, depth; };
    F32 e[3];

    inline F32 &operator[](const int &index) { return e[index]; }
};

union V4F
{
    struct { F32 x, y, z, w; };
    struct { F32 r, g, b, a; };
    F32 e[4];

#if MATH_USE_SSE
    __m128 internal;
#endif

    inline F32 &operator[](const int &index) { return e[index]; }
    inline const F32 &operator[](const int &index) const { return e[index]; }
};

union M4F
{
    F32 e[4][4];

#if MATH_USE_SSE
    __m128 cols[4];
#endif

    inline V4F operator[](const int &index)
    {
        F32* col = e[index];
        V4F result = {
            .x = col[0],
            .y = col[1],
            .z = col[2],
            .w = col[3]
        };
        return(result);
    }
};

union Quaternion
{
    struct {
        union { 
            V3F xyz;
            struct { F32 x, y, z; };
        };
        F32 w;
    };
    F32 e[4];
#if MATH_USE_SSE
    __m128 internal;
#endif
};

////////////////////////////////
// Float Constant Functions

function F32 inf_F32(void);
function F32 neg_inf_F32(void);
function F64 inf_F64(void);
function F64 neg_inf_F64(void);

////////////////////////////////
// Math Functions

function F32 abs_F32(F32 x);
function F64 abs_F64(F64 x);
function F32 sign_F32(F32 x);
function F64 sign_F64(F64 x);

function F32 sqrt_F32(F32 x);
function F32 sin_F32(F32 x);
function F32 cos_F32(F32 x);
function F32 tan_F32(F32 x);
function F32 atan_F32(F32 x);
function F32 ln_F32(F32 x);
function F32 pow_F32(F32 base, F32 x);

function F64 sqrt_F64(F64 x);
function F64 sin_F64(F64 x);
function F64 cos_F64(F64 x);
function F64 tan_F64(F64 x);
function F64 atan_F64(F64 x);
function F64 ln_F64(F64 x);
function F64 pow_F64(F64 base, F64 x);

function F32 lerp(F32 a, F32 t, F32 b);
function F32 unlerp(F32 a, F32 x, F32 b);

// function F32 lerp_range(I1F32 range, F32 t);

function F32 trunc_F32(F32 x);
function F32 floor_F32(F32 x);
function F32 ceil_F32(F32 x);

////////////////////////////////
// Compound Type Functions

function V2I v2i(S32 x, S32 y);
function V2F v2f(F32 x, F32 y);
function V3F v3f(F32 x, F32 y, F32 z);
function V4F v4f(F32 x, F32 y, F32 z, F32 w);

function V2I operator+(const V2I &a, const V2I &b);
function V2F operator+(const V2F &a, const V2F &b);
function V3F operator+(const V3F &a, const V3F &b);
function V4F operator+(const V4F &a, const V4F &b);

function V2I operator-(const V2I &a, const V2I &b);
function V2F operator-(const V2F &a, const V2F &b);
function V3F operator-(const V3F &a, const V3F &b);
function V4F operator-(const V4F &a, const V4F &b);

function V2I operator*(const V2I &v, const S32 &s);
function V2F operator*(const V2F &v, const F32 &s);
function V3F operator*(const V3F &v, const F32 &s);
function V4F operator*(const V4F &v, const F32 &s);

function V2I operator*(const S32 &s, const V2I &v);
function V2F operator*(const F32 &s, const V2F &v);
function V3F operator*(const F32 &s, const V3F &v);
function V4F operator*(const F32 &s, const V4F &v);

// function V2F32 vec_hadamard(V2F32 a, V2F32 b);
// function V3F32 vec_hadamard(V3F32 a, V3F32 b);
// function V4F32 vec_hadamard(V4F32 a, V4F32 b);

// function F32 vec_dot(V2F32 a, V2F32 b);
// function F32 vec_dot(V3F32 a, V3F32 b);
// function F32 vec_dot(V4F32 a, V4F32 b);

// function B32 intr_overlaps(I1F32 a, I1F32 b);
// function B32 intr_overlaps(I2F32 a, I2F32 b);
// function B32 intr_overlaps(I2S32 a, I2S32 b);

// function B32 intr_contains(I1F32 r, F32 x);
// function B32 intr_contains(I2F32 r, V2F32 p);
// function B32 intr_contains(I2S32 r, V2S32 p);

// function F32 intr_dim(I1F32 r);
// function U64 intr_dim(I1U64 r);
// function V2F32 intr_dim(I2F32 r);
// function V2S32 intr_dim(I2S32 r);

// function F32 intr_center(I1F32 r);
// function V2F32 intr_center(I2F32 r);

// function I1F32 intr_axis(I2F32 r, Axis axis);

// function I1F32 intr_clamp_top(I1F32 r, F32 top);
// function I1U64 intr_clamp_top(I1U64 r, U64 top);

#endif
