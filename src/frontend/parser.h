/*
 * GPLANG Parser Header
 * Recursive descent parser for building AST
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stddef.h>

// AST node types
typedef enum {
    AST_PROGRAM,
    AST_FUNCTION,
    AST_VARIABLE,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_CALL,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_MATCH,
    AST_RETURN,
    AST_BLOCK,
    AST_UNSAFE_BLOCK,
    AST_LITERAL,
    AST_IDENTIFIER,
    AST_NUMBER,
    AST_STRING,
    AST_BOOLEAN,
    AST_ARRAY,
    AST_OBJECT,
    AST_IMPORT,
    AST_EXPRESSION_STMT
} ast_node_type_t;

// AST node structure
typedef struct ast_node {
    ast_node_type_t type;
    struct ast_node** children;
    size_t child_count;
    size_t child_capacity;
    
    union {
        struct {
            char* name;
            struct ast_node* parameters;
            struct ast_node* return_type;
            struct ast_node* body;
        } function;
        
        struct {
            char* name;
            struct ast_node* type;
            struct ast_node* value;
            int is_const;
        } variable;
        
        struct {
            TokenType operator;
            struct ast_node* left;
            struct ast_node* right;
        } binary_op;

        struct {
            TokenType operator;
            struct ast_node* operand;
        } unary_op;
        
        struct {
            struct ast_node* condition;
            struct ast_node* then_block;
            struct ast_node* else_block;
        } if_stmt;
        
        struct {
            char* variable;
            struct ast_node* iterable;
            struct ast_node* body;
            int is_parallel;
        } for_stmt;
        
        struct {
            struct ast_node* condition;
            struct ast_node* body;
        } while_stmt;
        
        struct {
            struct ast_node* expression;
            struct ast_node** cases;
            size_t case_count;
        } match_stmt;
        
        struct {
            struct ast_node* expression;
        } return_stmt;
        
        struct {
            char* value;
        } literal;
        
        struct {
            char* name;
        } identifier;

        struct {
            struct ast_node** statements;
            size_t statement_count;
        } block;

        struct {
            struct ast_node** statements;
            size_t statement_count;
        } unsafe_block;
    } data;
} ast_node_t;

// Parse error structure
typedef struct {
    const char* message;
    size_t line;
    size_t column;
} parse_error_t;

// Error list
typedef struct {
    parse_error_t* errors;
    size_t count;
    size_t capacity;
} error_list_t;

// Function declarations
int parser_init(void);
ast_node_t* parse(Token* tokens, size_t token_count);
void parser_cleanup(void);

// AST utility functions
ast_node_t* create_ast_node(ast_node_type_t type);
ast_node_t* create_number_node(const char* value);
ast_node_t* create_string_node(const char* value);
ast_node_t* create_identifier_node(const char* name);
ast_node_t* create_boolean_node(int value);
void add_child(ast_node_t* parent, ast_node_t* child);
void free_ast_node(ast_node_t* node);
void print_ast(ast_node_t* node, int depth);
size_t count_ast_nodes(ast_node_t* node);

// Parser helper functions
ast_node_t* parse_parameter_list(void);
ast_node_t* parse_type(void);
ast_node_t* parse_return_statement(void);
ast_node_t* parse_variable_declaration(void);
ast_node_t* parse_import_statement(void);
ast_node_t* parse_expression_statement(void);
ast_node_t* parse_match_case(void);
ast_node_t* parse_array_literal(void);
ast_node_t* parse_object_literal(void);

#endif // PARSER_H
