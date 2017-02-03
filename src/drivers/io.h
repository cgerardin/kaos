/*
 * Low-level I/O functions
 */

#ifndef H_DRIVERS_IO
#define H_DRIVERS_IO

#include "../lib/types.h"

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint16_t inl(uint16_t port);
void outb(uint16_t port, uint8_t byte);
void outw(uint16_t port, uint16_t word);
void outl(uint16_t port, uint32_t lword);
void io_wait(void);

#endif /* H_DRIVERS_IO */
