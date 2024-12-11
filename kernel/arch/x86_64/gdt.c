#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/gdt.h>
#include <kernel/klog.h>

// Adapted from https://github.com/kora-org/ydin/blob/rewrite-old/src/kernel/cpu/gdt.c . Will need to check up on code.

/**
 *  uint16_t limit;
    uint16_t base_low16;
    uint8_t base_mid8;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high8;
 */
gdt_t gdt = {
//   Limit      basel16 basem8  access  granu   baseh8
    {0,         0,      0,      0,      0,      0}, // null
    {0,         0,      0,      0x9a,   0xa2,   0}, // 64-bit code
    {0,         0,      0,      0x92,   0xa0,   0}, // 64-bit data
    {0,         0,      0,      0xF2,   0,      0}, // user data
    {0,         0,      0,      0xFA,   0x20,   0} // user code
};

gdt_pointer_t gdt_pointer;

void gdt_initialize(void) {
    klog("Initializing GDT...\n");

    gdt_pointer.size = sizeof(gdt_t) - 1;
    gdt_pointer.offset = (uint64_t)&gdt;

    gdt_flush(&gdt_pointer);
    ksuccess("GDT initialized!\n");
}
