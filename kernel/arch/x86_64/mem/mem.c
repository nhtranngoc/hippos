#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <kernel/mem/mem.h>
#include <kernel/mem/pmm.h>
#include <kernel/log/ulog.h>

void mem_initialize(void) {
    pmm_initialize();
}