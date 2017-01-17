/*
 * Low-level I/O functions
 */

#ifndef H_DRIVERS_IO
#define H_DRIVERS_IO

#include <efi.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif /* H_DRIVERS_IO */
