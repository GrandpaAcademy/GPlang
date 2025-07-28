#ifndef GPLANG_COLLECTIONS_H
#define GPLANG_COLLECTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Generic value type for collections
typedef enum {
    GP_VALUE_NULL,
    GP_VALUE_BOOL,
    GP_VALUE_INT,
    GP_VALUE_FLOAT,
    GP_VALUE_STRING,
    GP_VALUE_POINTER
} GPValueType;

typedef struct {
    GPValueType type;
    union {
        bool bool_val;
        int64_t int_val;
        double float_val;
        char* string_val;
        void* pointer_val;
    } data;
} GPValue;

// Function pointer types for callbacks
typedef int (*GPCompareFunc)(const GPValue* a, const GPValue* b);
typedef uint32_t (*GPHashFunc)(const GPValue* value);
typedef void (*GPFreeFunc)(GPValue* value);
typedef bool (*GPPredicateFunc)(const GPValue* value, void* user_data);
typedef GPValue (*GPMapFunc)(const GPValue* value, void* user_data);
typedef GPValue (*GPReduceFunc)(const GPValue* accumulator, const GPValue* value, void* user_data);

// Value creation and manipulation
GPValue gp_value_null(void);
GPValue gp_value_bool(bool value);
GPValue gp_value_int(int64_t value);
GPValue gp_value_float(double value);
GPValue gp_value_string(const char* value);
GPValue gp_value_pointer(void* value);
GPValue gp_value_copy(const GPValue* value);
void gp_value_destroy(GPValue* value);
bool gp_value_equals(const GPValue* a, const GPValue* b);
int gp_value_compare(const GPValue* a, const GPValue* b);
uint32_t gp_value_hash(const GPValue* value);
char* gp_value_to_string(const GPValue* value);

// Dynamic Array (Vector)
typedef struct {
    GPValue* data;
    size_t size;
    size_t capacity;
    GPFreeFunc free_func;
} GPArray;

GPArray* gp_array_create(size_t initial_capacity);
GPArray* gp_array_create_with_values(GPValue* values, size_t count);
void gp_array_destroy(GPArray* array);
void gp_array_clear(GPArray* array);
size_t gp_array_size(const GPArray* array);
bool gp_array_is_empty(const GPArray* array);
void gp_array_reserve(GPArray* array, size_t capacity);
void gp_array_shrink_to_fit(GPArray* array);

// Array element access
GPValue* gp_array_get(const GPArray* array, size_t index);
void gp_array_set(GPArray* array, size_t index, const GPValue* value);
GPValue* gp_array_front(const GPArray* array);
GPValue* gp_array_back(const GPArray* array);

// Array modification
void gp_array_push_back(GPArray* array, const GPValue* value);
void gp_array_push_front(GPArray* array, const GPValue* value);
GPValue gp_array_pop_back(GPArray* array);
GPValue gp_array_pop_front(GPArray* array);
void gp_array_insert(GPArray* array, size_t index, const GPValue* value);
void gp_array_remove(GPArray* array, size_t index);
void gp_array_remove_range(GPArray* array, size_t start, size_t count);

// Array searching and sorting
int gp_array_find(const GPArray* array, const GPValue* value);
int gp_array_find_if(const GPArray* array, GPPredicateFunc predicate, void* user_data);
bool gp_array_contains(const GPArray* array, const GPValue* value);
void gp_array_sort(GPArray* array, GPCompareFunc compare);
void gp_array_reverse(GPArray* array);

// Array functional operations
GPArray* gp_array_filter(const GPArray* array, GPPredicateFunc predicate, void* user_data);
GPArray* gp_array_map(const GPArray* array, GPMapFunc map_func, void* user_data);
GPValue gp_array_reduce(const GPArray* array, GPReduceFunc reduce_func, 
                       const GPValue* initial, void* user_data);
void gp_array_for_each(const GPArray* array, void (*func)(const GPValue*, void*), void* user_data);

// Linked List
typedef struct GPListNode {
    GPValue value;
    struct GPListNode* next;
    struct GPListNode* prev;
} GPListNode;

typedef struct {
    GPListNode* head;
    GPListNode* tail;
    size_t size;
    GPFreeFunc free_func;
} GPList;

GPList* gp_list_create(void);
void gp_list_destroy(GPList* list);
void gp_list_clear(GPList* list);
size_t gp_list_size(const GPList* list);
bool gp_list_is_empty(const GPList* list);

// List element access
GPValue* gp_list_front(const GPList* list);
GPValue* gp_list_back(const GPList* list);
GPValue* gp_list_get(const GPList* list, size_t index);

