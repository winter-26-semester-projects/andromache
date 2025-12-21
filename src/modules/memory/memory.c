#include "modules/memory.h"

// Simple bump allocator for demonstration
// In a real OS, this would be a proper heap allocator (e.g., K&R malloc or slab)

static uint8_t* heap_start;
static uint8_t* heap_end;
static uint8_t* heap_curr;

void memory_init(uint32_t start_address, uint32_t size) {
    heap_start = (uint8_t*)start_address;
    heap_end = heap_start + size;
    heap_curr = heap_start;
}

void* kmalloc(size_t size) {
    // align to 4 bytes
    if (size % 4 != 0) {
        size += 4 - (size % 4);
    }

    if (heap_curr + size > heap_end) {
        return NULL; // OOM
    }

    void* ptr = heap_curr;
    heap_curr += size;
    return ptr;
}

void kfree(void* ptr) {
    // Bump allocator cannot free individual blocks easily without a list.
    // For this prototype, we assume unlimited memory or short-lived tasks.
    (void)ptr;
}
