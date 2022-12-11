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
acos_F32(F32 x)
{
    return acosf(x);
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
acos_F64(F64 x)
{
    return acos(x);
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
to_radians_F32(F32 degrees)
{
    F32 result = degrees * (pi_F32 / 180.0f);

    return result;
}

inline_function F64
to_radians_F64(F64 degrees)
{
    F64 result = degrees * (pi_F64 / 180.0f);

    return result;
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
clamp_F32(F32 min, F32 value, F32 max)
{
    F32 result = value;

    if(result < min)
    {
        result = min;
    }

    if(result > max)
    {
        result = max;
    }

    return result;
}

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

inline_function V2I operator-(const V2I &a)
{
    V2I r = {.x = -a.x, .y = -a.y};
    return r;
}

inline_function V2F operator-(const V2F &a)
{
    V2F r = {.x = -a.x, .y = -a.y};
    return r;
}

inline_function V3F operator-(const V3F &a)
{
    V3F r = {.x = -a.x, .y = -a.y, .z = -a.z};
    return r;
}

inline_function V4F operator-(const V4F &a)
{
#if MATH_USE_SSE
    V4F r = {.internal = _mm_xor_ps(a.internal, _mm_set1_ps(-0.0f))};
#else
    V4F r = {.x = -a.x, .y = -a.y, .z = -a.z, .w = -a.w};
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

#if MATH_USE_SSE
inline_function __m128 sse_linear_combine(__m128 left, M4F right)
{
    __m128 result;
    result = _mm_mul_ps(_mm_shuffle_ps(left, left, 0x00), right.cols[0]);
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(left, left, 0x55), right.cols[1]));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(left, left, 0xaa), right.cols[2]));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(left, left, 0xff), right.cols[3]));
    return result;
}
#endif

inline_function V3F
vec_cross(V3F v1, V3F v2)
{
    V3F result;

    result.x = (v1.y * v2.z) - (v1.z * v2.y);
    result.y = (v1.z * v2.x) - (v1.x * v2.z);
    result.z = (v1.x * v2.y) - (v1.y * v2.x);

    return result;
}

inline_function M4F m4f(void)
{
    M4F r = {0};
    return r;
}

inline_function M4F m4f(F32 diagonal)
{
    M4F r = {0};
    r.e[0][0] = diagonal;
    r.e[1][1] = diagonal;
    r.e[2][2] = diagonal;
    r.e[3][3] = diagonal;
    return r;
}

inline_function M4F m4f_identity(void)
{
    return m4f(1.0f);
}

inline_function M4F mat_transpose(M4F a)
{
    M4F result = a;

#if MATH_USE_SSE
    _MM_TRANSPOSE4_PS(result.cols[0], result.cols[1], result.cols[2], result.cols[3]);
#else
    for(int cols = 0; cols < 4; ++cols)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.e[rows][cols] = a.e[cols][rows];
        }
    }
#endif

    return result;
}

inline_function M4F operator+(const M4F &a, const M4F &b)
{
    M4F result;
#if MATH_USE_SSE
    result.cols[0] = _mm_add_ps(a.cols[0], b.cols[0]);
    result.cols[1] = _mm_add_ps(a.cols[1], b.cols[1]);
    result.cols[2] = _mm_add_ps(a.cols[2], b.cols[2]);
    result.cols[3] = _mm_add_ps(a.cols[3], b.cols[3]);
#else
    for(int cols = 0; cols < 4; ++cols)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.e[cols][rows] = a.e[cols][rows] + b.e[cols][rows];
        }
    }
#endif
    return result;
}
inline_function M4F operator-(const M4F &a, const M4F &b)
{
    M4F result;
#if MATH_USE_SSE
    result.cols[0] = _mm_sub_ps(a.cols[0], b.cols[0]);
    result.cols[1] = _mm_sub_ps(a.cols[1], b.cols[1]);
    result.cols[2] = _mm_sub_ps(a.cols[2], b.cols[2]);
    result.cols[3] = _mm_sub_ps(a.cols[3], b.cols[3]);
#else
    for(int cols = 0; cols < 4; ++cols)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.e[cols][rows] = a.e[cols][rows] - b.e[cols][rows];
        }
    }
