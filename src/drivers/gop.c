#include <efi.h>
#include "gop.h"

void putPixel(EFI_PHYSICAL_ADDRESS fb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t* pixel = (uint32_t*)fb_base_addr;

    pixel += (x+y*DESIRED_HREZ);
    *pixel++ = color;

}
