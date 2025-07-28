#include "ir.h"

// Simple strdup implementation
char* my_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) strcpy(copy, s);
    return copy;
}

// Create IR module
IRModule* ir_module_create(const char* name) {
    IRModule* module = malloc(sizeof(IRModule));
    if (!module) return NULL;
    
    module->name = my_strdup(name);
    module->functions = NULL;
    module->globals = NULL;
    module->global_count = 0;
    module->string_constants = NULL;
    module->string_constant_count = 0;
    module->source_file = NULL;
    module->target_triple = NULL;
    
    return module;
}

// Destroy IR module
void ir_module_destroy(IRModule* module) {
    if (!module) return;
    
    free(module->name);
    free(module->source_file);
    free(module->target_triple);
    
    // Free functions
    IRFunction* func = module->functions;
    while (func) {
        IRFunction* next = func->next;
        ir_function_destroy(func);
        func = next;
    }
    
    // Free globals
    for (int i = 0; i < module->global_count; i++) {
        ir_value_destroy(module->globals[i]);
    }
    free(module->globals);
    
    // Free string constants
    for (int i = 0; i < module->string_constant_count; i++) {
        free(module->string_constants[i]);
    }
    free(module->string_constants);
    
    free(module);
}

// Create IR function
IRFunction* ir_function_create(const char* name) {
    IRFunction* function = malloc(sizeof(IRFunction));
    if (!function) return NULL;
    
    function->name = my_strdup(name);
    function->parameters = NULL;
    function->parameter_count = 0;
    function->return_type = NULL;
    function->entry_block = NULL;
    function->blocks = NULL;
    function->next_register_id = 1;
    function->next = NULL;
    
    return function;
}

// Destroy IR function
void ir_function_destroy(IRFunction* function) {
    if (!function) return;
    
    free(function->name);
    
    // Free parameters
    for (int i = 0; i < function->parameter_count; i++) {
        ir_value_destroy(function->parameters[i]);
    }
    free(function->parameters);
    
    if (function->return_type) {
        ir_value_destroy(function->return_type);
    }
    
    // Free basic blocks
    IRBasicBlock* block = function->blocks;
    while (block) {
        IRBasicBlock* next = block->next;
        ir_basic_block_destroy(block);
        block = next;
    }
    
    free(function);
}

// Create basic block
IRBasicBlock* ir_basic_block_create(const char* label) {
    IRBasicBlock* block = malloc(sizeof(IRBasicBlock));
    if (!block) return NULL;
    
    block->label = my_strdup(label);
    block->instructions = NULL;
    block->last_instruction = NULL;
    block->successors = NULL;
    block->successor_count = 0;
    block->predecessors = NULL;
    block->predecessor_count = 0;
    block->next = NULL;
    
    return block;
}

// Destroy basic block
void ir_basic_block_destroy(IRBasicBlock* block) {
    if (!block) return;
    
    free(block->label);
    
    // Free instructions
    IRInstruction* instruction = block->instructions;
    while (instruction) {
        IRInstruction* next = instruction->next;
        ir_instruction_destroy(instruction);
        instruction = next;
    }
    
    free(block->successors);
    free(block->predecessors);
    free(block);
}

// Create instruction
IRInstruction* ir_instruction_create(IROpcode opcode) {
    IRInstruction* instruction = malloc(sizeof(IRInstruction));
    if (!instruction) return NULL;
    
    instruction->opcode = opcode;
    instruction->dest = NULL;
    instruction->src1 = NULL;
    instruction->src2 = NULL;
    instruction->src3 = NULL;
    instruction->line_number = 0;
    instruction->comment = NULL;
    instruction->next = NULL;
    
    return instruction;
}

// Destroy instruction
void ir_instruction_destroy(IRInstruction* instruction) {
    if (!instruction) return;
    
    if (instruction->dest) ir_value_destroy(instruction->dest);
    if (instruction->src1) ir_value_destroy(instruction->src1);
    if (instruction->src2) ir_value_destroy(instruction->src2);
    if (instruction->src3) ir_value_destroy(instruction->src3);
    free(instruction->comment);
    free(instruction);
}

// Create register value
IRValue* ir_value_create_register(int reg_id) {
    IRValue* value = malloc(sizeof(IRValue));
    if (!value) return NULL;
    
    value->type = IR_VALUE_REGISTER;
    value->reg_id = reg_id;
    
    return value;
}

// Create constant integer value
IRValue* ir_value_create_constant_int(long long int_val) {
    IRValue* value = malloc(sizeof(IRValue));
    if (!value) return NULL;
    
    value->type = IR_VALUE_CONSTANT;
    value->constant.const_type = IR_CONST_INT_VAL;
    value->constant.int_val = int_val;
    
    return value;
}

// Create constant string value
IRValue* ir_value_create_constant_string(const char* string_val) {
    IRValue* value = malloc(sizeof(IRValue));
    if (!value) return NULL;
    
    value->type = IR_VALUE_CONSTANT;
    value->constant.const_type = IR_CONST_STRING_VAL;
    value->constant.string_val = my_strdup(string_val);
    
    return value;
}

