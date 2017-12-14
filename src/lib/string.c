/*
 * Standard library functions - Strings
 */

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

int strlen(wchar_t *s) {

    int i = 0;
    while (*s++) {
        i++;
    }

    return i;
}

wchar_t *strcpy(wchar_t *dest, const wchar_t *src) {

    int p = 0;
    while (src[p] != '\0') {
        dest[p] = src[p];
        p++;
    }
    dest[p] = '\0';

    return dest;

}

wchar_t *strcat(wchar_t *dest, const wchar_t *src) {

    int p1 = 0, p2 = 0;
    while (dest[p1] != '\0') {
        p1++;
    }
    while (src[p2] != '\0') {
        dest[p1] = src[p2];
        p1++;
        p2++;
    }
    dest[p1] = '\0';

    return dest;

}

wchar_t *strf(wchar_t *dest, int n, ...) {

    va_list va;
    va_start(va, n);

    strcpy(dest, va_arg(va, wchar_t *));

    for (int i = 1; i < n; i++) {
        strcat(dest, va_arg(va, wchar_t *));
    }

    va_end(va);

    return dest;

}




