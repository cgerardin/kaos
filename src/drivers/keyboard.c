/*
 * PS/2 keyboard driver
 */
 
#include "keyboard.h"
#include "io.h"
#include "../keymaps/azerty.h"

char scanScancode() {

	char c=0;
	if(inb(0x60)!=c) {
	
		c=inb(0x60);
		if(c>0) {
			outb(0x60, 0);  // Empty buffer
		}
	
	}
	
	
	return c;
}

 
char getScancode() {

	char c=0;
	do {
	
		if(inb(0x60)!=c) {
		
			c=inb(0x60);
			if(c>0) {
				outb(0x60, 0);  // Empty buffer
				return c;
			}
		
		}
		
	} while(1);
	
}

char getChar() {

	extern char scancodes[128];

	return scancodes[(int)getScancode()];

}

char scancodeToChar(char scancode) {

	extern char scancodes[128];

	return scancodes[(int)scancode];

}
