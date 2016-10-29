#include "stdio.h"

// Some useful constants

static uint16_t* const frame_buffer = (uint16_t*) 0x000B8000;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Some useful variables

static size_t cursor_pos_x = 0, cursor_pos_y = 0;
static uint16_t active_color = VGA_COLOR_WHITE | VGA_COLOR_BLACK << 4;


// Some helper functions

// Get vga color byte out of given foreground and background color
static inline uint8_t get_vga_color(enum VgaColor fg, enum VgaColor bg) {
    return fg | bg << 4;
}

// Get vga entry for given character and color bytes
static inline uint16_t get_vga_entry(char ch, uint8_t color) {
    return (uint16_t)ch | (uint16_t) color << 8;
}

// Move cursor by 'n' position forward
static inline void move_cursor(size_t n) {
    cursor_pos_x += n;
    if (cursor_pos_x >= VGA_WIDTH) {
        cursor_pos_y += (cursor_pos_x / VGA_WIDTH);
        cursor_pos_x = (cursor_pos_x % VGA_WIDTH);
        if (cursor_pos_y >= VGA_HEIGHT) {
            scroll(cursor_pos_y - VGA_HEIGHT + 1);
        }
    }
}

static inline void new_line() {
    cursor_pos_x = 0;
    cursor_pos_y += 1;
    if (cursor_pos_y >= VGA_HEIGHT) {
        scroll(cursor_pos_y - VGA_HEIGHT + 1);
    }
}


// Standard input/output functions

void scroll(unsigned int n) {
    for (size_t y=0; y<VGA_HEIGHT; ++y) {
        for (size_t x=0; x<VGA_WIDTH; ++x) {
            if (y+n >= VGA_HEIGHT)
                frame_buffer[y*VGA_WIDTH+x] =
                    get_vga_entry(' ', active_color);
            else
                frame_buffer[y*VGA_WIDTH+x] =
                    frame_buffer[(y+n)*VGA_WIDTH+x];
        }
    }
    if (n < cursor_pos_y)
        cursor_pos_y -= n;
}

void cls() {
    for (size_t y=0; y<VGA_HEIGHT; ++y) {
        for (size_t x=0; x<VGA_WIDTH; ++x) {
            frame_buffer[y*VGA_WIDTH+x] = get_vga_entry(' ', active_color);
        }
    }
    cursor_pos_x = 0;
    cursor_pos_y = 0;
}

void putch(char ch) {
    switch (ch) {
    case '\t':
        for (size_t i=0; i<4; ++i)
            putch(' ');
        break;
    case '\n':
        new_line();
        break;
    default:
        frame_buffer[cursor_pos_y*VGA_WIDTH+cursor_pos_x] = get_vga_entry(
            ch, active_color);
        move_cursor(1);
    }
}

void puts(char* const str) {
    char* temp = str;
    while (*temp != '\0') {
        putch(*temp++);
    }
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
