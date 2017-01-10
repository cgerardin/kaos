/*
 * PS/2 keyboard driver
 * Missing : scancode table.
 */
 
 #include "keyboard.h"
 #include "../io.h"
 
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

	//return scancode[getScancode()+1];
	return getScancode();

}
