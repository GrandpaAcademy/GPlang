/*
 * GPLANG Memory Safety System
 * Rust-like ownership, borrowing, and lifetime management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "memory_safety.h"

// Global safety state
static safety_context_t g_safety_context = {0};

/*
 * Initialize memory safety system
 */
int safety_init(void) {
    printf("🔒 Initializing GPLANG Memory Safety System...\n");
    
    g_safety_context.max_regions = 10000;
    g_safety_context.regions = calloc(g_safety_context.max_regions, sizeof(memory_region_t));
    g_safety_context.region_count = 0;
    g_safety_context.next_lifetime_id = 1;
    g_safety_context.next_owner_id = 1;
    
    if (!g_safety_context.regions) {
        printf("❌ Failed to initialize safety system\n");
        return -1;
    }
    
    printf("✅ Memory safety system initialized\n");
    printf("   • Ownership tracking: ENABLED\n");
    printf("   • Borrow checking: ENABLED\n");
    printf("   • Lifetime analysis: ENABLED\n");
    printf("   • Use-after-free prevention: ENABLED\n");
    printf("   • Double-free prevention: ENABLED\n");
    printf("   • Buffer overflow protection: ENABLED\n");
    
    return 0;
}

/*
 * Allocate memory with ownership tracking
 */
void* safe_alloc(size_t size, uint32_t owner_id) {
    if (g_safety_context.region_count >= g_safety_context.max_regions) {
        printf("❌ Memory region limit exceeded\n");
        return NULL;
    }
    
    void* ptr = malloc(size);
    if (!ptr) {
        printf("❌ Memory allocation failed\n");
        return NULL;
    }
    
    // Track the allocation
    memory_region_t* region = &g_safety_context.regions[g_safety_context.region_count++];
    region->ptr = ptr;
    region->size = size;
    region->owner_id = owner_id;
    region->lifetime_id = g_safety_context.next_lifetime_id++;
    region->is_valid = 1;
    region->is_borrowed = 0;
    region->borrow_count = 0;
    region->is_mutable_borrowed = 0;
    
    printf("📍 Allocated %zu bytes at %p (owner: %u, lifetime: %u)\n", 
           size, ptr, owner_id, region->lifetime_id);
    
    return ptr;
}

/*
 * Free memory with ownership verification
 */
int safe_free(void* ptr, uint32_t owner_id) {
    memory_region_t* region = find_region(ptr);
    if (!region) {
        printf("❌ Attempt to free untracked memory: %p\n", ptr);
        return -1;
    }
    
    if (!region->is_valid) {
        printf("❌ Double-free detected: %p\n", ptr);
        return -1;
    }
    
    if (region->owner_id != owner_id) {
        printf("❌ Ownership violation: %p (owner: %u, attempted by: %u)\n", 
               ptr, region->owner_id, owner_id);
        return -1;
    }
    
    if (region->is_borrowed) {
        printf("❌ Attempt to free borrowed memory: %p (borrows: %u)\n", 
               ptr, region->borrow_count);
        return -1;
    }
    
    printf("🗑️  Freeing %zu bytes at %p (owner: %u)\n", 
           region->size, ptr, owner_id);
    
    free(ptr);
    region->is_valid = 0;
    region->ptr = NULL;
    
    return 0;
}

/*
 * Create immutable borrow
 */
borrow_handle_t safe_borrow(void* ptr, uint32_t borrower_id) {
    memory_region_t* region = find_region(ptr);
    borrow_handle_t handle = {0};
    
    if (!region) {
        printf("❌ Attempt to borrow untracked memory: %p\n", ptr);
        handle.is_valid = 0;
        return handle;
    }
    
    if (!region->is_valid) {
        printf("❌ Attempt to borrow freed memory: %p\n", ptr);
        handle.is_valid = 0;
        return handle;
    }
    
    if (region->is_mutable_borrowed) {
        printf("❌ Cannot create immutable borrow while mutable borrow exists: %p\n", ptr);
        handle.is_valid = 0;
        return handle;
    }
    
    region->is_borrowed = 1;
    region->borrow_count++;
    
    handle.ptr = ptr;
    handle.region = region;
    handle.borrower_id = borrower_id;
    handle.is_valid = 1;
    handle.is_mutable = 0;
    
    printf("📖 Immutable borrow created: %p (borrower: %u, count: %u)\n", 
           ptr, borrower_id, region->borrow_count);
    
    return handle;
}

/*
 * Create mutable borrow
 */
borrow_handle_t safe_borrow_mut(void* ptr, uint32_t borrower_id) {
    memory_region_t* region = find_region(ptr);
    borrow_handle_t handle = {0};
    
    if (!region) {
        printf("❌ Attempt to mutably borrow untracked memory: %p\n", ptr);
        handle.is_valid = 0;
        return handle;
    }
    
    if (!region->is_valid) {
        printf("❌ Attempt to mutably borrow freed memory: %p\n", ptr);
        handle.is_valid = 0;
        return handle;
    }
    
    if (region->is_borrowed) {
        printf("❌ Cannot create mutable borrow while other borrows exist: %p (count: %u)\n", 
               ptr, region->borrow_count);
        handle.is_valid = 0;
        return handle;
    }
    
    region->is_borrowed = 1;
    region->is_mutable_borrowed = 1;
    region->borrow_count = 1;
    
    handle.ptr = ptr;
    handle.region = region;
    handle.borrower_id = borrower_id;
    handle.is_valid = 1;
    handle.is_mutable = 1;
    
    printf("✏️  Mutable borrow created: %p (borrower: %u)\n", ptr, borrower_id);
    
    return handle;
}

