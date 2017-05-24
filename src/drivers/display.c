/*
 * GOP display driver
 */

#include "display.h"

#include "../fonts/system-8x16.h"

/*
 * Low-level functions
 */

uint32_t getPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y) {

    uint32_t *pixel = (uint32_t *)fb_base_addr;

    pixel += (x+y*KAOS_SCREEN_WIDTH);
	
	return *pixel++;

}

void putPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t *pixel = (uint32_t *)fb_base_addr;

    pixel += (x+y*KAOS_SCREEN_WIDTH);
    *pixel++ = color;

}

void blitScreenToBuffer(uint64_t fb_base_addr, uint32_t *buffer) {

    for(int k=0; k<KAOS_SCREEN_WIDTH*KAOS_SCREEN_HEIGHT; k++) { 
		buffer[k]=getPixel(fb_base_addr, k, 0);
	}

}

void blitBufferToScreen(uint64_t fb_base_addr, uint32_t *buffer) {

    for(int k=0; k<KAOS_SCREEN_WIDTH*KAOS_SCREEN_HEIGHT; k++) { 
		putPixel(fb_base_addr, k, 0, buffer[k]);
	}

}

/*
 * Drawing primitives
 */

void drawPoint(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color) {

	buffer[(x+y*KAOS_SCREEN_WIDTH)] = color;

}

void drawRectangle(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {

	for(int w=x; w<=x+width; w++) {
		drawPoint(buffer, w, y, color);
		drawPoint(buffer, w, y+height, color);
	}
	for(int h=y; h<=y+height; h++) {
		drawPoint(buffer, x, h, color);
		drawPoint(buffer, x+width, h, color);
	}

}

void drawFillRectangle(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {

	for(int w=x; w<=x+width; w++) {
	
		for(int h=y; h<=y+height; h++) {

			buffer[(w+h*KAOS_SCREEN_WIDTH)] = color;
		
		}
		
	}

}

void drawChar(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, wchar_t charcode) {

	extern wchar_t font_system_8x16[KAOS_FONTS_SIZE];
	
	wchar_t p=charcode*128-128; // Size of a font's character
	for(int l=0; l<16; l++) {
	
		for(int c=0; c<8; c++) {
		
			if(font_system_8x16[p]==1) {
				drawPoint(buffer, x+c, y+l, color);
			}
		
			p++;
		
		}
		
	}

}

void drawString(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, wchar_t *string) {

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
				drawChar(buffer, x, y, color, string[p]);
				x+=8;
				if(x>KAOS_SCREEN_WIDTH) break;
				
		}
		
		p++;
	
	}

}

void drawPicture(uint32_t *picture, uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t alpha) {

	uint32_t p=0;
	for(int h=y; h<=y+height-1; h++) {
		
		for(int w=x; w<=x+width-1; w++) {
	
			if(alpha==0xffffffff || picture[p]!=alpha)
				drawPoint(buffer, w, h, picture[p]);
			p++;

		}
	
	}

}

