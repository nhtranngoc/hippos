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

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    // for (size_t i = 0; i < 100; i++) {
    //     volatile uint32_t *fb_ptr = framebuffer->address;
    //     fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
    // }


    serial_initialize(SERIAL_COM1);
    // char *test_str = "Test";
    // serial_write(test_str, SERIAL_COM1);

    // Pass the framebuffer to the terminal, which will update it accordingly
    terminal_initialize(framebuffer);
	printf(1, title_card);
    printf(1, "This is a test to see how long a string can be before line wraps around. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum\n");

    volatile uint32_t *fb_ptr = framebuffer->address;

    // Let's draw a box
    int height = 30;
    int width = 30;
    for (int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            // fb_ptr[x + y * 1280] = 0xff0000;
            *(fb_ptr + x + y * 1280) = 0x00ff00;
        }
    }

    for(int i = 0; i < 45; i++) {
        ssfn_dst.x += ssfn_src->width * i;
        char n[10];
        itoa(i, n, 10);
        printf(1, n);
        if(i < 44) {
            printf(1, "\n");
        }
    }

    // We're done, just hang...
    hcf();

    

}
