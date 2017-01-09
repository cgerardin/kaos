#include <efi.h>
#include "gop.h"

#include "font-8x16.h"

void putPixel(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t* pixel = (uint32_t*)fb_base_addr;

    pixel += (x+y*DESIRED_HREZ);
    *pixel++ = color;

}

void putChar(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, char charcode) {

	extern unsigned char vga_font[VGA_FONT_SIZE];
	
	int p=charcode*128-128;
	for(int l=0; l<16; l++) {
	
		for(int c=0; c<8; c++) {
		
			if(vga_font[p]==1) {
				putPixel(fb_base_addr, x+c, y+l, color);
			}
		
			p++;
		
		}
		
	}

}

void putString(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, char *string) {

	int p=0;
	while(string[p]!='\0') {
		
		putChar(fb_base_addr, x, y, color, string[p]);
		p++;
		x+=12;
	
	}

}
