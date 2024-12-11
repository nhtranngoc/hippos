#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/gdt.h>

/**
 *  uint16_t limit;
    uint16_t base_low16;
    uint8_t base_mid8;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high8;
 */
gdt_t gdt = {
    {0, 0, 0, 0, 0, 0}, // null
    {0xffff, 0, 0, 0x9a, 0x80, 0}, // 16-bit code - Limit 0xffff, Base
    {0xffff, 0, 0, 0x92, 0x80, 0}, // 16-bit data
    {0xffff, 0, 0, 0x9a, 0xcf, 0}, // 32-bit code
    {0xffff, 0, 0, 0x92, 0xcf, 0}, // 32-bit data
    {0, 0, 0, 0x9a, 0xa2, 0}, // 64-bit code
    {0, 0, 0, 0x92, 0xa0, 0}, // 64-bit data
    {0, 0, 0, 0xF2, 0, 0}, // user data
    {0, 0, 0, 0xFA, 0x20, 0} // user code
    // {0x68, 0, 0, 0x89, 0x20, 0, 0, 0} // tss
};

gdt_pointer_t gdt_pointer;

void gdt_initialize(void) {
    // log("Initializing GDT...\n");

    gdt_pointer.size = sizeof(gdt_t) - 1;
    gdt_pointer.offset = (uint64_t)&gdt;

    gdt_flush(&gdt_pointer);
    // log("GDT initialized!\n");
}
