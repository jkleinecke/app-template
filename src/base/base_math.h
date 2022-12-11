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
#if MATH_USE_SSE
        V4F result = { .internal = cols[index] };
#else
        F32* col = e[index];
        V4F result = {
            .x = col[0],
            .y = col[1],
            .z = col[2],
            .w = col[3]
        };
#endif
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

inline_function F32 inf_F32(void);
inline_function F32 neg_inf_F32(void);
inline_function F64 inf_F64(void);
inline_function F64 neg_inf_F64(void);

////////////////////////////////
// Math Functions

inline_function F32 abs_F32(F32 x);
inline_function F64 abs_F64(F64 x);
inline_function F32 sign_F32(F32 x);
inline_function F64 sign_F64(F64 x);

inline_function F32 sqrt_F32(F32 x);
inline_function F32 rsqrt_F32(F32 x);
inline_function F32 sin_F32(F32 x);
inline_function F32 cos_F32(F32 x);
inline_function F32 acos_F32(F32 x);
inline_function F32 tan_F32(F32 x);
inline_function F32 atan_F32(F32 x);
inline_function F32 ln_F32(F32 x);
inline_function F32 pow_F32(F32 base, F32 x);

inline_function F64 sqrt_F64(F64 x);
inline_function F64 rsqrt_F64(F64 x);
inline_function F64 sin_F64(F64 x);
inline_function F64 cos_F64(F64 x);
inline_function F64 acos_F64(F64 x);
inline_function F64 tan_F64(F64 x);
inline_function F64 atan_F64(F64 x);
inline_function F64 ln_F64(F64 x);
inline_function F64 pow_F64(F64 base, F64 x);

inline_function F32 to_radians_F32(F32 degrees);
inline_function F64 to_radians_F64(F64 degrees);

inline_function F32 lerp(F32 a, F32 t, F32 b);
inline_function F32 unlerp(F32 a, F32 x, F32 b);

// inline_function F32 lerp_range(I1F32 range, F32 t);

inline_function F32 clamp_F32(F32 min, F32 value, F32 max);
inline_function F32 trunc_F32(F32 x);
inline_function F32 floor_F32(F32 x);
inline_function F32 ceil_F32(F32 x);

////////////////////////////////
// Compound Type Functions

inline_function V2I v2i(S32 x, S32 y);
inline_function V2F v2f(F32 x, F32 y);
inline_function V3F v3f(F32 x, F32 y, F32 z);
inline_function V4F v4f(F32 x, F32 y, F32 z, F32 w);

inline_function V2I operator+(const V2I &a, const V2I &b);
inline_function V2F operator+(const V2F &a, const V2F &b);
inline_function V3F operator+(const V3F &a, const V3F &b);
inline_function V4F operator+(const V4F &a, const V4F &b);

inline_function V2I operator-(const V2I &a);
inline_function V2F operator-(const V2F &a);
inline_function V3F operator-(const V3F &a);
inline_function V4F operator-(const V4F &a);

inline_function V2I operator-(const V2I &a, const V2I &b);
inline_function V2F operator-(const V2F &a, const V2F &b);
inline_function V3F operator-(const V3F &a, const V3F &b);
inline_function V4F operator-(const V4F &a, const V4F &b);

inline_function V2I operator*(const V2I &v, const S32 &s);
inline_function V2F operator*(const V2F &v, const F32 &s);
inline_function V3F operator*(const V3F &v, const F32 &s);
inline_function V4F operator*(const V4F &v, const F32 &s);

inline_function V2I operator*(const S32 &s, const V2I &v);
inline_function V2F operator*(const F32 &s, const V2F &v);
inline_function V3F operator*(const F32 &s, const V3F &v);
inline_function V4F operator*(const F32 &s, const V4F &v);

inline_function V2I operator*(const V2I &a, const V2I &b);
inline_function V2F operator*(const V2F &a, const V2F &b);
inline_function V3F operator*(const V3F &a, const V3F &b);
inline_function V4F operator*(const V4F &a, const V4F &b);

inline_function V2I operator/(const V2I &v, const S32 &s);
inline_function V2F operator/(const V2F &v, const F32 &s);
inline_function V3F operator/(const V3F &v, const F32 &s);
inline_function V4F operator/(const V4F &v, const F32 &s);

inline_function V2I operator/(const S32 &s, const V2I &v);
inline_function V2F operator/(const F32 &s, const V2F &v);
inline_function V3F operator/(const F32 &s, const V3F &v);
inline_function V4F operator/(const F32 &s, const V4F &v);

