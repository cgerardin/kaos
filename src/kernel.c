/*
 * Kernel
 */

#include "kernel.h"
#include "cpu.h"
#include "memory.h"
#include "gui.h"
#include "drivers/io.h"
#include "drivers/display.h"
#include "drivers/keyboard.h"
#include "lib/string.h"

#include "pictures/wallpaper.h"
#include "pictures/cpu.h"
#include "pictures/ram.h"

/* Kernel main */
void kmain(uint64_t totalMemory, uint64_t freeMemory, uint64_t lastAddress, EFI_GRAPHICS_OUTPUT_PROTOCOL *framebuffer) {

    // Initialize the quick'n'dirty memory manager
    init_memory_manager(totalMemory, freeMemory, lastAddress);

    // Buffer used for graphic output
    uint32_t bitmap[framebuffer->Mode->FrameBufferSize * sizeof (uint32_t)];

    int terminalX = 0;
    int terminalY = 0;
    wchar_t *terminalBuffer = kmalloc(8000 * sizeof (wchar_t));
    int terminalCursor = 0;
    int terminalRow = 0;
    int terminalLine = 0;
    wchar_t key;

    // Main loop
    while (1) {

        // Draw background
        if (KAOS_DISPLAY_WALLPAPER) {
            drawPicture(picture_wallpaper, bitmap, 0, 0, picture_wallpaper_width, picture_wallpaper_height, ALPHA_COLOR);
        } else {
            drawFillRectangle(bitmap, 0, 0, KAOS_SCREEN_WIDTH, KAOS_SCREEN_HEIGHT, 0x00babdb6);
        }

        // Draw a fake "shell-like" window
        drawWindowAdvanced(bitmap, 50, 100, 640, 360, kernelName(), 0x00000000);
        terminalX = 50 + 10;
        terminalY = 100 + KAOS_GUI_WINDOW_TITLE_HEIGHT + 10;

        key = getChar();
        if (key > 0) {
            switch (key) {
                case '\n':
                    terminalBuffer[terminalCursor] = key;
                    terminalCursor++;
                    terminalLine++;
                    terminalRow = 0;
                    break;
                case '\b':
                    if (terminalBuffer[terminalCursor - 1] == '\n') {
                        terminalLine--;
                    } else {
                        terminalRow--;
                    }
                    terminalCursor--;
                    break;
                default:
                    terminalBuffer[terminalCursor] = key;
                    terminalCursor++;
                    terminalRow++;
            }
            terminalBuffer[terminalCursor] = '\0';

        }
        drawString(bitmap, terminalX, terminalY, 0x00dddddd, terminalBuffer);
        drawFillRectangle(bitmap, terminalX + terminalRow * KAOS_FONTS_WIDTH + 8 - KAOS_FONTS_WIDTH, terminalY + KAOS_FONTS_HEIGHT*terminalLine, 2, KAOS_FONTS_HEIGHT, 0x00dddddd);

        // Draw another window displaying hardware informations
        int hInfoX = KAOS_SCREEN_WIDTH - 50 - 200;
        int hInfoY = 50;
        drawWindow(bitmap, hInfoX, hInfoY, 200, 128, L"Hardware informations");
        drawPicture(picture_cpu, bitmap, hInfoX + 10, hInfoY + KAOS_GUI_WINDOW_TITLE_HEIGHT + 10, picture_cpu_width, picture_cpu_height, ALPHA_COLOR);
        drawPicture(picture_ram, bitmap, hInfoX + 10, hInfoY + KAOS_GUI_WINDOW_TITLE_HEIGHT + 20 + picture_cpu_height, picture_ram_width, picture_ram_height, ALPHA_COLOR);
        drawString(bitmap, hInfoX + 10 + picture_cpu_width + 5, hInfoY + KAOS_GUI_WINDOW_TITLE_HEIGHT + 10 + KAOS_FONTS_HEIGHT / 2, 0x00000000, cpuInfo());
        drawString(bitmap, hInfoX + 10 + picture_ram_width + 5, hInfoY + KAOS_GUI_WINDOW_TITLE_HEIGHT + 20 + picture_cpu_height + KAOS_FONTS_HEIGHT / 2, 0x00000000, memInfo());

        // Display the entire frame
        blitBufferToScreen(framebuffer->Mode->FrameBufferBase, bitmap);

    }

}

/* Make a string with OS name and version */
wchar_t *kernelName() {

    wchar_t *osName = kmalloc(200 * sizeof (wchar_t));
    wchar_t *osMajor = kmalloc(3 * sizeof (wchar_t));
    wchar_t *osMinor = kmalloc(3 * sizeof (wchar_t));
    wchar_t *osRevision = kmalloc(3 * sizeof (wchar_t));
    strf(osName, 6, L"KaOS v", itoa(KAOS_VERSION_MAJOR, osMajor, 10), L".",
            itoa(KAOS_VERSION_MINOR, osMinor, 10), L".",
            itoa(KAOS_VERSION_REVISION, osRevision, 10));

    return osName;
}

/* Make a string with CPU informations */
wchar_t *cpuInfo() {

    wchar_t *strCpu = kmalloc(50 * sizeof (wchar_t));
    strf(strCpu, 2, detect_cpu(), L" CPU @ 0 Mhz");

    return strCpu;
}

/* Make a string with current memory status */
wchar_t *memInfo() {

    wchar_t *strMem = kmalloc(50 * sizeof (wchar_t));
    wchar_t *memTotal = kmalloc(20 * sizeof (wchar_t));
    wchar_t *memFree = kmalloc(20 * sizeof (wchar_t));

    return strf(strMem, 5, itoa(get_free_memory() / 1024 / 1024, memFree, 10), L" Mo",
            L" / ", itoa(get_total_memory() / 1024 / 1024 + 1, memTotal, 10), L" Mo");
}