#endif
    return result;
}
inline_function M4F operator*(const M4F &a, const M4F &b)
{
    M4F result;
#if MATH_USE_SSE
    result.cols[0] = sse_linear_combine(b.cols[0], a);
    result.cols[1] = sse_linear_combine(b.cols[1], a);
    result.cols[2] = sse_linear_combine(b.cols[2], a);
    result.cols[3] = sse_linear_combine(b.cols[3], a);
#else
    for(int cols = 0; cols < 4; ++cols)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            F32 sum = 0.0f;
            for(int k = 0; k < 4; ++k)
            {
                sum += a.e[k][rows] * b.e[cols][k];
            }

            result.e[cols][rows] = sum;
        }
    }
#endif
    return result;
}

inline_function M4F operator*(const M4F &m, const F32 &s)
{
    M4F result;
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    result.cols[0] = _mm_mul_ps(m.cols[0], scalar);
    result.cols[1] = _mm_mul_ps(m.cols[1], scalar);
    result.cols[2] = _mm_mul_ps(m.cols[2], scalar);
    result.cols[3] = _mm_mul_ps(m.cols[3], scalar);
#else
    for(int cols = 0; cols < 4; ++cols)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.e[cols][rows] = m.e[cols][rows] * s;
        }
    }
#endif
    return result;
}

inline_function V4F operator*(const M4F &m, const V4F &v)
{
    V4F result;
#if MATH_USE_SSE
    result.internal = sse_linear_combine(v.internal, m);
#else
    for(int rows = 0; rows < 4; ++rows)
    {
        float sum = 0.0f;
        for(int cols = 0; cols < 4; ++cols)
        {
            sum += m.e[cols][rows] * v.e[cols];
        }

        result.e[rows] = sum;
    }
#endif
    return result;
}

inline_function M4F operator/(const M4F &m, const F32 &s)
{
    M4F result;
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    result.cols[0] = _mm_div_ps(m.cols[0], scalar);
    result.cols[1] = _mm_div_ps(m.cols[1], scalar);
    result.cols[2] = _mm_div_ps(m.cols[2], scalar);
    result.cols[3] = _mm_div_ps(m.cols[3], scalar);
#else
    for(int cols = 0; cols < 4; ++cols)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.e[cols][rows] = m.e[cols][rows] / s;
        }
    }
#endif
    return result;
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

//////////////////////////////////////////////////////////////
// Graphics Functions

inline_function M4F mat_orthographic_2d(F32 left, F32 right, F32 bottom, F32 top)
{
    return mat_orthographic_3d(left, right, bottom, top, -1.0f, 1.0f);
}

inline_function M4F mat_orthographic_3d(F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far)
{
    M4F result = {0};

    // Unit Cube Scaling
    result.e[0][0] = 2.0f / (right - left);
    result.e[1][1] = 2.0f / (top - bottom);
    result.e[2][2] = 2.0f / (near - far);
    result.e[3][3] = 1.0f;

    // Translation
    result.e[3][0] = (left + right) / (left - right);
    result.e[3][1] = (bottom + top) / (bottom - top);
    result.e[3][2] = (far + near) / (near - far);

    return result;
}

inline_function M4F mat_perspective(F32 fov, F32 aspectRatio, F32 near, F32 far)
{
    M4F result = {0};

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    F32 cotangent = 1.0f / tan_F32(fov * (pi_F32 / 360.0f));

    result.e[0][0] = cotangent / aspectRatio;
    result.e[1][1] = cotangent;
    result.e[2][3] = -1.0f;
    result.e[2][2] = (near + far) / (near - far);
    result.e[3][2] = (2.0f * near * far) / (near - far);
    result.e[3][3] = 0.0f;

    return result;
}

