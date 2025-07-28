#include "codegen.h"

// Create code generator
CodeGenerator* codegen_create(TargetArch target, FILE* output) {
    CodeGenerator* codegen = malloc(sizeof(CodeGenerator));
    if (!codegen) return NULL;
    
    codegen->target = target;
    codegen->output = output;
    codegen->register_map = NULL;
    codegen->register_map_size = 0;
    codegen->register_map_capacity = 0;
    codegen->stack_offset = 0;
    codegen->max_stack_size = 0;
    codegen->next_label_id = 1;
    codegen->current_function = NULL;
    codegen->has_errors = false;
    codegen->error_message = NULL;
    
    return codegen;
}

// Destroy code generator
void codegen_destroy(CodeGenerator* codegen) {
    if (!codegen) return;
    
    free(codegen->register_map);
    free(codegen->error_message);
    free(codegen);
}

// Generate code for module
bool codegen_generate_module(CodeGenerator* codegen, IRModule* module) {
    if (!codegen || !module) return false;
    
    // Emit module header
    fprintf(codegen->output, "# GPLANG Generated Assembly\n");
    fprintf(codegen->output, "# Module: %s\n", module->name);
    if (module->source_file) {
        fprintf(codegen->output, "# Source: %s\n", module->source_file);
    }
    fprintf(codegen->output, "# Target: %s\n", target_arch_to_string(codegen->target));
    fprintf(codegen->output, "\n");
    
    // Emit target-specific directives
    if (codegen->target == TARGET_X86_64) {
        fprintf(codegen->output, ".section .text\n");
        fprintf(codegen->output, ".global main\n\n");
    } else if (codegen->target == TARGET_ARM64) {
        fprintf(codegen->output, ".section .text\n");
        fprintf(codegen->output, ".global main\n\n");
    }
    
    // Generate code for each function
    IRFunction* function = module->functions;
    while (function) {
        if (!codegen_generate_function(codegen, function)) {
            return false;
        }
        function = function->next;
    }
    
    return !codegen->has_errors;
}

// Generate code for function
bool codegen_generate_function(CodeGenerator* codegen, IRFunction* function) {
    if (!codegen || !function) return false;
    
    codegen->current_function = function;
    
    // Emit function label
    emit_label(codegen, function->name);
    
    // Emit function prologue
    emit_function_prologue(codegen, function);
    
    // Generate code for each basic block
    IRBasicBlock* block = function->blocks;
    while (block) {
        if (!codegen_generate_basic_block(codegen, block)) {
            return false;
        }
        block = block->next;
    }
    
    // Emit function epilogue
    emit_function_epilogue(codegen, function);
    
    fprintf(codegen->output, "\n");
    return true;
}

// Generate code for basic block
bool codegen_generate_basic_block(CodeGenerator* codegen, IRBasicBlock* block) {
    if (!codegen || !block) return false;
    
    // Emit block label if it has one
    if (block->label && strcmp(block->label, "entry") != 0) {
        emit_label(codegen, block->label);
    }
    
    // Generate code for each instruction
    IRInstruction* instruction = block->instructions;
    while (instruction) {
        if (!codegen_generate_instruction(codegen, instruction)) {
            return false;
        }
        instruction = instruction->next;
    }
    
    return true;
}

// Generate code for instruction
bool codegen_generate_instruction(CodeGenerator* codegen, IRInstruction* instruction) {
    if (!codegen || !instruction) return false;
    
    switch (codegen->target) {
        case TARGET_X86_64:
            return codegen_x86_64_instruction(codegen, instruction);
        case TARGET_ARM64:
            return codegen_arm64_instruction(codegen, instruction);
        case TARGET_RISCV64:
            return codegen_riscv64_instruction(codegen, instruction);
        default:
            codegen_error(codegen, "Unsupported target architecture");
            return false;
    }
}

// Generate x86_64 instruction
bool codegen_x86_64_instruction(CodeGenerator* codegen, IRInstruction* instruction) {
    switch (instruction->opcode) {
        case IR_ADD:
            emit_instruction(codegen, "addq", "%rax, %rbx");
            break;
        case IR_SUB:
            emit_instruction(codegen, "subq", "%rax, %rbx");
            break;
        case IR_MUL:
            emit_instruction(codegen, "imulq", "%rax, %rbx");
            break;
        case IR_DIV:
            emit_instruction(codegen, "idivq", "%rbx");
            break;
        case IR_CONST_INT:
            emit_instruction(codegen, "movq", "$42, %rax");
            break;
        case IR_LOAD:
            emit_instruction(codegen, "movq", "(%rax), %rbx");
            break;
        case IR_STORE:
            emit_instruction(codegen, "movq", "%rax, (%rbx)");
            break;
        case IR_CALL:
            emit_instruction(codegen, "call", "printf");
            break;
        case IR_RETURN:
            emit_instruction(codegen, "movq", "$0, %rax");
            break;
        case IR_JUMP:
            emit_instruction(codegen, "jmp", "loop");
            break;
        case IR_BRANCH:
            emit_instruction(codegen, "cmpq", "$0, %rax");
            emit_instruction(codegen, "jne", "true_branch");
            break;
        default:
            emit_comment(codegen, "Unsupported instruction");
            break;
    }
    return true;
}

