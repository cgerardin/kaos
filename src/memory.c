/*
 * Memory manager
 */

#include "memory.h"

int memcmp(const void *a, const void *b, size_t n) {

    for (size_t i = 0; i < n; ++i) {

        unsigned char cur_a = ((const unsigned char*) a)[i];
        unsigned char cur_b = ((const unsigned char*) b)[i];
 
        if (cur_a != cur_b) return cur_a - cur_b;

    }
 
    return 0;
}

void* malloc_stub(size_t size, uint64_t baseAddress) {

	return (void*) baseAddress;

}
