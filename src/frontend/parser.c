/*
 * GPLANG Parser - Recursive Descent Parser
 * Builds Abstract Syntax Tree (AST) from tokens
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "lexer.h"

// Parser state
typedef struct {
    Token* tokens;
    size_t token_count;
    size_t current;
    ast_node_t* root;
    error_list_t errors;
} parser_t;

static parser_t g_parser = {0};

// Forward declarations
static ast_node_t* parse_expression(void);
static ast_node_t* parse_statement(void);
static ast_node_t* parse_function(void);
static ast_node_t* parse_block(void);
static ast_node_t* parse_if_statement(void);
static ast_node_t* parse_for_statement(void);
static ast_node_t* parse_while_statement(void);
static ast_node_t* parse_match_statement(void);
static ast_node_t* parse_primary(void);
static ast_node_t* parse_binary(ast_node_t* left, int min_precedence);
static void add_error(const char* message);
static void print_parse_errors(void);
static Token* peek(void);
static Token* advance(void);
static int is_at_end(void);
static int check(TokenType type);
static int match(TokenType type);
static Token* consume(TokenType type, const char* message);
static int get_operator_precedence(TokenType type);
static int is_right_associative(TokenType type);

/*
 * Initialize parser
 */
int parser_init(void) {
    printf("🔧 Initializing GPLANG Parser...\n");
    
    memset(&g_parser, 0, sizeof(parser_t));
    g_parser.errors.capacity = 100;
    g_parser.errors.errors = calloc(g_parser.errors.capacity, sizeof(parse_error_t));
    
    if (!g_parser.errors.errors) {
        printf("❌ Failed to initialize parser error list\n");
        return -1;
    }
    
    printf("✅ Parser initialized\n");
    return 0;
}

/*
 * Parse tokens into AST
 */
ast_node_t* parse(Token* tokens, size_t token_count) {
    printf("🔧 Parsing %zu tokens into AST...\n", token_count);
    
    g_parser.tokens = tokens;
    g_parser.token_count = token_count;
    g_parser.current = 0;
    g_parser.errors.count = 0;
    
    // Create root program node
    g_parser.root = create_ast_node(AST_PROGRAM);
    if (!g_parser.root) {
        printf("❌ Failed to create root AST node\n");
        return NULL;
    }
    
    // Parse top-level declarations
    while (!is_at_end()) {
        ast_node_t* stmt = parse_statement();
        if (stmt) {
            add_child(g_parser.root, stmt);
        }
    }
    
    if (g_parser.errors.count > 0) {
        printf("❌ Parser found %zu errors\n", g_parser.errors.count);
        print_parse_errors();
        return NULL;
    }
    
    printf("✅ Parsing completed successfully\n");
    printf("   • AST nodes created: %zu\n", count_ast_nodes(g_parser.root));
    
    return g_parser.root;
}

/*
 * Parse statement
 */
static ast_node_t* parse_statement(void) {
    Token* current_token = peek();
    
    switch (current_token->type) {
        case TOKEN_FUNC:
            return parse_function();
        case TOKEN_IF:
            return parse_if_statement();
        case TOKEN_FOR:
        case TOKEN_PARALLEL:
            return parse_for_statement();
        case TOKEN_WHILE:
            return parse_while_statement();
        case TOKEN_MATCH:
            return parse_match_statement();
        case TOKEN_RETURN:
            return parse_return_statement();
        case TOKEN_VAR:
        case TOKEN_CONST:
            return parse_variable_declaration();
        case TOKEN_IMPORT:
            return parse_import_statement();
        case TOKEN_LBRACE:
            return parse_block();
        default:
            return parse_expression_statement();
    }
}

/*
 * Parse function declaration
 */
static ast_node_t* parse_function(void) {
    ast_node_t* func_node = create_ast_node(AST_FUNCTION);
    
    // Consume 'func' keyword
    consume(TOKEN_FUNC, "Expected 'func'");
    
    // Parse function name
    Token* name_token = consume(TOKEN_IDENTIFIER, "Expected function name");
    func_node->data.function.name = strdup(name_token->value);
    
    // Parse parameters
    consume(TOKEN_LPAREN, "Expected '(' after function name");
    
    func_node->data.function.parameters = parse_parameter_list();
    
    consume(TOKEN_RPAREN, "Expected ')' after parameters");
    
    // Parse return type (optional)
    if (match(TOKEN_ARROW)) {
        func_node->data.function.return_type = parse_type();
    }
    
    // Parse function body
    consume(TOKEN_COLON, "Expected ':' before function body");
    func_node->data.function.body = parse_block();
    
    return func_node;
}

