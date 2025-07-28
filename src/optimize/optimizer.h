#ifndef GPLANG_OPTIMIZER_H
#define GPLANG_OPTIMIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../ir/ir.h"

// Optimization Levels
typedef enum {
    OPT_NONE = 0,        // No optimization
    OPT_BASIC = 1,       // Basic optimizations (-O1)
    OPT_AGGRESSIVE = 2,  // Aggressive optimizations (-O2)
    OPT_EXTREME = 3,     // Extreme optimizations (-O3)
    OPT_ULTRA = 4        // Ultra optimizations (faster than C)
} OptimizationLevel;

// Optimization Passes
typedef enum {
    PASS_DEAD_CODE_ELIMINATION,
    PASS_CONSTANT_FOLDING,
    PASS_CONSTANT_PROPAGATION,
    PASS_COPY_PROPAGATION,
    PASS_COMMON_SUBEXPRESSION,
    PASS_LOOP_INVARIANT_MOTION,
    PASS_LOOP_UNROLLING,
    PASS_FUNCTION_INLINING,
    PASS_TAIL_CALL_OPTIMIZATION,
    PASS_VECTORIZATION,
    PASS_REGISTER_ALLOCATION,
    PASS_INSTRUCTION_SCHEDULING,
    PASS_BRANCH_PREDICTION,
    PASS_CACHE_OPTIMIZATION,
    PASS_SIMD_OPTIMIZATION,
    PASS_PARALLEL_OPTIMIZATION,
    PASS_MEMORY_LAYOUT,
    PASS_PREFETCH_INSERTION,
    PASS_COUNT
} OptimizationPass;

// Performance Metrics
typedef struct {
    double compilation_time;
    double execution_time;
    size_t code_size_before;
    size_t code_size_after;
    int instructions_eliminated;
    int loops_optimized;
    int functions_inlined;
    double speedup_factor;
    double memory_reduction;
} OptimizationMetrics;

// Optimizer Configuration
typedef struct {
    OptimizationLevel level;
    bool enable_passes[PASS_COUNT];
    bool aggressive_inlining;
    bool vectorization_enabled;
    bool parallel_optimization;
    bool cache_optimization;
    bool simd_optimization;
    int max_inline_size;
    int loop_unroll_factor;
    int optimization_rounds;
    bool profile_guided;
    bool link_time_optimization;
} OptimizerConfig;

// Optimizer Context
typedef struct {
    OptimizerConfig config;
    OptimizationMetrics metrics;
    IRModule* module;
    char* target_arch;
    bool debug_mode;
    FILE* log_file;
} OptimizerContext;

// Function declarations

// Optimizer initialization and cleanup
OptimizerContext* optimizer_create(OptimizationLevel level, const char* target_arch);
void optimizer_destroy(OptimizerContext* ctx);
void optimizer_set_config(OptimizerContext* ctx, OptimizerConfig* config);
void optimizer_enable_debug(OptimizerContext* ctx, const char* log_file);

// Main optimization entry point
int optimizer_optimize_module(OptimizerContext* ctx, IRModule* module);
OptimizationMetrics* optimizer_get_metrics(OptimizerContext* ctx);

// Individual optimization passes
int opt_dead_code_elimination(OptimizerContext* ctx, IRFunction* function);
int opt_constant_folding(OptimizerContext* ctx, IRFunction* function);
int opt_constant_propagation(OptimizerContext* ctx, IRFunction* function);
int opt_copy_propagation(OptimizerContext* ctx, IRFunction* function);
int opt_common_subexpression(OptimizerContext* ctx, IRFunction* function);
int opt_loop_invariant_motion(OptimizerContext* ctx, IRFunction* function);
int opt_loop_unrolling(OptimizerContext* ctx, IRFunction* function);
int opt_function_inlining(OptimizerContext* ctx, IRModule* module);
int opt_tail_call_optimization(OptimizerContext* ctx, IRFunction* function);
int opt_vectorization(OptimizerContext* ctx, IRFunction* function);
int opt_register_allocation(OptimizerContext* ctx, IRFunction* function);
int opt_instruction_scheduling(OptimizerContext* ctx, IRFunction* function);
int opt_branch_prediction(OptimizerContext* ctx, IRFunction* function);
int opt_cache_optimization(OptimizerContext* ctx, IRFunction* function);
int opt_simd_optimization(OptimizerContext* ctx, IRFunction* function);
int opt_parallel_optimization(OptimizerContext* ctx, IRFunction* function);
int opt_memory_layout(OptimizerContext* ctx, IRModule* module);
int opt_prefetch_insertion(OptimizerContext* ctx, IRFunction* function);

// Advanced optimizations (Ultra mode)
int opt_profile_guided_optimization(OptimizerContext* ctx, IRModule* module);
int opt_link_time_optimization(OptimizerContext* ctx, IRModule* module);
int opt_whole_program_optimization(OptimizerContext* ctx, IRModule* module);
int opt_interprocedural_analysis(OptimizerContext* ctx, IRModule* module);
int opt_escape_analysis(OptimizerContext* ctx, IRModule* module);
int opt_devirtualization(OptimizerContext* ctx, IRModule* module);

// Speed boosters (Assembly-level optimizations) - removed to avoid conflicts
// These are implemented in speed_booster.h with SpeedBooster* parameter

// Analysis functions
bool analyze_hot_paths(OptimizerContext* ctx, IRFunction* function);
bool analyze_memory_patterns(OptimizerContext* ctx, IRFunction* function);
bool analyze_loop_characteristics(OptimizerContext* ctx, IRFunction* function);
bool analyze_function_call_patterns(OptimizerContext* ctx, IRModule* module);

// Utility functions
void optimizer_log(OptimizerContext* ctx, const char* format, ...);
void optimizer_print_metrics(OptimizerContext* ctx);
bool optimizer_should_inline(OptimizerContext* ctx, IRFunction* function);
int optimizer_estimate_speedup(OptimizerContext* ctx);

// Configuration presets
OptimizerConfig optimizer_config_speed(void);      // Maximum speed
OptimizerConfig optimizer_config_size(void);       // Minimum size
OptimizerConfig optimizer_config_balanced(void);   // Speed/size balance
OptimizerConfig optimizer_config_ultra(void);      // Faster than C

#endif // GPLANG_OPTIMIZER_H
