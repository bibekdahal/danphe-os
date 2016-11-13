#include <system.h>


void kernel_main(multiboot_info_t* mbt) {
    cls();

    puts("Entering protected mode...\n");
    install_gdt();

    puts("Installing interrupt handlers...\n");
    install_idt();

    puts("Setting up paging and memory management...\n");
    init_memory(mbt);

    puts("\n\n");
    puts("Welcome to Danphe OS!\n\n");
}
