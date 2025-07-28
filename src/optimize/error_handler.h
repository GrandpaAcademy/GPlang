#ifndef GPLANG_ERROR_HANDLER_H
#define GPLANG_ERROR_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

// Error Levels
typedef enum {
    ERROR_LEVEL_DEBUG = 0,
    ERROR_LEVEL_INFO = 1,
    ERROR_LEVEL_WARNING = 2,
    ERROR_LEVEL_ERROR = 3,
    ERROR_LEVEL_FATAL = 4
} ErrorLevel;

// Error Categories
typedef enum {
    ERROR_CAT_LEXICAL,      // Lexical analysis errors
    ERROR_CAT_SYNTAX,       // Syntax errors
    ERROR_CAT_SEMANTIC,     // Semantic analysis errors
    ERROR_CAT_TYPE,         // Type checking errors
    ERROR_CAT_OPTIMIZATION, // Optimization errors
    ERROR_CAT_CODEGEN,      // Code generation errors
    ERROR_CAT_RUNTIME,      // Runtime errors
    ERROR_CAT_MEMORY,       // Memory management errors
    ERROR_CAT_IO,           // Input/output errors
    ERROR_CAT_SYSTEM,       // System errors
    ERROR_CAT_INTERNAL      // Internal compiler errors
} ErrorCategory;

// Error Codes
typedef enum {
    ERR_SUCCESS = 0,
    
    // Lexical errors (1000-1099)
    ERR_INVALID_TOKEN = 1000,
    ERR_UNTERMINATED_STRING = 1001,
    ERR_INVALID_NUMBER = 1002,
    ERR_INVALID_CHARACTER = 1003,
    
    // Syntax errors (1100-1199)
    ERR_UNEXPECTED_TOKEN = 1100,
    ERR_MISSING_SEMICOLON = 1101,
    ERR_MISSING_BRACE = 1102,
    ERR_MISSING_PARENTHESIS = 1103,
    ERR_INVALID_EXPRESSION = 1104,
    
    // Semantic errors (1200-1299)
    ERR_UNDEFINED_VARIABLE = 1200,
    ERR_UNDEFINED_FUNCTION = 1201,
    ERR_REDEFINED_VARIABLE = 1202,
    ERR_REDEFINED_FUNCTION = 1203,
    ERR_INVALID_ASSIGNMENT = 1204,
    
    // Type errors (1300-1399)
    ERR_TYPE_MISMATCH = 1300,
    ERR_INVALID_CAST = 1301,
    ERR_INCOMPATIBLE_TYPES = 1302,
    ERR_INVALID_OPERATION = 1303,
    
    // Optimization errors (1400-1499)
    ERR_OPT_FAILED = 1400,
    ERR_OPT_INFINITE_LOOP = 1401,
    ERR_OPT_STACK_OVERFLOW = 1402,
    ERR_OPT_MEMORY_LIMIT = 1403,
    
    // Code generation errors (1500-1599)
    ERR_CODEGEN_FAILED = 1500,
    ERR_INVALID_TARGET = 1501,
    ERR_UNSUPPORTED_FEATURE = 1502,
    
    // Runtime errors (1600-1699)
    ERR_DIVISION_BY_ZERO = 1600,
    ERR_NULL_POINTER = 1601,
    ERR_ARRAY_BOUNDS = 1602,
    ERR_STACK_OVERFLOW_RT = 1603,
    
    // Memory errors (1700-1799)
    ERR_OUT_OF_MEMORY = 1700,
    ERR_MEMORY_LEAK = 1701,
    ERR_DOUBLE_FREE = 1702,
    ERR_INVALID_FREE = 1703,
    
    // I/O errors (1800-1899)
    ERR_FILE_NOT_FOUND = 1800,
    ERR_FILE_READ_ERROR = 1801,
    ERR_FILE_WRITE_ERROR = 1802,
    ERR_PERMISSION_DENIED = 1803,
    
    // System errors (1900-1999)
    ERR_SYSTEM_CALL_FAILED = 1900,
    ERR_RESOURCE_UNAVAILABLE = 1901,
    ERR_TIMEOUT = 1902,
    
    // Internal errors (2000-2099)
    ERR_INTERNAL_ERROR = 2000,
    ERR_ASSERTION_FAILED = 2001,
    ERR_CORRUPTED_DATA = 2002,
    ERR_INVALID_STATE = 2003
} ErrorCode;

// Error Information
typedef struct {
    ErrorCode code;
    ErrorLevel level;
    ErrorCategory category;
    char* message;
    char* file;
    int line;
    int column;
    char* function;
    time_t timestamp;
    char* context;
    char* suggestion;
} ErrorInfo;

