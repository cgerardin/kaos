/*
 * Memory manager
 */

#include "memory.h"

uint64_t memory_current_address = 0;
uint64_t memory_total = 0;
uint64_t memory_free = 0;

int memcmp(const void *a, const void *b, size_t n) {

    for (size_t i = 0; i < n; ++i) {

        unsigned char cur_a = ((const unsigned char*) a)[i];
        unsigned char cur_b = ((const unsigned char*) b)[i];

        if (cur_a != cur_b) return cur_a - cur_b;

    }

    return 0;
}

void *memcpy(char *dst, char *src, int n) {

    char *p = dst;

    while (n--) {
        *dst++ = *src++;
    }

    return p;
}

void init_memory_manager(uint64_t totalMemory, uint64_t freeMemory, uint64_t lastAddress) {

    extern uint64_t memory_current_address;
    extern uint64_t memory_total;
    extern uint64_t memory_free;
    memory_current_address = lastAddress;
    memory_total = totalMemory;
    memory_free = freeMemory;

}

uint64_t get_total_memory() {
    return memory_total;
}

uint64_t get_free_memory() {
    return memory_free;
}

void* kmalloc(size_t size) {

    memory_current_address -= size;
    memory_free -= size;

    return (void*) memory_current_address;

}

void kfree(void* address) {

    // STUB

}
