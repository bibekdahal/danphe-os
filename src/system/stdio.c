#include <system/system.h>

// Some useful constants

// Virtual address of the frame buffer
static uint16_t* const frame_buffer = (uint16_t*) (0x000B8000 + VIRTUAL_KERNEL_BASE);

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

// Move cursor to given position
static inline void update_cursor() {
    uint16_t position = cursor_pos_y*80 + cursor_pos_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
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
    update_cursor();
}

static inline void new_line() {
    cursor_pos_x = 0;
    cursor_pos_y += 1;
    if (cursor_pos_y >= VGA_HEIGHT) {
        scroll(cursor_pos_y - VGA_HEIGHT + 1);
    }
    update_cursor();
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
    update_cursor();
}

void cls() {
    for (size_t y=0; y<VGA_HEIGHT; ++y) {
        for (size_t x=0; x<VGA_WIDTH; ++x) {
            frame_buffer[y*VGA_WIDTH+x] = get_vga_entry(' ', active_color);
        }
    }
    cursor_pos_x = 0;
    cursor_pos_y = 0;
    update_cursor();
}

void putch(char ch) {
    switch (ch) {
    // Tab
    case '\t':
        for (size_t i=0; i<4; ++i)
            putch(' ');
        break;
    // New line
    case '\n':
        new_line();
        break;
    // Carriage return: return to beginning of line
    case '\r':
        cursor_pos_x = 0;
        break;
    // Backspace
    case '\b':

        // Move back one character
        if (cursor_pos_x == 0) {
            if (cursor_pos_y == 0) {
                return;
            }
            cursor_pos_x = VGA_WIDTH - 1;
            cursor_pos_y--;
        } else {
            cursor_pos_x--;
        }

        // Draw an empty space there
        frame_buffer[cursor_pos_y*VGA_WIDTH+cursor_pos_x] = get_vga_entry(
            ' ', active_color);
        update_cursor();
        break;

    default:
        // Just show the printable characters
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

void puthex(uint32_t num) {
    char temp[100];
    puts(utoa(num, temp, 16));
}
void putdec(uint32_t num) {
    char temp[100];
    puts(utoa(num, temp, 10));
}
void putint(int num) {
    char temp[100];
    puts(itoa(num, temp, 10));
}


uint8_t keys[256];
uint8_t caps_on = 0;
uint8_t print_key_input(uint8_t code, uint8_t down) {

    // Get the character
    if(code >= sizeof(us_map))
        return 0;

    uint8_t key = us_map[code];
    uint8_t shift_key = us_shift_map[code];

    // Set key state
    keys[key] = down;

    // Print the printable characters
    if (down) {
        if (key < NON_PRINTABLE_KEY) {

            // Check if we need to print the SHIFT character
            uint8_t shift = 0;

            if (caps_on && key >= 97 && key <= 122) {
                shift = 1;
            }
            
            if (keys[KEY_LSHIFT] || keys[KEY_RSHIFT]) {
                shift = 1-shift;
            }
            
            uint8_t ch = shift ? shift_key : key; 
            putch(ch);
            return ch;
        }
        else if (key == KEY_CAPS_LOCK)
            caps_on = 1 - caps_on;
        return key;
    }

    return 0;
}
