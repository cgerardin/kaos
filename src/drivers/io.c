/*
 * Low-level I/O functions
 */

#include "io.h"

uint8_t inb(uint16_t port) {

    uint8_t byte;
    asm volatile ("inb %1, %0" : "=a"(byte) : "Nd"(port));
    
    return byte;
    
}

uint16_t inw(uint16_t port) {

    uint16_t word;
    asm volatile ("inw %1, %0" : "=a"(word) : "Nd"(port));

    return word;
}

uint16_t inl(uint16_t port) {

    uint32_t lword;
    asm volatile ("inl %1, %0" : "=a"(lword) : "Nd"(port));

    return lword;
}

void outb(uint16_t port, uint8_t byte) {

    asm volatile ("outb %0, %1" : : "a"(byte), "Nd"(port));
    
}

void outw(uint16_t port, uint16_t word) {

    asm volatile ("outw %0, %1" : : "a"(word), "Nd"(port));
    
}

void outl(uint16_t port, uint32_t lword) {

    asm volatile ("outl %0, %1" : : "a"(lword), "Nd"(port));
    
}

void io_wait(void) {

    // Port 0x80 is used for 'checkpoints' during POST. The Linux kernel seems to think it is free for use
    asm volatile ("outb %%al, $0x80" : : "a"(0));

}

