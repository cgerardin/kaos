/*
 * GOP display driver
 */

#include "display.h"
#include "../fonts/system-8x16.h"

void putPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t* pixel = (uint32_t*)fb_base_addr;

    pixel += (x+y*KAOS_SCREEN_WIDTH);
    *pixel++ = color;

}

void drawBox(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, uint32_t fcolor, uint32_t border_size) {

	for(int w=x; w<=x+width; w++) {
	
		for(int h=y; h<=y+height; h++) {
		
			putPixel(fb_base_addr, w, h, bcolor);
		
		}
		
	}

	for(int s=0; s<border_size; s++) {

		for(int w=x; w<=x+width; w++) {
			putPixel(fb_base_addr, w, y+s, fcolor);
			putPixel(fb_base_addr, w, y+height-s, fcolor);
		}
		for(int h=y; h<=y+height; h++) {
			putPixel(fb_base_addr, x+s, h, fcolor);
			putPixel(fb_base_addr, x+width-s, h, fcolor);
		}

	}

}

void drawWindow(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, wchar_t *title) {

	drawBox(fb_base_addr, x, y, width, height, bcolor, 0x00f57900, 2);
	drawBox(fb_base_addr, x, y, width, 20, 0x00f57900, 0, 0);
	putString(fb_base_addr, x+4, y+3, 0x00ffffff, title);

}

void putChar(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color, wchar_t charcode) {

	extern wchar_t font_system_8x16[KAOS_FONTS_SIZE];
	
	wchar_t p=charcode*128-128; // Size of a font's character
	for(int l=0; l<16; l++) {
	
		for(int c=0; c<8; c++) {
		
			if(font_system_8x16[p]==1) {
				putPixel(fb_base_addr, x+c, y+l, color);
			}
		
			p++;
		
		}
		
	}

}

void putString(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color, wchar_t *string) {

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
