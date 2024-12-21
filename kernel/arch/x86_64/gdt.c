#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/gdt.h>
#include <kernel/klog.h>

// Adapted from https://github.com/kora-org/ydin/blob/rewrite-old/src/kernel/cpu/gdt.c . Will need to check up on code.


// We are using a flat memory model, so everything is aligned at 0
gdt_t gdt = {
//   Limit      basel16 basem8  access  granu   baseh8
    {0,         0,      0,      0,      0,      0}, //  0x00    null
    {0xFFFF,    0,      0,      0x9A,   0xA0,   0}, //  0x08    64-bit code
    {0xFFFF,    0,      0,      0x92,   0xA0,   0}, //  0x10    64-bit data
    {0xFFFF,    0,      0,      0xF2,   0xC0,   0}, //  0x18    user data
    {0xFFFF,    0,      0,      0xFA,   0xA0,   0} //   0x20    user code
};

gdt_pointer_t gdt_pointer;

void gdt_initialize(void) {
    klog("Initializing GDT...\n");

    gdt_pointer.size = sizeof(gdt_t) - 1;
    gdt_pointer.offset = (uint64_t)&gdt;

    gdt_flush(&gdt_pointer);
    ksuccess("GDT initialized!\n");
}
