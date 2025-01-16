#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <kernel/mem/mem.h>
#include <kernel/mem/pmm.h>
#include <kernel/mem/vmm.h>
#include <kernel/log/ulog.h>

void mem_initialize(void) {
    pmm_initialize();
    vmm_initialize();
}

void load_cr3(void* cr3_value ) {
    asm volatile("mov %0, %%cr3" :: "r"((uint64_t)cr3_value) : "memory");
}