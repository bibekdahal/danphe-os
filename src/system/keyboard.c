#include <system/system.h>

// Keyboard scancode mapping for US layout:

const uint8_t us_map[] = {
    0,  27, '1',    '2',    '3',    '4',    '5',    '6',
    '7',    '8',    '9',    '0',    '-',    '=',    '\b',   '\t',
    'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',
    'o',    'p',    '[',    ']',    '\n',   KEY_LCTRL,  'a',    's',
    'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';',
    '\'',   '`',    KEY_LSHIFT, '\\',   'z',    'x',    'c',    'v',
    'b',    'n',    'm',    ',',    '.',    '/',    KEY_RSHIFT, '*',
    KEY_RALT,   ' ', KEY_CAPS_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM_LOCK, KEY_SCRL_LOCK, KEY_HOME,
    KEY_UP, KEY_PGUP, '-', KEY_LFT, 0, KEY_RT, '+', KEY_END,
    KEY_DN, KEY_PGDN, KEY_INS, KEY_DEL, 0, 0,  0, KEY_F11,
    KEY_F12
};


const uint8_t us_shift_map[] = {
    0,  27, '!',    '@',    '#',    '$',    '%',    '^',
    '&',    '*',    '(',    ')',    '_',    '+',    '\b',   '\t',
    'Q',    'W',    'E',    'R',    'T',    'Y',    'U',    'I',
    'O',    'P',    '{',    '}',    '\n',   KEY_LCTRL,  'A',    'S',
    'D',    'F',    'G',    'H',    'J',    'K',    'L',    ':',
    '"',   '~',    KEY_LSHIFT, '|',   'Z',    'X',    'C',    'V',
    'B',    'N',    'M',    '<',    '>',    '?',    KEY_RSHIFT, '*',
    KEY_RALT,   ' ', KEY_CAPS_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM_LOCK, KEY_SCRL_LOCK, KEY_HOME,
    KEY_UP, KEY_PGUP, '-', KEY_LFT, 0, KEY_RT, '+', KEY_END,
    KEY_DN, KEY_PGDN, KEY_INS, KEY_DEL, 0, 0,  0, KEY_F11,
    KEY_F12
};


void (*keyboard_listener)(uint8_t key, uint8_t down) = 0;
void set_keyboard_listener(void (*listener)(uint8_t key, uint8_t down)) {
    keyboard_listener = listener;
}

void kbd_handler() {
    if (!keyboard_listener)
        return;

    // Get the key code
    uint8_t code, down;
	code = inb(0x60);

    // Check if it was a UP or DOWN event
    if(code >= 0x80) {
        code &= 0x7F;
        down = 0;
    }
    else {
        down = 1;
    }

    // TODO BUFFERING
    keyboard_listener(code, down);
}

void install_keyboard() {
    irq_install_handler(1, &kbd_handler);
}