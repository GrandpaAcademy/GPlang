/*
 * GPLANG Cache-Friendly Data Structures
 * Optimized for modern CPU cache hierarchies
 */

#ifndef CACHE_FRIENDLY_H
#define CACHE_FRIENDLY_H

#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

// Cache line size for modern CPUs
#define CACHE_LINE_SIZE 64
#define CACHE_ALIGNED alignas(CACHE_LINE_SIZE)

// Structure of Arrays (SoA) for better cache utilization
typedef struct {
    float* x_coords;      // All X coordinates together
    float* y_coords;      // All Y coordinates together
    float* z_coords;      // All Z coordinates together
    size_t count;
    size_t capacity;
} soa_vec3_t;

// Array of Structures (AoS) - traditional layout
typedef struct {
    float x, y, z;
} aos_vec3_t;

// Cache-friendly hash table with linear probing
typedef struct {
    uint64_t* keys;       // Separate key array
    void** values;        // Separate value array
    uint8_t* metadata;    // Metadata for each slot
    size_t capacity;
    size_t count;
    size_t mask;          // For fast modulo
} cache_hash_table_t;

// Memory pool with cache-aligned blocks
typedef struct {
    void* memory;
    size_t block_size;
    size_t block_count;
    size_t next_free;
    uint64_t* free_bitmap;
} CACHE_ALIGNED cache_pool_t;

// Packed array for small integers (cache-friendly)
typedef struct {
    uint32_t* data;       // Packed data
    size_t count;
    size_t capacity;
    uint8_t bits_per_element;
} packed_array_t;

// Function declarations
soa_vec3_t* soa_vec3_create(size_t capacity);
void soa_vec3_destroy(soa_vec3_t* soa);
void soa_vec3_push(soa_vec3_t* soa, float x, float y, float z);
void soa_vec3_get(soa_vec3_t* soa, size_t index, float* x, float* y, float* z);

cache_hash_table_t* cache_hash_create(size_t capacity);
void cache_hash_destroy(cache_hash_table_t* table);
int cache_hash_insert(cache_hash_table_t* table, uint64_t key, void* value);
void* cache_hash_get(cache_hash_table_t* table, uint64_t key);

cache_pool_t* cache_pool_create(size_t block_size, size_t block_count);
void cache_pool_destroy(cache_pool_t* pool);
void* cache_pool_alloc(cache_pool_t* pool);
void cache_pool_free(cache_pool_t* pool, void* ptr);

packed_array_t* packed_array_create(size_t capacity, uint8_t bits_per_element);
void packed_array_destroy(packed_array_t* array);
void packed_array_set(packed_array_t* array, size_t index, uint32_t value);
uint32_t packed_array_get(packed_array_t* array, size_t index);

// Cache optimization utilities
void prefetch_read(const void* addr);
void prefetch_write(void* addr);
size_t get_cache_line_size(void);
void* aligned_alloc_cache(size_t size);

#endif // CACHE_FRIENDLY_H
