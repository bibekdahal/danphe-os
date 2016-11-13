#include <system/system.h>

struct IdtDescriptor 
{
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct IdtPtr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct IdtDescriptor idt_descriptors[256];
struct IdtPtr idt_ptr;

void idt_load();

void idt_set_gate(uint8_t index, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_descriptors[index].base_lo = base & 0xFFFF;
	idt_descriptors[index].base_hi = (base >> 16) & 0xFFFF;

	idt_descriptors[index].sel     = sel;
	idt_descriptors[index].always0 = 0;
	idt_descriptors[index].flags   = flags;
}


void install_isrs();
void install_irqs();

void install_idt()
{
    idt_ptr.limit = sizeof(struct IdtDescriptor) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_descriptors[0];

	memset((char*)&idt_descriptors, 0, sizeof(struct IdtDescriptor) * 256);

    install_isrs();
	install_irqs();
	idt_load();

    // Enable the irqs
	asm volatile("sti");
}
