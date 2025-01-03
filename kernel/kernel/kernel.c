#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/limine.h>
#include <kernel/utils.h>
#include <kernel/klog.h>
#include <kernel/framebuffer/tty.h>
#include <kernel/io/serial.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/cpu/cpudet.h>
#include <kernel/cpu/apic.h>

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

void kernel_main(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Terminal/framebuffer
    terminal_initialize();

    // GDT
    gdt_initialize();

    // Serial
    serial_initialize(SERIAL_COM1);

    // IDT
    idt_initialize();

    // Detect CPU
    detect_cpu();

    // APIC
    // WIP - Need to configure paging (I think.)
    // apic_initialize();

    // We're done, just hang...
    hcf();
}
