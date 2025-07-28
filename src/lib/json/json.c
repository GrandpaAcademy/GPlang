#include "json.h"

// Global error state
static JsonError last_error = JSON_ERROR_NONE;

// Simple strdup implementation
static char* gp_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) strcpy(copy, s);
    return copy;
}

// JSON Creation Functions
JsonValue* json_create_null(void) {
    JsonValue* value = malloc(sizeof(JsonValue));
    if (!value) return NULL;
    
    value->type = JSON_NULL;
    return value;
}

JsonValue* json_create_bool(bool bool_val) {
    JsonValue* value = malloc(sizeof(JsonValue));
    if (!value) return NULL;
    
    value->type = JSON_BOOL;
    value->data.bool_value = bool_val;
    return value;
}

JsonValue* json_create_number(double number_val) {
    JsonValue* value = malloc(sizeof(JsonValue));
    if (!value) return NULL;
    
    value->type = JSON_NUMBER;
    value->data.number_value = number_val;
    return value;
}

JsonValue* json_create_string(const char* string_val) {
    JsonValue* value = malloc(sizeof(JsonValue));
    if (!value) return NULL;
    
    value->type = JSON_STRING;
    value->data.string_value = gp_strdup(string_val);
    return value;
}

JsonValue* json_create_array(void) {
    JsonValue* value = malloc(sizeof(JsonValue));
    if (!value) return NULL;
    
    value->type = JSON_ARRAY;
    value->data.array_value = NULL;
    value->array_length = 0;
    return value;
}

JsonValue* json_create_object(void) {
    JsonValue* value = malloc(sizeof(JsonValue));
    if (!value) return NULL;
    
    value->type = JSON_OBJECT;
    value->data.object_value = NULL;
    value->object_length = 0;
    return value;
}

// JSON Destruction
void json_destroy(JsonValue* value) {
    if (!value) return;
    
    switch (value->type) {
        case JSON_STRING:
            free(value->data.string_value);
            break;
            
        case JSON_ARRAY: {
            JsonArrayEntry* entry = value->data.array_value;
            while (entry) {
                JsonArrayEntry* next = entry->next;
                json_destroy(entry->value);
                free(entry);
                entry = next;
            }
            break;
        }
        
        case JSON_OBJECT: {
            JsonObjectEntry* entry = value->data.object_value;
            while (entry) {
                JsonObjectEntry* next = entry->next;
                free(entry->key);
                json_destroy(entry->value);
                free(entry);
                entry = next;
            }
            break;
        }
        
        default:
            break;
    }
    
    free(value);
}

// JSON Object Operations
int json_object_set(JsonValue* object, const char* key, JsonValue* value) {
    if (!object || object->type != JSON_OBJECT || !key || !value) {
        return -1;
    }
    
    // Check if key already exists
    JsonObjectEntry* entry = object->data.object_value;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            json_destroy(entry->value);
            entry->value = value;
            return 0;
        }
        entry = entry->next;
    }
    
    // Create new entry
    JsonObjectEntry* new_entry = malloc(sizeof(JsonObjectEntry));
    if (!new_entry) return -1;
    
    new_entry->key = gp_strdup(key);
    new_entry->value = value;
    new_entry->next = object->data.object_value;
    object->data.object_value = new_entry;
    object->object_length++;
    
    return 0;
}

JsonValue* json_object_get(JsonValue* object, const char* key) {
    if (!object || object->type != JSON_OBJECT || !key) {
        return NULL;
    }
    
    JsonObjectEntry* entry = object->data.object_value;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;
}

int json_object_has(JsonValue* object, const char* key) {
    return json_object_get(object, key) != NULL;
}

int json_object_size(JsonValue* object) {
    if (!object || object->type != JSON_OBJECT) {
        return -1;
    }
    return object->object_length;
}

// JSON Object Convenience Functions
int json_object_set_string(JsonValue* object, const char* key, const char* value) {
    JsonValue* json_val = json_create_string(value);
    if (!json_val) return -1;
    return json_object_set(object, key, json_val);
}

int json_object_set_number(JsonValue* object, const char* key, double value) {
    JsonValue* json_val = json_create_number(value);
    if (!json_val) return -1;
    return json_object_set(object, key, json_val);
}

int json_object_set_bool(JsonValue* object, const char* key, bool value) {
    JsonValue* json_val = json_create_bool(value);
    if (!json_val) return -1;
    return json_object_set(object, key, json_val);
}

char* json_object_get_string(JsonValue* object, const char* key, const char* default_value) {
    JsonValue* value = json_object_get(object, key);
    if (value && value->type == JSON_STRING) {
        return value->data.string_value;
    }
    return (char*)default_value;
}

double json_object_get_number(JsonValue* object, const char* key, double default_value) {
    JsonValue* value = json_object_get(object, key);
    if (value && value->type == JSON_NUMBER) {
        return value->data.number_value;
    }
    return default_value;
}

bool json_object_get_bool(JsonValue* object, const char* key, bool default_value) {
    JsonValue* value = json_object_get(object, key);
    if (value && value->type == JSON_BOOL) {
        return value->data.bool_value;
    }
    return default_value;
}

