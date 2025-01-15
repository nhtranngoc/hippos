#include <stddef.h>

#include <kernel/mem/hhdm.h>
#include <kernel/log/ulog.h>
#include <kernel/limine.h>

uint64_t g_hhdm_offset;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

void hhdm_initialize(void) {
    if (hhdm_request.response == NULL) {
        ULOG_CRITICAL("HHDM Request Failed.");
        return;
    }

    // This should be  0x ffff ffff 8001 5a80?

    g_hhdm_offset = hhdm_request.response->offset;
    ULOG_INFO("HHDM Set.");
}