// List modification
void gp_list_push_front(GPList* list, const GPValue* value);
void gp_list_push_back(GPList* list, const GPValue* value);
GPValue gp_list_pop_front(GPList* list);
GPValue gp_list_pop_back(GPList* list);
void gp_list_insert(GPList* list, size_t index, const GPValue* value);
void gp_list_remove(GPList* list, size_t index);
void gp_list_remove_value(GPList* list, const GPValue* value);

// Hash Map
typedef struct GPHashMapEntry {
    GPValue key;
    GPValue value;
    struct GPHashMapEntry* next;
} GPHashMapEntry;

typedef struct {
    GPHashMapEntry** buckets;
    size_t bucket_count;
    size_t size;
    GPHashFunc hash_func;
    GPCompareFunc key_compare;
    GPFreeFunc key_free_func;
    GPFreeFunc value_free_func;
} GPHashMap;

GPHashMap* gp_hashmap_create(size_t initial_capacity);
void gp_hashmap_destroy(GPHashMap* map);
void gp_hashmap_clear(GPHashMap* map);
size_t gp_hashmap_size(const GPHashMap* map);
bool gp_hashmap_is_empty(const GPHashMap* map);

// HashMap operations
void gp_hashmap_put(GPHashMap* map, const GPValue* key, const GPValue* value);
GPValue* gp_hashmap_get(const GPHashMap* map, const GPValue* key);
bool gp_hashmap_contains_key(const GPHashMap* map, const GPValue* key);
bool gp_hashmap_remove(GPHashMap* map, const GPValue* key);
GPArray* gp_hashmap_keys(const GPHashMap* map);
GPArray* gp_hashmap_values(const GPHashMap* map);

// Set
typedef struct {
    GPHashMap* map;
} GPSet;

GPSet* gp_set_create(void);
void gp_set_destroy(GPSet* set);
void gp_set_clear(GPSet* set);
size_t gp_set_size(const GPSet* set);
bool gp_set_is_empty(const GPSet* set);

// Set operations
void gp_set_add(GPSet* set, const GPValue* value);
bool gp_set_contains(const GPSet* set, const GPValue* value);
bool gp_set_remove(GPSet* set, const GPValue* value);
GPArray* gp_set_to_array(const GPSet* set);
GPSet* gp_set_union(const GPSet* set1, const GPSet* set2);
GPSet* gp_set_intersection(const GPSet* set1, const GPSet* set2);
GPSet* gp_set_difference(const GPSet* set1, const GPSet* set2);

// Stack
typedef struct {
    GPArray* array;
} GPStack;

GPStack* gp_stack_create(void);
void gp_stack_destroy(GPStack* stack);
void gp_stack_clear(GPStack* stack);
size_t gp_stack_size(const GPStack* stack);
bool gp_stack_is_empty(const GPStack* stack);
void gp_stack_push(GPStack* stack, const GPValue* value);
GPValue gp_stack_pop(GPStack* stack);
GPValue* gp_stack_peek(const GPStack* stack);

// Queue
typedef struct {
    GPList* list;
} GPQueue;

GPQueue* gp_queue_create(void);
void gp_queue_destroy(GPQueue* queue);
void gp_queue_clear(GPQueue* queue);
size_t gp_queue_size(const GPQueue* queue);
bool gp_queue_is_empty(const GPQueue* queue);
void gp_queue_enqueue(GPQueue* queue, const GPValue* value);
GPValue gp_queue_dequeue(GPQueue* queue);
GPValue* gp_queue_front(const GPQueue* queue);
GPValue* gp_queue_back(const GPQueue* queue);

// Priority Queue (Binary Heap)
typedef struct {
    GPArray* array;
    GPCompareFunc compare;
} GPPriorityQueue;

GPPriorityQueue* gp_priority_queue_create(GPCompareFunc compare);
void gp_priority_queue_destroy(GPPriorityQueue* pq);
void gp_priority_queue_clear(GPPriorityQueue* pq);
size_t gp_priority_queue_size(const GPPriorityQueue* pq);
bool gp_priority_queue_is_empty(const GPPriorityQueue* pq);
void gp_priority_queue_push(GPPriorityQueue* pq, const GPValue* value);
GPValue gp_priority_queue_pop(GPPriorityQueue* pq);
GPValue* gp_priority_queue_top(const GPPriorityQueue* pq);

// Deque (Double-ended queue)
typedef struct {
    GPValue* data;
    size_t front;
    size_t back;
    size_t capacity;
    size_t size;
    GPFreeFunc free_func;
} GPDeque;

