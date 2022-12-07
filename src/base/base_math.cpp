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

// function F32 abs_F32(F32 x);
// function F64 abs_F64(F64 x);
// function F32 sign_F32(F32 x);
// function F64 sign_F64(F64 x);

// function F32 sqrt_F32(F32 x);
// function F32 sin_F32(F32 x);
// function F32 cos_F32(F32 x);
// function F32 tan_F32(F32 x);
// function F32 atan_F32(F32 x);
// function F32 ln_F32(F32 x);
// function F32 pow_F32(F32 base, F32 x);

// function F64 sqrt_F64(F64 x);
// function F64 sin_F64(F64 x);
// function F64 cos_F64(F64 x);
// function F64 tan_F64(F64 x);
// function F64 atan_F64(F64 x);
// function F64 ln_F64(F64 x);
// function F64 pow_F64(F64 base, F64 x);

// function F32 lerp(F32 a, F32 t, F32 b);
// function F32 unlerp(F32 a, F32 x, F32 b);

// function F32 lerp_range(I1F32 range, F32 t);

// function F32 trunc_F32(F32 x);
// function F32 floor_F32(F32 x);
// function F32 ceil_F32(F32 x);

// ////////////////////////////////
// // Compound Type Functions

// function V2S32 v2s32(S32 x, S32 y);

// function V2F32 v2f32(F32 x, F32 y);
// function V3F32 v3f32(F32 x, F32 y, F32 z);
// function V4F32 v4f32(F32 x, F32 y, F32 z, F32 w);

// function I1F32 i1f32(F32 min, F32 max);
// function I1U64 i1u64(U64 min, U64 max);

// function I2S32 i2s32(S32 x0, S32 y0, S32 x1, S32 y1);
// function I2S32 i2s32_vec(V2S32 min, V2S32 max);

// function I2F32 i2f32(F32 x0, F32 y0, F32 x1, F32 y1);
// function I2F32 i2f32_vec(V2F32 min, V2F32 max);
// function I2F32 i2f32_range(I1F32 x, I1F32 y);

// function V2S32 operator+(const V2S32 &a, const V2S32 &b);
// function V2F32 operator+(const V2F32 &a, const V2F32 &b);
// function V3F32 operator+(const V3F32 &a, const V3F32 &b);
// function V4F32 operator+(const V4F32 &a, const V4F32 &b);

// function V2S32 operator-(const V2S32 &a, const V2S32 &b);
// function V2F32 operator-(const V2F32 &a, const V2F32 &b);
// function V3F32 operator-(const V3F32 &a, const V3F32 &b);
// function V4F32 operator-(const V4F32 &a, const V4F32 &b);

// function V2S32 operator*(const V2S32 &v, const S32 &s);
// function V2F32 operator*(const V2F32 &v, const F32 &s);
// function V3F32 operator*(const V3F32 &v, const F32 &s);
// function V4F32 operator*(const V4F32 &v, const F32 &s);

// function V2S32 operator*(const S32 &s, const V2S32 &v);
// function V2F32 operator*(const F32 &s, const V2F32 &v);
// function V3F32 operator*(const F32 &s, const V3F32 &v);
// function V4F32 operator*(const F32 &s, const V4F32 &v);

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
