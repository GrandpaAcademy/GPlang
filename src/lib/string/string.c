#include "string.h"
#include <time.h>
#include <stdint.h>
#include <strings.h>

// String creation and destruction
char* gp_str_create(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* new_str = malloc(len + 1);
    if (!new_str) return NULL;
    
    strcpy(new_str, str);
    return new_str;
}

char* gp_str_create_empty(size_t capacity) {
    char* str = malloc(capacity + 1);
    if (!str) return NULL;
    
    str[0] = '\0';
    return str;
}

char* gp_str_create_from_char(char c, size_t count) {
    char* str = malloc(count + 1);
    if (!str) return NULL;
    
    memset(str, c, count);
    str[count] = '\0';
    return str;
}

void gp_str_destroy(char* str) {
    free(str);
}

// String information
size_t gp_str_length(const char* str) {
    return str ? strlen(str) : 0;
}

bool gp_str_is_empty(const char* str) {
    return !str || str[0] == '\0';
}

bool gp_str_is_null_or_empty(const char* str) {
    return gp_str_is_empty(str);
}

// String comparison
int gp_str_compare(const char* str1, const char* str2) {
    if (!str1 && !str2) return 0;
    if (!str1) return -1;
    if (!str2) return 1;
    return strcmp(str1, str2);
}

int gp_str_compare_ignore_case(const char* str1, const char* str2) {
    if (!str1 && !str2) return 0;
    if (!str1) return -1;
    if (!str2) return 1;
    return strcasecmp(str1, str2);
}

bool gp_str_equals(const char* str1, const char* str2) {
    return gp_str_compare(str1, str2) == 0;
}

bool gp_str_equals_ignore_case(const char* str1, const char* str2) {
    return gp_str_compare_ignore_case(str1, str2) == 0;
}

bool gp_str_starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return false;
    
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    
    if (prefix_len > str_len) return false;
    return strncmp(str, prefix, prefix_len) == 0;
}

bool gp_str_ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    
    if (suffix_len > str_len) return false;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

// String searching
int gp_str_find(const char* str, const char* substr) {
    if (!str || !substr) return -1;
    
    char* pos = strstr(str, substr);
    return pos ? (int)(pos - str) : -1;
}

int gp_str_find_char(const char* str, char c) {
    if (!str) return -1;
    
    char* pos = strchr(str, c);
    return pos ? (int)(pos - str) : -1;
}

int gp_str_find_char_last(const char* str, char c) {
    if (!str) return -1;
    
    char* pos = strrchr(str, c);
    return pos ? (int)(pos - str) : -1;
}

bool gp_str_contains(const char* str, const char* substr) {
    return gp_str_find(str, substr) != -1;
}

// String modification
char* gp_str_concat(const char* str1, const char* str2) {
    if (!str1 && !str2) return NULL;
    if (!str1) return gp_str_create(str2);
    if (!str2) return gp_str_create(str1);
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = malloc(len1 + len2 + 1);
    if (!result) return NULL;
    
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

char* gp_str_replace(const char* str, const char* old_str, const char* new_str) {
    if (!str || !old_str || !new_str) return gp_str_create(str);
    
    char* pos = strstr(str, old_str);
    if (!pos) return gp_str_create(str);
    
    size_t old_len = strlen(old_str);
    size_t new_len = strlen(new_str);
    size_t prefix_len = pos - str;
    size_t suffix_len = strlen(pos + old_len);
    
    char* result = malloc(prefix_len + new_len + suffix_len + 1);
    if (!result) return NULL;
    
    strncpy(result, str, prefix_len);
    result[prefix_len] = '\0';
    strcat(result, new_str);
    strcat(result, pos + old_len);
    
    return result;
}

// String extraction
char* gp_str_substring(const char* str, size_t start, size_t length) {
    if (!str) return NULL;
    
    size_t str_len = strlen(str);
    if (start >= str_len) return gp_str_create("");
    
    if (start + length > str_len) {
        length = str_len - start;
    }
    
    char* result = malloc(length + 1);
    if (!result) return NULL;
    
    strncpy(result, str + start, length);
    result[length] = '\0';
    return result;
}

char* gp_str_left(const char* str, size_t count) {
    return gp_str_substring(str, 0, count);
}

char* gp_str_right(const char* str, size_t count) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (count >= len) return gp_str_create(str);
    
    return gp_str_substring(str, len - count, count);
}

