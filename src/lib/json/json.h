#ifndef GPLANG_JSON_H
#define GPLANG_JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// JSON Value Types
typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

// Forward declaration
typedef struct JsonValue JsonValue;

// JSON Object Entry
typedef struct JsonObjectEntry {
    char* key;
    JsonValue* value;
    struct JsonObjectEntry* next;
} JsonObjectEntry;

// JSON Array Entry
typedef struct JsonArrayEntry {
    JsonValue* value;
    struct JsonArrayEntry* next;
} JsonArrayEntry;

// JSON Value
struct JsonValue {
    JsonType type;
    union {
        bool bool_value;
        double number_value;
        char* string_value;
        JsonObjectEntry* object_value;
        JsonArrayEntry* array_value;
    } data;
    int array_length;      // For arrays
    int object_length;     // For objects
};

// JSON Parser
typedef struct {
    const char* input;
    size_t position;
    size_t length;
    char* error_message;
    int line;
    int column;
} JsonParser;

// JSON Creation Functions
JsonValue* json_create_null(void);
JsonValue* json_create_bool(bool value);
JsonValue* json_create_number(double value);
JsonValue* json_create_string(const char* value);
JsonValue* json_create_array(void);
JsonValue* json_create_object(void);

// JSON Destruction
void json_destroy(JsonValue* value);

// JSON Parsing
JsonValue* json_parse(const char* json_string);
JsonValue* json_parse_file(const char* filename);
JsonParser* json_parser_create(const char* input);
void json_parser_destroy(JsonParser* parser);
JsonValue* json_parser_parse(JsonParser* parser);

// JSON Serialization
char* json_stringify(JsonValue* value);
char* json_stringify_pretty(JsonValue* value);
int json_write_file(JsonValue* value, const char* filename);
int json_write_file_pretty(JsonValue* value, const char* filename);

// JSON Object Operations
int json_object_set(JsonValue* object, const char* key, JsonValue* value);
JsonValue* json_object_get(JsonValue* object, const char* key);
int json_object_has(JsonValue* object, const char* key);
int json_object_remove(JsonValue* object, const char* key);
char** json_object_keys(JsonValue* object, int* count);
JsonValue** json_object_values(JsonValue* object, int* count);
int json_object_size(JsonValue* object);
void json_object_clear(JsonValue* object);

// JSON Object Convenience Functions
int json_object_set_null(JsonValue* object, const char* key);
int json_object_set_bool(JsonValue* object, const char* key, bool value);
int json_object_set_number(JsonValue* object, const char* key, double value);
int json_object_set_string(JsonValue* object, const char* key, const char* value);
int json_object_set_array(JsonValue* object, const char* key, JsonValue* array);
int json_object_set_object(JsonValue* object, const char* key, JsonValue* nested_object);

bool json_object_get_bool(JsonValue* object, const char* key, bool default_value);
double json_object_get_number(JsonValue* object, const char* key, double default_value);
char* json_object_get_string(JsonValue* object, const char* key, const char* default_value);
JsonValue* json_object_get_array(JsonValue* object, const char* key);
JsonValue* json_object_get_object(JsonValue* object, const char* key);

// JSON Array Operations
int json_array_append(JsonValue* array, JsonValue* value);
int json_array_prepend(JsonValue* array, JsonValue* value);
int json_array_insert(JsonValue* array, int index, JsonValue* value);
JsonValue* json_array_get(JsonValue* array, int index);
int json_array_set(JsonValue* array, int index, JsonValue* value);
int json_array_remove(JsonValue* array, int index);
int json_array_size(JsonValue* array);
void json_array_clear(JsonValue* array);

// JSON Array Convenience Functions
int json_array_append_null(JsonValue* array);
int json_array_append_bool(JsonValue* array, bool value);
int json_array_append_number(JsonValue* array, double value);
int json_array_append_string(JsonValue* array, const char* value);
int json_array_append_array(JsonValue* array, JsonValue* nested_array);
int json_array_append_object(JsonValue* array, JsonValue* object);

bool json_array_get_bool(JsonValue* array, int index, bool default_value);
double json_array_get_number(JsonValue* array, int index, double default_value);
char* json_array_get_string(JsonValue* array, int index, const char* default_value);
JsonValue* json_array_get_array(JsonValue* array, int index);
JsonValue* json_array_get_object(JsonValue* array, int index);

// JSON Type Checking
bool json_is_null(JsonValue* value);
bool json_is_bool(JsonValue* value);
bool json_is_number(JsonValue* value);
bool json_is_string(JsonValue* value);
bool json_is_array(JsonValue* value);
bool json_is_object(JsonValue* value);

// JSON Value Extraction
bool json_get_bool(JsonValue* value);
double json_get_number(JsonValue* value);
char* json_get_string(JsonValue* value);

// JSON Path Operations (JSONPath-like)
JsonValue* json_path_get(JsonValue* root, const char* path);
int json_path_set(JsonValue* root, const char* path, JsonValue* value);
int json_path_exists(JsonValue* root, const char* path);
int json_path_remove(JsonValue* root, const char* path);

// JSON Comparison
bool json_equals(JsonValue* a, JsonValue* b);
bool json_deep_equals(JsonValue* a, JsonValue* b);

// JSON Merging
JsonValue* json_merge(JsonValue* target, JsonValue* source);
JsonValue* json_deep_merge(JsonValue* target, JsonValue* source);

// JSON Validation
bool json_validate(const char* json_string);
bool json_validate_schema(JsonValue* data, JsonValue* schema);

// JSON Utilities
JsonValue* json_clone(JsonValue* value);
JsonValue* json_deep_clone(JsonValue* value);
size_t json_memory_usage(JsonValue* value);
void json_print_debug(JsonValue* value);

// JSON Iteration
typedef struct {
    JsonValue* current;
    JsonObjectEntry* object_entry;
    JsonArrayEntry* array_entry;
    int array_index;
    bool is_object;
} JsonIterator;

JsonIterator* json_iterator_create(JsonValue* value);
bool json_iterator_has_next(JsonIterator* iterator);
JsonValue* json_iterator_next(JsonIterator* iterator);
char* json_iterator_current_key(JsonIterator* iterator);
void json_iterator_destroy(JsonIterator* iterator);

// JSON Error Handling
typedef enum {
    JSON_ERROR_NONE,
    JSON_ERROR_INVALID_JSON,
    JSON_ERROR_UNEXPECTED_TOKEN,
    JSON_ERROR_UNEXPECTED_END,
    JSON_ERROR_INVALID_NUMBER,
    JSON_ERROR_INVALID_STRING,
    JSON_ERROR_INVALID_ESCAPE,
    JSON_ERROR_MEMORY_ERROR,
    JSON_ERROR_TYPE_MISMATCH,
    JSON_ERROR_INDEX_OUT_OF_BOUNDS,
    JSON_ERROR_KEY_NOT_FOUND
} JsonError;

JsonError json_get_last_error(void);
char* json_get_error_string(JsonError error);
void json_set_error(JsonError error);

#endif // GPLANG_JSON_H