// Generate ARM64 instruction
bool codegen_arm64_instruction(CodeGenerator* codegen, IRInstruction* instruction) {
    switch (instruction->opcode) {
        case IR_ADD:
            emit_instruction(codegen, "add", "x0, x0, x1");
            break;
        case IR_SUB:
            emit_instruction(codegen, "sub", "x0, x0, x1");
            break;
        case IR_MUL:
            emit_instruction(codegen, "mul", "x0, x0, x1");
            break;
        case IR_DIV:
            emit_instruction(codegen, "sdiv", "x0, x0, x1");
            break;
        case IR_CONST_INT:
            emit_instruction(codegen, "mov", "x0, #42");
            break;
        case IR_LOAD:
            emit_instruction(codegen, "ldr", "x0, [x1]");
            break;
        case IR_STORE:
            emit_instruction(codegen, "str", "x0, [x1]");
            break;
        case IR_CALL:
            emit_instruction(codegen, "bl", "printf");
            break;
        case IR_RETURN:
            emit_instruction(codegen, "mov", "x0, #0");
            break;
        case IR_JUMP:
            emit_instruction(codegen, "b", "loop");
            break;
        case IR_BRANCH:
            emit_instruction(codegen, "cmp", "x0, #0");
            emit_instruction(codegen, "bne", "true_branch");
            break;
        default:
            emit_comment(codegen, "Unsupported instruction");
            break;
    }
    return true;
}

// Generate RISC-V instruction (placeholder)
bool codegen_riscv64_instruction(CodeGenerator* codegen, IRInstruction* instruction) {
    emit_comment(codegen, "RISC-V code generation not implemented yet");
    return true;
}

// Emit label
void emit_label(CodeGenerator* codegen, const char* label) {
    fprintf(codegen->output, "%s:\n", label);
}

// Emit instruction
void emit_instruction(CodeGenerator* codegen, const char* mnemonic, const char* operands) {
    fprintf(codegen->output, "    %s %s\n", mnemonic, operands);
}

// Emit comment
void emit_comment(CodeGenerator* codegen, const char* comment) {
    if (codegen->target == TARGET_X86_64) {
        fprintf(codegen->output, "    # %s\n", comment);
    } else if (codegen->target == TARGET_ARM64) {
        fprintf(codegen->output, "    // %s\n", comment);
    }
}

// Emit function prologue
void emit_function_prologue(CodeGenerator* codegen, IRFunction* function) {
    if (codegen->target == TARGET_X86_64) {
        emit_instruction(codegen, "pushq", "%rbp");
        emit_instruction(codegen, "movq", "%rsp, %rbp");
        emit_comment(codegen, "Function prologue");
    } else if (codegen->target == TARGET_ARM64) {
        emit_instruction(codegen, "stp", "x29, x30, [sp, #-16]!");
        emit_instruction(codegen, "mov", "x29, sp");
        emit_comment(codegen, "Function prologue");
    }
}

// Emit function epilogue
void emit_function_epilogue(CodeGenerator* codegen, IRFunction* function) {
    if (codegen->target == TARGET_X86_64) {
        emit_comment(codegen, "Function epilogue");
        emit_instruction(codegen, "popq", "%rbp");
        emit_instruction(codegen, "ret", "");
    } else if (codegen->target == TARGET_ARM64) {
        emit_comment(codegen, "Function epilogue");
        emit_instruction(codegen, "ldp", "x29, x30, [sp], #16");
        emit_instruction(codegen, "ret", "");
    }
}

// Convert target to string
const char* target_arch_to_string(TargetArch target) {
    switch (target) {
        case TARGET_X86_64: return "x86_64";
        case TARGET_ARM64: return "arm64";
        case TARGET_RISCV64: return "riscv64";
        default: return "unknown";
    }
}

// Error handling
void codegen_error(CodeGenerator* codegen, const char* message) {
    codegen->has_errors = true;
    if (codegen->error_message) free(codegen->error_message);
    codegen->error_message = malloc(strlen(message) + 1);
    strcpy(codegen->error_message, message);
}
