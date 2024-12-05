OBJECTS = loader.o kmain.o drivers/framebuffer.o io.o drivers/utils.o drivers/serialport.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fwritable-strings \
			-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -g
LDFLAGS = -T link.ld -melf_i386 -g
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o os.iso iso
	
run: os.iso
	bochs -f bochsrc.txt -q

debug: debug-qemu debug-ddd

debug-qemu: os.iso
	qemu-system-i386 -s -S os.iso

debug-ddd: 
	ddd ---eval-command="target remote localhost:1234"

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso drivers/*.o