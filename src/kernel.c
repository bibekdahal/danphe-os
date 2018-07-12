#include <system/system.h>
#include <device/usb.h>
#include <tools/shell.h>


void key_listener(uint8_t code, uint8_t down) {
    shell_handle_input(code, down);
}


void kernel_main(multiboot_info_t* mbt) {
    cls();

    puts("Entering protected mode...\n");
    install_gdt();

    puts("Installing interrupt handlers...\n");
    install_idt();

    puts("Setting up paging and memory management...\n");
    init_memory(mbt);

    // FIXME If we don't print this string, we are out of memory
    // something goes wrong when linking.
    puts("Initialising different drivers...\n");
    init_usb();

    puts("\n\n");
    puts("Welcome to Danphe OS!\n\n");

    install_keyboard();
    set_keyboard_listener(key_listener);

    init_shell();
    while(1)
        ;
}
