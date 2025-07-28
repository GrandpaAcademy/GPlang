#include "collections.h"

// Value creation and manipulation
GPValue gp_value_null(void) {
    GPValue value = {GP_VALUE_NULL, {0}};
    return value;
}

GPValue gp_value_bool(bool val) {
    GPValue value = {GP_VALUE_BOOL, {0}};
    value.data.bool_val = val;
    return value;
}

GPValue gp_value_int(int64_t val) {
    GPValue value = {GP_VALUE_INT, {0}};
    value.data.int_val = val;
    return value;
}

GPValue gp_value_float(double val) {
    GPValue value = {GP_VALUE_FLOAT, {0}};
    value.data.float_val = val;
    return value;
}

GPValue gp_value_string(const char* val) {
    GPValue value = {GP_VALUE_STRING, {0}};
    if (val) {
        value.data.string_val = malloc(strlen(val) + 1);
        if (value.data.string_val) {
            strcpy(value.data.string_val, val);
        }
    } else {
        value.data.string_val = NULL;
    }
    return value;
}

GPValue gp_value_pointer(void* val) {
    GPValue value = {GP_VALUE_POINTER, {0}};
    value.data.pointer_val = val;
    return value;
}

GPValue gp_value_copy(const GPValue* value) {
    if (!value) return gp_value_null();
    
    switch (value->type) {
        case GP_VALUE_STRING:
            return gp_value_string(value->data.string_val);
        default:
            return *value;
    }
}

void gp_value_destroy(GPValue* value) {
    if (!value) return;
    
    if (value->type == GP_VALUE_STRING) {
        free(value->data.string_val);
        value->data.string_val = NULL;
    }
    value->type = GP_VALUE_NULL;
}

bool gp_value_equals(const GPValue* a, const GPValue* b) {
    if (!a || !b) return false;
    if (a->type != b->type) return false;
    
    switch (a->type) {
        case GP_VALUE_NULL:
            return true;
        case GP_VALUE_BOOL:
            return a->data.bool_val == b->data.bool_val;
        case GP_VALUE_INT:
            return a->data.int_val == b->data.int_val;
        case GP_VALUE_FLOAT:
            return a->data.float_val == b->data.float_val;
        case GP_VALUE_STRING:
            if (!a->data.string_val && !b->data.string_val) return true;
            if (!a->data.string_val || !b->data.string_val) return false;
            return strcmp(a->data.string_val, b->data.string_val) == 0;
        case GP_VALUE_POINTER:
            return a->data.pointer_val == b->data.pointer_val;
    }
    return false;
}

int gp_value_compare(const GPValue* a, const GPValue* b) {
    if (!a || !b) return 0;
    if (a->type != b->type) return (int)a->type - (int)b->type;
    
    switch (a->type) {
        case GP_VALUE_NULL:
            return 0;
        case GP_VALUE_BOOL:
            return (int)a->data.bool_val - (int)b->data.bool_val;
        case GP_VALUE_INT:
            if (a->data.int_val < b->data.int_val) return -1;
            if (a->data.int_val > b->data.int_val) return 1;
            return 0;
        case GP_VALUE_FLOAT:
            if (a->data.float_val < b->data.float_val) return -1;
            if (a->data.float_val > b->data.float_val) return 1;
            return 0;
        case GP_VALUE_STRING:
            if (!a->data.string_val && !b->data.string_val) return 0;
            if (!a->data.string_val) return -1;
            if (!b->data.string_val) return 1;
            return strcmp(a->data.string_val, b->data.string_val);
        case GP_VALUE_POINTER:
            if (a->data.pointer_val < b->data.pointer_val) return -1;
            if (a->data.pointer_val > b->data.pointer_val) return 1;
            return 0;
    }
    return 0;
}

uint32_t gp_value_hash(const GPValue* value) {
    if (!value) return 0;
    
    uint32_t hash = 5381;
    
    switch (value->type) {
        case GP_VALUE_NULL:
            return 0;
        case GP_VALUE_BOOL:
            return value->data.bool_val ? 1 : 0;
        case GP_VALUE_INT:
            return (uint32_t)value->data.int_val;
        case GP_VALUE_FLOAT: {
            union { double d; uint64_t i; } u;
            u.d = value->data.float_val;
            return (uint32_t)u.i;
        }
        case GP_VALUE_STRING:
            if (value->data.string_val) {
                const char* str = value->data.string_val;
                int c;
                while ((c = *str++)) {
                    hash = ((hash << 5) + hash) + c;
                }
            }
            return hash;
        case GP_VALUE_POINTER:
            return (uint32_t)(uintptr_t)value->data.pointer_val;
    }
    return hash;
}

