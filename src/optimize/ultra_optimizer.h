/*
 * GPLANG Ultra Optimizer Header
 * Faster than C performance engine
 */

#ifndef ULTRA_OPTIMIZER_H
#define ULTRA_OPTIMIZER_H

#include <stddef.h>
#include <stdint.h>

// Ultra optimizer statistics
typedef struct {
    int simd_width;           // SIMD vector width (128/256/512)
    int parallel_lanes;       // Number of parallel operations
    int thread_count;         // OpenMP thread count
    size_t memory_pool_size;  // Memory pool size
    size_t memory_used;       // Memory currently used
    int optimization_level;   // Optimization level (0-3)
} ultra_stats_t;

// Function declarations
int ultra_optimizer_init(void);
void ultra_optimizer_cleanup(void);

// CPU feature detection
void detect_cpu_features(void);

// Memory management
void init_ultra_allocator(void);
void* ultra_alloc(size_t size);

// SIMD engine
void setup_simd_engine(void);

// Optimization functions
void enable_extreme_optimizations(void);
void optimize_parallel_loop(int start, int end, void (*operation)(int));

// Ultra-fast operations
void ultra_math_operations(float* input, float* output, int count);
void ultra_string_operations(char* input, char* output, int length);

// Benchmarking
void benchmark_vs_c(void);

// Statistics
void get_ultra_stats(ultra_stats_t* stats);

// Performance macros for ultra-fast code generation
#define ULTRA_INLINE __attribute__((always_inline)) inline
#define ULTRA_HOT __attribute__((hot))
#define ULTRA_PURE __attribute__((pure))
#define ULTRA_CONST __attribute__((const))
#define ULTRA_RESTRICT __restrict__
#define ULTRA_LIKELY(x) __builtin_expect(!!(x), 1)
#define ULTRA_UNLIKELY(x) __builtin_expect(!!(x), 0)

// Ultra-fast loop macros
#define ULTRA_PARALLEL_FOR(start, end) \
    _Pragma("omp parallel for simd schedule(static)") \
    for (int i = (start); i < (end); i++)

#define ULTRA_SIMD_LOOP(count) \
    _Pragma("omp simd aligned(data:64)") \
    for (int i = 0; i < (count); i++)

// Memory prefetching for ultra performance
#define ULTRA_PREFETCH_READ(addr) __builtin_prefetch((addr), 0, 3)
#define ULTRA_PREFETCH_WRITE(addr) __builtin_prefetch((addr), 1, 3)

// Branch prediction hints
#define ULTRA_BRANCH_LIKELY(cond) __builtin_expect(!!(cond), 1)
#define ULTRA_BRANCH_UNLIKELY(cond) __builtin_expect(!!(cond), 0)

#endif // ULTRA_OPTIMIZER_H