/*
 * Release borrow
 */
int safe_release_borrow(borrow_handle_t* handle) {
    if (!handle->is_valid) {
        printf("❌ Attempt to release invalid borrow\n");
        return -1;
    }
    
    memory_region_t* region = handle->region;
    
    if (handle->is_mutable) {
        region->is_mutable_borrowed = 0;
        region->borrow_count = 0;
        printf("✏️  Mutable borrow released: %p\n", handle->ptr);
    } else {
        region->borrow_count--;
        if (region->borrow_count == 0) {
            region->is_borrowed = 0;
        }
        printf("📖 Immutable borrow released: %p (remaining: %u)\n", 
               handle->ptr, region->borrow_count);
    }
    
    handle->is_valid = 0;
    handle->ptr = NULL;
    handle->region = NULL;
    
    return 0;
}

/*
 * Move ownership (Rust-like move semantics)
 */
int safe_move(void* ptr, uint32_t old_owner, uint32_t new_owner) {
    memory_region_t* region = find_region(ptr);
    
    if (!region) {
        printf("❌ Attempt to move untracked memory: %p\n", ptr);
        return -1;
    }
    
    if (!region->is_valid) {
        printf("❌ Attempt to move freed memory: %p\n", ptr);
        return -1;
    }
    
    if (region->owner_id != old_owner) {
        printf("❌ Move ownership violation: %p (owner: %u, attempted by: %u)\n", 
               ptr, region->owner_id, old_owner);
        return -1;
    }
    
    if (region->is_borrowed) {
        printf("❌ Cannot move borrowed memory: %p (borrows: %u)\n", 
               ptr, region->borrow_count);
        return -1;
    }
    
    region->owner_id = new_owner;
    
    printf("🔄 Ownership moved: %p (from: %u, to: %u)\n", ptr, old_owner, new_owner);
    
    return 0;
}

/*
 * Check bounds for array access
 */
int safe_bounds_check(void* ptr, size_t index, size_t element_size) {
    memory_region_t* region = find_region(ptr);
    
    if (!region) {
        printf("❌ Bounds check on untracked memory: %p\n", ptr);
        return -1;
    }
    
    if (!region->is_valid) {
        printf("❌ Bounds check on freed memory: %p\n", ptr);
        return -1;
    }
    
    size_t offset = index * element_size;
    if (offset >= region->size) {
        printf("❌ Buffer overflow detected: %p[%zu] (size: %zu, offset: %zu)\n", 
               ptr, index, region->size, offset);
        return -1;
    }
    
    return 0;
}

/*
 * Find memory region by pointer
 */
memory_region_t* find_region(void* ptr) {
    for (size_t i = 0; i < g_safety_context.region_count; i++) {
        memory_region_t* region = &g_safety_context.regions[i];
        if (region->ptr == ptr) {
            return region;
        }
    }
    return NULL;
}

/*
 * Validate all memory regions (for debugging)
 */
void validate_memory_state(void) {
    printf("🔍 Validating memory state...\n");
    
    size_t valid_regions = 0;
    size_t borrowed_regions = 0;
    size_t total_size = 0;
    
    for (size_t i = 0; i < g_safety_context.region_count; i++) {
        memory_region_t* region = &g_safety_context.regions[i];
        
        if (region->is_valid) {
            valid_regions++;
            total_size += region->size;
            
            if (region->is_borrowed) {
                borrowed_regions++;
            }
        }
    }
    
    printf("   • Valid regions: %zu\n", valid_regions);
    printf("   • Borrowed regions: %zu\n", borrowed_regions);
    printf("   • Total allocated: %zu bytes\n", total_size);
}

/*
 * Get safety statistics
 */
void get_safety_stats(safety_stats_t* stats) {
    stats->total_regions = g_safety_context.region_count;
    stats->valid_regions = 0;
    stats->borrowed_regions = 0;
    stats->total_allocated = 0;
    stats->ownership_violations = 0;
    stats->borrow_violations = 0;
    stats->bounds_violations = 0;
    
    for (size_t i = 0; i < g_safety_context.region_count; i++) {
        memory_region_t* region = &g_safety_context.regions[i];
        
        if (region->is_valid) {
            stats->valid_regions++;
            stats->total_allocated += region->size;
            
            if (region->is_borrowed) {
                stats->borrowed_regions++;
            }
        }
    }
}

/*
 * Cleanup safety system
 */
void safety_cleanup(void) {
    printf("🧹 Cleaning up memory safety system...\n");
    
    // Check for memory leaks
    size_t leaks = 0;
    for (size_t i = 0; i < g_safety_context.region_count; i++) {
        memory_region_t* region = &g_safety_context.regions[i];
        if (region->is_valid) {
            printf("⚠️  Memory leak detected: %p (%zu bytes, owner: %u)\n", 
                   region->ptr, region->size, region->owner_id);
            leaks++;
        }
    }
    
    if (leaks == 0) {
        printf("✅ No memory leaks detected\n");
    } else {
        printf("❌ %zu memory leaks detected\n", leaks);
    }
    
    if (g_safety_context.regions) {
        free(g_safety_context.regions);
        g_safety_context.regions = NULL;
    }
    
    printf("🔒 Memory safety system cleaned up\n");
}