// JSON Array Operations
int json_array_append(JsonValue* array, JsonValue* value) {
    if (!array || array->type != JSON_ARRAY || !value) {
        return -1;
    }
    
    JsonArrayEntry* new_entry = malloc(sizeof(JsonArrayEntry));
    if (!new_entry) return -1;
    
    new_entry->value = value;
    new_entry->next = NULL;
    
    if (!array->data.array_value) {
        array->data.array_value = new_entry;
    } else {
        JsonArrayEntry* entry = array->data.array_value;
        while (entry->next) {
            entry = entry->next;
        }
        entry->next = new_entry;
    }
    
    array->array_length++;
    return 0;
}

JsonValue* json_array_get(JsonValue* array, int index) {
    if (!array || array->type != JSON_ARRAY || index < 0) {
        return NULL;
    }
    
    JsonArrayEntry* entry = array->data.array_value;
    int current_index = 0;
    
    while (entry && current_index < index) {
        entry = entry->next;
        current_index++;
    }
    
    return entry ? entry->value : NULL;
}

int json_array_size(JsonValue* array) {
    if (!array || array->type != JSON_ARRAY) {
        return -1;
    }
    return array->array_length;
}

// JSON Array Convenience Functions
int json_array_append_string(JsonValue* array, const char* value) {
    JsonValue* json_val = json_create_string(value);
    if (!json_val) return -1;
    return json_array_append(array, json_val);
}

int json_array_append_number(JsonValue* array, double value) {
    JsonValue* json_val = json_create_number(value);
    if (!json_val) return -1;
    return json_array_append(array, json_val);
}

int json_array_append_bool(JsonValue* array, bool value) {
    JsonValue* json_val = json_create_bool(value);
    if (!json_val) return -1;
    return json_array_append(array, json_val);
}

// JSON Type Checking
bool json_is_null(JsonValue* value) {
    return value && value->type == JSON_NULL;
}

bool json_is_bool(JsonValue* value) {
    return value && value->type == JSON_BOOL;
}

bool json_is_number(JsonValue* value) {
    return value && value->type == JSON_NUMBER;
}

bool json_is_string(JsonValue* value) {
    return value && value->type == JSON_STRING;
}

bool json_is_array(JsonValue* value) {
    return value && value->type == JSON_ARRAY;
}

bool json_is_object(JsonValue* value) {
    return value && value->type == JSON_OBJECT;
}

// JSON Value Extraction
bool json_get_bool(JsonValue* value) {
    if (value && value->type == JSON_BOOL) {
        return value->data.bool_value;
    }
    return false;
}

double json_get_number(JsonValue* value) {
    if (value && value->type == JSON_NUMBER) {
        return value->data.number_value;
    }
    return 0.0;
}

char* json_get_string(JsonValue* value) {
    if (value && value->type == JSON_STRING) {
        return value->data.string_value;
    }
    return NULL;
}

// Simple JSON Serialization
char* json_stringify(JsonValue* value) {
    if (!value) return gp_strdup("null");
    
    switch (value->type) {
        case JSON_NULL:
            return gp_strdup("null");
            
        case JSON_BOOL:
            return gp_strdup(value->data.bool_value ? "true" : "false");
            
        case JSON_NUMBER: {
            char* buffer = malloc(32);
            if (buffer) {
                snprintf(buffer, 32, "%.15g", value->data.number_value);
            }
            return buffer;
        }
        
        case JSON_STRING: {
            size_t len = strlen(value->data.string_value);
            char* buffer = malloc(len + 3);
            if (buffer) {
                sprintf(buffer, "\"%s\"", value->data.string_value);
            }
            return buffer;
        }
        
        case JSON_ARRAY: {
            // Simplified array serialization
            return gp_strdup("[]");
        }
        
        case JSON_OBJECT: {
            // Simplified object serialization
            return gp_strdup("{}");
        }
    }
    
    return gp_strdup("null");
}

// Error Handling
JsonError json_get_last_error(void) {
    return last_error;
}

void json_set_error(JsonError error) {
    last_error = error;
}

char* json_get_error_string(JsonError error) {
    switch (error) {
        case JSON_ERROR_NONE: return gp_strdup("No error");
        case JSON_ERROR_INVALID_JSON: return gp_strdup("Invalid JSON");
        case JSON_ERROR_UNEXPECTED_TOKEN: return gp_strdup("Unexpected token");
        case JSON_ERROR_UNEXPECTED_END: return gp_strdup("Unexpected end of input");
        case JSON_ERROR_INVALID_NUMBER: return gp_strdup("Invalid number");
        case JSON_ERROR_INVALID_STRING: return gp_strdup("Invalid string");
        case JSON_ERROR_INVALID_ESCAPE: return gp_strdup("Invalid escape sequence");
        case JSON_ERROR_MEMORY_ERROR: return gp_strdup("Memory allocation error");
        case JSON_ERROR_TYPE_MISMATCH: return gp_strdup("Type mismatch");
        case JSON_ERROR_INDEX_OUT_OF_BOUNDS: return gp_strdup("Index out of bounds");
        case JSON_ERROR_KEY_NOT_FOUND: return gp_strdup("Key not found");
        default: return gp_strdup("Unknown error");
    }
}
