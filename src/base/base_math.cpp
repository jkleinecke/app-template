#include <math.h>

inline_function F32
inf_F32(void){
    union{ F32 f; U32 u; } r;
    r.u = 0x7f800000;
    return(r.f);
}

inline_function F32
neg_inf_F32(void){
    union{ F32 f; U32 u; } r;
    r.u = 0xff800000;
    return(r.f);
}

inline_function F64
inf_F64(void){
    union{ F64 f; U64 u; } r;
    r.u = 0x7ff0000000000000;
    return(r.f);
}

inline_function F64
neg_inf_F64(void){
    union{ F64 f; U64 u; } r;
    r.u = 0xfff0000000000000;
    return(r.f);
}

// ////////////////////////////////
// // Math Functions

inline_function F32
abs_F32(F32 x)
{
    union{ F32 f; U32 u; } r;
    r.u = 0x7fffffff;
    return(r.f);
}

inline_function F64
abs_F64(F64 x)
{
    union{ F64 f; U64 u; } r;
    r.u = 0x7fffffffffffffff;
    return(r.f);
}

inline_function F32 
sign_F32(F32 x)
{
    union{ F32 f; U32 u; } r;
    r.f = x;
    F32 result = (r.u&0x80000000)?-1.f:1.f;
    return(result);
}

inline_function F64 
sign_F64(F64 x)
{
    union{ F64 f; U64 u; } r;
    r.f = x;
    F32 result = (r.u&0x8000000000000000)?-1.f:1.f;
    return(result);
}

inline_function F32
sqrt_F32(F32 x)
{
    F32 result;

#if MATH_USE_SSE
    __m128 in = _mm_set_ss(x);
    __m128 out = _mm_sqrt_ss(in);
    result = _mm_cvtss_f32(out);
#else
    result = sqrtf(x);
#endif

    return result;
}

inline_function F32
rsqrt_F32(F32 x)
{
    F32 result;
#if MATH_USE_SSE
    __m128 in = _mm_set_ss(x);
    __m128 out = _mm_rsqrt_ss(in);
    result = _mm_cvtss_f32(out);
#else
    result = 1.0f/rsqrt(x);
#endif
    return result;
}

inline_function F32
sin_F32(F32 x)
{
    return sinf(x);
}

inline_function F32
cos_F32(F32 x)
{
    return cosf(x);
}

inline_function F32
tan_F32(F32 x)
{
    return tanf(x);
}

inline_function F32 
atan_F32(F32 x)
{
    return atanf(x);
}

inline_function F32
ln_F32(F32 x)
{
    return logf(x);
}

inline_function F32
pow_F32(F32 base, F32 x)
{
    return powf(base,x);
}

inline_function F64 
sqrt_F64(F64 x)
{
    // TODO(james): Use AVX?
    return sqrt(x);
}

inline_function F64
rsqrt_F64(F64 x)
{
    // TODO(james): Use AVX?
    return 1.0/sqrt(x);
}

inline_function F64
sin_F64(F64 x)
{
    return sin(x);
}

inline_function F64
cos_F64(F64 x)
{
    return cos(x);
}

inline_function F64
tan_F64(F64 x)
{
    return tan(x);
}

inline_function F64
atan_F64(F64 x)
{
    return atan(x);
}

inline_function F64
ln_F64(F64 x)
{
    return log(x);
}

inline_function F64
pow_F64(F64 base, F64 x)
{
    return pow(base,x);
}

inline_function F32 
lerp(F32 a, F32 t, F32 b)
{
    return a + (b-a)*t;
}

inline_function F32
unlerp(F32 a, F32 x, F32 b)
{
    F32 t = 0.0f;
    if(a != b)
    {
        t = (x-a)/(b-a);
    }
    return t;
}

// inline_function F32 lerp_range(I1F32 range, F32 t);

inline_function F32
trunc_F32(F32 x)
{
    // NOTE: could we mask off the mantissa?
    return (F32)(S32)x;
}

inline_function F32
floor_F32(F32 x)
{
    F32 r;
    if( x >= 0.0f )
    {
        r = (F32)(S32)x;
    }
    else
    {
        r = (F32)((S32)x - 1);
    }
    return r;
}

inline_function F32
ceil_F32(F32 x)
{
    F32 r;
    if( x >= 0.0f )
    {
        r = (F32)((S32)x + 1);
    }
    else
    {
        r = (F32)(S32)x;
    }
    return r;
}

////////////////////////////////
// Compound Type Functions

inline_function V2I 
v2i(S32 x, S32 y)
{
    V2I v = {x,y};
    return v;
}

