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

#include "pictures/cpu.h"
#include "pictures/ram.h"


/* Kernel main */
void kmain(uint64_t totalMemory, uint64_t freeMemory, uint64_t lastAddress, EFI_GRAPHICS_OUTPUT_PROTOCOL *framebuffer) {

	uint32_t bitmap[framebuffer->Mode->FrameBufferSize*sizeof(uint32_t)]; // Buffer used for graphic output

	// Initialize the quick'n'dirty memory manager
	init_memory_manager(totalMemory, freeMemory, lastAddress);

	struct gdtr {
		uint16_t limit;
		uint64_t base ;
	} __attribute__ ((packed));

	struct gdtr gdt;
	asm volatile ("lgdtq %0" : : "m" (gdt));

	// Main loop
	while(1) {

		// Draw background
		drawFillRectangle(bitmap, 0, 0, KAOS_SCREEN_WIDTH, KAOS_SCREEN_HEIGHT, 0x00babdb6);

		// Draw a window
		drawWindow(bitmap, 50, 130, 800, 480, 0x00ffffff, kernelName());
		drawPicture(picture_cpu, bitmap, 60, 170, picture_cpu_width, picture_cpu_height, ALPHA_COLOR);
		drawPicture(picture_ram, bitmap, 60, 212, picture_ram_width, picture_ram_height, ALPHA_COLOR);
		drawString(bitmap, 102, 178, 0x00000000, cpuInfo());
		drawString(bitmap, 102, 220, 0x00000000, memInfo());

		// Display the entire frame
		blitBufferToScreen(framebuffer->Mode->FrameBufferBase, bitmap);

	}

}


/* Make a string with OS name and version */
wchar_t *kernelName() {

	
	wchar_t *osName = kmalloc(200 * sizeof(wchar_t));
	wchar_t *osMajor = kmalloc(3 * sizeof(wchar_t));
	wchar_t *osMinor = kmalloc(3 * sizeof(wchar_t));
	wchar_t *osRevision = kmalloc(3 * sizeof(wchar_t));
	strf(osName, 7, L"KaOS v", itoa(KAOS_VERSION_MAJOR, osMajor, 10), L".", 
		itoa(KAOS_VERSION_MINOR, osMinor, 10), L".", 
			itoa(KAOS_VERSION_REVISION, osRevision, 10), L", the Karrot OS !");

	return osName;
}

/* Make a string with CPU informations */
wchar_t *cpuInfo() {

	wchar_t *strCpu = kmalloc(50 * sizeof(wchar_t));
	strf(strCpu, 2, detect_cpu(), L" CPU @ 0 Mhz");

	return strCpu;
}

/* Make a string with current memory status */
wchar_t *memInfo() {

	wchar_t *strMem = kmalloc(50 * sizeof(wchar_t));
	wchar_t *memTotal = kmalloc(20 * sizeof(wchar_t));
	wchar_t *memFree = kmalloc(20 * sizeof(wchar_t));

	return strf(strMem, 5, itoa(get_free_memory()/1024/1024, memFree, 10), L" Mo", 
			L" / ", itoa(get_total_memory()/1024/1024+1, memTotal, 10), L" Mo");
}
