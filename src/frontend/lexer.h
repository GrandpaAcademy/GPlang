#ifndef GPLANG_LEXER_H
#define GPLANG_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Token types for GPLANG
typedef enum {
    // Literals
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    
    // Keywords
    TOKEN_FUNC, TOKEN_FUN, TOKEN_FU,
    TOKEN_VAR, TOKEN_IF, TOKEN_ELIF, TOKEN_ELSE,
    TOKEN_WHILE, TOKEN_FOR, TOKEN_IN, TOKEN_RETURN,
    TOKEN_AND, TOKEN_OR, TOKEN_NOT,
    TOKEN_TRUE, TOKEN_FALSE, TOKEN_IMPORT,
    
    // Async keywords
    TOKEN_ASYNC, TOKEN_AWAIT, TOKEN_SPAWN,
    
    // Safety keywords
    TOKEN_MATCH, TOKEN_SOME, TOKEN_NONE,
    TOKEN_OK, TOKEN_ERR, TOKEN_OPTION, TOKEN_RESULT,
    
    // Testing keywords
    TOKEN_TEST, TOKEN_BENCH,
    
    // Type keywords
    TOKEN_INT, TOKEN_FLOAT, TOKEN_STR, TOKEN_STRING_TYPE,
    TOKEN_BOOL, TOKEN_LIST, TOKEN_DICT, TOKEN_SET, TOKEN_TUPLE,
    
    // Advanced types
    TOKEN_FUTURE, TOKEN_CHANNEL, TOKEN_TASK,
    TOKEN_VEC2, TOKEN_VEC3, TOKEN_VEC4, TOKEN_MATRIX4,
    TOKEN_COLOR, TOKEN_TIME, TOKEN_REF, TOKEN_MUT,
    
    // Module keywords
    TOKEN_OS, TOKEN_SYS, TOKEN_ENV, TOKEN_PROCESS, TOKEN_STD,
    
    // Operators
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_MODULO,
    TOKEN_ASSIGN, TOKEN_EQUAL, TOKEN_NOT_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_ARROW,
    
    // Delimiters
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_COLON, TOKEN_SEMICOLON,
    
    // Special
    TOKEN_NEWLINE, TOKEN_INDENT, TOKEN_DEDENT,
    TOKEN_COMMENT, TOKEN_EOF, TOKEN_ERROR
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
    int length;
} Token;

// Lexer structure
typedef struct {
    const char* source;
    int source_length;
    int position;
    int line;
    int column;
    
    // Indentation tracking
    int* indent_stack;
    int indent_stack_size;
    int indent_stack_capacity;
    
    // Error handling
    bool has_errors;
    char* error_message;
} Lexer;

// Function declarations
Lexer* lexer_create(const char* source);
void lexer_destroy(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);
void token_destroy(Token* token);
const char* token_type_to_string(TokenType type);
void lexer_error(Lexer* lexer, const char* message);

// Utility functions
bool is_keyword(const char* text);
TokenType get_keyword_type(const char* text);
bool is_identifier_char(char ch);
bool is_digit(char ch);
bool is_alpha(char ch);

#endif // GPLANG_LEXER_H
