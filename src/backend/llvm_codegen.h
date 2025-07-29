/*
 * GPLANG LLVM IR Code Generator Header
 * Translates AST to LLVM IR for high performance execution
 */

#ifndef LLVM_CODEGEN_H
#define LLVM_CODEGEN_H

#include "../frontend/parser.h"

// Function declarations
int llvm_codegen_init(const char* output_filename);
int llvm_generate_code(ast_node_t* root);
void llvm_codegen_cleanup(void);

#endif // LLVM_CODEGEN_H