inline_function M4F mat_translate(V3F translation)
{
    M4F result = m4f(1.0f);

    result.e[3][0] = translation.x;
    result.e[3][1] = translation.y;
    result.e[3][2] = translation.z;

    return result;
}

inline_function M4F mat_rotate(F32 angle, V3F axis)
{
    M4F result = m4f(1.0f);

    axis = vec_norm(axis);

    F32 radians = to_radians_F32(angle);
    F32 sinTheta = sin_F32(radians);
    F32 cosTheta = cos_F32(radians);
    F32 cosValue = 1.0f - cosTheta;

    result.e[0][0] = (axis.x * axis.x * cosValue) + cosTheta;
    result.e[0][1] = (axis.x * axis.y * cosValue) + (axis.z * sinTheta);
    result.e[0][2] = (axis.x * axis.z * cosValue) - (axis.y * sinTheta);

    result.e[1][0] = (axis.y * axis.x * cosValue) - (axis.z * sinTheta);
    result.e[1][1] = (axis.y * axis.y * cosValue) + cosTheta;
    result.e[1][2] = (axis.y * axis.z * cosValue) + (axis.x * sinTheta);

    result.e[2][0] = (axis.z * axis.x * cosValue) + (axis.y * sinTheta);
    result.e[2][1] = (axis.z * axis.y * cosValue) - (axis.x * sinTheta);
    result.e[2][2] = (axis.z * axis.z * cosValue) + cosTheta;

    return result;
}

inline_function M4F mat_scale(V3F scale)
{
    M4F result = m4f(1.0f);

    result.e[0][0] = scale.x;
    result.e[1][1] = scale.y;
    result.e[2][2] = scale.z;

    return result;
}

inline_function M4F mat_lookat(V3F eye, V3F target, V3F up)
{
    M4F result;

    V3F f = vec_norm(target - eye);
    V3F s = vec_norm(vec_cross(f, up));
    V3F u = vec_cross(s, f);

    result.e[0][0] = s.x;
    result.e[0][1] = u.x;
    result.e[0][2] = -f.x;
    result.e[0][3] = 0.0f;
    
    result.e[1][0] = s.y;
    result.e[1][1] = u.y;
    result.e[1][2] = -f.y;
    result.e[1][3] = 0.0f;
    
    result.e[2][0] = s.z;
    result.e[2][1] = u.z;
    result.e[2][2] = -f.z;
    result.e[2][3] = 0.0f;

    result.e[3][0] = -vec_dot(s, eye);
    result.e[3][1] = -vec_dot(u, eye);
    result.e[3][2] = vec_dot(f, eye);
    result.e[3][3] = 1.0f;

    return result;
}

//////////////////////////////////////////////////////////////
// Quaternion operations

inline_function Quaternion quat(F32 x, F32 y, F32 z, F32 w)
{
    Quaternion result;

#if MATH_USE_SSE
    result.internal = _mm_setr_ps(x, y, z, w);
#else
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
#endif

    return result;
}

inline_function Quaternion quat(V4F v)
{
    Quaternion result;

#if MATH_USE_SSE
    result.internal = _mm_setr_ps(v.x, v.y, v.z, v.w);
#else
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    result.w = v.w;
#endif

    return result;
}

