#include "error_handler.h"

// Simple strdup implementation
static char* gp_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) strcpy(copy, s);
    return copy;
}

// Error handler initialization
ErrorHandler* error_handler_create(void) {
    ErrorHandler* handler = malloc(sizeof(ErrorHandler));
    if (!handler) return NULL;
    
    // Initialize configuration with defaults
    handler->config.enable_colors = true;
    handler->config.show_context = true;
    handler->config.show_suggestions = true;
    handler->config.log_to_file = false;
    handler->config.abort_on_error = false;
    handler->config.collect_statistics = true;
    handler->config.log_file_path = NULL;
    handler->config.min_level = ERROR_LEVEL_WARNING;
    handler->config.max_errors = 100;
    handler->config.max_warnings = 500;
    
    // Initialize statistics
    memset(&handler->stats, 0, sizeof(ErrorStats));
    
    // Initialize error stack
    handler->error_capacity = 100;
    handler->error_stack = malloc(handler->error_capacity * sizeof(ErrorInfo));
    handler->error_count = 0;
    handler->log_file = NULL;
    handler->initialized = true;
    
    return handler;
}

void error_handler_destroy(ErrorHandler* handler) {
    if (!handler) return;
    
    // Free error stack
    for (int i = 0; i < handler->error_count; i++) {
        free(handler->error_stack[i].message);
        free(handler->error_stack[i].file);
        free(handler->error_stack[i].function);
        free(handler->error_stack[i].context);
        free(handler->error_stack[i].suggestion);
    }
    free(handler->error_stack);
    
    // Close log file
    if (handler->log_file && handler->log_file != stdout && handler->log_file != stderr) {
        fclose(handler->log_file);
    }
    
    free(handler->config.log_file_path);
    free(handler->stats.most_common_error);
    free(handler);
}

void error_handler_configure(ErrorHandler* handler, ErrorConfig* config) {
    if (!handler || !config) return;
    
    handler->config = *config;
    
    // Open log file if needed
    if (config->log_to_file && config->log_file_path) {
        handler->log_file = fopen(config->log_file_path, "a");
        if (!handler->log_file) {
            fprintf(stderr, "Warning: Could not open log file %s\n", config->log_file_path);
            handler->config.log_to_file = false;
        }
    }
}

// Error reporting functions
void error_report(ErrorHandler* handler, ErrorCode code, ErrorLevel level, 
                 const char* file, int line, const char* function, 
                 const char* format, ...) {
    if (!handler || level < handler->config.min_level) return;
    
    // Check error limits
    if (level == ERROR_LEVEL_ERROR && handler->stats.total_errors >= handler->config.max_errors) {
        return;
    }
    if (level == ERROR_LEVEL_WARNING && handler->stats.total_warnings >= handler->config.max_warnings) {
        return;
    }
    
    // Expand error stack if needed
    if (handler->error_count >= handler->error_capacity) {
        handler->error_capacity *= 2;
        handler->error_stack = realloc(handler->error_stack, 
                                     handler->error_capacity * sizeof(ErrorInfo));
    }
    
    // Create error info
    ErrorInfo* error = &handler->error_stack[handler->error_count++];
    error->code = code;
    error->level = level;
    error->category = error_code_to_category(code);
    error->file = gp_strdup(file);
    error->line = line;
    error->column = 0;
    error->function = gp_strdup(function);
    error->timestamp = time(NULL);
    error->context = NULL;
    error->suggestion = gp_strdup(error_get_suggestion(code));
    
    // Format message
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    error->message = gp_strdup(buffer);
    va_end(args);
    
    // Update statistics
    if (handler->config.collect_statistics) {
        if (level == ERROR_LEVEL_ERROR) {
            handler->stats.total_errors++;
            handler->stats.errors_by_category[error->category]++;
        } else if (level == ERROR_LEVEL_WARNING) {
            handler->stats.total_warnings++;
            handler->stats.warnings_by_category[error->category]++;
        }
        handler->stats.error_count_by_code[code]++;
    }
    
    // Print error immediately
    error_print_error(handler, error);
    
    // Log to file if enabled
    if (handler->config.log_to_file && handler->log_file) {
        char* formatted = error_format_message(handler, error);
        fprintf(handler->log_file, "%s\n", formatted);
        fflush(handler->log_file);
        free(formatted);
    }
    
    // Abort if configured to do so
    if (handler->config.abort_on_error && level >= ERROR_LEVEL_ERROR) {
        error_print_summary(handler);
        exit(1);
    }
}

// Error formatting and display
char* error_format_message(ErrorHandler* handler, ErrorInfo* error) {
    if (!handler || !error) return NULL;
    
    char* buffer = malloc(2048);
    if (!buffer) return NULL;
    
    const char* level_str = error_level_to_string(error->level);
    const char* category_str = error_category_to_string(error->category);
    
    // Color codes
    const char* color_reset = handler->config.enable_colors ? "\033[0m" : "";
    const char* color_error = handler->config.enable_colors ? "\033[1;31m" : "";
    const char* color_warning = handler->config.enable_colors ? "\033[1;33m" : "";
    const char* color_info = handler->config.enable_colors ? "\033[1;36m" : "";
    
    const char* color = color_reset;
    switch (error->level) {
        case ERROR_LEVEL_ERROR:
        case ERROR_LEVEL_FATAL:
            color = color_error;
            break;
        case ERROR_LEVEL_WARNING:
            color = color_warning;
            break;
        case ERROR_LEVEL_INFO:
            color = color_info;
            break;
        default:
            color = color_reset;
            break;
    }
    
    snprintf(buffer, 2048, 
             "%s[%s:%s] %s:%d in %s(): %s%s",
             color, level_str, category_str,
             error->file ? error->file : "unknown",
             error->line,
             error->function ? error->function : "unknown",
             error->message ? error->message : "No message",
             color_reset);
    
    return buffer;
}

