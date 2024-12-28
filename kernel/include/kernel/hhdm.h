#ifndef KERNEL_HHDM_H
#define KERNEL_HHDM_H

#include <stdint.h>
#include <kernel/limine.h>

struct limine_hhdm_response *hhdm_response;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

#endif // KERNEL_HHDM_H