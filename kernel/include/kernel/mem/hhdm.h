#ifndef KERNEL_HHDM_H
#define KERNEL_HHDM_H

#include <stdint.h>

#define HHDM(logical_address)               logical_address + 0xffffffff80000000

extern uint64_t g_hhdm_offset;
void hhdm_initialize(void);

#endif // KERNEL_HHDM_H