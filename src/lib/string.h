/*
 * Standard library functions - Strings
 */

#ifndef H_LIB_STRING
#define H_LIB_STRING

#include <wchar.h>

wchar_t *itoa(int value, wchar_t *string, int base);
wchar_t *strcat(wchar_t *string1, const wchar_t *string2);

#endif /* H_LIB_STRING */