// Dynamic Array implementation
GPArray* gp_array_create(size_t initial_capacity) {
    GPArray* array = malloc(sizeof(GPArray));
    if (!array) return NULL;
    
    array->capacity = initial_capacity > 0 ? initial_capacity : 16;
    array->data = malloc(array->capacity * sizeof(GPValue));
    if (!array->data) {
        free(array);
        return NULL;
    }
    
    array->size = 0;
    array->free_func = NULL;
    return array;
}

void gp_array_destroy(GPArray* array) {
    if (!array) return;
    
    gp_array_clear(array);
    free(array->data);
    free(array);
}

void gp_array_clear(GPArray* array) {
    if (!array) return;
    
    if (array->free_func) {
        for (size_t i = 0; i < array->size; i++) {
            array->free_func(&array->data[i]);
        }
    } else {
        for (size_t i = 0; i < array->size; i++) {
            gp_value_destroy(&array->data[i]);
        }
    }
    array->size = 0;
}

size_t gp_array_size(const GPArray* array) {
    return array ? array->size : 0;
}

bool gp_array_is_empty(const GPArray* array) {
    return gp_array_size(array) == 0;
}

static void gp_array_resize(GPArray* array, size_t new_capacity) {
    if (!array || new_capacity <= array->capacity) return;
    
    GPValue* new_data = realloc(array->data, new_capacity * sizeof(GPValue));
    if (!new_data) return;
    
    array->data = new_data;
    array->capacity = new_capacity;
}

void gp_array_push_back(GPArray* array, const GPValue* value) {
    if (!array || !value) return;
    
    if (array->size >= array->capacity) {
        gp_array_resize(array, array->capacity * 2);
    }
    
    array->data[array->size] = gp_value_copy(value);
    array->size++;
}

GPValue* gp_array_get(const GPArray* array, size_t index) {
    if (!array || index >= array->size) return NULL;
    return &array->data[index];
}

void gp_array_set(GPArray* array, size_t index, const GPValue* value) {
    if (!array || !value || index >= array->size) return;
    
    if (array->free_func) {
        array->free_func(&array->data[index]);
    } else {
        gp_value_destroy(&array->data[index]);
    }
    
    array->data[index] = gp_value_copy(value);
}

GPValue gp_array_pop_back(GPArray* array) {
    if (!array || array->size == 0) return gp_value_null();
    
    array->size--;
    GPValue result = array->data[array->size];
    return result;
}

int gp_array_find(const GPArray* array, const GPValue* value) {
    if (!array || !value) return -1;
    
    for (size_t i = 0; i < array->size; i++) {
        if (gp_value_equals(&array->data[i], value)) {
            return (int)i;
        }
    }
    return -1;
}

bool gp_array_contains(const GPArray* array, const GPValue* value) {
    return gp_array_find(array, value) != -1;
}

// Stack implementation
GPStack* gp_stack_create(void) {
    GPStack* stack = malloc(sizeof(GPStack));
    if (!stack) return NULL;
    
    stack->array = gp_array_create(16);
    if (!stack->array) {
        free(stack);
        return NULL;
    }
    
    return stack;
}

void gp_stack_destroy(GPStack* stack) {
    if (!stack) return;
    gp_array_destroy(stack->array);
    free(stack);
}

size_t gp_stack_size(const GPStack* stack) {
    return stack ? gp_array_size(stack->array) : 0;
}

bool gp_stack_is_empty(const GPStack* stack) {
    return gp_stack_size(stack) == 0;
}

void gp_stack_push(GPStack* stack, const GPValue* value) {
    if (!stack) return;
    gp_array_push_back(stack->array, value);
}

GPValue gp_stack_pop(GPStack* stack) {
    if (!stack) return gp_value_null();
    return gp_array_pop_back(stack->array);
}

GPValue* gp_stack_peek(const GPStack* stack) {
    if (!stack || gp_stack_is_empty(stack)) return NULL;
    return gp_array_get(stack->array, gp_array_size(stack->array) - 1);
}

// Utility functions
void gp_collections_init(void) {
    // Initialize any global state if needed
}

void gp_collections_cleanup(void) {
    // Cleanup any global state if needed
}