char gp_str_char_at(const char* str, size_t index) {
    if (!str || index >= strlen(str)) return '\0';
    return str[index];
}

// String trimming
char* gp_str_trim(const char* str) {
    if (!str) return NULL;
    
    // Find start of non-whitespace
    while (*str && isspace(*str)) str++;
    
    if (*str == '\0') return gp_str_create("");
    
    // Find end of non-whitespace
    const char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    
    size_t len = end - str + 1;
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    strncpy(result, str, len);
    result[len] = '\0';
    return result;
}

// String case conversion
char* gp_str_to_upper(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = toupper(str[i]);
    }
    result[len] = '\0';
    return result;
}

char* gp_str_to_lower(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

// String splitting
StringArray gp_str_split(const char* str, const char* delimiter) {
    StringArray result = {NULL, 0};
    if (!str || !delimiter) return result;
    
    // Count occurrences
    size_t count = 1;
    char* temp = strstr(str, delimiter);
    while (temp) {
        count++;
        temp = strstr(temp + strlen(delimiter), delimiter);
    }
    
    result.parts = malloc(count * sizeof(char*));
    if (!result.parts) return result;
    
    char* str_copy = gp_str_create(str);
    char* token = strtok(str_copy, delimiter);
    size_t index = 0;
    
    while (token && index < count) {
        result.parts[index] = gp_str_create(token);
        token = strtok(NULL, delimiter);
        index++;
    }
    
    result.count = index;
    free(str_copy);
    return result;
}

void gp_str_array_destroy(StringArray* array) {
    if (!array || !array->parts) return;
    
    for (size_t i = 0; i < array->count; i++) {
        free(array->parts[i]);
    }
    free(array->parts);
    array->parts = NULL;
    array->count = 0;
}

// String formatting
char* gp_str_format(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    // Calculate required size
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    
    if (size < 0) {
        va_end(args);
        return NULL;
    }
    
    char* result = malloc(size + 1);
    if (!result) {
        va_end(args);
        return NULL;
    }
    
    vsnprintf(result, size + 1, format, args);
    va_end(args);
    return result;
}

char* gp_str_reverse(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = str[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}

// String validation
bool gp_str_is_alpha(const char* str) {
    if (!str || *str == '\0') return false;
    
    while (*str) {
        if (!isalpha(*str)) return false;
        str++;
    }
    return true;
}

bool gp_str_is_digit(const char* str) {
    if (!str || *str == '\0') return false;
    
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

// String conversion
int gp_str_to_int(const char* str) {
    return str ? atoi(str) : 0;
}

double gp_str_to_double(const char* str) {
    return str ? atof(str) : 0.0;
}

char* gp_int_to_str(int value) {
    return gp_str_format("%d", value);
}

char* gp_double_to_str(double value) {
    return gp_str_format("%.6f", value);
}

// String hashing
uint32_t gp_str_hash(const char* str) {
    if (!str) return 0;
    
    uint32_t hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

// String builder
StringBuilder* gp_str_builder_create(size_t initial_capacity) {
    StringBuilder* builder = malloc(sizeof(StringBuilder));
    if (!builder) return NULL;
    
    builder->capacity = initial_capacity > 0 ? initial_capacity : 64;
    builder->buffer = malloc(builder->capacity);
    if (!builder->buffer) {
        free(builder);
        return NULL;
    }
    
    builder->length = 0;
    builder->buffer[0] = '\0';
    return builder;
}

void gp_str_builder_destroy(StringBuilder* builder) {
    if (!builder) return;
    free(builder->buffer);
    free(builder);
}

void gp_str_builder_append(StringBuilder* builder, const char* str) {
    if (!builder || !str) return;
    
    size_t str_len = strlen(str);
    if (builder->length + str_len >= builder->capacity) {
        builder->capacity = (builder->length + str_len + 1) * 2;
        builder->buffer = realloc(builder->buffer, builder->capacity);
        if (!builder->buffer) return;
    }
    
    strcpy(builder->buffer + builder->length, str);
    builder->length += str_len;
}

char* gp_str_builder_to_string(StringBuilder* builder) {
    if (!builder) return NULL;
    return gp_str_create(builder->buffer);
}