// This method taken from Mike Day at Insomniac Games.
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
//
// Note that as mentioned at the top of the paper, the paper assumes the matrix
// would be *post*-multiplied to a vector to rotate it, meaning the matrix is
// the transpose of what we're dealing with. But, because our matrices are
// stored in column-major order, the indices *appear* to match the paper.
//
// For example, m12 in the paper is row 1, column 2. We need to transpose it to
// row 2, column 1. But, because the column comes first when referencing
// elements, it looks like M.Elements[1][2].
//
// Don't be confused! Or if you must be confused, at least trust this
// comment. :)
inline_function Quaternion quat_from_mat4(M4F m)
{
    F32 t;
    Quaternion q;

    if(m.e[2][2] < 0.0f) 
    {
        if(m.e[0][0] > m.e[1][1])
        {
            t = 1 + m.e[0][0] - m.e[1][1] - m.e[2][2];
            q = quat(t, m.e[0][1] + m.e[1][0], m.e[2][0] + m.e[0][2], m.e[1][2] - m.e[2][1]);
        }
        else
        {
            t = 1 - m.e[0][0] + m.e[1][1] - m.e[2][2];
            q = quat(m.e[0][1] + m.e[1][0], t, m.e[1][2] + m.e[2][1], m.e[2][0] - m.e[0][2]);
        }
    }
    else
    {
        if(m.e[0][0] < -m.e[1][1])
        {
            t = 1 - m.e[0][0] - m.e[1][1] + m.e[2][2];
            q = quat(m.e[2][0] + m.e[0][2], m.e[1][2] + m.e[2][1], t, m.e[0][1] - m.e[1][0]);
        }
        else
        {
            t = 1 + m.e[0][0] + m.e[1][1] + m.e[2][2];
            q = quat(m.e[1][2] - m.e[2][1], m.e[2][0] - m.e[0][2], m.e[0][1] - m.e[1][0], t);
        }
    }

    q = q * (0.5f / sqrt_F32(t));

    return q;
}

inline_function Quaternion quat_from_angle(V3F axis, F32 angleOfRotation)
{
    Quaternion result;

    axis = vec_norm(axis);
    F32 sineOfRotation = sin_F32(angleOfRotation / 2.0f);

    result.xyz = axis * sineOfRotation;
    result.w = cos_F32(angleOfRotation / 2.0f);

    return result;
}

inline_function Quaternion operator+(const Quaternion &a, const Quaternion &b)
{
    Quaternion result;

#if MATH_USE_SSE
    result.internal = _mm_add_ps(a.internal, b.internal);
#else
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
#endif

    return result;
}

inline_function Quaternion operator-(const Quaternion &a, const Quaternion &b)
{
    Quaternion result;

#if MATH_USE_SSE
    result.internal = _mm_sub_ps(a.internal, b.internal);
#else
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
#endif

    return result;
}

