/*
 * CPU management
 */

#include "cpu.h"

#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

wchar_t *detect_cpu() {

    unsigned long ebx, unused;
    cpuid(0, unused, ebx, unused, unused);

    switch (ebx) {
        case 0x756e6547: /* Intel Magic Code */
            return L"Intel";
        case 0x68747541: /* AMD Magic Code */
            return L"AMD";
        default:
            return L"Unknown";
    }

}

