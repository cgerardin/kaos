/*
 * GOP display driver
 */

#include "display.h"
#include "../lib/string.h"
#include "../fonts/system-8x16.h"

void putPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t *pixel = (uint32_t *)fb_base_addr;

    pixel += (x+y*KAOS_SCREEN_WIDTH);
    *pixel++ = color;

}

uint32_t getPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y) {

    uint32_t *pixel = (uint32_t *)fb_base_addr;

    pixel += (x+y*KAOS_SCREEN_WIDTH);
	
	return *pixel++;

}

void zputPixel(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color) {

	buffer[(x+y*KAOS_SCREEN_WIDTH)] = color;

}

void zblit(uint32_t *source, uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

	uint32_t p=0;
	for(int h=y; h<=y+height-1; h++) {
		
		for(int w=x; w<=x+width-1; w++) {
	
			if(source[p]!=ALPHA_COLOR)
				zputPixel(buffer, w, h, source[p]);
			p++;

		}
	
	}

}

void zdrawBox(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, uint32_t fcolor, uint32_t border_size) {

	for(int h=y; h<=y+height; h++) {

		for(int w=x; w<=x+width; w++) {
		
			zputPixel(buffer, w, h, bcolor);
		
		}
		
	}

	for(int s=0; s<border_size; s++) {

		for(int w=x; w<=x+width; w++) {
			zputPixel(buffer, w, y+s, fcolor);
			zputPixel(buffer, w, y+height-s, fcolor);
		}
		for(int h=y; h<=y+height; h++) {
			zputPixel(buffer, x+s, h, fcolor);
			zputPixel(buffer, x+width-s, h, fcolor);
		}

	}

}

void zdrawWindow(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, wchar_t *title) {

	zdrawBox(buffer, x, y, width, height, bcolor, 0x006e3800, 2);
	zdrawBox(buffer, x, y, width, 20, 0x006e3800, 0, 0);
	zputString(buffer, x+4, y+3, 0x00ffffff, title);

}




void zputChar(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, wchar_t charcode) {

	extern wchar_t font_system_8x16[KAOS_FONTS_SIZE];
	
	wchar_t p=charcode*128-128; // Size of a font's character
	for(int l=0; l<16; l++) {
	
		for(int c=0; c<8; c++) {
		
			if(font_system_8x16[p]==1) {
				zputPixel(buffer, x+c, y+l, color);
			}
		
			p++;
		
		}
		
	}

}

void zputString(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, wchar_t *string) {

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
				zputChar(buffer, x, y, color, string[p]);
				x+=8;
				if(x>KAOS_SCREEN_WIDTH) break;
				
		}
		
		p++;
	
	}

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

void drawBoxToBuffer(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {

	for(int w=x; w<=x+width; w++) {
	
		for(int h=y; h<=y+height; h++) {

			buffer[(w+h*KAOS_SCREEN_WIDTH)] = color;
		
		}
		
	}

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
