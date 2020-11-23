/*
 * Kernel
 */

#include "kernel.h"
#include "memory.h"
#include "drivers/display.h"
#include "lib/string.h"

/* Kernel main */
void kmain(uint64_t totalMemory, uint64_t freeMemory, uint64_t lastAddress, EFI_GRAPHICS_OUTPUT_PROTOCOL *framebuffer) {

    // Initialize the quick'n'dirty memory manager
    init_memory_manager(totalMemory, freeMemory, lastAddress);

    // Buffer used for graphic output
    uint32_t bitmap[framebuffer->Mode->FrameBufferSize * sizeof(uint32_t)];

    // Main loop
    while (1) {

        drawFillRectangle(bitmap, 0, 0, KAOS_SCREEN_WIDTH, KAOS_SCREEN_HEIGHT, 0x00000000);
        drawString(bitmap, 10, 10, 0x00ff0000, L"Welcome to KaOS 0.3.0");
        drawString(bitmap, 10, KAOS_FONTS_HEIGHT * 2, 0x00ffffff, memInfo());
        blitBufferToScreen(framebuffer->Mode->FrameBufferBase, bitmap);

    }

}

/* Make a string with current memory status */
wchar_t *memInfo() {

    wchar_t *strMem = kmalloc(50 * sizeof(wchar_t));
    wchar_t *memTotal = kmalloc(20 * sizeof(wchar_t));
    wchar_t *memFree = kmalloc(20 * sizeof(wchar_t));

    return strf(strMem, 6, L"Free memory : ",
                itoa(get_free_memory() / 1024 / 1024, memFree, 10), L" Mo", L" / ",
                itoa(get_total_memory() / 1024 / 1024 + 1, memTotal, 10), L" Mo");

}
