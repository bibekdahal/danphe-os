#include <string.h>


void* memcpy(void *dest, const void *src, unsigned int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void* memset(void *dest, char val, unsigned int count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short* memsetw(unsigned short *dest, unsigned short val, unsigned int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned int strlen(const char *str)
{
    unsigned int retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}


char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    char* ptr = str;

    // Negative sign for negative decimals
    if (value < 0 && base == 10)
        *ptr++ = '-';

    char* tptr = ptr;
    do {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);

    *ptr-- = '\0';

    // Invert the digits
    while (tptr < ptr) {
        char temp = *tptr;
        *tptr++ = *ptr;
        *ptr-- = temp;
    }
    return str;
}


char* utoa(uint32_t value, char* str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    char* ptr = str;
    char* tptr = ptr;
    do {
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[value % base];
        value /= base;
    } while (value);

    *ptr-- = '\0';

    // Invert the digits
    while (tptr < ptr) {
        char temp = *tptr;
        *tptr++ = *ptr;
        *ptr-- = temp;
    }
    return str;
}