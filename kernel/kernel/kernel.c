#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/limine.h>
#include <kernel/tty.h>
#include <kernel/serial.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR        /* use the special renderer for 32 bit truecolor packed pixels */
#include <kernel/ssfn.h>


// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

char *title_card = 
    "\n\
       888      d8b                    .d88888b.   .d8888b.  \n\
       888      Y8P                   d88P\" \"Y88b d88P  Y88b \n\
       888                            888     888 Y88b.      \n\
       88888b.  888 88888b.  88888b.  888     888  \"Y888b.   \n\
       888 \"88b 888 888 \"88b 888 \"88b 888     888     \"Y88b. \n\
       888  888 888 888  888 888  888 888     888       \"888 \n\
       888  888 888 888 d88P 888 d88P Y88b. .d88P Y88b  d88P \n\
       888  888 888 88888P\"  88888P\"   \"Y88888P\"   \"Y8888P\"  \n\
                     888      888                             \n\
                     888      888                             \n\
                     888      888                             \n\n";

// In the future, we want to:
// 1) Decouple printf function to only one output and return it to standard UNIX compat (without the pipe argument)
// 2) Move all of this formatting to a klog (heh, Kellogg) library.
void print_ok(void) {
    printf(PIPE_TERMINAL, "[");
    ssfn_dst.fg = 0x00ff00;
    printf(PIPE_TERMINAL, " OK ");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, "] ");
}

void print_debug(void) {
    printf(PIPE_TERMINAL, "[");
    ssfn_dst.fg = 0x0000ff;
    printf(PIPE_TERMINAL, " DEBUG ");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, "] ");
}

void print_error(void) {
    printf(PIPE_TERMINAL, "[");
    ssfn_dst.fg = 0xff0000;
    printf(PIPE_TERMINAL, " ERROR ");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, "] ");
}

void kernel_main(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    terminal_initialize(framebuffer);
    printf(PIPE_TERMINAL, title_card);
    // Pass the framebuffer to the terminal, which will update it accordingly
    print_ok();
    printf(PIPE_TERMINAL, "Framebuffer Initialized.\n");
    print_ok();
    printf(PIPE_TERMINAL, "Terminal Initialized.\n");

    serial_initialize(SERIAL_COM1);
    print_ok();
    printf(PIPE_TERMINAL, "Serial Port Initialized on COM 1.\n");

    // We're done, just hang...
    hcf();
}
