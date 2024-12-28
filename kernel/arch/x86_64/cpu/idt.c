#include <stdio.h>

#include <kernel/cpu/idt.h>
#include <kernel/klog.h>

cpu_status_t *exception_handler(cpu_status_t *context) {
    switch (context->vector_number) {
        case 0:
        kerror("Interrupt: Divide Error.\n");
        break;
        
        case 1:
        kerror("Interrupt: Debug Exception.\n");
        break;

        case 2:
        kerror("Interrupt: Non Maskable External Interrupt.\n");
        break;

        case 3:
        kerror("Interrupt: Breakpoint Reached.\n");
        break;

        case 4:
        kerror("Interrupt: Overflow.\n");
        break;

        case 5:
        kerror("Interrupt: BOUND Range Exceeded.\n");
        break;

        case 6:
        kerror("Interrupt: Invalid Opcode (Undefined Opcode).\n");
        break;

        case 7:
        kerror("Interrupt: Device Not Available (No Math Coprocessor).\n");
        break;

        case 8:
        kerror("Interrupt: Double Fault.\n");
        break;

        case 9:
        kerror("Interrupt: Coprocessor Segment Overrun.\n");
        break;

        case 10:
        kerror("Interrupt: Invalid TSS.\n");
        break;

        case 11:
        kerror("Interrupt: Segment Not Present.\n");
        break;

        case 12:
        kerror("Interrupt: Stack-Segment Fault.\n");

        case 13:
        kerror("Interrupt: General Protection Fault.\n");
        break;

        case 14:
        kerror("Interrupt: Page Fault.\n");
        break;

        case 15:
        kerror("Interrupt: This interrupt is reserved by Intel. This should not be used.\n");
        break;

        case 16:
        kerror("Interrupt: x87 FPU Floating-Point Error (Math Fault).\n");
        break;

        case 17:
        kerror("Interrupt: Alignment Check Fault.\n");
        break;

        case 18:
        kerror("Interrupt: Machine Check. Abort.\n");
        break;

        case 19:
        kerror("Interrupt: SIMD Floating-Point Exception Fault.\n");
        break;

        case 20:
        kerror("Interrupt: Virtualization Exception Fault.\n");

        case 21: 
        kerror("Interrupt: Control Protection Exception Fault.\n");
        break;

        default:
        kerror("Interrupt: Unexpected Interrupt.\n");
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