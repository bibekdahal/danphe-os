global loader

; Some constants

MAGIC       equ 0x1BADB002
FLAGS       equ 0x0
CHECKSUM    equ -(MAGIC + FLAGS)    ; magic + checksum + flags = 0

section .multiboot                  ; multiboot header
align 4

    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .bss                        ; bss section
align 4

stack_bottom:
    resb 16384                      ; 16 KB
stack_top:                          ; the stack


section .text                       ; code section
align 4

; Entry point for ELF
loader:
    mov esp, stack_top
    ; call kernel main function
    extern kernel_main
    call kernel_main

    cli
    hlt

.hang:
    jmp .hang
