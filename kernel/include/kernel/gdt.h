#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

/**
 * 0x0000	Null Descriptor	
Base = 0
Limit = 0x00000000
Access Byte = 0x00
Flags = 0x0

0x0008	Kernel Mode Code Segment	
Base = 0
Limit = 0xFFFFF
Access Byte = 0x9A
Flags = 0xA

0x0010	Kernel Mode Data Segment	
Base = 0
Limit = 0xFFFFF
Access Byte = 0x92
Flags = 0xC

0x0018	User Mode Code Segment	
Base = 0
Limit = 0xFFFFF
Access Byte = 0xFA
Flags = 0xA

0x0020	User Mode Data Segment	
Base = 0
Limit = 0xFFFFF
Access Byte = 0xF2
Flags = 0xC

0x0028	Task State Segment
(64-bit System Segment)	
Base = &TSS
Limit = sizeof(TSS)-1
Access Byte = 0x89
Flags = 0x0
 */

typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint16_t base_low16;
    uint8_t base_mid8;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high8;
} gdt_entry_t;

typedef struct  __attribute__((packed)) {
    uint16_t size;
    uint64_t offset;
} gdt_pointer_t;

typedef struct  __attribute__((packed)) {
    gdt_entry_t null;
    gdt_entry_t _64bit_code;
    gdt_entry_t _64bit_data;
    gdt_entry_t user_data;
    gdt_entry_t user_code;
}gdt_t;


void gdt_initialize(void);
extern void gdt_flush(gdt_pointer_t *);

#endif // _KERNEL_GDT_H