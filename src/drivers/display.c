/*
 * GOP display driver
 */

#include <efi.h>
#include "display.h"
#include "../fonts/system-8x16.h"

void putPixel(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t* pixel = (uint32_t*)fb_base_addr;

    pixel += (x+y*KAOS_SCREEN_WIDTH);
    *pixel++ = color;

}

void putChar(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, char charcode) {

	extern unsigned char font_system_8x16[KAOS_FONTS_SIZE];
	
	int p=charcode*128-128;
	for(int l=0; l<16; l++) {
	
		for(int c=0; c<8; c++) {
		
			if(font_system_8x16[p]==1) {
				putPixel(fb_base_addr, x+c, y+l, color);
			}
		
			p++;
		
		}
		
	}

}

void putString(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, char *string) {

	uint32_t oldx=x;
	
	int p=0;
	while(string[p]!='\0') {
		
		switch(string[p]) {
		
			case '\n':
				x=oldx;
				y+=16;
				break;
				
			case '\t':
				x+=32;
				break;
			
			default:
				putChar(fb_base_addr, x, y, color, string[p]);
				x+=8;
				if(x>KAOS_SCREEN_WIDTH) break;
				
		}
		
		p++;
	
	}

}
