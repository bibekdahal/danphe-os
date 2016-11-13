#include <system/stdio.h>

struct GdtDescriptor {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;    // limit_high and flags
    uint8_t base_high;
} __attribute__((packed));


struct GdtPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


struct GdtDescriptor gdt_descriptors[5];
struct GdtPtr gdt_ptr;


static void set_gdt_descriptor(int index, uint32_t base,
    uint32_t limit, uint8_t access, uint8_t flag)
{
    gdt_descriptors[index].base_low = (base & 0xFFFF);
    gdt_descriptors[index].base_middle = (base >> 16) & 0xFF;
    gdt_descriptors[index].base_high = (base >> 24) & 0xFF;

    gdt_descriptors[index].limit_low = (limit & 0xFFFF);
    gdt_descriptors[index].granularity = ((limit >> 16) & 0x0F);
    gdt_descriptors[index].granularity |= (flag & 0xF0);
    gdt_descriptors[index].access = access;
}

void load_gdt();
void install_gdt() {
    set_gdt_descriptor(0, 0, 0, 0, 0);                  // Null segment
    set_gdt_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);   // Code segment
    set_gdt_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   // Data segment
    set_gdt_descriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);   // User mode code segment
    set_gdt_descriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);   // User mode data segment

    gdt_ptr.limit = sizeof(struct GdtDescriptor)*5 - 1;
    gdt_ptr.base = (uint32_t)&gdt_descriptors;

    load_gdt();
}
