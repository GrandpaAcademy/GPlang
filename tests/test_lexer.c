/*
 * GPLANG Lexer Tests
 * Comprehensive test suite for the lexical analyzer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
 * Test basic tokenization
 */
int test_basic_tokens(void) {
    const char* source = "func main(): return 0";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    // Check token types
    ASSERT(tokens[0].type == TOKEN_FUNC);
    ASSERT(strcmp(tokens[0].value, "func") == 0);
    
    ASSERT(tokens[1].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[1].value, "main") == 0);
    
    ASSERT(tokens[2].type == TOKEN_LPAREN);
    ASSERT(tokens[3].type == TOKEN_RPAREN);
    ASSERT(tokens[4].type == TOKEN_COLON);
    
    ASSERT(tokens[5].type == TOKEN_RETURN);
    ASSERT(strcmp(tokens[5].value, "return") == 0);
    
    ASSERT(tokens[6].type == TOKEN_NUMBER);
    ASSERT(strcmp(tokens[6].value, "0") == 0);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test keywords
 */
int test_keywords(void) {
    const char* source = "func if else elif while for parallel match var const import";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    ASSERT(tokens[0].type == TOKEN_FUNC);
    ASSERT(tokens[1].type == TOKEN_IF);
    ASSERT(tokens[2].type == TOKEN_ELSE);
    ASSERT(tokens[3].type == TOKEN_ELIF);
    ASSERT(tokens[4].type == TOKEN_WHILE);
    ASSERT(tokens[5].type == TOKEN_FOR);
    ASSERT(tokens[6].type == TOKEN_PARALLEL);
    ASSERT(tokens[7].type == TOKEN_MATCH);
    ASSERT(tokens[8].type == TOKEN_VAR);
    ASSERT(tokens[9].type == TOKEN_CONST);
    ASSERT(tokens[10].type == TOKEN_IMPORT);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test operators
 */
int test_operators(void) {
    const char* source = "+ - * / % == != < <= > >= && ||";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    ASSERT(tokens[0].type == TOKEN_PLUS);
    ASSERT(tokens[1].type == TOKEN_MINUS);
    ASSERT(tokens[2].type == TOKEN_MULTIPLY);
    ASSERT(tokens[3].type == TOKEN_DIVIDE);
    ASSERT(tokens[4].type == TOKEN_MODULO);
    ASSERT(tokens[5].type == TOKEN_EQ);
    ASSERT(tokens[6].type == TOKEN_NE);
    ASSERT(tokens[7].type == TOKEN_LT);
    ASSERT(tokens[8].type == TOKEN_LE);
    ASSERT(tokens[9].type == TOKEN_GT);
    ASSERT(tokens[10].type == TOKEN_GE);
    ASSERT(tokens[11].type == TOKEN_AND);
    ASSERT(tokens[12].type == TOKEN_OR);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test string literals
 */
int test_string_literals(void) {
    const char* source = "\"hello world\" \"escape\\ntest\" \"\"";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    ASSERT(tokens[0].type == TOKEN_STRING);
    ASSERT(strcmp(tokens[0].value, "hello world") == 0);
    
    ASSERT(tokens[1].type == TOKEN_STRING);
    ASSERT(strcmp(tokens[1].value, "escape\\ntest") == 0);
    
    ASSERT(tokens[2].type == TOKEN_STRING);
    ASSERT(strcmp(tokens[2].value, "") == 0);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test number literals
 */
int test_number_literals(void) {
    const char* source = "42 3.14159 0 -123 1.0e10";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    ASSERT(tokens[0].type == TOKEN_NUMBER);
    ASSERT(strcmp(tokens[0].value, "42") == 0);
    
    ASSERT(tokens[1].type == TOKEN_NUMBER);
    ASSERT(strcmp(tokens[1].value, "3.14159") == 0);
    
    ASSERT(tokens[2].type == TOKEN_NUMBER);
    ASSERT(strcmp(tokens[2].value, "0") == 0);
    
    ASSERT(tokens[3].type == TOKEN_MINUS);
    ASSERT(tokens[4].type == TOKEN_NUMBER);
    ASSERT(strcmp(tokens[4].value, "123") == 0);
    
    ASSERT(tokens[5].type == TOKEN_NUMBER);
    ASSERT(strcmp(tokens[5].value, "1.0e10") == 0);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test identifiers
 */
int test_identifiers(void) {
    const char* source = "variable_name CamelCase _underscore var123 _";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    ASSERT(tokens[0].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[0].value, "variable_name") == 0);
    
    ASSERT(tokens[1].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[1].value, "CamelCase") == 0);
    
    ASSERT(tokens[2].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[2].value, "_underscore") == 0);
    
    ASSERT(tokens[3].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[3].value, "var123") == 0);
    
    ASSERT(tokens[4].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[4].value, "_") == 0);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test comments
 */
int test_comments(void) {
    const char* source = "func main(): # This is a comment\n    return 0 # Another comment";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    // Comments should be ignored
    ASSERT(tokens[0].type == TOKEN_FUNC);
    ASSERT(tokens[1].type == TOKEN_IDENTIFIER);
    ASSERT(tokens[2].type == TOKEN_LPAREN);
    ASSERT(tokens[3].type == TOKEN_RPAREN);
    ASSERT(tokens[4].type == TOKEN_COLON);
    ASSERT(tokens[5].type == TOKEN_RETURN);
    ASSERT(tokens[6].type == TOKEN_NUMBER);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test parallel for syntax
 */
int test_parallel_for(void) {
    const char* source = "parallel for i in range(1000000):";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    if (!tokens) {
        lexer_cleanup();
        return 0;
    }
    
    ASSERT(tokens[0].type == TOKEN_PARALLEL);
    ASSERT(tokens[1].type == TOKEN_FOR);
    ASSERT(tokens[2].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[2].value, "i") == 0);
    ASSERT(tokens[3].type == TOKEN_IN);
    ASSERT(tokens[4].type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(tokens[4].value, "range") == 0);
    
    free_tokens(tokens);
    lexer_cleanup();
    return 1;
}

/*
 * Test error handling
 */
int test_error_handling(void) {
    const char* source = "func main(): return @invalid_char";
    
    if (lexer_init() != 0) {
        return 0;
    }
    
    token_t* tokens = tokenize(source);
    
    // Should handle invalid characters gracefully
    // Implementation specific - may return NULL or error tokens
    
    if (tokens) {
        free_tokens(tokens);
    }
    lexer_cleanup();
    return 1; // Test passes if no crash occurs
}

/*
 * Main test runner
 */
int main(void) {
    printf("🧪 Running GPLANG Lexer Tests\n");
    printf("==============================\n");
    
    TEST(test_basic_tokens);
    TEST(test_keywords);
    TEST(test_operators);
    TEST(test_string_literals);
    TEST(test_number_literals);
    TEST(test_identifiers);
    TEST(test_comments);
    TEST(test_parallel_for);
    TEST(test_error_handling);
    
    printf("\n📊 Test Results:\n");
    printf("   • Tests run: %d\n", tests_run);
    printf("   • Tests passed: %d\n", tests_passed);
    printf("   • Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("✅ All lexer tests passed!\n");
        return 0;
    } else {
        printf("❌ Some lexer tests failed!\n");
        return 1;
    }
}