inline_function V2F 
v2f(F32 x, F32 y)
{
    V2F v = {x,y};
    return v;
}

inline_function V3F
v3f(F32 x, F32 y, F32 z)
{
    V3F v = {x,y,z};
    return v;
}
inline_function V4F 
v4f(F32 x, F32 y, F32 z, F32 w)
{
#if MATH_USE_SSE
    V4F v = {.internal = _mm_setr_ps(x,y,z,w)};
#else
    V4F v = {x,y,z,w};
#endif
    return v;
}

inline_function V2I operator+(const V2I &a, const V2I &b)
{
    V2I r = {a.x + b.x, a.y + b.y};
    return r;
}

inline_function V2F operator+(const V2F &a, const V2F &b)
{
    V2F r = {a.x + b.x, a.y + b.y};
    return r;
}

inline_function V3F operator+(const V3F &a, const V3F &b)
{
    V3F r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
}

inline_function V4F operator+(const V4F &a, const V4F &b)
{
#if MATH_USE_SSE
    V4F r = {.internal = _mm_add_ps(a.internal,b.internal)};
#else
    V4F r = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
#endif
    return r;
}

inline_function V2I operator-(const V2I &a, const V2I &b)
{
    V2I r = {a.x - b.x, a.y - b.y};
    return r;
}

inline_function V2F operator-(const V2F &a, const V2F &b)
{
    V2F r = {a.x - b.x, a.y - b.y};
    return r;
}

inline_function V3F operator-(const V3F &a, const V3F &b)
{
    V3F r = {a.x - b.x, a.y - b.y, a.z - b.z};
    return r;
}

inline_function V4F operator-(const V4F &a, const V4F &b)
{
#if MATH_USE_SSE
    V4F r = {.internal=_mm_sub_ps(a.internal, b.internal)};
#else
    V4F r = {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
#endif
    return r;
}

inline_function V2I operator*(const V2I &v, const S32 &s)
{
    V2I r = {v.x*s, v.y*s};
    return r;
}

inline_function V2F operator*(const V2F &v, const F32 &s)
{
    V2F r = {v.x*s, v.y*s};
    return r;
}

inline_function V3F operator*(const V3F &v, const F32 &s)
{
    V3F r = {v.x*s, v.y*s, v.z*s};
    return r;
}

inline_function V4F operator*(const V4F &v, const F32 &s)
{
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    V4F r = {.internal=_mm_mul_ps(v.internal,scalar)};
#else
    V4F r = {v.x*s, v.y*s, v.z*s, v.w*s};
#endif
    return r;
}

inline_function V2I operator*(const S32 &s, const V2I &v)
{
    V2I r = {v.x*s, v.y*s};
    return r;
}

inline_function V2F operator*(const F32 &s, const V2F &v)
{
    V2F r = {v.x*s, v.y*s};
    return r;
}

inline_function V3F operator*(const F32 &s, const V3F &v)
{
    V3F r = {v.x*s, v.y*s, v.z*s};
    return r;
}

inline_function V4F operator*(const F32 &s, const V4F &v)
{
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    V4F r = {.internal=_mm_mul_ps(v.internal,scalar)};
#else
    V4F r = {v.x*s, v.y*s, v.z*s, v.w*s};
#endif
    return r;
}

inline_function V2I operator*(const V2I &a, const V2I &b)
{
    V2I r = {a.x*b.x, a.y*b.y};
    return r;
}

inline_function V2F operator*(const V2F &a, const V2F &b)
{
    V2F r = {a.x*b.x, a.y*b.y};
    return r;
}

inline_function V3F operator*(const V3F &a, const V3F &b)
{
    V3F r = {a.x*b.x, a.y*b.y, a.z*b.z};
    return r;
}

inline_function V4F operator*(const V4F &a, const V4F &b)
{
#if MATH_USE_SSE
    V4F r = {.internal = _mm_mul_ps(a.internal,b.internal)};
#else
    V4F r = {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};
#endif
    return r;
}

inline_function V2I operator/(const V2I &v, const S32 &s)
{
    V2I r = {v.x/s, v.y/s};
    return r;
}

inline_function V2F operator/(const V2F &v, const F32 &s)
{
    V2F r = {v.x/s, v.y/s};
    return r;
}

inline_function V3F operator/(const V3F &v, const F32 &s)
{
    V3F r = {v.x/s, v.y/s, v.z/s};
    return r;
}

inline_function V4F operator/(const V4F &v, const F32 &s)
{
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    V4F r = {.internal=_mm_div_ps(v.internal,scalar)};
#else
    V4F r = {v.x/s, v.y/s, v.z/s, v.w/s};
#endif
    return r;
}

inline_function V2I operator/(const S32 &s, const V2I &v)
{
    V2I r = {v.x/s, v.y/s};
    return r;
}

inline_function V2F operator/(const F32 &s, const V2F &v)
{
    V2F r = {v.x/s, v.y/s};
    return r;
}

inline_function V3F operator/(const F32 &s, const V3F &v)
{
    V3F r = {v.x/s, v.y/s, v.z/s};
    return r;
}

inline_function V4F operator/(const F32 &s, const V4F &v)
{
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    V4F r = {.internal=_mm_div_ps(v.internal,scalar)};
#else
    V4F r = {v.x/s, v.y/s, v.z/s, v.w/s};
#endif
    return r;
}

inline_function V2I operator/(const V2I &a, const V2I &b)
{
    V2I v = {a.x/b.x, a.y/b.y};
    return v;
}

inline_function V2F operator/(const V2F &a, const V2F &b)
{
    V2F v = {a.x/b.x, a.y/b.y};
    return v;
}

inline_function V3F operator/(const V3F &a, const V3F &b)
{
    V3F v = {a.x/b.x, a.y/b.y, a.z/b.z};
    return v;
}

inline_function V4F operator/(const V4F &a, const V4F &b)
{
#if MATH_USE_SSE
    V4F v = {.internal = _mm_div_ps(a.internal,b.internal)};
#else
    V4F v = {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w};
#endif
    return v;
}

inline_function F32 
vec_dot(V2F a, V2F b)
{
    return (a.x*b.x) + (a.y*b.y);
}

inline_function F32 
vec_dot(V3F a, V3F b)
{
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

inline_function F32 
vec_dot(V4F a, V4F b)
{
    F32 result;

#if MATH_USE_SSE
# if 0
    // NOTE(james): Requires SSE4 and most info says that it does not
    // work very well.
    __m128 c = _mm_dp_ps(a.interal, b.internal, 0xff);
    result = _mm_cvtss_f32(c);
# endif
# if 1
    // NOTE: Requires SSE3
    __m128 c = _mm_mul_ps(a.internal, b.internal);
    c = _mm_hadd_ps(c, c);
    c = _mm_hadd_ps(c, c);
    result = _mm_cvtss_f32(c);
# endif
# if 0
    // NOTE: Original SSE version 
    __m128 r1 = _mm_mul_ps(a.internal, b.internal);
    __m128 r2 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 3, 0, 1));
    r1 = _mm_add_ps(r1, r2);
    r2 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(0, 1, 2, 3));
    r1 = _mm_add_ps(r1, r2);
    _mm_store_ss(&Result, r1);
