#include "lexer.h"

// Keywords mapping
typedef struct {
    const char* keyword;
    TokenType type;
} KeywordMapping;

static KeywordMapping keywords[] = {
    {"func", TOKEN_FUNC}, {"fun", TOKEN_FUN}, {"fu", TOKEN_FU},
    {"var", TOKEN_VAR}, {"if", TOKEN_IF}, {"elif", TOKEN_ELIF}, {"else", TOKEN_ELSE},
    {"while", TOKEN_WHILE}, {"for", TOKEN_FOR}, {"in", TOKEN_IN}, {"return", TOKEN_RETURN},
    {"and", TOKEN_AND}, {"or", TOKEN_OR}, {"not", TOKEN_NOT},
    {"true", TOKEN_TRUE}, {"false", TOKEN_FALSE}, {"import", TOKEN_IMPORT},
    {"unsafe", TOKEN_UNSAFE}, {"async", TOKEN_ASYNC}, {"await", TOKEN_AWAIT}, {"spawn", TOKEN_SPAWN},
    {"match", TOKEN_MATCH}, {"Some", TOKEN_SOME}, {"None", TOKEN_NONE},
    {"Ok", TOKEN_OK}, {"Err", TOKEN_ERR}, {"Option", TOKEN_OPTION}, {"Result", TOKEN_RESULT},
    {"test", TOKEN_TEST}, {"bench", TOKEN_BENCH},
    {"int", TOKEN_INT}, {"float", TOKEN_FLOAT}, {"str", TOKEN_STR}, {"string", TOKEN_STRING_TYPE},
    {"bool", TOKEN_BOOL}, {"list", TOKEN_LIST}, {"dict", TOKEN_DICT}, {"set", TOKEN_SET}, {"tuple", TOKEN_TUPLE},
    {"Future", TOKEN_FUTURE}, {"Channel", TOKEN_CHANNEL}, {"Task", TOKEN_TASK},
    {"Vec2", TOKEN_VEC2}, {"Vec3", TOKEN_VEC3}, {"Vec4", TOKEN_VEC4}, {"Matrix4", TOKEN_MATRIX4},
    {"Color", TOKEN_COLOR}, {"Time", TOKEN_TIME}, {"Ref", TOKEN_REF}, {"Mut", TOKEN_MUT},
    {"os", TOKEN_OS}, {"sys", TOKEN_SYS}, {"env", TOKEN_ENV}, {"process", TOKEN_PROCESS}, {"std", TOKEN_STD},
    {NULL, 0}
};

// Create lexer
Lexer* lexer_create(const char* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    if (!lexer) return NULL;
    
    lexer->source_length = strlen(source);
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    
    // Initialize indent stack
    lexer->indent_stack_capacity = 16;
    lexer->indent_stack = malloc(sizeof(int) * lexer->indent_stack_capacity);
    lexer->indent_stack[0] = 0;
    lexer->indent_stack_size = 1;
    
    lexer->has_errors = false;
    lexer->error_message = NULL;
    
    return lexer;
}

// Destroy lexer
void lexer_destroy(Lexer* lexer) {
    if (lexer) {
        free(lexer->indent_stack);
        free(lexer->error_message);
        free(lexer);
    }
}

// Create token
static Token* token_create(TokenType type, const char* value, int line, int column) {
    Token* token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = type;
    token->line = line;
    token->column = column;
    
    if (value) {
        token->length = strlen(value);
        token->value = malloc(token->length + 1);
        strcpy(token->value, value);
    } else {
        token->value = NULL;
        token->length = 0;
    }
    
    return token;
}

