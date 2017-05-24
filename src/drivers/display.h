/*
 * GOP display driver
 */

#ifndef H_DRIVERS_DISPLAY
#define H_DRIVERS_DISPLAY

#include "../00_configuration.h"
#include "../lib/types.h"

#define ALPHA_COLOR 15991028

/*
 * Low-level functions
 */

uint32_t getPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y);
void putPixel(uint64_t fb_base_addr, uint32_t x, uint32_t y, uint32_t color);

void blitScreenToBuffer(uint64_t fb_base_addr, uint32_t *buffer);
void blitBufferToScreen(uint64_t fb_base_addr, uint32_t *buffer);

/*
 * Drawing primitives
 */

void drawPoint(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color);
void drawRectangle(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void drawFillRectangle(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void drawChar(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, wchar_t charcode);
void drawString(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, wchar_t *string);
void drawPicture(uint32_t *picture, uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t alpha);

#endif /* H_DRIVERS_DISPLAY */
