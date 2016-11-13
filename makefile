
SRC_DIRS := descriptors interrupts memory asm system
CFILES := $(wildcard $(SRC_DIRS:%=src/%/*.c)) $(wildcard src/*.c)
ASMFILES := $(wildcard src/asm/*.s)
INCLUDE_DIR := include

COBJS := $(CFILES:src/%.c=%.o)
ASMOBJS := $(ASMFILES:src/%.s=%.o)

CFLAGS := -I $(INCLUDE_DIR) -m32 -ffreestanding -MMD -Wall -Wextra -c
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

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) $< -o $@

obj:
	mkdir obj $(SRC_DIRS:%=obj/%)

obj/%.o: src/%.s | obj
	nasm $(ASMFLAGS) $^ -o $@

clean:
	rm bin/ -rf
	rm obj/ -rf

run:
	qemu-system-i386 -cdrom bin/danphe.iso -k en-us


## Include auto-generated dependencies rules
-include $(OBJS:.o=.d)
