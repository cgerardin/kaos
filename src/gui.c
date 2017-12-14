/*
 * GUI fonctions
 */

#include "gui.h"
#include "drivers/display.h"

void drawWindow(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, wchar_t *title) {

    drawWindowAdvanced(buffer, x, y, width, height, title, KAOS_GUI_WINDOW_BACKGROUND_COLOR);

}

void drawWindowAdvanced(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, wchar_t *title, uint32_t bcolor) {

    // Title bar
    drawFillRectangle(buffer, x, y, width + 1, KAOS_GUI_WINDOW_TITLE_HEIGHT, KAOS_GUI_WINDOW_TITLE_BG_COLOR);
    drawString(buffer, x + 5, y + KAOS_GUI_WINDOW_TITLE_HEIGHT / 2 - KAOS_FONTS_HEIGHT / 2, KAOS_GUI_WINDOW_TITLE_FG_COLOR, title);

    // Borders
    drawFillRectangle(buffer, x - KAOS_GUI_WINDOW_BORDER_SIZE, y - KAOS_GUI_WINDOW_BORDER_SIZE, width + KAOS_GUI_WINDOW_BORDER_SIZE * 2, KAOS_GUI_WINDOW_BORDER_SIZE, KAOS_GUI_WINDOW_BORDER_COLOR); // Top
    drawFillRectangle(buffer, x - KAOS_GUI_WINDOW_BORDER_SIZE, y + height + KAOS_GUI_WINDOW_TITLE_HEIGHT, width + KAOS_GUI_WINDOW_BORDER_SIZE * 2, KAOS_GUI_WINDOW_BORDER_SIZE, KAOS_GUI_WINDOW_BORDER_COLOR); // Bottom
    drawFillRectangle(buffer, x - KAOS_GUI_WINDOW_BORDER_SIZE, y, KAOS_GUI_WINDOW_BORDER_SIZE, height + KAOS_GUI_WINDOW_TITLE_HEIGHT, KAOS_GUI_WINDOW_BORDER_COLOR); // Left
    drawFillRectangle(buffer, x + width, y, KAOS_GUI_WINDOW_BORDER_SIZE, height + KAOS_GUI_WINDOW_TITLE_HEIGHT, KAOS_GUI_WINDOW_BORDER_COLOR); // Right

    // Background
    drawFillRectangle(buffer, x, y + KAOS_GUI_WINDOW_TITLE_HEIGHT, width, height, bcolor);

}
