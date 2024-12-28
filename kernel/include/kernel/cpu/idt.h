#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <stdint.h>
#include <stdbool.h>

#define IDT_MAX_DESCRIPTORS 32

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint64_t r15;
	uint64_t r14;
	// Other pushed registers
	uint64_t rbx;
	uint64_t rax;

	uint64_t vector_number;
	uint64_t error_code;

	uint64_t iret_rip;
	uint64_t iret_cs;
	uint64_t iret_flags;
	uint64_t iret_rsp;
	uint64_t iret_ss;
} cpu_status_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;

__attribute__((noreturn))
cpu_status_t *exception_handler(cpu_status_t *context);
static bool vectors[IDT_MAX_DESCRIPTORS];
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
extern void* isr_stub_table[];
void idt_initialize(void);

#endif // KERNEL_DT_H