#pragma once
#include <stddef.h>
#include <stdint.h>

struct Registers {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void* memcpy(void *dest, const void *src, unsigned int count);
void* memset(void *dest, char val, unsigned int count);

unsigned short *memsetw(unsigned short *dest, unsigned short val,
	unsigned int count);
unsigned int strlen(const char *str);


char* itoa(int value, char* str, int base);
char* utoa(uint32_t value, char* str, int base);