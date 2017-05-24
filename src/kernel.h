/*
 * Kernel
 */

#ifndef H_KERNEL
#define H_KERNEL

#include "00_configuration.h"

#include <efi.h>
#include "lib/types.h"

void kmain(uint64_t totalMemory, uint64_t freeMemory, uint64_t lastAddress, EFI_GRAPHICS_OUTPUT_PROTOCOL *framebuffer);

wchar_t *kernelName();
wchar_t *cpuInfo();
wchar_t *memInfo();

#endif /* H_KERNEL */
