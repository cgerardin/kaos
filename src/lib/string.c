/*
 * Standard library functions - Strings
 */

#include <wchar.h>
#include "string.h"

wchar_t *itoa(int value, wchar_t *string, int base) {

	wchar_t *rc;
	wchar_t *ptr;
	wchar_t *low;
	
	// Check for supported base.
	if (base < 2 || base > 36) {
		*string = '\0';
		return string;
	}
	
	rc = ptr = string;
	
	// Set '-' for negative decimals.
	if (value < 0 && base == 10) {
		*ptr++ = '-';
	}
	
	// Remember where the numbers start.
	low = ptr;
	
	// The actual conversion.
	do {
		// Modulo is negative for negative value. This trick makes abs() unnecessary.
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
		value /= base;
	} while (value);
	
	// Terminating the string.
	*ptr-- = '\0';
	
	// Invert the numbers.
	while (low < ptr) {
		wchar_t tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
	
	return rc;
	
}

wchar_t *strcat(wchar_t *string1, const wchar_t *string2) {
	
	int p1=0, p2=0;
	while(string1[p1]!='\0') {
		p1++;
	}
	while(string2[p2]!='\0') {
		string1[p1]=string2[p2];
		p1++;
		p2++;
	}
	string1[p1]='\0';

	return string1;

}





