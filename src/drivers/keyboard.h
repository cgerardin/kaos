/*
 * PS/2 keyboard driver
 */

#ifndef H_DRIVERS_KEYBOARD
#define H_DRIVERS_KEYBOARD

char scanScancode();
char getScancode();
wchar_t getChar();
wchar_t scancodeToChar(char scancode);

#endif /* H_DRIVERS_KEYBOARD */