void error_print_error(ErrorHandler* handler, ErrorInfo* error) {
    if (!handler || !error) return;
    
    char* formatted = error_format_message(handler, error);
    if (formatted) {
        fprintf(stderr, "%s\n", formatted);
        
        // Show suggestion if available and enabled
        if (handler->config.show_suggestions && error->suggestion) {
            fprintf(stderr, "  Suggestion: %s\n", error->suggestion);
        }
        
        free(formatted);
    }
}

void error_print_summary(ErrorHandler* handler) {
    if (!handler) return;
    
    printf("\n=== GPLANG Error Summary ===\n");
    printf("Total Errors: %d\n", handler->stats.total_errors);
    printf("Total Warnings: %d\n", handler->stats.total_warnings);
    printf("============================\n\n");
}

// Utility functions
const char* error_level_to_string(ErrorLevel level) {
    switch (level) {
        case ERROR_LEVEL_DEBUG: return "DEBUG";
        case ERROR_LEVEL_INFO: return "INFO";
        case ERROR_LEVEL_WARNING: return "WARNING";
        case ERROR_LEVEL_ERROR: return "ERROR";
        case ERROR_LEVEL_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

const char* error_category_to_string(ErrorCategory category) {
    switch (category) {
        case ERROR_CAT_LEXICAL: return "LEXICAL";
        case ERROR_CAT_SYNTAX: return "SYNTAX";
        case ERROR_CAT_SEMANTIC: return "SEMANTIC";
        case ERROR_CAT_TYPE: return "TYPE";
        case ERROR_CAT_OPTIMIZATION: return "OPTIMIZATION";
        case ERROR_CAT_CODEGEN: return "CODEGEN";
        case ERROR_CAT_RUNTIME: return "RUNTIME";
        case ERROR_CAT_MEMORY: return "MEMORY";
        case ERROR_CAT_IO: return "IO";
        case ERROR_CAT_SYSTEM: return "SYSTEM";
        case ERROR_CAT_INTERNAL: return "INTERNAL";
        default: return "UNKNOWN";
    }
}

ErrorCategory error_code_to_category(ErrorCode code) {
    if (code >= 1000 && code < 1100) return ERROR_CAT_LEXICAL;
    if (code >= 1100 && code < 1200) return ERROR_CAT_SYNTAX;
    if (code >= 1200 && code < 1300) return ERROR_CAT_SEMANTIC;
    if (code >= 1300 && code < 1400) return ERROR_CAT_TYPE;
    if (code >= 1400 && code < 1500) return ERROR_CAT_OPTIMIZATION;
    if (code >= 1500 && code < 1600) return ERROR_CAT_CODEGEN;
    if (code >= 1600 && code < 1700) return ERROR_CAT_RUNTIME;
    if (code >= 1700 && code < 1800) return ERROR_CAT_MEMORY;
    if (code >= 1800 && code < 1900) return ERROR_CAT_IO;
    if (code >= 1900 && code < 2000) return ERROR_CAT_SYSTEM;
    if (code >= 2000 && code < 2100) return ERROR_CAT_INTERNAL;
    return ERROR_CAT_INTERNAL;
}

const char* error_get_suggestion(ErrorCode code) {
    switch (code) {
        case ERR_UNDEFINED_VARIABLE:
            return "Check variable name spelling or declare the variable before use";
        case ERR_TYPE_MISMATCH:
            return "Ensure operand types are compatible or add explicit type conversion";
        case ERR_MISSING_SEMICOLON:
            return "Add a semicolon ';' at the end of the statement";
        case ERR_DIVISION_BY_ZERO:
            return "Add a check to ensure the divisor is not zero";
        case ERR_OUT_OF_MEMORY:
            return "Reduce memory usage or increase available memory";
        case ERR_FILE_NOT_FOUND:
            return "Check file path and ensure the file exists";
        default:
            return "Consult the GPLANG documentation for more information";
    }
}

// Configuration presets
ErrorConfig error_config_development(void) {
    ErrorConfig config = {0};
    config.enable_colors = true;
    config.show_context = true;
    config.show_suggestions = true;
    config.log_to_file = false;
    config.abort_on_error = false;
    config.collect_statistics = true;
    config.min_level = ERROR_LEVEL_DEBUG;
    config.max_errors = 1000;
    config.max_warnings = 2000;
    return config;
}

// Analysis functions
bool error_has_errors(ErrorHandler* handler) {
    return handler && handler->stats.total_errors > 0;
}

bool error_has_warnings(ErrorHandler* handler) {
    return handler && handler->stats.total_warnings > 0;
}

bool error_should_abort(ErrorHandler* handler) {
    return handler && handler->config.abort_on_error && error_has_errors(handler);
}
