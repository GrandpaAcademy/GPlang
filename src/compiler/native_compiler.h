/*
 * GPLANG Native Compiler Header
 * Zero-overhead systems programming with memory safety
 */

#ifndef NATIVE_COMPILER_H
#define NATIVE_COMPILER_H

#include <stddef.h>
#include <stdint.h>

// Native compilation modes
typedef enum {
    NATIVE_DEBUG = 0,
    NATIVE_RELEASE = 1,
    NATIVE_ULTRA = 2
} native_mode_t;

// Native compiler statistics
typedef struct {
    native_mode_t mode;
    uint8_t zero_overhead;
    uint8_t memory_safe;
    uint8_t simd_enabled;
    uint8_t inline_asm_enabled;
    uint8_t manual_memory;
    size_t binary_size_kb;
    uint32_t startup_time_us;
} native_stats_t;

// Function declarations
int native_compiler_init(native_mode_t mode);
void native_compiler_cleanup(void);

// Compilation functions
int compile_to_native(const char* source_file, const char* output_file);
int parse_and_analyze(const char* source_file);

// Memory safety (Rust-like)
int analyze_memory_safety(void);
int check_ownership_rules(void);
int check_borrowing_rules(void);
int check_lifetime_rules(void);

// Code generation
int generate_native_code(const char* output_file);
void enable_native_optimizations(void);
int generate_assembly(void);
int optimize_assembly(void);
int assemble_to_object(void);
int link_executable(const char* output_file);

// Manual memory management
void* native_alloc(size_t size);
void native_free(void* ptr);
void track_allocation(void* ptr, size_t size);
void untrack_allocation(void* ptr);
int verify_ownership(void* ptr);

// Statistics
void get_native_stats(native_stats_t* stats);

// Zero-overhead macros
#define NATIVE_INLINE __attribute__((always_inline)) inline
#define NATIVE_NOINLINE __attribute__((noinline))
#define NATIVE_PURE __attribute__((pure))
#define NATIVE_CONST __attribute__((const))
#define NATIVE_HOT __attribute__((hot))
#define NATIVE_COLD __attribute__((cold))
#define NATIVE_LIKELY(x) __builtin_expect(!!(x), 1)
#define NATIVE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define NATIVE_RESTRICT __restrict__
#define NATIVE_ALIGNED(n) __attribute__((aligned(n)))
#define NATIVE_PACKED __attribute__((packed))

// Memory safety macros (Rust-like)
#define NATIVE_MOVE(x) (x) // Move semantics
#define NATIVE_BORROW(x) (&(x)) // Immutable borrow
#define NATIVE_BORROW_MUT(x) (&mut (x)) // Mutable borrow
#define NATIVE_UNSAFE unsafe // Unsafe block marker

// Manual memory management macros
#define NATIVE_ALLOC(size) native_alloc(size)
#define NATIVE_FREE(ptr) native_free(ptr)
#define NATIVE_STACK_ALLOC(size) __builtin_alloca(size)

// Inline assembly support
#define NATIVE_ASM(code) __asm__ volatile(code)
#define NATIVE_ASM_INPUT(code, input) __asm__ volatile(code : : input)
#define NATIVE_ASM_OUTPUT(code, output) __asm__ volatile(code : output)
#define NATIVE_ASM_FULL(code, output, input, clobber) \
    __asm__ volatile(code : output : input : clobber)

// SIMD intrinsics support
#define NATIVE_SIMD_LOAD(ptr) _mm256_load_ps(ptr)
#define NATIVE_SIMD_STORE(ptr, vec) _mm256_store_ps(ptr, vec)
#define NATIVE_SIMD_ADD(a, b) _mm256_add_ps(a, b)
#define NATIVE_SIMD_MUL(a, b) _mm256_mul_ps(a, b)

// Cache optimization
#define NATIVE_PREFETCH_READ(addr) __builtin_prefetch(addr, 0, 3)
#define NATIVE_PREFETCH_WRITE(addr) __builtin_prefetch(addr, 1, 3)
#define NATIVE_CACHE_LINE_SIZE 64

// Branch prediction
#define NATIVE_BRANCH_LIKELY(cond) __builtin_expect(!!(cond), 1)
#define NATIVE_BRANCH_UNLIKELY(cond) __builtin_expect(!!(cond), 0)

// Compiler barriers
#define NATIVE_COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
#define NATIVE_MEMORY_BARRIER() __sync_synchronize()

// Function attributes for optimization
#define NATIVE_FLATTEN __attribute__((flatten))
#define NATIVE_OPTIMIZE(level) __attribute__((optimize(level)))
#define NATIVE_TARGET(arch) __attribute__((target(arch)))

#endif // NATIVE_COMPILER_H
