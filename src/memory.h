/*
 * Memory manager
 */

#ifndef H_MEMORY
#define H_MEMORY

#include "lib/types.h"

int memcmp(const void *a, const void *b, size_t n);
void* malloc_stub(size_t size, uint64_t baseAddress);

#endif /* H_MEMORY */
