#pragma once
#include "stdio.h"
#include "string.h"
#include "memory.h"

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);


void install_gdt();
void install_idt();

void isr_install_handler(int isr, void (*handler)(struct Registers* regs));
void isr_uninstall_handler(int isr);

void irq_install_handler(int irq, void (*handler)(struct Registers* regs));
void irq_uninstall_handler(int irq);


