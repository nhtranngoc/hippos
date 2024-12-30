#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/cpu/apic.h>
#include <kernel/cpu/cpudet.h>
#include <kernel/klog.h>
#include <kernel/io/io.h>
#include <kernel/mem/hhdm.h>

uint64_t apic_base;

void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

static inline uint64_t x86_64_msr_read(uint64_t msr) {
    uint32_t low;
    uint32_t high;
    asm volatile("rdmsr" : "=a" (low), "=d" (high) : "c" (msr));
    return low + ((uint64_t) high << 32);
}

/**
 * @brief Write to machine specific register
 */
static inline void x86_64_msr_write(uint64_t msr, uint64_t value) {
    asm volatile("wrmsr" : : "a" ((uint32_t) value), "d" ((uint32_t) (value >> 32)), "c" (msr));
}


/* Set the physical address for local APIC registers */
void cpu_set_apic_base(uintptr_t apic) {
   uint32_t edx = 0;
   uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

   cpu_set_msr(IA32_APIC_BASE_MSR, eax, edx);
}

/**
 * Get the physical address of the APIC registers page
 * make sure you map it to virtual memory ;)
 */
uintptr_t cpu_get_apic_base() {
    uint32_t eax, edx;
    cpu_get_msr(IA32_APIC_BASE_MSR, &eax, &edx);

    #ifdef __PHYSICAL_MEMORY_EXTENSION__
        return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
    #else
        return (eax & 0xfffff000);
    #endif
}

#define BASE_MASK 0xFFFFFFFFFF000

static inline uint32_t lapic_read(uint32_t offset)
{
    volatile uint32_t *pAddr = (uint32_t*)(g_hhdm_offset + apic_base + offset);
    return *pAddr;
}

static inline void lapic_write(uint32_t offset, uint32_t val)
{
    volatile uint32_t *pAddr = (uint32_t*)(g_hhdm_offset + apic_base + offset);
    *pAddr = val;
}

void apic_initialize(void) {
    // Detect APIC support
    if (!check_apic()) {
        kerror("APIC Not Detected.\n");
        // idk, die I guess?
        return;
    }

    // Check for presence of MSR
    if(!check_msr()) {
       kerror("MSR Not Detected.\n");
       return;
    }

    ksuccess("APIC & MSR Detected.\n");

    // Disable 8259PIC
    // Initialization Signal
    outb(PIC_MASTER_COMMAND, PIC_ICW1);
    outb(PIC_SLAVE_COMMAND, PIC_ICW1);

    outb(PIC_MASTER_DATA, PIC_ICW2_MASTER);
    outb(PIC_SLAVE_DATA, PIC_ICW2_SLAVE);

    outb(PIC_MASTER_DATA, PIC_ICW3_MASTER);
    outb(PIC_SLAVE_DATA, PIC_ICW3_SLAVE);

    outb(PIC_MASTER_DATA, PIC_ICW4);
    outb(PIC_SLAVE_DATA, PIC_ICW4);
    
    outb(PIC_MASTER_DATA, 0xFF);
    outb(PIC_SLAVE_DATA, 0xFF);
    klog("8259 PIC Disabled.\n");
 
    // Enable LAPIC
    // Hardware enable local APIC
    apic_base = cpu_get_apic_base();
    cpu_set_apic_base(apic_base);
    printf(PIPE_TERMINAL, "APIC Base address: %x\n", apic_base - g_hhdm_offset);

    printf(PIPE_TERMINAL, "Higher half offset at: %x\n", g_hhdm_offset);

    #define APIC_BASE_PA 0xFEE00000
    // uintptr_t apic_base_test = APIC_BASE_PA;
    uintptr_t addr = (uintptr_t) (g_hhdm_offset + apic_base + 0x0F0);
    volatile uint32_t *apic = (uint32_t *) addr;
    *apic = 0xFF;


    /* Set the Spurious Interrupt Vector Register bit 8 to start receiving interrupts */
    // printf(PIPE_TERMINAL, "Spurious Interrupt Vector Register with value %x\n", apic_reg);
   // mmio_write32(0x00F0, mmio_read32(0x00F0) | 0x100);

   // uint32_t sivr = mmio_read32(0x00000F0);
   // printf(PIPE_TERMINAL, "Spurious Interrupt Vector Register at (dec): %d \n", sivr);

    // Test by printing out LAPIC ID
}