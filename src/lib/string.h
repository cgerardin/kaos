/*
 * Standard library functions - Strings
 */

#ifndef H_LIB_STRING
#define H_LIB_STRING

#include "types.h"

wchar_t *itoa(int value, wchar_t *string, int base);
wchar_t *strcpy(wchar_t *dest, const wchar_t *src);
wchar_t *strcat(wchar_t *dest, const wchar_t *src);
wchar_t *strf(wchar_t *dest, int n, ...);

#endif /* H_LIB_STRING */
