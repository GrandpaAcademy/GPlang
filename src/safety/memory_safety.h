/*
 * GPLANG Memory Safety System Header
 * Rust-like ownership, borrowing, and lifetime management
 */

#ifndef MEMORY_SAFETY_H
#define MEMORY_SAFETY_H

#include <stddef.h>
#include <stdint.h>

// Memory region tracking
typedef struct {
    void* ptr;
    size_t size;
    uint32_t owner_id;
    uint32_t lifetime_id;
    uint8_t is_valid;
    uint8_t is_borrowed;
    uint32_t borrow_count;
    uint8_t is_mutable_borrowed;
} memory_region_t;

// Borrow handle (like Rust references)
typedef struct {
    void* ptr;
    memory_region_t* region;
    uint32_t borrower_id;
    uint8_t is_valid;
    uint8_t is_mutable;
} borrow_handle_t;

// Safety context
typedef struct {
    memory_region_t* regions;
    size_t region_count;
    size_t max_regions;
    uint32_t next_lifetime_id;
    uint32_t next_owner_id;
} safety_context_t;

// Safety statistics
typedef struct {
    size_t total_regions;
    size_t valid_regions;
    size_t borrowed_regions;
    size_t total_allocated;
    uint32_t ownership_violations;
    uint32_t borrow_violations;
    uint32_t bounds_violations;
} safety_stats_t;

// Function declarations
int safety_init(void);
void safety_cleanup(void);

// Memory management with ownership
void* safe_alloc(size_t size, uint32_t owner_id);
int safe_free(void* ptr, uint32_t owner_id);

// Borrowing system (Rust-like)
borrow_handle_t safe_borrow(void* ptr, uint32_t borrower_id);
borrow_handle_t safe_borrow_mut(void* ptr, uint32_t borrower_id);
int safe_release_borrow(borrow_handle_t* handle);

// Ownership transfer
int safe_move(void* ptr, uint32_t old_owner, uint32_t new_owner);

// Bounds checking
int safe_bounds_check(void* ptr, size_t index, size_t element_size);

// Utility functions
memory_region_t* find_region(void* ptr);
void validate_memory_state(void);
void get_safety_stats(safety_stats_t* stats);

// Safety macros
#define SAFE_ALLOC(size, owner) safe_alloc(size, owner)
#define SAFE_FREE(ptr, owner) safe_free(ptr, owner)
#define SAFE_BORROW(ptr, borrower) safe_borrow(ptr, borrower)
#define SAFE_BORROW_MUT(ptr, borrower) safe_borrow_mut(ptr, borrower)
#define SAFE_MOVE(ptr, old_owner, new_owner) safe_move(ptr, old_owner, new_owner)
#define SAFE_BOUNDS_CHECK(ptr, index, size) safe_bounds_check(ptr, index, size)

#endif // MEMORY_SAFETY_H
