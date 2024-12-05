DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: hippos.kernel

hippos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot hippos.kernel

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f hippos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: hippos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp hippos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)


# OBJECTS = loader.o kmain.o drivers/framebuffer.o io.o drivers/utils.o drivers/serialport.o
# CC = i686-elf-gcc
# CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra 
# LDFLAGS = -T link.ld -o hippos.bin -ffreestanding -O2 -nostdlib
# AS = i686-elf-as

# all: hippos.bin

# hippos.bin: $(OBJECTS)
# 	$(CC) $(LDFLAGS) $(OBJECTS) -lgcc

# hippos.iso: hippos.bin
# 	cp hippos.bin iso/boot/hippos.bin
# 	grub-mkrescue -o hippos.iso iso
	
# run: hippos.iso
# 	qemu-system-i386 -cdrom hippos.iso

# run-kernel: hippos.bin
# 	qemu-system-i386 -kernel hippos.bin

# debug: hippos.iso
# 	qemu-system-i386 -s -S hippos.iso

# debug-ddd: 
# 	ddd ---eval-command="target remote localhost:1234"

# %.o: %.c
# 	$(CC) -c $< -o $@ $(CFLAGS)

# %.o: %.s
# 	$(AS) $< -o $@

# clean:
# 	rm -rf *.o hippos.bin hippos.iso drivers/*.o