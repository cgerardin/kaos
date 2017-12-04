/*
 * GUI fonctions
 */

#ifndef H_GUI
#define H_GUI

#define KAOS_GUI_WINDOW_TITLE_HEIGHT 28
#define KAOS_GUI_WINDOW_BORDER_SIZE 1
#define KAOS_GUI_WINDOW_TITLE_BG_COLOR 0x005289c1
#define KAOS_GUI_WINDOW_TITLE_FG_COLOR 0x00ffffff
#define KAOS_GUI_WINDOW_BORDER_COLOR 0x002c6bab
#define KAOS_GUI_WINDOW_BACKGROUND_COLOR 0x00eeeeee

#include "lib/types.h"

void drawWindow(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, wchar_t *title);

void drawWindowAdvanced(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, wchar_t *title, uint32_t bcolor);

#endif /* H_GUI */
