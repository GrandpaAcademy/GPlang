#ifndef GPLANG_IR_H
#define GPLANG_IR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// IR Instruction Types
typedef enum {
    // Arithmetic operations
    IR_ADD, IR_SUB, IR_MUL, IR_DIV, IR_MOD,
    
    // Comparison operations
    IR_EQ, IR_NE, IR_LT, IR_LE, IR_GT, IR_GE,
    
    // Logical operations
    IR_AND, IR_OR, IR_NOT,
    
    // Memory operations
    IR_LOAD, IR_STORE, IR_ALLOCA,
    
    // Control flow
    IR_JUMP, IR_BRANCH, IR_CALL, IR_RETURN,
    
    // Function operations
    IR_FUNC_BEGIN, IR_FUNC_END,
    
    // Constants
    IR_CONST_INT, IR_CONST_FLOAT, IR_CONST_STRING,
    
    // Variables
    IR_VAR_DECL, IR_VAR_ASSIGN,
    
    // I/O operations
    IR_PRINT, IR_READ,
    
    // Loop operations
    IR_LOOP_BEGIN, IR_LOOP_END, IR_LOOP_CONTINUE, IR_LOOP_BREAK,
    
    // Async operations
    IR_ASYNC_CALL, IR_AWAIT, IR_SPAWN,
    
    // Type operations
    IR_CAST, IR_TYPEOF,
    
    // Special
    IR_NOP, IR_LABEL, IR_PHI
} IROpcode;

// IR Value types
typedef enum {
    IR_VALUE_REGISTER,
    IR_VALUE_CONSTANT,
    IR_VALUE_LABEL,
    IR_VALUE_GLOBAL
} IRValueType;

// IR Value
typedef struct IRValue {
    IRValueType type;
    union {
        int reg_id;
        struct {
            enum { IR_CONST_INT_VAL, IR_CONST_FLOAT_VAL, IR_CONST_STRING_VAL } const_type;
            union {
                long long int_val;
                double float_val;
                char* string_val;
            };
        } constant;
        char* label;
        char* global_name;
    };
} IRValue;

// IR Instruction
typedef struct IRInstruction {
    IROpcode opcode;
    IRValue* dest;      // Destination (can be NULL)
    IRValue* src1;      // First source operand
    IRValue* src2;      // Second source operand (can be NULL)
    IRValue* src3;      // Third source operand (can be NULL)
    
    // Metadata
    int line_number;
    char* comment;
    
    struct IRInstruction* next;
} IRInstruction;

// IR Basic Block
typedef struct IRBasicBlock {
    char* label;
    IRInstruction* instructions;
    IRInstruction* last_instruction;
    
    // Control flow
    struct IRBasicBlock** successors;
    int successor_count;
    struct IRBasicBlock** predecessors;
    int predecessor_count;
    
    struct IRBasicBlock* next;
} IRBasicBlock;

// IR Function
typedef struct IRFunction {
    char* name;
    IRValue** parameters;
    int parameter_count;
    IRValue* return_type;
    
    IRBasicBlock* entry_block;
    IRBasicBlock* blocks;
    
    // Register allocation
    int next_register_id;
    
    struct IRFunction* next;
} IRFunction;

// IR Module (compilation unit)
typedef struct IRModule {
    char* name;
    IRFunction* functions;
    
    // Global variables
    IRValue** globals;
    int global_count;
    
    // String constants
    char** string_constants;
    int string_constant_count;
    
    // Metadata
    char* source_file;
    char* target_triple;
} IRModule;

// IR Builder (for constructing IR)
typedef struct IRBuilder {
    IRModule* module;
    IRFunction* current_function;
    IRBasicBlock* current_block;
    
    // Symbol table
    struct {
        char* name;
        IRValue* value;
    }* symbols;
    int symbol_count;
    int symbol_capacity;
} IRBuilder;

// Function declarations

// Module management
IRModule* ir_module_create(const char* name);
void ir_module_destroy(IRModule* module);
void ir_module_print(IRModule* module, FILE* output);
bool ir_module_save(IRModule* module, const char* filename);
IRModule* ir_module_load(const char* filename);

// Function management
IRFunction* ir_function_create(const char* name);
void ir_function_destroy(IRFunction* function);
void ir_function_add_parameter(IRFunction* function, IRValue* param);

// Basic block management
IRBasicBlock* ir_basic_block_create(const char* label);
void ir_basic_block_destroy(IRBasicBlock* block);
void ir_basic_block_add_instruction(IRBasicBlock* block, IRInstruction* instruction);

// Instruction creation
IRInstruction* ir_instruction_create(IROpcode opcode);
void ir_instruction_destroy(IRInstruction* instruction);
void ir_instruction_set_dest(IRInstruction* instruction, IRValue* dest);
void ir_instruction_set_src(IRInstruction* instruction, int index, IRValue* src);

// Value creation
IRValue* ir_value_create_register(int reg_id);
IRValue* ir_value_create_constant_int(long long value);
IRValue* ir_value_create_constant_float(double value);
IRValue* ir_value_create_constant_string(const char* value);
IRValue* ir_value_create_label(const char* label);
IRValue* ir_value_create_global(const char* name);
void ir_value_destroy(IRValue* value);

// Builder functions
IRBuilder* ir_builder_create(IRModule* module);
void ir_builder_destroy(IRBuilder* builder);
void ir_builder_set_function(IRBuilder* builder, IRFunction* function);
void ir_builder_set_block(IRBuilder* builder, IRBasicBlock* block);

// High-level instruction building
IRValue* ir_builder_add(IRBuilder* builder, IRValue* lhs, IRValue* rhs);
IRValue* ir_builder_sub(IRBuilder* builder, IRValue* lhs, IRValue* rhs);
IRValue* ir_builder_mul(IRBuilder* builder, IRValue* lhs, IRValue* rhs);
IRValue* ir_builder_div(IRBuilder* builder, IRValue* lhs, IRValue* rhs);
IRValue* ir_builder_load(IRBuilder* builder, IRValue* ptr);
void ir_builder_store(IRBuilder* builder, IRValue* value, IRValue* ptr);
void ir_builder_call(IRBuilder* builder, const char* function_name, IRValue** args, int arg_count);
void ir_builder_return(IRBuilder* builder, IRValue* value);
void ir_builder_print(IRBuilder* builder, IRValue* value);

// Symbol table
void ir_builder_add_symbol(IRBuilder* builder, const char* name, IRValue* value);
IRValue* ir_builder_get_symbol(IRBuilder* builder, const char* name);

// Utility functions
const char* ir_opcode_to_string(IROpcode opcode);
void ir_value_print(IRValue* value, FILE* output);
void ir_instruction_print(IRInstruction* instruction, FILE* output);

// Optimization passes
void ir_optimize_module(IRModule* module);
void ir_optimize_function(IRFunction* function);
void ir_dead_code_elimination(IRFunction* function);
void ir_constant_folding(IRFunction* function);

#endif // GPLANG_IR_H
