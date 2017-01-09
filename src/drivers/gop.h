#ifndef H_GOP
#define H_GOP

#define DESIRED_HREZ            800
#define DESIRED_VREZ             600
#define DESIRED_PIXEL_FORMAT    PixelBlueGreenRedReserved8BitPerColor

void putPixel(EFI_PHYSICAL_ADDRESS fb_base_addr, UINT32 x, UINT32 y, UINT32 color);
void putChar(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, char charcode);
void putString(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color, char *string);

#endif /* H_GOP */
