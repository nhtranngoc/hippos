#include <stdio.h>

#include <kernel/cpu/idt.h>
#include <kernel/log/ulog.h>

cpu_status_t *exception_handler(cpu_status_t *context) {
    switch (context->vector_number) {
        case 0:
        ULOG_CRITICAL("Critical, Interrupt: Divide Error.");
        break;
        
        case 1:
        ULOG_CRITICAL("Critical, Interrupt: Debug Exception.");
        break;

        case 2:
        ULOG_CRITICAL("Critical, Interrupt: Non Maskable External Interrupt.");
        break;

        case 3:
        ULOG_CRITICAL("Critical, Interrupt: Breakpoint Reached.");
        break;

        case 4:
        ULOG_CRITICAL("Critical, Interrupt: Overflow.");
        break;

        case 5:
        ULOG_CRITICAL("Critical, Interrupt: BOUND Range Exceeded.");
        break;

        case 6:
        ULOG_CRITICAL("Critical, Interrupt: Invalid Opcode (Undefined Opcode).");
        break;

        case 7:
        ULOG_CRITICAL("Critical, Interrupt: Device Not Available (No Math Coprocessor).");
        break;

        case 8:
        ULOG_CRITICAL("Critical, Interrupt: Double Fault.");
        break;

        case 9:
        ULOG_CRITICAL("Critical, Interrupt: Coprocessor Segment Overrun.");
        break;

        case 10:
        ULOG_CRITICAL("Critical, Interrupt: Invalid TSS.");
        break;

        case 11:
        ULOG_CRITICAL("Critical, Interrupt: Segment Not Present.");
        break;

        case 12:
        ULOG_CRITICAL("Critical, Interrupt: Stack-Segment Fault.");

        case 13:
        ULOG_CRITICAL("Critical, Interrupt: General Protection Fault.");
        break;

        case 14:
        ULOG_CRITICAL("Critical, Interrupt: Page Fault.");
        break;

        case 15:
        ULOG_CRITICAL("Critical, Interrupt: This interrupt is reserved by Intel. This should not be used.");
        break;

        case 16:
        ULOG_CRITICAL("Critical, Interrupt: x87 FPU Floating-Point Error (Math Fault).");
        break;

        case 17:
        ULOG_CRITICAL("Critical, Interrupt: Alignment Check Fault.");
        break;

        case 18:
        ULOG_CRITICAL("Critical, Interrupt: Machine Check. Abort.");
        break;

        case 19:
        ULOG_CRITICAL("Critical, Interrupt: SIMD Floating-Point Exception Fault.");
        break;

        case 20:
        ULOG_CRITICAL("Critical, Interrupt: Virtualization Exception Fault.");

        case 21: 
        ULOG_CRITICAL("Critical, Interrupt: Control Protection Exception Fault.");
        break;

        default:
        ULOG_CRITICAL("Critical, Interrupt: Unexpected Interrupt.");
        break;
    }

    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
    return context;
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}

void idt_initialize() {
    __asm__ volatile("cli");

    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}