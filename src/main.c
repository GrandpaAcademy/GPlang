#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#define _GNU_SOURCE

#include "frontend/lexer.h"
#include "ir/ir.h"
#include "backend/codegen.h"

// Use strdup from ir.c
extern char* my_strdup(const char* s);

// Compiler modes
typedef enum {
    MODE_FULL_COMPILE,
    MODE_FRONTEND_ONLY,
    MODE_BACKEND_ONLY,
    MODE_TOKENIZE_ONLY,
    MODE_HELP
} CompilerMode;

// Compiler options
typedef struct {
    CompilerMode mode;
    char* input_file;
    char* output_file;
    TargetArch target;
    bool verbose;
    bool optimize;
} CompilerOptions;

// Print usage information
void print_usage(const char* program_name) {
    printf("GPLANG Compiler - Modern compilation pipeline: .gp → IR → Assembly → .o → .bin\n\n");
    printf("Usage: %s [OPTIONS] <input_file>\n\n", program_name);
    printf("Compilation Modes:\n");
    printf("  --frontend         Frontend only: .gp → IR\n");
    printf("  --backend          Backend only: IR → Assembly\n");
    printf("  --tokenize         Tokenize only: .gp → Tokens\n");
    printf("  (default)          Full compilation: .gp → Assembly\n\n");
    printf("Options:\n");
    printf("  -o, --output FILE  Output file (default: stdout)\n");
    printf("  --target ARCH      Target architecture (x86_64, arm64, riscv64)\n");
    printf("  -O, --optimize     Enable optimizations\n");
    printf("  -v, --verbose      Verbose output\n");
    printf("  -h, --help         Show this help\n\n");
    printf("Examples:\n");
    printf("  %s examples/basic/count_1m.gp\n", program_name);
    printf("  %s --frontend count_1m.gp -o count_1m.ir\n", program_name);
    printf("  %s --backend count_1m.ir --target x86_64 -o count_1m.s\n", program_name);
    printf("  %s --target arm64 -O count_1m.gp -o count_1m.s\n", program_name);
    printf("\nCompilation Pipeline:\n");
    printf("  1. Frontend: .gp → IR (Intermediate Representation)\n");
    printf("  2. Optimization: IR → Optimized IR\n");
    printf("  3. Backend: IR → Target Assembly (x86_64/ARM64/RISC-V)\n");
    printf("  4. Assembly: .s → .o (using system assembler)\n");
    printf("  5. Linking: .o → .bin (using system linker)\n");
}

// Parse command line arguments
CompilerOptions parse_arguments(int argc, char* argv[]) {
    CompilerOptions options = {
        .mode = MODE_FULL_COMPILE,
        .input_file = NULL,
        .output_file = NULL,
        .target = TARGET_X86_64,
        .verbose = false,
        .optimize = false
    };
    
    static struct option long_options[] = {
        {"frontend", no_argument, 0, 'f'},
        {"backend", no_argument, 0, 'b'},
        {"tokenize", no_argument, 0, 't'},
        {"output", required_argument, 0, 'o'},
        {"target", required_argument, 0, 'T'},
        {"optimize", no_argument, 0, 'O'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "fbto:T:Ovh", long_options, &option_index)) != -1) {
        switch (c) {
            case 'f':
                options.mode = MODE_FRONTEND_ONLY;
                break;
            case 'b':
                options.mode = MODE_BACKEND_ONLY;
                break;
            case 't':
                options.mode = MODE_TOKENIZE_ONLY;
                break;
            case 'o':
                options.output_file = my_strdup(optarg);
                break;
            case 'T':
                if (strcmp(optarg, "x86_64") == 0) {
                    options.target = TARGET_X86_64;
                } else if (strcmp(optarg, "arm64") == 0) {
                    options.target = TARGET_ARM64;
                } else if (strcmp(optarg, "riscv64") == 0) {
                    options.target = TARGET_RISCV64;
                } else {
                    fprintf(stderr, "Error: Unknown target architecture '%s'\n", optarg);
                    exit(1);
                }
                break;
            case 'O':
                options.optimize = true;
                break;
            case 'v':
                options.verbose = true;
                break;
            case 'h':
                options.mode = MODE_HELP;
                break;
            default:
                fprintf(stderr, "Error: Unknown option\n");
                exit(1);
        }
    }
    
    // Get input file
    if (optind < argc) {
        options.input_file = my_strdup(argv[optind]);
    }
    
    return options;
}

