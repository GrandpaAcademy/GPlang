#ifndef GPLANG_SPEED_BOOSTER_H
#define GPLANG_SPEED_BOOSTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <immintrin.h>  // For SIMD intrinsics
#include "../ir/ir.h"

// CPU Architecture Detection
typedef enum {
    CPU_ARCH_UNKNOWN,
    CPU_ARCH_X86_64,
    CPU_ARCH_ARM64,
    CPU_ARCH_RISCV64,
    CPU_ARCH_AVX2,
    CPU_ARCH_AVX512
} CPUArchitecture;

// CPU Features
typedef struct {
    bool has_sse;
    bool has_sse2;
    bool has_sse3;
    bool has_sse4_1;
    bool has_sse4_2;
    bool has_avx;
    bool has_avx2;
    bool has_avx512;
    bool has_fma;
    bool has_bmi;
    bool has_bmi2;
    bool has_popcnt;
    bool has_lzcnt;
    bool has_prefetch;
    bool has_aes;
    bool has_rdrand;
    int cache_line_size;
    int l1_cache_size;
    int l2_cache_size;
    int l3_cache_size;
    int num_cores;
    int num_threads;
} CPUFeatures;

// Speed Boost Configuration
typedef struct {
    bool enable_simd;
    bool enable_vectorization;
    bool enable_prefetching;
    bool enable_branch_hints;
    bool enable_cache_optimization;
    bool enable_parallel_loops;
    bool enable_cpu_specific;
    bool enable_memory_alignment;
    bool enable_instruction_fusion;
    bool enable_micro_optimizations;
    int vectorization_width;
    int prefetch_distance;
    int unroll_factor;
    bool aggressive_inlining;
    bool use_builtin_functions;
} SpeedBoostConfig;

// Performance Counters
typedef struct {
    uint64_t instructions_optimized;
    uint64_t loops_vectorized;
    uint64_t branches_predicted;
    uint64_t cache_misses_reduced;
    uint64_t simd_operations_added;
    uint64_t prefetch_instructions_added;
    double speedup_estimate;
    double memory_bandwidth_improvement;
    double cache_hit_rate_improvement;
} PerformanceCounters;

// Speed Booster Context
typedef struct {
    CPUArchitecture target_arch;
    CPUFeatures cpu_features;
    SpeedBoostConfig config;
    PerformanceCounters counters;
    bool profile_guided;
    char* profile_data;
} SpeedBooster;

// Function declarations

// Speed booster initialization
SpeedBooster* speed_booster_create(CPUArchitecture arch);
void speed_booster_destroy(SpeedBooster* booster);
void speed_booster_configure(SpeedBooster* booster, SpeedBoostConfig* config);
CPUFeatures speed_booster_detect_cpu_features(void);

// Main speed boosting functions
int speed_boost_function(SpeedBooster* booster, IRFunction* function);
int speed_boost_module(SpeedBooster* booster, IRModule* module);
PerformanceCounters* speed_booster_get_counters(SpeedBooster* booster);

// SIMD and Vectorization
int speed_boost_vectorize_loops(SpeedBooster* booster, IRFunction* function);
int speed_boost_simd_operations(SpeedBooster* booster, IRFunction* function);
int speed_boost_auto_vectorization(SpeedBooster* booster, IRFunction* function);
bool can_vectorize_loop(SpeedBooster* booster, IRInstruction* loop_start);
int generate_simd_code(SpeedBooster* booster, IRInstruction* inst);

// Memory Optimizations
int speed_boost_memory_prefetch(SpeedBooster* booster, IRFunction* function);
int speed_boost_cache_optimization(SpeedBooster* booster, IRFunction* function);
int speed_boost_memory_alignment(SpeedBooster* booster, IRFunction* function);
int speed_boost_data_layout(SpeedBooster* booster, IRModule* module);
int insert_prefetch_instructions(SpeedBooster* booster, IRFunction* function);

// Branch Optimizations
int speed_boost_branch_prediction(SpeedBooster* booster, IRFunction* function);
int speed_boost_branch_elimination(SpeedBooster* booster, IRFunction* function);
int speed_boost_conditional_moves(SpeedBooster* booster, IRFunction* function);
int add_branch_hints(SpeedBooster* booster, IRFunction* function);

// CPU-Specific Optimizations
int speed_boost_x86_64_specific(SpeedBooster* booster, IRFunction* function);
int speed_boost_arm64_specific(SpeedBooster* booster, IRFunction* function);
int speed_boost_avx_optimizations(SpeedBooster* booster, IRFunction* function);
int speed_boost_cpu_intrinsics(SpeedBooster* booster, IRFunction* function);

