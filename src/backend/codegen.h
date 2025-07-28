#ifndef GPLANG_CODEGEN_H
#define GPLANG_CODEGEN_H

#include "../ir/ir.h"
#include <stdio.h>

// Target architectures
typedef enum {
    TARGET_X86_64,
    TARGET_ARM64,
    TARGET_RISCV64
} TargetArch;

// Register allocation
typedef struct {
    int virtual_reg;
    int physical_reg;
    bool is_spilled;
    int spill_offset;
} RegisterMapping;

// Code generator
typedef struct CodeGenerator {
    TargetArch target;
    FILE* output;
    
    // Register allocation
    RegisterMapping* register_map;
    int register_map_size;
    int register_map_capacity;
    
    // Stack management
    int stack_offset;
    int max_stack_size;
    
    // Label generation
    int next_label_id;
    
    // Current function context
    IRFunction* current_function;
    
    // Error handling
    bool has_errors;
    char* error_message;
} CodeGenerator;

// Function declarations

// Code generator management
CodeGenerator* codegen_create(TargetArch target, FILE* output);
void codegen_destroy(CodeGenerator* codegen);

// Main code generation
bool codegen_generate_module(CodeGenerator* codegen, IRModule* module);
bool codegen_generate_function(CodeGenerator* codegen, IRFunction* function);
bool codegen_generate_basic_block(CodeGenerator* codegen, IRBasicBlock* block);
bool codegen_generate_instruction(CodeGenerator* codegen, IRInstruction* instruction);

// Target-specific code generation
bool codegen_x86_64_instruction(CodeGenerator* codegen, IRInstruction* instruction);
bool codegen_arm64_instruction(CodeGenerator* codegen, IRInstruction* instruction);
bool codegen_riscv64_instruction(CodeGenerator* codegen, IRInstruction* instruction);

// Register allocation
void codegen_allocate_registers(CodeGenerator* codegen, IRFunction* function);
int codegen_get_physical_register(CodeGenerator* codegen, int virtual_reg);
void codegen_spill_register(CodeGenerator* codegen, int virtual_reg);

// Assembly emission helpers
void emit_label(CodeGenerator* codegen, const char* label);
void emit_instruction(CodeGenerator* codegen, const char* mnemonic, const char* operands);
void emit_comment(CodeGenerator* codegen, const char* comment);
void emit_directive(CodeGenerator* codegen, const char* directive);

// Target-specific helpers
const char* get_register_name_x86_64(int reg_id);
const char* get_register_name_arm64(int reg_id);
const char* get_register_name_riscv64(int reg_id);

// Stack management
int allocate_stack_slot(CodeGenerator* codegen, int size);
void emit_function_prologue(CodeGenerator* codegen, IRFunction* function);
void emit_function_epilogue(CodeGenerator* codegen, IRFunction* function);

// Utility functions
const char* target_arch_to_string(TargetArch target);
void codegen_error(CodeGenerator* codegen, const char* message);

#endif // GPLANG_CODEGEN_H
