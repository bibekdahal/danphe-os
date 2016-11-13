#include <memory/string.h>


void* memcpy(void *dest, const void *src, uint32_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void* memset(void *dest, char val, uint32_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

uint16_t* memsetw(uint16_t *dest, uint16_t val, uint32_t count)
{
    uint16_t *temp = (uint16_t*)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

uint32_t strlen(const char *str)
{
    uint32_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

uint32_t strcmp(const char* str1, const char* str2) {
    uint32_t i = 0;
    while (str1[i] != '\0') {
        if (str2[i] != str1[i])
            return str1[i] - str2[i];
        i++;
    }
    return 0;
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