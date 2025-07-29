/*
 * GPLANG AST Utilities
 * Abstract Syntax Tree creation and manipulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/*
 * Create AST node
 */
ast_node_t* create_ast_node(ast_node_type_t type) {
    ast_node_t* node = calloc(1, sizeof(ast_node_t));
    if (!node) {
        printf("❌ Failed to allocate AST node\n");
        return NULL;
    }
    
    node->type = type;
    node->child_capacity = 4; // Initial capacity
    node->children = calloc(node->child_capacity, sizeof(ast_node_t*));
    
    if (!node->children) {
        free(node);
        return NULL;
    }
    
    return node;
}

/*
 * Create number node
 */
ast_node_t* create_number_node(const char* value) {
    ast_node_t* node = create_ast_node(AST_NUMBER);
    if (node) {
        node->data.literal.value = strdup(value);
    }
    return node;
}

/*
 * Create string node
 */
ast_node_t* create_string_node(const char* value) {
    ast_node_t* node = create_ast_node(AST_STRING);
    if (node) {
        node->data.literal.value = strdup(value);
    }
    return node;
}

/*
 * Create identifier node
 */
ast_node_t* create_identifier_node(const char* name) {
    ast_node_t* node = create_ast_node(AST_IDENTIFIER);
    if (node) {
        node->data.identifier.name = strdup(name);
    }
    return node;
}

/*
 * Create boolean node
 */
ast_node_t* create_boolean_node(int value) {
    ast_node_t* node = create_ast_node(AST_BOOLEAN);
    if (node) {
        node->data.literal.value = strdup(value ? "true" : "false");
    }
    return node;
}

/*
 * Add child to AST node
 */
void add_child(ast_node_t* parent, ast_node_t* child) {
    if (!parent || !child) return;
    
    // Resize if needed
    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity *= 2;
        parent->children = realloc(parent->children, 
                                 parent->child_capacity * sizeof(ast_node_t*));
        if (!parent->children) {
            printf("❌ Failed to resize AST children array\n");
            return;
        }
    }
    
    parent->children[parent->child_count++] = child;
}

/*
 * Free AST node recursively
 */
void free_ast_node(ast_node_t* node) {
    if (!node) return;
    
    // Free children recursively
    for (size_t i = 0; i < node->child_count; i++) {
        free_ast_node(node->children[i]);
    }
    free(node->children);
    
    // Free node-specific data
    switch (node->type) {
        case AST_FUNCTION:
            free(node->data.function.name);
            break;
        case AST_VARIABLE:
            free(node->data.variable.name);
            break;
        case AST_FOR:
            free(node->data.for_stmt.variable);
            break;
        case AST_LITERAL:
        case AST_NUMBER:
        case AST_STRING:
            free(node->data.literal.value);
            break;
        case AST_IDENTIFIER:
            free(node->data.identifier.name);
            break;
        default:
            break;
    }
    
    free(node);
}

/*
 * Print AST for debugging
 */
void print_ast(ast_node_t* node, int depth) {
    if (!node) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    // Print node type and data
    switch (node->type) {
        case AST_PROGRAM:
            printf("PROGRAM\n");
            break;
        case AST_FUNCTION:
            printf("FUNCTION: %s\n", node->data.function.name);
            break;
        case AST_VARIABLE:
            printf("VARIABLE: %s\n", node->data.variable.name);
            break;
        case AST_BINARY_OP:
            printf("BINARY_OP: %d\n", node->data.binary_op.operator);
            break;
        case AST_IF:
            printf("IF\n");
            break;
        case AST_FOR:
            printf("FOR: %s%s\n", 
                   node->data.for_stmt.is_parallel ? "parallel " : "",
                   node->data.for_stmt.variable);
            break;
        case AST_WHILE:
            printf("WHILE\n");
            break;
        case AST_MATCH:
            printf("MATCH\n");
            break;
        case AST_RETURN:
            printf("RETURN\n");
            break;
        case AST_BLOCK:
            printf("BLOCK\n");
            break;
        case AST_NUMBER:
            printf("NUMBER: %s\n", node->data.literal.value);
            break;
        case AST_STRING:
            printf("STRING: %s\n", node->data.literal.value);
            break;
        case AST_IDENTIFIER:
            printf("IDENTIFIER: %s\n", node->data.identifier.name);
            break;
        case AST_BOOLEAN:
            printf("BOOLEAN: %s\n", node->data.literal.value);
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
    
    // Print children
    for (size_t i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

/*
 * Count AST nodes
 */
size_t count_ast_nodes(ast_node_t* node) {
    if (!node) return 0;
    
    size_t count = 1; // Count this node
    
    // Count children recursively
    for (size_t i = 0; i < node->child_count; i++) {
        count += count_ast_nodes(node->children[i]);
    }
    
    return count;
}

/*
 * Parse parameter list
 */
ast_node_t* parse_parameter_list(void) {
    ast_node_t* params = create_ast_node(AST_BLOCK);
    
    // TODO: Implement parameter parsing
    // For now, return empty parameter list
    
    return params;
}

/*
 * Parse type annotation
 */
ast_node_t* parse_type(void) {
    // TODO: Implement type parsing
    // For now, return identifier node
    return create_identifier_node("auto");
}

/*
 * Parse return statement
 */
ast_node_t* parse_return_statement(void) {
    ast_node_t* return_node = create_ast_node(AST_RETURN);
    
    // TODO: Implement return statement parsing
    
    return return_node;
}

/*
 * Parse variable declaration
 */
ast_node_t* parse_variable_declaration(void) {
    ast_node_t* var_node = create_ast_node(AST_VARIABLE);
    
    // TODO: Implement variable declaration parsing
    
    return var_node;
}

/*
 * Parse import statement
 */
ast_node_t* parse_import_statement(void) {
    ast_node_t* import_node = create_ast_node(AST_IMPORT);
    
    // TODO: Implement import statement parsing
    
    return import_node;
}

/*
 * Parse expression statement
 */
ast_node_t* parse_expression_statement(void) {
    ast_node_t* expr_stmt = create_ast_node(AST_EXPRESSION_STMT);
    
    // TODO: Implement expression statement parsing
    
    return expr_stmt;
}

/*
 * Parse match case
 */
ast_node_t* parse_match_case(void) {
    ast_node_t* case_node = create_ast_node(AST_BLOCK);
    
    // TODO: Implement match case parsing
    
    return case_node;
}

/*
 * Parse array literal
 */
ast_node_t* parse_array_literal(void) {
    ast_node_t* array_node = create_ast_node(AST_ARRAY);
    
    // TODO: Implement array literal parsing
    
    return array_node;
}

/*
 * Parse object literal
 */
ast_node_t* parse_object_literal(void) {
    ast_node_t* object_node = create_ast_node(AST_OBJECT);
    
    // TODO: Implement object literal parsing
    
    return object_node;
}