inline_function V2I operator/(const V2I &a, const V2I &b);
inline_function V2F operator/(const V2F &a, const V2F &b);
inline_function V3F operator/(const V3F &a, const V3F &b);
inline_function V4F operator/(const V4F &a, const V4F &b);

inline_function F32 vec_dot(V2F a, V2F b);
inline_function F32 vec_dot(V3F a, V3F b);
inline_function F32 vec_dot(V4F a, V4F b);

inline_function F32 vec_len(V2F a);
inline_function F32 vec_len(V3F a);
inline_function F32 vec_len(V4F a);

inline_function F32 vec_lensq(V2F a);
inline_function F32 vec_lensq(V3F a);
inline_function F32 vec_lensq(V4F a);

inline_function V2F vec_norm(V2F a);
inline_function V3F vec_norm(V3F a);
inline_function V4F vec_norm(V4F a);

inline_function V2F vec_fastnorm(V2F a);
inline_function V3F vec_fastnorm(V3F a);
inline_function V4F vec_fastnorm(V4F a);

inline_function V3F vec_cross(V3F v1, V3F v2);

inline_function M4F m4f(void);
inline_function M4F m4f(F32 diagonal);
inline_function M4F m4f_identity(void);
inline_function M4F mat_transpose(M4F a);


inline_function M4F operator+(const M4F &a, const M4F &b);
inline_function M4F operator-(const M4F &a, const M4F &b);
inline_function M4F operator*(const M4F &a, const M4F &b);
inline_function M4F operator*(const M4F &m, const F32 &s);
inline_function V4F operator*(const M4F &m, const V4F &v);
inline_function M4F operator/(const M4F &m, const F32 &s);

// inline_function B32 intr_overlaps(I1F32 a, I1F32 b);
// inline_function B32 intr_overlaps(I2F32 a, I2F32 b);
// inline_function B32 intr_overlaps(I2S32 a, I2S32 b);

// inline_function B32 intr_contains(I1F32 r, F32 x);
// inline_function B32 intr_contains(I2F32 r, V2F32 p);
// inline_function B32 intr_contains(I2S32 r, V2S32 p);

// inline_function F32 intr_dim(I1F32 r);
// inline_function U64 intr_dim(I1U64 r);
// inline_function V2F32 intr_dim(I2F32 r);
// inline_function V2S32 intr_dim(I2S32 r);

// inline_function F32 intr_center(I1F32 r);
// inline_function V2F32 intr_center(I2F32 r);

// inline_function I1F32 intr_axis(I2F32 r, Axis axis);

// inline_function I1F32 intr_clamp_top(I1F32 r, F32 top);
// inline_function I1U64 intr_clamp_top(I1U64 r, U64 top);

//////////////////////////////////////////////////////////////
// Graphics Functions

inline_function M4F mat_orthographic_2d(F32 left, F32 right, F32 bottom, F32 top);
inline_function M4F mat_orthographic_3d(F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far);
inline_function M4F mat_perspective(F32 fov, F32 aspectRatio, F32 near, F32 far);

inline_function M4F mat_translate(V3F translation);
inline_function M4F mat_rotate(F32 angle, V3F axis);
inline_function M4F mat_scale(V3F scale);
inline_function M4F mat_lookat(V3F eye, V3F target, V3F up);

//////////////////////////////////////////////////////////////
// Quaternion operations

inline_function Quaternion quat(F32 x, F32 y, F32 z, F32 w);
inline_function Quaternion quat(V4F v);
inline_function Quaternion quat_from_mat4(M4F m);
inline_function Quaternion quat_from_angle(V3F axis, F32 angleOfRotation);

inline_function Quaternion operator+(const Quaternion &a, const Quaternion &b);
inline_function Quaternion operator-(const Quaternion &a, const Quaternion &b);
inline_function Quaternion operator*(const Quaternion &a, const Quaternion &b);
inline_function Quaternion operator*(const Quaternion &q, const F32 &s);
inline_function Quaternion operator/(const Quaternion &q, const F32 &s);

inline_function F32 quat_dot(Quaternion a, Quaternion b);
inline_function Quaternion quat_inverse(Quaternion q);
inline_function Quaternion quat_norm(Quaternion q);

inline_function Quaternion quat_nlerp(Quaternion a, F32 time, Quaternion b);
inline_function Quaternion quat_slerp(Quaternion a, F32 time, Quaternion b);

inline_function M4F quat_to_mat4(Quaternion q);


#endif