GPDeque* gp_deque_create(size_t initial_capacity);
void gp_deque_destroy(GPDeque* deque);
void gp_deque_clear(GPDeque* deque);
size_t gp_deque_size(const GPDeque* deque);
bool gp_deque_is_empty(const GPDeque* deque);
void gp_deque_push_front(GPDeque* deque, const GPValue* value);
void gp_deque_push_back(GPDeque* deque, const GPValue* value);
GPValue gp_deque_pop_front(GPDeque* deque);
GPValue gp_deque_pop_back(GPDeque* deque);
GPValue* gp_deque_front(const GPDeque* deque);
GPValue* gp_deque_back(const GPDeque* deque);
GPValue* gp_deque_get(const GPDeque* deque, size_t index);

// Binary Search Tree
typedef struct GPTreeNode {
    GPValue key;
    GPValue value;
    struct GPTreeNode* left;
    struct GPTreeNode* right;
    int height;  // For AVL balancing
} GPTreeNode;

typedef struct {
    GPTreeNode* root;
    size_t size;
    GPCompareFunc key_compare;
    GPFreeFunc key_free_func;
    GPFreeFunc value_free_func;
} GPTree;

GPTree* gp_tree_create(GPCompareFunc key_compare);
void gp_tree_destroy(GPTree* tree);
void gp_tree_clear(GPTree* tree);
size_t gp_tree_size(const GPTree* tree);
bool gp_tree_is_empty(const GPTree* tree);
void gp_tree_insert(GPTree* tree, const GPValue* key, const GPValue* value);
GPValue* gp_tree_find(const GPTree* tree, const GPValue* key);
bool gp_tree_remove(GPTree* tree, const GPValue* key);
GPArray* gp_tree_keys_inorder(const GPTree* tree);
GPArray* gp_tree_values_inorder(const GPTree* tree);

// Trie (Prefix Tree)
typedef struct GPTrieNode {
    struct GPTrieNode* children[256];  // ASCII characters
    bool is_end_of_word;
    GPValue value;
} GPTrieNode;

typedef struct {
    GPTrieNode* root;
    size_t size;
    GPFreeFunc value_free_func;
} GPTrie;

GPTrie* gp_trie_create(void);
void gp_trie_destroy(GPTrie* trie);
void gp_trie_clear(GPTrie* trie);
size_t gp_trie_size(const GPTrie* trie);
bool gp_trie_is_empty(const GPTrie* trie);
void gp_trie_insert(GPTrie* trie, const char* key, const GPValue* value);
GPValue* gp_trie_find(const GPTrie* trie, const char* key);
bool gp_trie_remove(GPTrie* trie, const char* key);
bool gp_trie_has_prefix(const GPTrie* trie, const char* prefix);
GPArray* gp_trie_find_with_prefix(const GPTrie* trie, const char* prefix);

// Graph (Adjacency List)
typedef struct {
    GPValue vertex;
    GPValue weight;
} GPEdge;

typedef struct {
    GPValue vertex;
    GPArray* edges;  // Array of GPEdge
} GPVertex;

typedef struct {
    GPArray* vertices;  // Array of GPVertex
    bool is_directed;
    GPCompareFunc vertex_compare;
    GPFreeFunc vertex_free_func;
} GPGraph;

GPGraph* gp_graph_create(bool is_directed, GPCompareFunc vertex_compare);
void gp_graph_destroy(GPGraph* graph);
void gp_graph_add_vertex(GPGraph* graph, const GPValue* vertex);
void gp_graph_add_edge(GPGraph* graph, const GPValue* from, const GPValue* to, const GPValue* weight);
bool gp_graph_has_vertex(const GPGraph* graph, const GPValue* vertex);
bool gp_graph_has_edge(const GPGraph* graph, const GPValue* from, const GPValue* to);
GPArray* gp_graph_get_neighbors(const GPGraph* graph, const GPValue* vertex);
GPArray* gp_graph_breadth_first_search(const GPGraph* graph, const GPValue* start);
GPArray* gp_graph_depth_first_search(const GPGraph* graph, const GPValue* start);

// Utility functions
void gp_collections_init(void);
void gp_collections_cleanup(void);

// Iterator interface
typedef struct {
    void* collection;
    void* current;
    bool (*has_next)(void* iterator);
    GPValue* (*next)(void* iterator);
    void (*reset)(void* iterator);
} GPIterator;

GPIterator* gp_array_iterator(const GPArray* array);
GPIterator* gp_list_iterator(const GPList* list);
GPIterator* gp_hashmap_iterator(const GPHashMap* map);
void gp_iterator_destroy(GPIterator* iterator);

#endif // GPLANG_COLLECTIONS_H