/*
 * Parse if statement
 */
static ast_node_t* parse_if_statement(void) {
    ast_node_t* if_node = create_ast_node(AST_IF);
    
    consume(TOKEN_IF, "Expected 'if'");
    
    // Parse condition
    if_node->data.if_stmt.condition = parse_expression();
    
    consume(TOKEN_COLON, "Expected ':' after if condition");
    
    // Parse then block
    if_node->data.if_stmt.then_block = parse_block();
    
    // Parse optional else/elif
    if (match(TOKEN_ELIF)) {
        if_node->data.if_stmt.else_block = parse_if_statement(); // Recursive for elif
    } else if (match(TOKEN_ELSE)) {
        consume(TOKEN_COLON, "Expected ':' after else");
        if_node->data.if_stmt.else_block = parse_block();
    }
    
    return if_node;
}

/*
 * Parse for statement (including parallel for)
 */
static ast_node_t* parse_for_statement(void) {
    ast_node_t* for_node = create_ast_node(AST_FOR);
    
    // Check for parallel keyword
    if (match(TOKEN_PARALLEL)) {
        for_node->data.for_stmt.is_parallel = 1;
    }
    
    consume(TOKEN_FOR, "Expected 'for'");
    
    // Parse iterator variable
    Token* var_token = consume(TOKEN_IDENTIFIER, "Expected variable name");
    for_node->data.for_stmt.variable = strdup(var_token->value);
    
    consume(TOKEN_IN, "Expected 'in' after for variable");
    
    // Parse iterable expression
    for_node->data.for_stmt.iterable = parse_expression();
    
    consume(TOKEN_COLON, "Expected ':' after for expression");
    
    // Parse body
    for_node->data.for_stmt.body = parse_block();
    
    return for_node;
}

/*
 * Parse while statement
 */
static ast_node_t* parse_while_statement(void) {
    ast_node_t* while_node = create_ast_node(AST_WHILE);
    
    consume(TOKEN_WHILE, "Expected 'while'");
    
    // Parse condition
    while_node->data.while_stmt.condition = parse_expression();
    
    consume(TOKEN_COLON, "Expected ':' after while condition");
    
    // Parse body
    while_node->data.while_stmt.body = parse_block();
    
    return while_node;
}

/*
 * Parse match statement
 */
static ast_node_t* parse_match_statement(void) {
    ast_node_t* match_node = create_ast_node(AST_MATCH);
    
    consume(TOKEN_MATCH, "Expected 'match'");
    
    // Parse expression to match
    match_node->data.match_stmt.expression = parse_expression();
    
    consume(TOKEN_COLON, "Expected ':' after match expression");
    consume(TOKEN_LBRACE, "Expected '{' to start match cases");
    
    // Parse match cases
    while (!check(TOKEN_RBRACE) && !is_at_end()) {
        ast_node_t* case_node = parse_match_case();
        add_child(match_node, case_node);
    }
    
    consume(TOKEN_RBRACE, "Expected '}' after match cases");
    
    return match_node;
}

/*
 * Parse expression with operator precedence
 */
static ast_node_t* parse_expression(void) {
    return parse_binary(parse_primary(), 0);
}

/*
 * Parse binary expression with precedence climbing
 */
static ast_node_t* parse_binary(ast_node_t* left, int min_precedence) {
    while (true) {
        Token* op_token = peek();
        int precedence = get_operator_precedence(op_token->type);
        
        if (precedence < min_precedence) {
            break;
        }
        
        advance(); // Consume operator
        
        ast_node_t* right = parse_primary();
        
        // Handle right-associative operators
        Token* next_op = peek();
        int next_precedence = get_operator_precedence(next_op->type);
        
        if (precedence < next_precedence || 
            (precedence == next_precedence && is_right_associative(op_token->type))) {
            right = parse_binary(right, precedence + 1);
        }
        
        // Create binary operation node
        ast_node_t* binary_node = create_ast_node(AST_BINARY_OP);
        binary_node->data.binary_op.operator = op_token->type;
        binary_node->data.binary_op.left = left;
        binary_node->data.binary_op.right = right;
        
        left = binary_node;
    }
    
    return left;
}