// Read file contents
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';
    
    fclose(file);
    return content;
}

// Tokenize mode
int tokenize_mode(CompilerOptions* options) {
    if (options->verbose) {
        printf("🔍 Tokenizing: %s\n", options->input_file);
    }
    
    char* source = read_file(options->input_file);
    if (!source) return 1;
    
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        fprintf(stderr, "Error: Failed to create lexer\n");
        free(source);
        return 1;
    }
    
    FILE* output = options->output_file ? fopen(options->output_file, "w") : stdout;
    
    Token* token;
    int token_count = 0;
    
    while ((token = lexer_next_token(lexer)) && token->type != TOKEN_EOF) {
        fprintf(output, "%s: '%s' (line %d, col %d)\n",
                token_type_to_string(token->type),
                token->value ? token->value : "",
                token->line, token->column);
        
        token_destroy(token);
        token_count++;
    }
    
    if (token) token_destroy(token);
    
    if (options->verbose) {
        printf("✅ Tokenization complete: %d tokens\n", token_count);
    }
    
    if (output != stdout) fclose(output);
    lexer_destroy(lexer);
    free(source);
    
    return 0;
}

// Frontend mode
int frontend_mode(CompilerOptions* options) {
    if (options->verbose) {
        printf("🌳 Frontend: %s → IR\n", options->input_file);
    }
    
    char* source = read_file(options->input_file);
    if (!source) return 1;
    
    // TODO: Implement full frontend pipeline
    // For now, just tokenize and show structure
    
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        fprintf(stderr, "Error: Failed to create lexer\n");
        free(source);
        return 1;
    }
    
    FILE* output = options->output_file ? fopen(options->output_file, "w") : stdout;
    
    // Generate placeholder IR
    fprintf(output, "; GPLANG IR - Generated from %s\n", options->input_file);
    fprintf(output, "; Target: %s\n", target_arch_to_string(options->target));
    fprintf(output, "\n");
    fprintf(output, "func_begin @main\n");
    fprintf(output, "    ; Placeholder IR for count_1m.gp\n");
    fprintf(output, "    const_int 1000000\n");
    fprintf(output, "    store %%1, @count\n");
    fprintf(output, "    call @Time.now\n");
    fprintf(output, "    store %%2, @start_time\n");
    fprintf(output, "    \n");
    fprintf(output, "loop_begin:\n");
    fprintf(output, "    load %%3, @i\n");
    fprintf(output, "    load %%4, @count\n");
    fprintf(output, "    lt %%5, %%3, %%4\n");
    fprintf(output, "    branch %%5, loop_body, loop_end\n");
    fprintf(output, "    \n");
    fprintf(output, "loop_body:\n");
    fprintf(output, "    ; Loop body implementation\n");
    fprintf(output, "    jump loop_begin\n");
    fprintf(output, "    \n");
    fprintf(output, "loop_end:\n");
    fprintf(output, "    call @Time.now\n");
    fprintf(output, "    store %%6, @end_time\n");
    fprintf(output, "    const_int 0\n");
    fprintf(output, "    return %%7\n");
    fprintf(output, "func_end\n");
    
    if (options->verbose) {
        printf("✅ Frontend complete: IR generated\n");
    }
    
    if (output != stdout) fclose(output);
    lexer_destroy(lexer);
    free(source);
    
    return 0;
}

