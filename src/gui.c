/*
 * GUI fonctions
 */

#include "gui.h"
#include "drivers/display.h"

void drawWindow(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bcolor, wchar_t *title) {

	drawFillRectangle(buffer, x, y, width, height, bcolor);
	drawFillRectangle(buffer, x, y, width, 20, 0x006e3800);
	drawString(buffer, x+4, y+3, 0x00ffffff, title);

}