/*
 * Parse primary expression
 */
static ast_node_t* parse_primary(void) {
    Token* token = peek();
    
    switch (token->type) {
        case TOKEN_NUMBER:
            advance();
            return create_number_node(token->value);
            
        case TOKEN_STRING:
            advance();
            return create_string_node(token->value);
            
        case TOKEN_IDENTIFIER:
            advance();
            return create_identifier_node(token->value);
            
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            advance();
            return create_boolean_node(token->type == TOKEN_TRUE);
            
        case TOKEN_LPAREN:
            advance();
            ast_node_t* expr = parse_expression();
            consume(TOKEN_RPAREN, "Expected ')' after expression");
            return expr;
            
        case TOKEN_LBRACKET:
            return parse_array_literal();
            
        case TOKEN_LBRACE:
            return parse_object_literal();
            
        default:
            add_error("Unexpected token in expression");
            return NULL;
    }
}

/*
 * Utility functions
 */




/*
 * Get operator precedence
 */




/*
 * Error handling
 */
static void add_error(const char* message) {
    if (g_parser.errors.count >= g_parser.errors.capacity) {
        return; // Error list full
    }
    
    parse_error_t* error = &g_parser.errors.errors[g_parser.errors.count++];
    error->message = strdup(message);
    error->line = peek()->line;
    error->column = peek()->column;
}

static void print_parse_errors(void) {
    printf("❌ Parse Errors:\n");
    for (size_t i = 0; i < g_parser.errors.count; i++) {
        parse_error_t* error = &g_parser.errors.errors[i];
        printf("   Line %zu:%zu: %s\n", error->line, error->column, error->message);
    }
}

/*
 * Missing function implementations
 */
static ast_node_t* parse_block(void) {
    ast_node_t* block = create_ast_node(AST_BLOCK);
    // TODO: Implement block parsing
    return block;
}

static Token* peek(void) {
    if (g_parser.current >= g_parser.token_count) {
        return &g_parser.tokens[g_parser.token_count - 1]; // Return EOF token
    }
    return &g_parser.tokens[g_parser.current];
}

static Token* advance(void) {
    if (!is_at_end()) {
        g_parser.current++;
    }
    return &g_parser.tokens[g_parser.current - 1];
}

static int is_at_end(void) {
    return g_parser.current >= g_parser.token_count ||
           peek()->type == TOKEN_EOF;
}

static int check(TokenType type) {
    if (is_at_end()) return 0;
    return peek()->type == type;
}

static int match(TokenType type) {
    if (check(type)) {
        advance();
        return 1;
    }
    return 0;
}

static Token* consume(TokenType type, const char* message) {
    if (check(type)) {
        return advance();
    }
    add_error(message);
    return NULL;
}

static int get_operator_precedence(TokenType type) {
    switch (type) {
        case TOKEN_OR: return 1;
        case TOKEN_AND: return 2;
        case TOKEN_EQ: case TOKEN_NE: return 3;
        case TOKEN_LT: case TOKEN_LE: case TOKEN_GT: case TOKEN_GE: return 4;
        case TOKEN_PLUS: case TOKEN_MINUS: return 5;
        case TOKEN_MULTIPLY: case TOKEN_DIVIDE: case TOKEN_MODULO: return 6;
        case TOKEN_POWER: return 7;
        default: return 0;
    }
}

static int is_right_associative(TokenType type) {
    return type == TOKEN_POWER;
}

static ast_node_t* parse_unary_operation(ast_node_t* node) {
    // TODO: Implement unary operation analysis
    return NULL;
}

static ast_node_t* parse_function_call(ast_node_t* node) {
    // TODO: Implement function call analysis
    return NULL;
}





/*
 * Cleanup parser
 */
void parser_cleanup(void) {
    if (g_parser.errors.errors) {
        for (size_t i = 0; i < g_parser.errors.count; i++) {
            free((void*)g_parser.errors.errors[i].message);
        }
        free(g_parser.errors.errors);
        g_parser.errors.errors = NULL;
    }

    if (g_parser.root) {
        free_ast_node(g_parser.root);
        g_parser.root = NULL;
    }

    printf("🧹 Parser cleaned up\n");
}
