#pragma once
#include <stddef.h>
#include <stdint.h>

struct Registers {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void* memcpy(void* dest, const void* src, uint32_t count);
void* memset(void* dest, char val, uint32_t count);

uint16_t* memsetw(uint16_t* dest, uint16_t val,
	uint32_t count);
uint32_t strlen(const char* str);
uint32_t strcmp(const char* str1, const char* str2);


char* itoa(int value, char* str, int base);
char* utoa(uint32_t value, char* str, int base);