// Parallel Optimizations
int speed_boost_parallel_loops(SpeedBooster* booster, IRFunction* function);
int speed_boost_thread_optimization(SpeedBooster* booster, IRFunction* function);
int speed_boost_atomic_operations(SpeedBooster* booster, IRFunction* function);
bool can_parallelize_loop(SpeedBooster* booster, IRInstruction* loop_start);

// Micro-optimizations
int speed_boost_instruction_fusion(SpeedBooster* booster, IRFunction* function);
int speed_boost_register_pressure(SpeedBooster* booster, IRFunction* function);
int speed_boost_instruction_scheduling(SpeedBooster* booster, IRFunction* function);
int speed_boost_peephole_optimizations(SpeedBooster* booster, IRFunction* function);

// Mathematical Optimizations
int speed_boost_fast_math(SpeedBooster* booster, IRFunction* function);
int speed_boost_strength_reduction(SpeedBooster* booster, IRFunction* function);
int speed_boost_polynomial_evaluation(SpeedBooster* booster, IRFunction* function);
int replace_with_builtin_functions(SpeedBooster* booster, IRFunction* function);

// Profile-Guided Optimizations
int speed_boost_profile_guided(SpeedBooster* booster, IRModule* module);
int speed_boost_hot_path_optimization(SpeedBooster* booster, IRFunction* function);
int speed_boost_cold_code_elimination(SpeedBooster* booster, IRFunction* function);
void speed_booster_load_profile(SpeedBooster* booster, const char* profile_file);

// Assembly-Level Speed Boosters
int speed_boost_inline_assembly(SpeedBooster* booster, IRFunction* function);
int speed_boost_custom_calling_convention(SpeedBooster* booster, IRFunction* function);
int speed_boost_register_allocation(SpeedBooster* booster, IRFunction* function);
int speed_boost_instruction_selection(SpeedBooster* booster, IRFunction* function);

// Ultra-Fast Mode (Faster than C)
int speed_boost_ultra_mode(SpeedBooster* booster, IRModule* module);
int speed_boost_zero_cost_abstractions(SpeedBooster* booster, IRModule* module);
int speed_boost_compile_time_evaluation(SpeedBooster* booster, IRModule* module);
int speed_boost_template_specialization(SpeedBooster* booster, IRModule* module);

// Performance Analysis
double estimate_speedup(SpeedBooster* booster, IRFunction* function);
double measure_performance_improvement(SpeedBooster* booster, IRFunction* before, IRFunction* after);
void analyze_bottlenecks(SpeedBooster* booster, IRFunction* function);
void generate_performance_report(SpeedBooster* booster, const char* output_file);

// Utility Functions
bool is_loop_vectorizable(IRInstruction* loop);
bool is_memory_access_pattern_regular(IRInstruction* inst);
int calculate_optimal_unroll_factor(SpeedBooster* booster, IRInstruction* loop);
int calculate_optimal_vector_width(SpeedBooster* booster, IRInstruction* inst);
bool can_use_simd_instruction(SpeedBooster* booster, IRInstruction* inst);

// Configuration Presets
SpeedBoostConfig speed_boost_config_maximum(void);     // Maximum speed, all optimizations
SpeedBoostConfig speed_boost_config_balanced(void);    // Balanced speed/compile time
SpeedBoostConfig speed_boost_config_conservative(void); // Safe optimizations only
SpeedBoostConfig speed_boost_config_ultra(void);       // Experimental ultra-fast mode

// Benchmarking and Testing
typedef struct {
    double baseline_time;
    double optimized_time;
    double speedup_factor;
    uint64_t instructions_before;
    uint64_t instructions_after;
    double cache_miss_rate_before;
    double cache_miss_rate_after;
    double branch_miss_rate_before;
    double branch_miss_rate_after;
} BenchmarkResult;

BenchmarkResult* speed_booster_benchmark(SpeedBooster* booster, IRFunction* function);
void speed_booster_run_performance_tests(SpeedBooster* booster, IRModule* module);
void speed_booster_validate_optimizations(SpeedBooster* booster, IRModule* module);

// Debug and Profiling
void speed_booster_enable_profiling(SpeedBooster* booster);
void speed_booster_print_optimization_log(SpeedBooster* booster);
void speed_booster_dump_performance_counters(SpeedBooster* booster);

#endif // GPLANG_SPEED_BOOSTER_H
