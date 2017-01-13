/*
 * Memory manager
 */
 
 #include "memory.h"
 #include "drivers/io.h"
 
unsigned short getCmosMemSize() {

	unsigned short total;
	unsigned char lowmem, highmem;

	outb(0x70, 0x30);
	lowmem = inb(0x71);
	outb(0x70, 0x31);
	highmem = inb(0x71);

	total = lowmem | highmem << 8;
	
	return total;
	
}
