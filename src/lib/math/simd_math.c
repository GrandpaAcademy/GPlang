/*
 * GPLANG SIMD Math Library
 * Ultra-fast vectorized mathematical operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <immintrin.h>
#include "simd_math.h"

/*
 * Vector2 operations
 */
gp_vec2_t gp_vec2_create(float x, float y) {
    gp_vec2_t v = {x, y};
    return v;
}

gp_vec2_t gp_vec2_add(gp_vec2_t a, gp_vec2_t b) {
    return gp_vec2_create(a.x + b.x, a.y + b.y);
}

gp_vec2_t gp_vec2_sub(gp_vec2_t a, gp_vec2_t b) {
    return gp_vec2_create(a.x - b.x, a.y - b.y);
}

gp_vec2_t gp_vec2_mul(gp_vec2_t a, float scalar) {
    return gp_vec2_create(a.x * scalar, a.y * scalar);
}

float gp_vec2_dot(gp_vec2_t a, gp_vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

float gp_vec2_length(gp_vec2_t v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

gp_vec2_t gp_vec2_normalize(gp_vec2_t v) {
    float len = gp_vec2_length(v);
    if (len > 0.0f) {
        return gp_vec2_mul(v, 1.0f / len);
    }
    return v;
}

/*
 * Vector3 operations
 */
gp_vec3_t gp_vec3_create(float x, float y, float z) {
    gp_vec3_t v = {x, y, z};
    return v;
}

gp_vec3_t gp_vec3_add(gp_vec3_t a, gp_vec3_t b) {
    return gp_vec3_create(a.x + b.x, a.y + b.y, a.z + b.z);
}

gp_vec3_t gp_vec3_sub(gp_vec3_t a, gp_vec3_t b) {
    return gp_vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

gp_vec3_t gp_vec3_mul(gp_vec3_t a, float scalar) {
    return gp_vec3_create(a.x * scalar, a.y * scalar, a.z * scalar);
}

float gp_vec3_dot(gp_vec3_t a, gp_vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

gp_vec3_t gp_vec3_cross(gp_vec3_t a, gp_vec3_t b) {
    return gp_vec3_create(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float gp_vec3_length(gp_vec3_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

gp_vec3_t gp_vec3_normalize(gp_vec3_t v) {
    float len = gp_vec3_length(v);
    if (len > 0.0f) {
        return gp_vec3_mul(v, 1.0f / len);
    }
    return v;
}

/*
 * Vector4 operations (SIMD optimized)
 */
gp_vec4_t gp_vec4_create(float x, float y, float z, float w) {
    gp_vec4_t v;
    v.simd = _mm_set_ps(w, z, y, x);
    return v;
}

gp_vec4_t gp_vec4_add(gp_vec4_t a, gp_vec4_t b) {
    gp_vec4_t result;
    result.simd = _mm_add_ps(a.simd, b.simd);
    return result;
}

gp_vec4_t gp_vec4_sub(gp_vec4_t a, gp_vec4_t b) {
    gp_vec4_t result;
    result.simd = _mm_sub_ps(a.simd, b.simd);
    return result;
}

gp_vec4_t gp_vec4_mul(gp_vec4_t a, float scalar) {
    gp_vec4_t result;
    __m128 scalar_vec = _mm_set1_ps(scalar);
    result.simd = _mm_mul_ps(a.simd, scalar_vec);
    return result;
}

float gp_vec4_dot(gp_vec4_t a, gp_vec4_t b) {
    __m128 mul = _mm_mul_ps(a.simd, b.simd);
    __m128 sum = _mm_hadd_ps(mul, mul);
    sum = _mm_hadd_ps(sum, sum);
    return _mm_cvtss_f32(sum);
}

float gp_vec4_length(gp_vec4_t v) {
    return sqrtf(gp_vec4_dot(v, v));
}

gp_vec4_t gp_vec4_normalize(gp_vec4_t v) {
    float len = gp_vec4_length(v);
    if (len > 0.0f) {
        return gp_vec4_mul(v, 1.0f / len);
    }
    return v;
}

/*
 * Matrix4x4 operations (SIMD optimized)
 */
gp_mat4_t gp_mat4_identity(void) {
    gp_mat4_t m;
    m.rows[0] = gp_vec4_create(1.0f, 0.0f, 0.0f, 0.0f);
    m.rows[1] = gp_vec4_create(0.0f, 1.0f, 0.0f, 0.0f);
    m.rows[2] = gp_vec4_create(0.0f, 0.0f, 1.0f, 0.0f);
    m.rows[3] = gp_vec4_create(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

gp_mat4_t gp_mat4_multiply(gp_mat4_t a, gp_mat4_t b) {
    gp_mat4_t result;
    
    // Transpose matrix b for efficient SIMD operations
    __m128 b_col0 = _mm_set_ps(b.data[12], b.data[8], b.data[4], b.data[0]);
    __m128 b_col1 = _mm_set_ps(b.data[13], b.data[9], b.data[5], b.data[1]);
    __m128 b_col2 = _mm_set_ps(b.data[14], b.data[10], b.data[6], b.data[2]);
    __m128 b_col3 = _mm_set_ps(b.data[15], b.data[11], b.data[7], b.data[3]);
    
    for (int i = 0; i < 4; i++) {
        __m128 a_row = a.rows[i].simd;
        
        __m128 result_row = _mm_mul_ps(_mm_shuffle_ps(a_row, a_row, 0x00), b_col0);
        result_row = _mm_add_ps(result_row, _mm_mul_ps(_mm_shuffle_ps(a_row, a_row, 0x55), b_col1));
        result_row = _mm_add_ps(result_row, _mm_mul_ps(_mm_shuffle_ps(a_row, a_row, 0xAA), b_col2));
        result_row = _mm_add_ps(result_row, _mm_mul_ps(_mm_shuffle_ps(a_row, a_row, 0xFF), b_col3));
        
        result.rows[i].simd = result_row;
    }
    
    return result;
}

gp_vec4_t gp_mat4_transform(gp_mat4_t m, gp_vec4_t v) {
    gp_vec4_t result;
    
    __m128 x = _mm_shuffle_ps(v.simd, v.simd, 0x00);
    __m128 y = _mm_shuffle_ps(v.simd, v.simd, 0x55);
    __m128 z = _mm_shuffle_ps(v.simd, v.simd, 0xAA);
    __m128 w = _mm_shuffle_ps(v.simd, v.simd, 0xFF);
    
    result.simd = _mm_mul_ps(m.rows[0].simd, x);
    result.simd = _mm_add_ps(result.simd, _mm_mul_ps(m.rows[1].simd, y));
    result.simd = _mm_add_ps(result.simd, _mm_mul_ps(m.rows[2].simd, z));
    result.simd = _mm_add_ps(result.simd, _mm_mul_ps(m.rows[3].simd, w));
    
    return result;
}

/*
 * SIMD array operations for ultra performance
 */
void gp_simd_add_arrays(const float* a, const float* b, float* result, size_t count) {
    size_t simd_count = count & ~3; // Process 4 elements at a time
    
    for (size_t i = 0; i < simd_count; i += 4) {
        __m128 va = _mm_load_ps(&a[i]);
        __m128 vb = _mm_load_ps(&b[i]);
        __m128 vr = _mm_add_ps(va, vb);
        _mm_store_ps(&result[i], vr);
    }
    
    // Handle remaining elements
    for (size_t i = simd_count; i < count; i++) {
        result[i] = a[i] + b[i];
    }
}

void gp_simd_mul_arrays(const float* a, const float* b, float* result, size_t count) {
    size_t simd_count = count & ~3;
    
    for (size_t i = 0; i < simd_count; i += 4) {
        __m128 va = _mm_load_ps(&a[i]);
        __m128 vb = _mm_load_ps(&b[i]);
        __m128 vr = _mm_mul_ps(va, vb);
        _mm_store_ps(&result[i], vr);
    }
    
    for (size_t i = simd_count; i < count; i++) {
        result[i] = a[i] * b[i];
    }
}

void gp_simd_scale_array(const float* input, float scalar, float* result, size_t count) {
    size_t simd_count = count & ~3;
    __m128 scale_vec = _mm_set1_ps(scalar);
    
    for (size_t i = 0; i < simd_count; i += 4) {
        __m128 va = _mm_load_ps(&input[i]);
        __m128 vr = _mm_mul_ps(va, scale_vec);
        _mm_store_ps(&result[i], vr);
    }
    
    for (size_t i = simd_count; i < count; i++) {
        result[i] = input[i] * scalar;
    }
}

float gp_simd_dot_product(const float* a, const float* b, size_t count) {
    size_t simd_count = count & ~3;
    __m128 sum = _mm_setzero_ps();
    
    for (size_t i = 0; i < simd_count; i += 4) {
        __m128 va = _mm_load_ps(&a[i]);
        __m128 vb = _mm_load_ps(&b[i]);
        __m128 mul = _mm_mul_ps(va, vb);
        sum = _mm_add_ps(sum, mul);
    }
    
    // Horizontal sum
    __m128 sum2 = _mm_hadd_ps(sum, sum);
    sum2 = _mm_hadd_ps(sum2, sum2);
    float result = _mm_cvtss_f32(sum2);
    
    // Handle remaining elements
    for (size_t i = simd_count; i < count; i++) {
        result += a[i] * b[i];
    }
    
    return result;
}

/*
 * Fast mathematical functions
 */
void gp_fast_sin_array(const float* input, float* output, size_t count) {
    for (size_t i = 0; i < count; i++) {
        output[i] = sinf(input[i]);
    }
}

void gp_fast_cos_array(const float* input, float* output, size_t count) {
    for (size_t i = 0; i < count; i++) {
        output[i] = cosf(input[i]);
    }
}

void gp_fast_sqrt_array(const float* input, float* output, size_t count) {
    size_t simd_count = count & ~3;
    
    for (size_t i = 0; i < simd_count; i += 4) {
        __m128 va = _mm_load_ps(&input[i]);
        __m128 vr = _mm_sqrt_ps(va);
        _mm_store_ps(&output[i], vr);
    }
    
    for (size_t i = simd_count; i < count; i++) {
        output[i] = sqrtf(input[i]);
    }
}

/*
 * Statistics functions
 */
float gp_array_sum(const float* array, size_t count) {
    size_t simd_count = count & ~3;
    __m128 sum = _mm_setzero_ps();
    
    for (size_t i = 0; i < simd_count; i += 4) {
        __m128 va = _mm_load_ps(&array[i]);
        sum = _mm_add_ps(sum, va);
    }
    
    __m128 sum2 = _mm_hadd_ps(sum, sum);
    sum2 = _mm_hadd_ps(sum2, sum2);
    float result = _mm_cvtss_f32(sum2);
    
    for (size_t i = simd_count; i < count; i++) {
        result += array[i];
    }
    
    return result;
}

float gp_array_mean(const float* array, size_t count) {
    if (count == 0) return 0.0f;
    return gp_array_sum(array, count) / (float)count;
}

float gp_array_min(const float* array, size_t count) {
    if (count == 0) return 0.0f;
    
    float min_val = array[0];
    for (size_t i = 1; i < count; i++) {
        if (array[i] < min_val) {
            min_val = array[i];
        }
    }
    return min_val;
}

float gp_array_max(const float* array, size_t count) {
    if (count == 0) return 0.0f;
    
    float max_val = array[0];
    for (size_t i = 1; i < count; i++) {
        if (array[i] > max_val) {
            max_val = array[i];
        }
    }
    return max_val;
}