inline_function Quaternion operator*(const Quaternion &a, const Quaternion &b)
{
    Quaternion result;

#if MATH_USE_SSE
    __m128 r1 = _mm_xor_ps(_mm_shuffle_ps(a.internal, a.internal, _MM_SHUFFLE(0,0,0,0)), _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
    __m128 r2 = _mm_shuffle_ps(b.internal, b.internal, _MM_SHUFFLE(0,1,2,3));
    __m128 r3 = _mm_mul_ps(r2, r1);

    r1 = _mm_xor_ps(_mm_shuffle_ps(a.internal, a.internal, _MM_SHUFFLE(1,1,1,1)), _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
    r2 = _mm_shuffle_ps(b.internal, b.internal, _MM_SHUFFLE(1,0,3,2));
    r3 = _mm_add_ps(r3, _mm_mul_ps(r2, r1));

    r1 = _mm_xor_ps(_mm_shuffle_ps(a.internal, a.internal, _MM_SHUFFLE(2,2,2,2)), _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
    r2 = _mm_shuffle_ps(b.internal, b.internal, _MM_SHUFFLE(2,3,0,1));
    r3 = _mm_add_ps(r3, _mm_mul_ps(r2, r1));

    r1 = _mm_shuffle_ps(a.internal, a.internal, _MM_SHUFFLE(3,3,3,3));
    r2 = _mm_shuffle_ps(b.internal, b.internal, _MM_SHUFFLE(3,2,1,0));
    result.internal = _mm_add_ps(r3, _mm_mul_ps(r2, r1));
#else
    result.x = (a.x * b.w) + (a.y * b.z) - (a.z * b.y) + (a.w * b.x);
    result.y = (-a.x * b.z) + (a.y * b.w) + (a.z * b.x) + (a.w * b.y);
    result.z = (a.x * b.y) - (a.y * b.x) + (a.z * b.w) + (a.w * b.z);
    result.w = (-a.x * b.x) - (a.y * b.y) - (a.z * b.z) + (a.w * b.w);
#endif

    return result;
}

inline_function Quaternion operator*(const Quaternion &q, const F32 &s)
{
    Quaternion result;

#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    result.internal = _mm_mul_ps(q.internal, scalar);
#else
    result.x = q.x * s;
    result.y = q.y * s;
    result.z = q.z * s;
    result.w = q.w * s;
#endif

    return result;
}

inline_function Quaternion operator/(const Quaternion &q, const F32 &s)
{
    Quaternion result;

#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    result.internal = _mm_div_ps(q.internal, scalar);
#else
    result.x = q.x / s;
    result.y = q.y / s;
    result.z = q.z / s;
    result.w = q.w / s;
#endif

    return result;
}

inline_function F32 quat_dot(Quaternion a, Quaternion b)
{
    // NOTE(james): Same exact implementation as the V4F dot product
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

inline_function Quaternion quat_inverse(Quaternion q)
{
    Quaternion result = quat(-q.x, -q.y, -q.z, q.w);
    result = result / quat_dot(q, q);

    return result;
}

inline_function Quaternion quat_norm(Quaternion q)
{
    Quaternion result;

    F32 length = sqrt_F32(quat_dot(q,q));
    result = q / length;

    return result;
}

inline_function Quaternion quat_nlerp(Quaternion a, F32 time, Quaternion b)
{
    Quaternion result;

#if MATH_USE_SSE
    __m128 sleft = _mm_set1_ps(1.0f - time);
    __m128 sright = _mm_set1_ps(time);
    __m128 r1 = _mm_mul_ps(a.internal, sleft);
    __m128 r2 = _mm_mul_ps(b.internal, sright);
    result.internal = _mm_add_ps(r1, r2);
#else
    result.x = lerp(a.x, time, b.x);
    result.y = lerp(a.y, time, b.y);
    result.z = lerp(a.z, time, b.z);
    result.w = lerp(a.w, time, b.w);
#endif

    result = quat_norm(result);

    return result;
}

inline_function Quaternion quat_slerp(Quaternion a, F32 time, Quaternion b)
{
    Quaternion result;
    Quaternion qleft;
    Quaternion qright;

    F32 cosTheta = quat_dot(a, b);
    F32 angle = acos_F32(cosTheta);

    F32 s1 = sin_F32((1.0f - time) * angle);
    F32 s2 = sin_F32(time * angle);
    F32 sI = 1.0f / sin_F32(angle);

    qleft = a * s1;
    qright = b * s2;

    result = qleft + qright;
    result = result * sI;

    return result;
}

inline_function M4F quat_to_mat4(Quaternion q)
{
    M4F result;

    Quaternion nq = quat_norm(q);

    F32 xx = nq.x * nq.x;
    F32 yy = nq.y * nq.y;
    F32 zz = nq.z * nq.z;
    F32 xy = nq.x * nq.y;
    F32 xz = nq.x * nq.z;
    F32 yz = nq.y * nq.z;
    F32 wx = nq.w * nq.x;
    F32 wy = nq.w * nq.y;
    F32 wz = nq.w * nq.z;

    result.e[0][0] = 1.0f - 2.0f * (yy + zz);
    result.e[0][1] = 2.0f * (xy + wz);
    result.e[0][2] = 2.0f * (xz - wy);
    result.e[0][3] = 0.0f;

    result.e[1][0] = 2.0f * (xy - wz);
    result.e[1][1] = 1.0f - 2.0f * (xx + zz);
    result.e[1][2] = 2.0f * (yz + wx);
    result.e[1][3] = 0.0f;

    result.e[2][0] = 2.0f * (xz + wy);
    result.e[2][1] = 2.0f * (yz - wx);
    result.e[2][2] = 1.0f - 2.0f * (xx + yy);
    result.e[2][3] = 0.0f;

    result.e[3][0] = 0.0f;
    result.e[3][1] = 0.0f;
    result.e[3][2] = 0.0f;
    result.e[3][3] = 1.0f;

    return result;
}
