/*
 * Memory manager
 */

#ifndef H_MEMORY
#define H_MEMORY

#include "lib/types.h"

int memcmp(const void *a, const void *b, size_t n);

void init_memory_manager(uint64_t totalMemory, uint64_t freeMemory, uint64_t lastAddress);
uint64_t get_total_memory();
uint64_t get_free_memory();
void* kmalloc(size_t size);

#endif /* H_MEMORY */
