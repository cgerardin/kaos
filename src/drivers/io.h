#ifndef H_IO
#define H_IO

#include <efi.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif /* H_IO */
