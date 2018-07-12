#pragma once
#include <stddef.h>
#include <stdint.h>
#include <device/keyboard.h>

#define VIRTUAL_KERNEL_BASE 0xC0000000


enum VgaColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

void scroll(unsigned int n);

void cls();

void putch(char ch);
void puts(char* str);

void puthex(uint32_t num);
void putdec(uint32_t num);
void putint(int num);

uint8_t print_key_input(uint8_t code, uint8_t down);
