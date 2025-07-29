/*
 * GPLANG Parser Tests
 * Comprehensive test suite for the parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/frontend/parser.h"
#include "../src/frontend/lexer.h"

// Test framework
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    do { \
        printf("Running test: %s... ", #name); \
        tests_run++; \
        if (name()) { \
            printf("PASSED\n"); \
            tests_passed++; \
        } else { \
            printf("FAILED\n"); \
        } \
    } while(0)

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("Assertion failed: %s\n", #condition); \
            return 0; \
        } \
    } while(0)

/*
 * Helper function to parse source code
 */
ast_node_t* parse_source(const char* source) {
    if (lexer_init() != 0) return NULL;
    if (parser_init() != 0) {
        lexer_cleanup();
        return NULL;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        parser_cleanup();
        lexer_cleanup();
        return NULL;
    }
    
    size_t token_count = 0;
    while (tokens[token_count].type != TOKEN_EOF) {
        token_count++;
    }
    
    ast_node_t* ast = parse(tokens, token_count);
    
    free_tokens(tokens);
    return ast;
}

void cleanup_parse_test(ast_node_t* ast) {
    if (ast) {
        free_ast_node(ast);
    }
    parser_cleanup();
    lexer_cleanup();
}

/*
 * Test simple function parsing
 */
