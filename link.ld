ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    . = 1M;                  /* the code should be loaded at 1 MB */

    .text ALIGN(4K):
    {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K):
    {
        *(.rodata)
    }

    .data ALIGN(4K):
    {
        *(.data)
    }

    .bss ALIGN(4K):
    {
        *(COMMON)
        *(.bss)
    }
}