# endif
#else
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w);
#endif

    return result;
}


inline_function F32 
vec_len(V2F a)
{
    F32 lensq = vec_dot(a,a);
    return sqrt_F32(lensq);
}

inline_function F32 
vec_len(V3F a)
{
    F32 lensq = vec_dot(a,a);
    return sqrt_F32(lensq);
}

inline_function F32 
vec_len(V4F a)
{
    F32 lensq = vec_dot(a,a);
    return sqrt_F32(lensq);
}

inline_function F32 
vec_lensq(V2F a)
{
    return vec_dot(a,a);
}
inline_function F32 
vec_lensq(V3F a)
{
    return vec_dot(a,a);
}
inline_function F32 
vec_lensq(V4F a)
{
    return vec_dot(a,a);
}

inline_function V2F vec_norm(V2F a)
{
    F32 len = vec_len(a);
    F32 rlen = 1.0f / len;
    V2F r = { a.x * rlen, a.y * rlen };
    return r;
}

inline_function V3F vec_norm(V3F a)
{
    F32 len = vec_len(a);
    F32 rlen = 1.0f * len;
    V3F r = { a.x * rlen, a.y * rlen, a.z * rlen };
    return r;
}

inline_function V4F vec_norm(V4F a)
{
    F32 len = vec_len(a);
#if MATH_USE_SSE
    F32 rlen = 1.0f / len;
    __m128 scalar = _mm_set1_ps(rlen);
    V4F r = {.internal = _mm_mul_ps(a.internal, scalar)};
#else
    V4F r = { a.x / len, a.y / len, a.z / len, a.w / len };
#endif
    return r;
}

inline_function V2F vec_fastnorm(V2F a)
{
    return a * rsqrt_F32(vec_dot(a,a));
}

inline_function V3F vec_fastnorm(V3F a)
{
    return a * rsqrt_F32(vec_dot(a,a));
}

inline_function V4F vec_fastnorm(V4F a)
{
    return a * rsqrt_F32(vec_dot(a,a));
}

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