int test_simple_function(void) {
    const char* source = "func main(): return 0";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* func = ast->children[0];
    ASSERT(func->type == AST_FUNCTION);
    ASSERT(strcmp(func->data.function.name, "main") == 0);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test variable declaration
 */
int test_variable_declaration(void) {
    const char* source = "var x = 42";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* var = ast->children[0];
    ASSERT(var->type == AST_VARIABLE);
    ASSERT(strcmp(var->data.variable.name, "x") == 0);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test binary expressions
 */
int test_binary_expressions(void) {
    const char* source = "var result = 2 + 3 * 4";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* var = ast->children[0];
    ASSERT(var->type == AST_VARIABLE);
    ASSERT(var->data.variable.value != NULL);
    ASSERT(var->data.variable.value->type == AST_BINARY_OP);
    
    // Should parse as: 2 + (3 * 4) due to operator precedence
    ast_node_t* add = var->data.variable.value;
    ASSERT(add->data.binary_op.operator == TOKEN_PLUS);
    ASSERT(add->data.binary_op.right->type == AST_BINARY_OP);
    ASSERT(add->data.binary_op.right->data.binary_op.operator == TOKEN_MULTIPLY);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test if statement
 */
int test_if_statement(void) {
    const char* source = "if x > 0: print(\"positive\") else: print(\"not positive\")";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* if_stmt = ast->children[0];
    ASSERT(if_stmt->type == AST_IF);
    ASSERT(if_stmt->data.if_stmt.condition != NULL);
    ASSERT(if_stmt->data.if_stmt.then_block != NULL);
    ASSERT(if_stmt->data.if_stmt.else_block != NULL);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test for loop
 */
int test_for_loop(void) {
    const char* source = "for i in range(10): print(i)";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* for_stmt = ast->children[0];
    ASSERT(for_stmt->type == AST_FOR);
    ASSERT(strcmp(for_stmt->data.for_stmt.variable, "i") == 0);
    ASSERT(for_stmt->data.for_stmt.iterable != NULL);
    ASSERT(for_stmt->data.for_stmt.body != NULL);
    ASSERT(for_stmt->data.for_stmt.is_parallel == 0);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test parallel for loop
 */
int test_parallel_for_loop(void) {
    const char* source = "parallel for i in range(1000000): result += i * 2";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* for_stmt = ast->children[0];
    ASSERT(for_stmt->type == AST_FOR);
    ASSERT(strcmp(for_stmt->data.for_stmt.variable, "i") == 0);
    ASSERT(for_stmt->data.for_stmt.iterable != NULL);
    ASSERT(for_stmt->data.for_stmt.body != NULL);
    ASSERT(for_stmt->data.for_stmt.is_parallel == 1);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test while loop
 */
int test_while_loop(void) {
    const char* source = "while x < 100: x = x + 1";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* while_stmt = ast->children[0];
    ASSERT(while_stmt->type == AST_WHILE);
    ASSERT(while_stmt->data.while_stmt.condition != NULL);
    ASSERT(while_stmt->data.while_stmt.body != NULL);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test function with parameters
 */
int test_function_with_parameters(void) {
    const char* source = "func add(a: i32, b: i32) -> i32: return a + b";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* func = ast->children[0];
    ASSERT(func->type == AST_FUNCTION);
    ASSERT(strcmp(func->data.function.name, "add") == 0);
    ASSERT(func->data.function.parameters != NULL);
    ASSERT(func->data.function.return_type != NULL);
    ASSERT(func->data.function.body != NULL);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test nested expressions
 */
int test_nested_expressions(void) {
    const char* source = "var result = (2 + 3) * (4 - 1)";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* var = ast->children[0];
    ASSERT(var->type == AST_VARIABLE);
    ASSERT(var->data.variable.value != NULL);
    ASSERT(var->data.variable.value->type == AST_BINARY_OP);
    
    // Should parse as: (2 + 3) * (4 - 1)
    ast_node_t* mul = var->data.variable.value;
    ASSERT(mul->data.binary_op.operator == TOKEN_MULTIPLY);
    ASSERT(mul->data.binary_op.left->type == AST_BINARY_OP);
    ASSERT(mul->data.binary_op.right->type == AST_BINARY_OP);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test match statement
 */
int test_match_statement(void) {
    const char* source = "match value: case 1: print(\"one\") case 2: print(\"two\")";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 1);
    
    ast_node_t* match_stmt = ast->children[0];
    ASSERT(match_stmt->type == AST_MATCH);
    ASSERT(match_stmt->data.match_stmt.expression != NULL);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Test complex program
 */
int test_complex_program(void) {
    const char* source = 
        "func fibonacci(n: i32) -> i32:\n"
        "    if n <= 1:\n"
        "        return n\n"
        "    else:\n"
        "        return fibonacci(n - 1) + fibonacci(n - 2)\n"
        "\n"
        "func main():\n"
        "    var result = 0\n"
        "    parallel for i in range(1000000):\n"
        "        result += i * 2\n"
        "    print(result)\n"
        "    return 0";
    
    ast_node_t* ast = parse_source(source);
    ASSERT(ast != NULL);
    ASSERT(ast->type == AST_PROGRAM);
    ASSERT(ast->child_count == 2); // Two functions
    
    // Check first function (fibonacci)
    ast_node_t* fib_func = ast->children[0];
    ASSERT(fib_func->type == AST_FUNCTION);
    ASSERT(strcmp(fib_func->data.function.name, "fibonacci") == 0);
    
    // Check second function (main)
    ast_node_t* main_func = ast->children[1];
    ASSERT(main_func->type == AST_FUNCTION);
    ASSERT(strcmp(main_func->data.function.name, "main") == 0);
    
    cleanup_parse_test(ast);
    return 1;
}

/*
 * Main test runner
 */
int main(void) {
    printf("🧪 Running GPLANG Parser Tests\n");
    printf("===============================\n");
    
    TEST(test_simple_function);
    TEST(test_variable_declaration);
    TEST(test_binary_expressions);
    TEST(test_if_statement);
    TEST(test_for_loop);
    TEST(test_parallel_for_loop);
    TEST(test_while_loop);
    TEST(test_function_with_parameters);
    TEST(test_nested_expressions);
    TEST(test_match_statement);
    TEST(test_complex_program);
    
    printf("\n📊 Test Results:\n");
    printf("   • Tests run: %d\n", tests_run);
    printf("   • Tests passed: %d\n", tests_passed);
    printf("   • Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("✅ All parser tests passed!\n");
        return 0;
    } else {
        printf("❌ Some parser tests failed!\n");
        return 1;
    }
}
