/*
 * GOP display driver
 */

#ifndef H_DRIVERS_DISPLAY
#define H_DRIVERS_DISPLAY

#include "../lib/types.h"

#define KAOS_FONTS_SIZE 8*16*255
#define KAOS_SCREEN_WIDTH 1920
#define KAOS_SCREEN_HEIGHT 1080

void putPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color);
void drawBox(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, uint32_t fcolor, uint32_t border_size);
void drawWindow(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, wchar_t *title);
void putChar(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color, wchar_t charcode);
void putString(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color, wchar_t *string);

#endif /* H_DRIVERS_DISPLAY */
