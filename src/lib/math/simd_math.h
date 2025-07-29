/*
 * GPLANG SIMD Math Library Header
 * Ultra-fast vectorized mathematical operations
 */

#ifndef GP_SIMD_MATH_H
#define GP_SIMD_MATH_H

#include <stddef.h>
#include <immintrin.h>

// Vector types
typedef struct {
    float x, y;
} gp_vec2_t;

typedef struct {
    float x, y, z;
} gp_vec3_t;

typedef union {
    struct { float x, y, z, w; };
    float data[4];
    __m128 simd;
} gp_vec4_t;

// Matrix type (4x4)
typedef union {
    gp_vec4_t rows[4];
    float data[16];
} gp_mat4_t;

// Vector2 operations
gp_vec2_t gp_vec2_create(float x, float y);
gp_vec2_t gp_vec2_add(gp_vec2_t a, gp_vec2_t b);
gp_vec2_t gp_vec2_sub(gp_vec2_t a, gp_vec2_t b);
gp_vec2_t gp_vec2_mul(gp_vec2_t a, float scalar);
float gp_vec2_dot(gp_vec2_t a, gp_vec2_t b);
float gp_vec2_length(gp_vec2_t v);
gp_vec2_t gp_vec2_normalize(gp_vec2_t v);

// Vector3 operations
gp_vec3_t gp_vec3_create(float x, float y, float z);
gp_vec3_t gp_vec3_add(gp_vec3_t a, gp_vec3_t b);
gp_vec3_t gp_vec3_sub(gp_vec3_t a, gp_vec3_t b);
gp_vec3_t gp_vec3_mul(gp_vec3_t a, float scalar);
float gp_vec3_dot(gp_vec3_t a, gp_vec3_t b);
gp_vec3_t gp_vec3_cross(gp_vec3_t a, gp_vec3_t b);
float gp_vec3_length(gp_vec3_t v);
gp_vec3_t gp_vec3_normalize(gp_vec3_t v);

// Vector4 operations (SIMD optimized)
gp_vec4_t gp_vec4_create(float x, float y, float z, float w);
gp_vec4_t gp_vec4_add(gp_vec4_t a, gp_vec4_t b);
gp_vec4_t gp_vec4_sub(gp_vec4_t a, gp_vec4_t b);
gp_vec4_t gp_vec4_mul(gp_vec4_t a, float scalar);
float gp_vec4_dot(gp_vec4_t a, gp_vec4_t b);
float gp_vec4_length(gp_vec4_t v);
gp_vec4_t gp_vec4_normalize(gp_vec4_t v);

// Matrix4x4 operations (SIMD optimized)
gp_mat4_t gp_mat4_identity(void);
gp_mat4_t gp_mat4_multiply(gp_mat4_t a, gp_mat4_t b);
gp_vec4_t gp_mat4_transform(gp_mat4_t m, gp_vec4_t v);

// SIMD array operations for ultra performance
void gp_simd_add_arrays(const float* a, const float* b, float* result, size_t count);
void gp_simd_mul_arrays(const float* a, const float* b, float* result, size_t count);
void gp_simd_scale_array(const float* input, float scalar, float* result, size_t count);
float gp_simd_dot_product(const float* a, const float* b, size_t count);

// Fast mathematical functions
void gp_fast_sin_array(const float* input, float* output, size_t count);
void gp_fast_cos_array(const float* input, float* output, size_t count);
void gp_fast_sqrt_array(const float* input, float* output, size_t count);

// Statistics functions
float gp_array_sum(const float* array, size_t count);
float gp_array_mean(const float* array, size_t count);
float gp_array_min(const float* array, size_t count);
float gp_array_max(const float* array, size_t count);

#endif // GP_SIMD_MATH_H
