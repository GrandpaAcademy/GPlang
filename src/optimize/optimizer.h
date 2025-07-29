/*
 * GPLANG Optimizer Header
 * Advanced optimization passes for maximum performance
 */

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <stddef.h>
#include <stdint.h>

// Optimizer statistics
typedef struct {
    int simd_width;           // SIMD vector width (128/256/512)
    int parallel_lanes;       // Number of parallel operations
    int thread_count;         // OpenMP thread count
    size_t memory_pool_size;  // Memory pool size
    size_t memory_used;       // Memory currently used
    int optimization_level;   // Optimization level (0-3)
} optimizer_stats_t;

// Function declarations
int optimizer_init(void);
void optimizer_cleanup(void);

// CPU feature detection
void detect_cpu_features(void);

// Memory management
void init_fast_allocator(void);
void* fast_alloc(size_t size);

// SIMD engine
void setup_simd_engine(void);

// Optimization functions
void enable_advanced_optimizations(void);
void optimize_parallel_loop(int start, int end, void (*operation)(int));

// High-performance operations
void fast_math_operations(float* input, float* output, int count);
void fast_string_operations(char* input, char* output, int length);

// Benchmarking
void benchmark_vs_c(void);

// Statistics
void get_optimizer_stats(optimizer_stats_t* stats);

// Performance macros for high-performance code generation
#define FAST_INLINE __attribute__((always_inline)) inline
#define FAST_HOT __attribute__((hot))
#define FAST_PURE __attribute__((pure))
#define FAST_CONST __attribute__((const))
#define FAST_RESTRICT __restrict__
#define FAST_LIKELY(x) __builtin_expect(!!(x), 1)
#define FAST_UNLIKELY(x) __builtin_expect(!!(x), 0)

// High-performance loop macros
#define FAST_PARALLEL_FOR(start, end) \
    _Pragma("omp parallel for simd schedule(static)") \
    for (int i = (start); i < (end); i++)

#define FAST_SIMD_LOOP(count) \
    _Pragma("omp simd aligned(data:64)") \
    for (int i = 0; i < (count); i++)

// Memory prefetching for high performance
#define FAST_PREFETCH_READ(addr) __builtin_prefetch((addr), 0, 3)
#define FAST_PREFETCH_WRITE(addr) __builtin_prefetch((addr), 1, 3)

// Branch prediction hints
#define FAST_BRANCH_LIKELY(cond) __builtin_expect(!!(cond), 1)
#define FAST_BRANCH_UNLIKELY(cond) __builtin_expect(!!(cond), 0)

#endif // OPTIMIZER_H