// Destroy value
void ir_value_destroy(IRValue* value) {
    if (!value) return;
    
    if (value->type == IR_VALUE_CONSTANT && 
        value->constant.const_type == IR_CONST_STRING_VAL) {
        free(value->constant.string_val);
    } else if (value->type == IR_VALUE_LABEL) {
        free(value->label);
    } else if (value->type == IR_VALUE_GLOBAL) {
        free(value->global_name);
    }
    
    free(value);
}

// Convert opcode to string
const char* ir_opcode_to_string(IROpcode opcode) {
    switch (opcode) {
        case IR_ADD: return "add";
        case IR_SUB: return "sub";
        case IR_MUL: return "mul";
        case IR_DIV: return "div";
        case IR_MOD: return "mod";
        case IR_EQ: return "eq";
        case IR_NE: return "ne";
        case IR_LT: return "lt";
        case IR_LE: return "le";
        case IR_GT: return "gt";
        case IR_GE: return "ge";
        case IR_AND: return "and";
        case IR_OR: return "or";
        case IR_NOT: return "not";
        case IR_LOAD: return "load";
        case IR_STORE: return "store";
        case IR_ALLOCA: return "alloca";
        case IR_JUMP: return "jump";
        case IR_BRANCH: return "branch";
        case IR_CALL: return "call";
        case IR_RETURN: return "return";
        case IR_FUNC_BEGIN: return "func_begin";
        case IR_FUNC_END: return "func_end";
        case IR_CONST_INT: return "const_int";
        case IR_CONST_FLOAT: return "const_float";
        case IR_CONST_STRING: return "const_string";
        case IR_VAR_DECL: return "var_decl";
        case IR_VAR_ASSIGN: return "var_assign";
        case IR_PRINT: return "print";
        case IR_READ: return "read";
        case IR_LOOP_BEGIN: return "loop_begin";
        case IR_LOOP_END: return "loop_end";
        case IR_LOOP_CONTINUE: return "loop_continue";
        case IR_LOOP_BREAK: return "loop_break";
        case IR_ASYNC_CALL: return "async_call";
        case IR_AWAIT: return "await";
        case IR_SPAWN: return "spawn";
        case IR_CAST: return "cast";
        case IR_TYPEOF: return "typeof";
        case IR_NOP: return "nop";
        case IR_LABEL: return "label";
        case IR_PHI: return "phi";
        default: return "unknown";
    }
}

// Print IR module
void ir_module_print(IRModule* module, FILE* output) {
    if (!module || !output) return;
    
    fprintf(output, "; GPLANG IR Module: %s\n", module->name);
    if (module->source_file) {
        fprintf(output, "; Source: %s\n", module->source_file);
    }
    if (module->target_triple) {
        fprintf(output, "; Target: %s\n", module->target_triple);
    }
    fprintf(output, "\n");
    
    // Print functions
    IRFunction* function = module->functions;
    while (function) {
        fprintf(output, "func_begin @%s\n", function->name);
        
        IRBasicBlock* block = function->blocks;
        while (block) {
            if (block->label) {
                fprintf(output, "%s:\n", block->label);
            }
            
            IRInstruction* instruction = block->instructions;
            while (instruction) {
                fprintf(output, "    ");
                ir_instruction_print(instruction, output);
                fprintf(output, "\n");
                instruction = instruction->next;
            }
            
            block = block->next;
        }
        
        fprintf(output, "func_end\n\n");
        function = function->next;
    }
}

// Print IR instruction
void ir_instruction_print(IRInstruction* instruction, FILE* output) {
    if (!instruction || !output) return;
    
    fprintf(output, "%s", ir_opcode_to_string(instruction->opcode));
    
    if (instruction->dest) {
        fprintf(output, " ");
        ir_value_print(instruction->dest, output);
    }
    
    if (instruction->src1) {
        fprintf(output, ", ");
        ir_value_print(instruction->src1, output);
    }
    
    if (instruction->src2) {
        fprintf(output, ", ");
        ir_value_print(instruction->src2, output);
    }
    
    if (instruction->comment) {
        fprintf(output, " ; %s", instruction->comment);
    }
}

// Print IR value
void ir_value_print(IRValue* value, FILE* output) {
    if (!value || !output) return;
    
    switch (value->type) {
        case IR_VALUE_REGISTER:
            fprintf(output, "%%%d", value->reg_id);
            break;
        case IR_VALUE_CONSTANT:
            switch (value->constant.const_type) {
                case IR_CONST_INT_VAL:
                    fprintf(output, "%lld", value->constant.int_val);
                    break;
                case IR_CONST_FLOAT_VAL:
                    fprintf(output, "%f", value->constant.float_val);
                    break;
                case IR_CONST_STRING_VAL:
                    fprintf(output, "\"%s\"", value->constant.string_val);
                    break;
            }
            break;
        case IR_VALUE_LABEL:
            fprintf(output, "%s", value->label);
            break;
        case IR_VALUE_GLOBAL:
            fprintf(output, "@%s", value->global_name);
            break;
    }
}
