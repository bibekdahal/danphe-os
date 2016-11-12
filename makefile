CFILES := kernel.c system.c stdio.c string.c gdt.c idt.c irq.c isr.c memory_physical.c memory_virtual.c memory.c
ASMFILES := loader.s libasm.s

COBJS := $(CFILES:%.c=%.o)
ASMOBJS := $(ASMFILES:%.s=%.o)

CFLAGS := -m32 -ffreestanding -MMD -Wall -Wextra -c
LDFlAGS := -T link.ld -melf_i386
ASMFLAGS := -felf32

OBJS := $(addprefix obj/,$(ASMOBJS) $(COBJS))

all: bin/danphe.iso

bin/danphe.iso: iso/boot/kernel.elf
	grub-mkrescue iso/ -o bin/danphe.iso

iso/boot/kernel.elf: bin/kernel.elf
	cp bin/kernel.elf iso/boot/

bin/kernel.elf: $(OBJS) bin
	 ld $(LDFlAGS) $(OBJS) -o bin/kernel.elf

bin:
	mkdir bin

obj/%.o: %.c | obj
	$(CC) $(CFLAGS) $< -o $@

obj:
	mkdir obj

obj/%.o: %.s | obj
	nasm $(ASMFLAGS) $^ -o $@

clean:
	rm bin/ -rf
	rm obj/ -rf

run:
	qemu-system-i386 -cdrom bin/danphe.iso


## Include auto-generated dependencies rules
-include $(OBJS:.o=.d)
