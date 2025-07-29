/*
 * GPLANG Cache-Friendly Data Structures Implementation
 * Optimized for modern CPU cache hierarchies
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cache_friendly.h"

/*
 * Structure of Arrays (SoA) implementation
 * Better cache utilization for vectorized operations
 */
soa_vec3_t* soa_vec3_create(size_t capacity) {
    soa_vec3_t* soa = malloc(sizeof(soa_vec3_t));
    if (!soa) return NULL;
    
    // Allocate aligned arrays for better cache performance
    soa->x_coords = aligned_alloc_cache(capacity * sizeof(float));
    soa->y_coords = aligned_alloc_cache(capacity * sizeof(float));
    soa->z_coords = aligned_alloc_cache(capacity * sizeof(float));
    
    if (!soa->x_coords || !soa->y_coords || !soa->z_coords) {
        soa_vec3_destroy(soa);
        return NULL;
    }
    
    soa->count = 0;
    soa->capacity = capacity;
    return soa;
}

void soa_vec3_destroy(soa_vec3_t* soa) {
    if (soa) {
        free(soa->x_coords);
        free(soa->y_coords);
        free(soa->z_coords);
        free(soa);
    }
}

void soa_vec3_push(soa_vec3_t* soa, float x, float y, float z) {
    assert(soa && soa->count < soa->capacity);
    
    soa->x_coords[soa->count] = x;
    soa->y_coords[soa->count] = y;
    soa->z_coords[soa->count] = z;
    soa->count++;
}

void soa_vec3_get(soa_vec3_t* soa, size_t index, float* x, float* y, float* z) {
    assert(soa && index < soa->count);
    
    *x = soa->x_coords[index];
    *y = soa->y_coords[index];
    *z = soa->z_coords[index];
}

/*
 * Cache-friendly hash table with linear probing
 * Separate arrays for better cache utilization
 */
cache_hash_table_t* cache_hash_create(size_t capacity) {
    // Ensure capacity is power of 2 for fast modulo
    size_t actual_capacity = 1;
    while (actual_capacity < capacity) {
        actual_capacity <<= 1;
    }
    
    cache_hash_table_t* table = malloc(sizeof(cache_hash_table_t));
    if (!table) return NULL;
    
    table->keys = aligned_alloc_cache(actual_capacity * sizeof(uint64_t));
    table->values = aligned_alloc_cache(actual_capacity * sizeof(void*));
    table->metadata = aligned_alloc_cache(actual_capacity * sizeof(uint8_t));
    
    if (!table->keys || !table->values || !table->metadata) {
        cache_hash_destroy(table);
        return NULL;
    }
    
    // Initialize metadata (0 = empty, 1 = occupied, 2 = deleted)
    memset(table->metadata, 0, actual_capacity);
    
    table->capacity = actual_capacity;
    table->count = 0;
    table->mask = actual_capacity - 1;
    
    return table;
}

void cache_hash_destroy(cache_hash_table_t* table) {
    if (table) {
        free(table->keys);
        free(table->values);
        free(table->metadata);
        free(table);
    }
}

static uint64_t hash_key(uint64_t key) {
    // FNV-1a hash for good distribution
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccd;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53;
    key ^= key >> 33;
    return key;
}

int cache_hash_insert(cache_hash_table_t* table, uint64_t key, void* value) {
    if (table->count >= table->capacity * 0.75) {
        return -1; // Table too full
    }
    
    uint64_t hash = hash_key(key);
    size_t index = hash & table->mask;
    
    // Linear probing with prefetching
    while (table->metadata[index] == 1) {
        if (table->keys[index] == key) {
            table->values[index] = value; // Update existing
            return 0;
        }
        index = (index + 1) & table->mask;
        
        // Prefetch next cache line
        prefetch_read(&table->metadata[index + 8]);
    }
    
    table->keys[index] = key;
    table->values[index] = value;
    table->metadata[index] = 1;
    table->count++;
    
    return 0;
}

void* cache_hash_get(cache_hash_table_t* table, uint64_t key) {
    uint64_t hash = hash_key(key);
    size_t index = hash & table->mask;
    
    while (table->metadata[index] != 0) {
        if (table->metadata[index] == 1 && table->keys[index] == key) {
            return table->values[index];
        }
        index = (index + 1) & table->mask;
        
        // Prefetch next cache line
        prefetch_read(&table->metadata[index + 8]);
    }
    
    return NULL; // Not found
}

/*
 * Cache-aligned memory pool
 */
cache_pool_t* cache_pool_create(size_t block_size, size_t block_count) {
    // Align block size to cache line boundary
    block_size = (block_size + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1);
    
    cache_pool_t* pool = aligned_alloc_cache(sizeof(cache_pool_t));
    if (!pool) return NULL;
    
    pool->memory = aligned_alloc_cache(block_size * block_count);
    pool->free_bitmap = aligned_alloc_cache((block_count + 63) / 64 * sizeof(uint64_t));
    
    if (!pool->memory || !pool->free_bitmap) {
        cache_pool_destroy(pool);
        return NULL;
    }
    
    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->next_free = 0;
    
    // Initialize all blocks as free
    memset(pool->free_bitmap, 0, (block_count + 63) / 64 * sizeof(uint64_t));
    
    return pool;
}

