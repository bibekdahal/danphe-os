#pragma once
#include <memory/string.h>
#include <memory/memory.h>


void install_gdt();
void install_idt();

void isr_install_handler(int isr, void (*handler)(struct Registers* regs));
void isr_uninstall_handler(int isr);

void irq_install_handler(int irq, void (*handler)(struct Registers* regs));
void irq_uninstall_handler(int irq);




static inline uint8_t inb (uint16_t port) {
    uint8_t rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

static inline void outb (uint16_t port, uint8_t data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline uint16_t inw (uint16_t port) {
    uint16_t rv;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

static inline void outw (uint16_t port, uint16_t data) {
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data));
}

static inline uint32_t inl (uint16_t port) {
    uint32_t rv;
    __asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

static inline void outl (uint16_t port, uint32_t data) {
    __asm__ __volatile__ ("outl %1, %0" : : "dN" (port), "a" (data));
}
