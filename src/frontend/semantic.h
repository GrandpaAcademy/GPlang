/*
 * GPLANG Semantic Analyzer Header
 * Type checking, scope resolution, and semantic validation
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"
#include <stddef.h>
#include <stdarg.h>

// Type system
typedef enum {
    TYPE_VOID,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_FLOAT32,
    TYPE_FLOAT64,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_POINTER,
    TYPE_FUNCTION,
    TYPE_STRUCT,
    TYPE_ENUM,
    TYPE_OPTION,
    TYPE_RESULT,
    TYPE_VEC2,
    TYPE_VEC3,
    TYPE_VEC4,
    TYPE_PTR
} type_kind_t;

typedef struct type {
    type_kind_t kind;
    size_t size;
    size_t alignment;
    
    union {
        struct {
            struct type* element_type;
            size_t length;
        } array;
        
        struct {
            struct type* target_type;
        } pointer;
        
        struct {
            struct type** param_types;
            size_t param_count;
            struct type* return_type;
        } function;
        
        struct {
            char* name;
            struct symbol** fields;
            size_t field_count;
        } struct_type;
        
        struct {
            struct type* inner_type;
        } option;
        
        struct {
            struct type* ok_type;
            struct type* err_type;
        } result;
    } data;
} type_t;

// Symbol types
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_TYPE,
    SYMBOL_CONSTANT
} symbol_kind_t;

// Symbol structure
typedef struct symbol {
    char* name;
    symbol_kind_t kind;
    type_t* type;
    int is_const;
    int is_mutable;
    int is_borrowed;
    int ownership_id;
    struct symbol* next; // For hash table chaining
} symbol_t;

// Symbol table
typedef struct symbol_table {
    symbol_t** symbols;
    size_t capacity;
    size_t count;
    struct symbol_table* parent;
} symbol_table_t;

// Type system
typedef struct {
    type_t** types;
    size_t type_count;
    size_t type_capacity;
} type_system_t;

// Semantic error
typedef struct {
    char message[256];
    size_t line;
    size_t column;
} semantic_error_t;

// Use error_list_t from parser.h

// Function declarations
int semantic_init(void);
int semantic_analyze(ast_node_t* root);
void semantic_cleanup(void);

// Analysis functions
static void analyze_node(ast_node_t* node);
static void analyze_program(ast_node_t* node);
static void analyze_function(ast_node_t* node);
static void analyze_variable_declaration(ast_node_t* node);
static void analyze_binary_operation(ast_node_t* node);
static void analyze_unary_operation(ast_node_t* node);
static void analyze_function_call(ast_node_t* node);
static void analyze_if_statement(ast_node_t* node);
static void analyze_for_statement(ast_node_t* node);
static void analyze_while_statement(ast_node_t* node);
static void analyze_match_statement(ast_node_t* node);
static void analyze_return_statement(ast_node_t* node);
static void analyze_block(ast_node_t* node);
static void analyze_identifier(ast_node_t* node);

// Symbol table functions
symbol_table_t* create_symbol_table(symbol_table_t* parent);
void free_symbol_table(symbol_table_t* table);
symbol_t* create_symbol(const char* name, symbol_kind_t kind);
void add_symbol(symbol_table_t* table, symbol_t* symbol);
symbol_t* lookup_symbol(symbol_table_t* table, const char* name);
symbol_t* lookup_symbol_local(symbol_table_t* table, const char* name);

// Type system functions
void init_type_system(type_system_t* ts);
void cleanup_type_system(type_system_t* ts);
type_t* create_type(type_kind_t kind);
type_t* create_function_type(void);
type_t* get_void_type(void);
type_t* resolve_type(ast_node_t* type_node);
type_t* get_expression_type(ast_node_t* expr);
void set_expression_type(ast_node_t* expr, type_t* type);
int types_compatible(type_t* t1, type_t* t2);
int is_binary_operator_valid(TokenType op, type_t* left, type_t* right);
type_t* get_binary_result_type(TokenType op, type_t* left, type_t* right);

// Built-in types and functions
static void add_builtin_types(void);
static void add_builtin_functions(void);
static void add_builtin_type(const char* name, type_kind_t kind);
static void add_builtin_function(const char* name, type_kind_t return_type, type_kind_t param_type);

// Ownership and borrowing
void check_ownership_rules(ast_node_t* node, symbol_t* symbol);

// Error handling
static void add_semantic_error(const char* format, ...);
static void print_semantic_errors(void);

#endif // SEMANTIC_H