void cache_pool_destroy(cache_pool_t* pool) {
    if (pool) {
        free(pool->memory);
        free(pool->free_bitmap);
        free(pool);
    }
}

void* cache_pool_alloc(cache_pool_t* pool) {
    // Find first free block using bitmap
    for (size_t i = pool->next_free; i < pool->block_count; i++) {
        size_t word_index = i / 64;
        size_t bit_index = i % 64;
        
        if (!(pool->free_bitmap[word_index] & (1ULL << bit_index))) {
            // Mark as allocated
            pool->free_bitmap[word_index] |= (1ULL << bit_index);
            pool->next_free = i + 1;
            
            return (char*)pool->memory + i * pool->block_size;
        }
    }
    
    return NULL; // Pool exhausted
}

void cache_pool_free(cache_pool_t* pool, void* ptr) {
    if (!ptr) return;
    
    size_t offset = (char*)ptr - (char*)pool->memory;
    size_t block_index = offset / pool->block_size;
    
    if (block_index >= pool->block_count) return; // Invalid pointer
    
    size_t word_index = block_index / 64;
    size_t bit_index = block_index % 64;
    
    // Mark as free
    pool->free_bitmap[word_index] &= ~(1ULL << bit_index);
    
    if (block_index < pool->next_free) {
        pool->next_free = block_index;
    }
}

/*
 * Packed array for space-efficient storage
 */
packed_array_t* packed_array_create(size_t capacity, uint8_t bits_per_element) {
    assert(bits_per_element <= 32);
    
    packed_array_t* array = malloc(sizeof(packed_array_t));
    if (!array) return NULL;
    
    size_t total_bits = capacity * bits_per_element;
    size_t data_size = (total_bits + 31) / 32 * sizeof(uint32_t);
    
    array->data = aligned_alloc_cache(data_size);
    if (!array->data) {
        free(array);
        return NULL;
    }
    
    memset(array->data, 0, data_size);
    array->count = 0;
    array->capacity = capacity;
    array->bits_per_element = bits_per_element;
    
    return array;
}

void packed_array_destroy(packed_array_t* array) {
    if (array) {
        free(array->data);
        free(array);
    }
}

void packed_array_set(packed_array_t* array, size_t index, uint32_t value) {
    assert(array && index < array->capacity);
    
    size_t bit_offset = index * array->bits_per_element;
    size_t word_index = bit_offset / 32;
    size_t bit_index = bit_offset % 32;
    
    uint32_t mask = (1U << array->bits_per_element) - 1;
    value &= mask; // Ensure value fits in allocated bits
    
    // Clear existing bits
    array->data[word_index] &= ~(mask << bit_index);
    // Set new value
    array->data[word_index] |= (value << bit_index);
    
    // Handle overflow to next word
    if (bit_index + array->bits_per_element > 32) {
        size_t overflow_bits = bit_index + array->bits_per_element - 32;
        uint32_t overflow_mask = (1U << overflow_bits) - 1;
        
        array->data[word_index + 1] &= ~overflow_mask;
        array->data[word_index + 1] |= (value >> (array->bits_per_element - overflow_bits));
    }
}

uint32_t packed_array_get(packed_array_t* array, size_t index) {
    assert(array && index < array->capacity);
    
    size_t bit_offset = index * array->bits_per_element;
    size_t word_index = bit_offset / 32;
    size_t bit_index = bit_offset % 32;
    
    uint32_t mask = (1U << array->bits_per_element) - 1;
    uint32_t value = (array->data[word_index] >> bit_index) & mask;
    
    // Handle overflow from next word
    if (bit_index + array->bits_per_element > 32) {
        size_t overflow_bits = bit_index + array->bits_per_element - 32;
        uint32_t overflow_mask = (1U << overflow_bits) - 1;
        
        value |= (array->data[word_index + 1] & overflow_mask) << (array->bits_per_element - overflow_bits);
    }
    
    return value;
}

/*
 * Cache optimization utilities
 */
void prefetch_read(const void* addr) {
    __builtin_prefetch(addr, 0, 3); // Read, high temporal locality
}

void prefetch_write(void* addr) {
    __builtin_prefetch(addr, 1, 3); // Write, high temporal locality
}

size_t get_cache_line_size(void) {
    return CACHE_LINE_SIZE;
}

void* aligned_alloc_cache(size_t size) {
    void* ptr;
    if (posix_memalign(&ptr, CACHE_LINE_SIZE, size) == 0) {
        return ptr;
    }
    return NULL;
}