// Error Handler Configuration
typedef struct {
    bool enable_colors;
    bool show_context;
    bool show_suggestions;
    bool log_to_file;
    bool abort_on_error;
    bool collect_statistics;
    char* log_file_path;
    ErrorLevel min_level;
    int max_errors;
    int max_warnings;
} ErrorConfig;

// Error Statistics
typedef struct {
    int total_errors;
    int total_warnings;
    int errors_by_category[ERROR_CAT_INTERNAL + 1];
    int warnings_by_category[ERROR_CAT_INTERNAL + 1];
    double total_time;
    char* most_common_error;
    int error_count_by_code[2100];
} ErrorStats;

// Error Handler Context
typedef struct {
    ErrorConfig config;
    ErrorStats stats;
    ErrorInfo* error_stack;
    int error_count;
    int error_capacity;
    FILE* log_file;
    bool initialized;
} ErrorHandler;

// Function declarations

// Error handler initialization and cleanup
ErrorHandler* error_handler_create(void);
void error_handler_destroy(ErrorHandler* handler);
void error_handler_configure(ErrorHandler* handler, ErrorConfig* config);
void error_handler_reset(ErrorHandler* handler);

// Error reporting functions
void error_report(ErrorHandler* handler, ErrorCode code, ErrorLevel level, 
                 const char* file, int line, const char* function, 
                 const char* format, ...);
void error_report_with_context(ErrorHandler* handler, ErrorCode code, ErrorLevel level,
                              const char* file, int line, int column, const char* function,
                              const char* context, const char* format, ...);
void error_report_with_suggestion(ErrorHandler* handler, ErrorCode code, ErrorLevel level,
                                 const char* file, int line, const char* function,
                                 const char* suggestion, const char* format, ...);

// Convenience macros for error reporting
#define ERROR_FATAL(handler, code, ...) \
    error_report(handler, code, ERROR_LEVEL_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define ERROR_ERROR(handler, code, ...) \
    error_report(handler, code, ERROR_LEVEL_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define ERROR_WARNING(handler, code, ...) \
    error_report(handler, code, ERROR_LEVEL_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define ERROR_INFO(handler, code, ...) \
    error_report(handler, code, ERROR_LEVEL_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define ERROR_DEBUG(handler, code, ...) \
    error_report(handler, code, ERROR_LEVEL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)

// Error recovery functions
bool error_can_recover(ErrorHandler* handler, ErrorCode code);
bool error_attempt_recovery(ErrorHandler* handler, ErrorCode code);
void error_skip_to_sync_point(ErrorHandler* handler);

// Error analysis functions
bool error_has_errors(ErrorHandler* handler);
bool error_has_warnings(ErrorHandler* handler);
bool error_should_abort(ErrorHandler* handler);
int error_get_count(ErrorHandler* handler, ErrorLevel level);
ErrorInfo* error_get_last(ErrorHandler* handler);
ErrorInfo* error_get_by_index(ErrorHandler* handler, int index);

// Error formatting and display
char* error_format_message(ErrorHandler* handler, ErrorInfo* error);
void error_print_error(ErrorHandler* handler, ErrorInfo* error);
void error_print_all_errors(ErrorHandler* handler);
void error_print_summary(ErrorHandler* handler);
void error_print_statistics(ErrorHandler* handler);

// Error utilities
const char* error_level_to_string(ErrorLevel level);
const char* error_category_to_string(ErrorCategory category);
const char* error_code_to_string(ErrorCode code);
const char* error_get_suggestion(ErrorCode code);
ErrorCategory error_code_to_category(ErrorCode code);

// Error configuration presets
ErrorConfig error_config_development(void);  // Verbose, helpful for development
ErrorConfig error_config_production(void);   // Minimal, for production
ErrorConfig error_config_debug(void);        // Maximum detail for debugging
ErrorConfig error_config_silent(void);       // Minimal output

// Performance error handling (zero-cost when disabled)
#ifdef GPLANG_FAST_MODE
#define FAST_ERROR_CHECK(condition, handler, code, ...) \
    do { if (!(condition)) { /* No-op in fast mode */ } } while(0)
#else
#define FAST_ERROR_CHECK(condition, handler, code, ...) \
    do { if (!(condition)) { ERROR_ERROR(handler, code, __VA_ARGS__); } } while(0)
#endif

// Memory error detection
void error_enable_memory_tracking(ErrorHandler* handler);
void error_check_memory_leaks(ErrorHandler* handler);
void error_report_memory_usage(ErrorHandler* handler);

#endif // GPLANG_ERROR_HANDLER_H