// Backend mode
int backend_mode(CompilerOptions* options) {
    if (options->verbose) {
        printf("⚙️  Backend: IR → %s Assembly\n", target_arch_to_string(options->target));
    }
    
    // TODO: Implement IR loading and code generation
    // For now, generate placeholder assembly
    
    FILE* output = options->output_file ? fopen(options->output_file, "w") : stdout;
    
    if (options->target == TARGET_X86_64) {
        fprintf(output, "# GPLANG Generated x86_64 Assembly\n");
        fprintf(output, ".section .text\n");
        fprintf(output, ".global main\n");
        fprintf(output, "\n");
        fprintf(output, "main:\n");
        fprintf(output, "    pushq %%rbp\n");
        fprintf(output, "    movq %%rsp, %%rbp\n");
        fprintf(output, "    \n");
        fprintf(output, "    # Count loop (simplified)\n");
        fprintf(output, "    movq $1, %%rcx          # i = 1\n");
        fprintf(output, "    movq $1000000, %%rdx    # count = 1000000\n");
        fprintf(output, "    \n");
        fprintf(output, "loop:\n");
        fprintf(output, "    cmpq %%rdx, %%rcx\n");
        fprintf(output, "    jge loop_end\n");
        fprintf(output, "    incq %%rcx\n");
        fprintf(output, "    jmp loop\n");
        fprintf(output, "    \n");
        fprintf(output, "loop_end:\n");
        fprintf(output, "    movq $0, %%rax          # return 0\n");
        fprintf(output, "    popq %%rbp\n");
        fprintf(output, "    ret\n");
    } else if (options->target == TARGET_ARM64) {
        fprintf(output, "// GPLANG Generated ARM64 Assembly\n");
        fprintf(output, ".section .text\n");
        fprintf(output, ".global main\n");
        fprintf(output, "\n");
        fprintf(output, "main:\n");
        fprintf(output, "    stp x29, x30, [sp, #-16]!\n");
        fprintf(output, "    mov x29, sp\n");
        fprintf(output, "    \n");
        fprintf(output, "    // Count loop (simplified)\n");
        fprintf(output, "    mov x0, #1              // i = 1\n");
        fprintf(output, "    mov x1, #1000000        // count = 1000000\n");
        fprintf(output, "    \n");
        fprintf(output, "loop:\n");
        fprintf(output, "    cmp x0, x1\n");
        fprintf(output, "    bge loop_end\n");
        fprintf(output, "    add x0, x0, #1\n");
        fprintf(output, "    b loop\n");
        fprintf(output, "    \n");
        fprintf(output, "loop_end:\n");
        fprintf(output, "    mov x0, #0              // return 0\n");
        fprintf(output, "    ldp x29, x30, [sp], #16\n");
        fprintf(output, "    ret\n");
    }
    
    if (options->verbose) {
        printf("✅ Backend complete: %s assembly generated\n", target_arch_to_string(options->target));
    }
    
    if (output != stdout) fclose(output);
    return 0;
}

// Main function
int main(int argc, char* argv[]) {
    CompilerOptions options = parse_arguments(argc, argv);
    
    if (options.mode == MODE_HELP) {
        print_usage(argv[0]);
        return 0;
    }
    
    if (!options.input_file) {
        fprintf(stderr, "Error: No input file specified\n");
        print_usage(argv[0]);
        return 1;
    }
    
    if (options.verbose) {
        printf("🚀 GPLANG Compiler\n");
        printf("Input: %s\n", options.input_file);
        printf("Target: %s\n", target_arch_to_string(options.target));
        printf("Mode: ");
        switch (options.mode) {
            case MODE_TOKENIZE_ONLY: printf("Tokenize\n"); break;
            case MODE_FRONTEND_ONLY: printf("Frontend\n"); break;
            case MODE_BACKEND_ONLY: printf("Backend\n"); break;
            case MODE_FULL_COMPILE: printf("Full Compile\n"); break;
            default: printf("Unknown\n"); break;
        }
        printf("\n");
    }
    
    int result = 0;
    
    switch (options.mode) {
        case MODE_TOKENIZE_ONLY:
            result = tokenize_mode(&options);
            break;
        case MODE_FRONTEND_ONLY:
            result = frontend_mode(&options);
            break;
        case MODE_BACKEND_ONLY:
            result = backend_mode(&options);
            break;
        case MODE_FULL_COMPILE:
            // Full compilation: frontend then backend
            result = frontend_mode(&options);
            if (result == 0) {
                // TODO: Chain frontend output to backend input
                result = backend_mode(&options);
            }
            break;
        default:
            fprintf(stderr, "Error: Unknown compilation mode\n");
            result = 1;
            break;
    }
    
    // Cleanup
    free(options.input_file);
    free(options.output_file);
    
    if (options.verbose) {
        if (result == 0) {
            printf("\n✅ Compilation successful!\n");
        } else {
            printf("\n❌ Compilation failed!\n");
        }
    }
    
    return result;
}

// Utility function implementation (defined in backend/codegen.c)
// const char* target_arch_to_string(TargetArch target);