// Destroy token
void token_destroy(Token* token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

// Get current character
static char current_char(Lexer* lexer) {
    if (lexer->position >= lexer->source_length) return '\0';
    return lexer->source[lexer->position];
}

// Advance position
static char advance(Lexer* lexer) {
    if (lexer->position >= lexer->source_length) return '\0';
    
    char ch = lexer->source[lexer->position++];
    if (ch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return ch;
}

// Skip whitespace
static void skip_whitespace(Lexer* lexer) {
    while (current_char(lexer) && (current_char(lexer) == ' ' || 
           current_char(lexer) == '\t' || current_char(lexer) == '\r')) {
        advance(lexer);
    }
}

// Check if keyword
bool is_keyword(const char* text) {
    for (int i = 0; keywords[i].keyword != NULL; i++) {
        if (strcmp(text, keywords[i].keyword) == 0) {
            return true;
        }
    }
    return false;
}

// Get keyword type
TokenType get_keyword_type(const char* text) {
    for (int i = 0; keywords[i].keyword != NULL; i++) {
        if (strcmp(text, keywords[i].keyword) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

// Character classification
bool is_identifier_char(char ch) { return isalnum(ch) || ch == '_'; }
bool is_digit(char ch) { return isdigit(ch); }
bool is_alpha(char ch) { return isalpha(ch); }

// Read identifier or keyword
static Token* read_identifier(Lexer* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    int start_pos = lexer->position;
    
    while (is_identifier_char(current_char(lexer))) {
        advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* text = malloc(length + 1);
    strncpy(text, lexer->source + start_pos, length);
    text[length] = '\0';
    
    TokenType type = is_keyword(text) ? get_keyword_type(text) : TOKEN_IDENTIFIER;
    Token* token = token_create(type, text, start_line, start_column);
    
    free(text);
    return token;
}

// Read number
static Token* read_number(Lexer* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    int start_pos = lexer->position;
    
    while (is_digit(current_char(lexer))) {
        advance(lexer);
    }
    
    // Handle decimal point
    if (current_char(lexer) == '.') {
        advance(lexer);
        while (is_digit(current_char(lexer))) {
            advance(lexer);
        }
    }
    
    // Handle float suffix
    if (current_char(lexer) == 'f') {
        advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* text = malloc(length + 1);
    strncpy(text, lexer->source + start_pos, length);
    text[length] = '\0';
    
    Token* token = token_create(TOKEN_NUMBER, text, start_line, start_column);
    free(text);
    return token;
}

// Read string
static Token* read_string(Lexer* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    advance(lexer); // Skip opening quote
    int start_pos = lexer->position;
    
    while (current_char(lexer) && current_char(lexer) != '"') {
        if (current_char(lexer) == '\\') {
            advance(lexer); // Skip escape character
        }
        advance(lexer);
    }
    
    if (current_char(lexer) == '"') {
        int length = lexer->position - start_pos;
        char* text = malloc(length + 1);
        strncpy(text, lexer->source + start_pos, length);
        text[length] = '\0';
        
        advance(lexer); // Skip closing quote
        
        Token* token = token_create(TOKEN_STRING, text, start_line, start_column);
        free(text);
        return token;
    } else {
        lexer_error(lexer, "Unterminated string literal");
        return token_create(TOKEN_ERROR, NULL, start_line, start_column);
    }
}

// Main tokenization function
Token* lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->source_length) {
        return token_create(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    
    char ch = current_char(lexer);
    int line = lexer->line;
    int column = lexer->column;
    
    // Single character tokens
    switch (ch) {
        case '\n': advance(lexer); return token_create(TOKEN_NEWLINE, "\n", line, column);
        case '+': advance(lexer); return token_create(TOKEN_PLUS, "+", line, column);
        case '-': advance(lexer); return token_create(TOKEN_MINUS, "-", line, column);
        case '*': advance(lexer); return token_create(TOKEN_MULTIPLY, "*", line, column);
        case '/': advance(lexer); return token_create(TOKEN_DIVIDE, "/", line, column);
        case '%': advance(lexer); return token_create(TOKEN_MODULO, "%", line, column);
        case '(': advance(lexer); return token_create(TOKEN_LEFT_PAREN, "(", line, column);
        case ')': advance(lexer); return token_create(TOKEN_RIGHT_PAREN, ")", line, column);
        case '[': advance(lexer); return token_create(TOKEN_LEFT_BRACKET, "[", line, column);
        case ']': advance(lexer); return token_create(TOKEN_RIGHT_BRACKET, "]", line, column);
        case '{': advance(lexer); return token_create(TOKEN_LEFT_BRACE, "{", line, column);
        case '}': advance(lexer); return token_create(TOKEN_RIGHT_BRACE, "}", line, column);
        case ',': advance(lexer); return token_create(TOKEN_COMMA, ",", line, column);
        case '.': advance(lexer); return token_create(TOKEN_DOT, ".", line, column);
        case ':': advance(lexer); return token_create(TOKEN_COLON, ":", line, column);
        case ';': advance(lexer); return token_create(TOKEN_SEMICOLON, ";", line, column);
    }
    
    // Multi-character tokens
    if (ch == '=') {
        advance(lexer);
        if (current_char(lexer) == '=') {
            advance(lexer);
            return token_create(TOKEN_EQ, "==", line, column);
        }
        return token_create(TOKEN_ASSIGN, "=", line, column);
    }
    
    if (ch == '!') {
        advance(lexer);
        if (current_char(lexer) == '=') {
            advance(lexer);
            return token_create(TOKEN_NE, "!=", line, column);
        }
        return token_create(TOKEN_NOT, "!", line, column);
    }
    
    if (ch == '<') {
        advance(lexer);
        if (current_char(lexer) == '=') {
            advance(lexer);
            return token_create(TOKEN_LE, "<=", line, column);
        }
        return token_create(TOKEN_LT, "<", line, column);
    }
    
    if (ch == '>') {
        advance(lexer);
        if (current_char(lexer) == '=') {
            advance(lexer);
            return token_create(TOKEN_GE, ">=", line, column);
        }
        return token_create(TOKEN_GT, ">", line, column);
    }
    
    // Comments
    if (ch == '#') {
        int start_pos = lexer->position;
        while (current_char(lexer) && current_char(lexer) != '\n') {
            advance(lexer);
        }
        int length = lexer->position - start_pos;
        char* text = malloc(length + 1);
        strncpy(text, lexer->source + start_pos, length);
        text[length] = '\0';
        
        Token* token = token_create(TOKEN_COMMENT, text, line, column);
        free(text);
        return token;
    }
    
    // Strings
    if (ch == '"') {
        return read_string(lexer);
    }
    
    // Numbers
    if (is_digit(ch)) {
        return read_number(lexer);
    }
    
    // Identifiers and keywords
    if (is_alpha(ch) || ch == '_') {
        return read_identifier(lexer);
    }
    
    // Unknown character
    advance(lexer);
    return token_create(TOKEN_ERROR, NULL, line, column);
}

// Convert token type to string
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_FUNC: return "FUNC";
        case TOKEN_FUN: return "FUN";
        case TOKEN_FU: return "FU";
        case TOKEN_VAR: return "VAR";
        case TOKEN_CONST: return "CONST";
        case TOKEN_IF: return "IF";
        case TOKEN_ELIF: return "ELIF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_IN: return "IN";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_IMPORT: return "IMPORT";
        case TOKEN_PARALLEL: return "PARALLEL";
        case TOKEN_UNSAFE: return "UNSAFE";
        case TOKEN_ASYNC: return "ASYNC";
        case TOKEN_AWAIT: return "AWAIT";
        case TOKEN_SPAWN: return "SPAWN";
        case TOKEN_MATCH: return "MATCH";
        case TOKEN_SOME: return "SOME";
        case TOKEN_NONE: return "NONE";
        case TOKEN_OK: return "OK";
        case TOKEN_ERR: return "ERR";
        case TOKEN_OPTION: return "OPTION";
        case TOKEN_RESULT: return "RESULT";
        case TOKEN_TEST: return "TEST";
        case TOKEN_BENCH: return "BENCH";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STR: return "STR";
        case TOKEN_STRING_TYPE: return "STRING_TYPE";
        case TOKEN_BOOL: return "BOOL";
        case TOKEN_LIST: return "LIST";
        case TOKEN_DICT: return "DICT";
        case TOKEN_SET: return "SET";
        case TOKEN_TUPLE: return "TUPLE";
        case TOKEN_FUTURE: return "FUTURE";
        case TOKEN_CHANNEL: return "CHANNEL";
        case TOKEN_TASK: return "TASK";
        case TOKEN_VEC2: return "VEC2";
        case TOKEN_VEC3: return "VEC3";
        case TOKEN_VEC4: return "VEC4";
        case TOKEN_MATRIX4: return "MATRIX4";
        case TOKEN_COLOR: return "COLOR";
        case TOKEN_TIME: return "TIME";
        case TOKEN_REF: return "REF";
        case TOKEN_MUT: return "MUT";
        case TOKEN_OS: return "OS";
        case TOKEN_SYS: return "SYS";
        case TOKEN_ENV: return "ENV";
        case TOKEN_PROCESS: return "PROCESS";
        case TOKEN_STD: return "STD";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MODULO: return "MODULO";
        case TOKEN_POWER: return "POWER";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQ: return "EQ";
        case TOKEN_NE: return "NE";
        case TOKEN_LT: return "LT";
        case TOKEN_LE: return "LE";
        case TOKEN_GT: return "GT";
        case TOKEN_GE: return "GE";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_COLON: return "COLON";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_NEWLINE: return "NEWLINE";
        case TOKEN_INDENT: return "INDENT";
        case TOKEN_DEDENT: return "DEDENT";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Error handling
void lexer_error(Lexer* lexer, const char* message) {
    lexer->has_errors = true;
    if (lexer->error_message) free(lexer->error_message);
    lexer->error_message = malloc(strlen(message) + 1);
    strcpy(lexer->error_message, message);
}
