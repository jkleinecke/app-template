#include <math.h>

function F32
inf_F32(void){
    union{ F32 f; U32 u; } r;
    r.u = 0x7f800000;
    return(r.f);
}

function F32
neg_inf_F32(void){
    union{ F32 f; U32 u; } r;
    r.u = 0xff800000;
    return(r.f);
}

function F64
inf_F64(void){
    union{ F64 f; U64 u; } r;
    r.u = 0x7ff0000000000000;
    return(r.f);
}

function F64
neg_inf_F64(void){
    union{ F64 f; U64 u; } r;
    r.u = 0xfff0000000000000;
    return(r.f);
}

// ////////////////////////////////
// // Math Functions

function F32
abs_F32(F32 x)
{
    union{ F32 f; U32 u; } r;
    r.u = 0x7fffffff;
    return(r.f);
}

function F64
abs_F64(F64 x)
{
    union{ F64 f; U64 u; } r;
    r.u = 0x7fffffffffffffff;
    return(r.f);
}

function F32 
sign_F32(F32 x)
{
    union{ F32 f; U32 u; } r;
    r.f = x;
    F32 result = (r.u&0x80000000)?-1.f:1.f;
    return(result);
}

function F64 
sign_F64(F64 x)
{
    union{ F64 f; U64 u; } r;
    r.f = x;
    F32 result = (r.u&0x8000000000000000)?-1.f:1.f;
    return(result);
}

function F32
sqrt_F32(F32 x)
{
    return sqrtf(x);
}

function F32
sin_F32(F32 x)
{
    return sinf(x);
}

function F32
cos_F32(F32 x)
{
    return cosf(x);
}

function F32
tan_F32(F32 x)
{
    return tanf(x);
}

function F32 
atan_F32(F32 x)
{
    return atanf(x);
}

function F32
ln_F32(F32 x)
{
    return logf(x);
}

function F32
pow_F32(F32 base, F32 x)
{
    return powf(base,x);
}

function F64 
sqrt_F64(F64 x)
{
    return sqrt(x);
}

function F64
sin_F64(F64 x)
{
    return sin(x);
}

function F64
cos_F64(F64 x)
{
    return cos(x);
}

function F64
tan_F64(F64 x)
{
    return tan(x);
}

function F64
atan_F64(F64 x)
{
    return atan(x);
}

function F64
ln_F64(F64 x)
{
    return log(x);
}

function F64
pow_F64(F64 base, F64 x)
{
    return pow(base,x);
}

function F32 
lerp(F32 a, F32 t, F32 b)
{
    return a + (b-a)*t;
}

function F32
unlerp(F32 a, F32 x, F32 b)
{
    F32 t = 0.0f;
    if(a != b)
    {
        t = (x-a)/(b-a);
    }
    return t;
}

// function F32 lerp_range(I1F32 range, F32 t);

function F32
trunc_F32(F32 x)
{
    // NOTE: could we mask off the mantissa?
    return (F32)(S32)x;
}

function F32
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

function F32
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

function V2I 
v2i(S32 x, S32 y)
{
    V2I v = {x,y};
    return v;
}

function V2F 
v2f(F32 x, F32 y)
{
    V2F v = {x,y};
    return v;
}

function V3F
v3f(F32 x, F32 y, F32 z)
{
    V3F v = {x,y,z};
    return v;
}
function V4F 
v4f(F32 x, F32 y, F32 z, F32 w)
{
#if MATH_USE_SSE
    V4F v = {.internal = _mm_setr_ps(x,y,z,w)};
#else
    V4F v = {x,y,z,w};
#endif
    return v;
}

function V2I operator+(const V2I &a, const V2I &b)
{
    V2I r = {a.x + b.x, a.y + b.y};
    return r;
}

function V2F operator+(const V2F &a, const V2F &b)
{
    V2F r = {a.x + b.x, a.y + b.y};
    return r;
}

function V3F operator+(const V3F &a, const V3F &b)
{
    V3F r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
}

function V4F operator+(const V4F &a, const V4F &b)
{
#if MATH_USE_SSE
    V4F r = {.internal = _mm_add_ps(a.internal,b.internal)};
#else
    V4F r = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
#endif
    return r;
}

function V2I operator-(const V2I &a, const V2I &b)
{
    V2I r = {a.x - b.x, a.y - b.y};
    return r;
}

function V2F operator-(const V2F &a, const V2F &b)
{
    V2F r = {a.x - b.x, a.y - b.y};
    return r;
}

function V3F operator-(const V3F &a, const V3F &b)
{
    V3F r = {a.x - b.x, a.y - b.y, a.z - b.z};
    return r;
}

function V4F operator-(const V4F &a, const V4F &b)
{
#if MATH_USE_SSE
    V4F r = {.internal=_mm_sub_ps(a.internal, b.internal)};
#else
    V4F r = {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
#endif
    return r;
}

function V2I operator*(const V2I &v, const S32 &s)
{
    V2I r = {v.x*s, v.y*s};
    return r;
}

function V2F operator*(const V2F &v, const F32 &s)
{
    V2F r = {v.x*s, v.y*s};
    return r;
}

function V3F operator*(const V3F &v, const F32 &s)
{
    V3F r = {v.x*s, v.y*s, v.z*s};
    return r;
}

function V4F operator*(const V4F &v, const F32 &s)
{
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    V4F r = {.internal=_mm_mul_ps(v.internal,scalar)};
#else
    V4F r = {v.x*s, v.y*s, v.z*s, v.w*s};
#endif
    return r;
}

function V2I operator*(const S32 &s, const V2I &v)
{
    V2I r = {v.x*s, v.y*s};
    return r;
}

function V2F operator*(const F32 &s, const V2F &v)
{
    V2F r = {v.x*s, v.y*s};
    return r;
}

function V3F operator*(const F32 &s, const V3F &v)
{
    V3F r = {v.x*s, v.y*s, v.z*s};
    return r;
}

function V4F operator*(const F32 &s, const V4F &v)
{
#if MATH_USE_SSE
    __m128 scalar = _mm_set1_ps(s);
    V4F r = {.internal=_mm_mul_ps(v.internal,scalar)};
#else
    V4F r = {v.x*s, v.y*s, v.z*s, v.w*s};
#endif
    return r;
}

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
