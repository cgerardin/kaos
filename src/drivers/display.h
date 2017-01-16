/*
 * GOP display driver
 */

#ifndef H_DISPLAY
#define H_DISPLAY

#define KAOS_FONTS_SIZE 8*16*255
#define KAOS_SCREEN_WIDTH 800
#define KAOS_SCREEN_HEIGHT 600
#define KAOS_SCREEN_PIXEL_FORMAT PixelBlueGreenRedReserved8BitPerColor

void putPixel(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color);
void putChar(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, wchar_t charcode);
void putString(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, wchar_t *string);

#endif /* H_DISPLAY */
