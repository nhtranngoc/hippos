#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR        /* use the special renderer for 32 bit truecolor packed pixels */

#include <colors/colors.h>
#include <kernel/utils.h>
#include <kernel/framebuffer/tty.h>
#include <kernel/ssfn.h>
#include <kernel/io/io.h>
#include <kernel/limine.h>
#include <kernel/log/ulog.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

char *title_card[12] = 
    {
        "\n",
        "       888      d8b                    .d88888b.   .d8888b.  \n",
        "       888      Y8P                   d88P\" \"Y88b d88P  Y88b \n",
        "       888                            888     888 Y88b.      \n",
        "       88888b.  888 88888b.  88888b.  888     888  \"Y888b.   \n",
        "       888 \"88b 888 888 \"88b 888 \"88b 888     888     \"Y88b. \n",
        "       888  888 888 888  888 888  888 888     888       \"888 \n",
        "       888  888 888 888 d88P 888 d88P Y88b. .d88P Y88b  d88P \n",
        "       888  888 888 88888P\"  88888P\"   \"Y88888P\"   \"Y8888P\"  \n"
        "                     888      888                             \n",
        "                     888      888                             \n",
        "                     888      888                             \n\n"
    };

void terminal_initialize(void) {    
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    

    // Name of the font symbol to use, can be found by readelf after linking u_vga16.sfn to .o
    extern ssfn_font_t _binary_arch_x86_64_u_vga16_sfn_start;

    /* set up context by global variables */
    ssfn_src = &_binary_arch_x86_64_u_vga16_sfn_start;      /* the bitmap font to use */
    volatile uint32_t *fb_ptr = framebuffer->address;

    ssfn_dst.ptr = (uint8_t*)fb_ptr;                  /* address of the linear frame buffer */
    ssfn_dst.w = framebuffer->width;                          /* width */
    ssfn_dst.h = framebuffer->height;                           /* height */
    ssfn_dst.p = framebuffer->pitch;
    ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
    terminal_reset_text_color();

    for (int y = 0; y < framebuffer->height; y++) {
        for (int x = 0; x < framebuffer->width; x++) {
            volatile uint32_t *fb_ptr = framebuffer->address;
            // This line also causes a crash too.
            fb_ptr[x + y * framebuffer->width] = COLORS_BG;
        }
    }

    for(int i = 0; i < 11; i++) {
        uint32_t color = 0xff0000 + (i * (0xffffff / 8));
        terminal_set_text_color(color);
        printf(title_card[i]);
    }

    ULOG_INFO("Success, Terminal initialized.");
}

void terminal_putentryat(uint32_t c, uint32_t color, size_t x, size_t y) {
    ssfn_dst.fg = color;
    ssfn_dst.x = x;
    ssfn_dst.y = y;
    ssfn_putc(c);
}

/** terminal_scroll
 *      Scroll the screen to a new line by moving the buffer of row 1 up to 0, then clear the last row, then readjust cursor positioning
 *      
 *      @param pos Current position of the cursor 
 *      @return New position of the cursor
*/
void terminal_scroll(void) {
    // We want to copy the frame from line 1 -> line end, and up by 1 line

    // Somehow the framebuffer is changed, so we are making local copies from requests instead of using a global variable.
    // Otherwise, this would crash.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    int src_x = 0;
    int src_y = ssfn_src->height;
    int dst_x = 0;
    int dst_y = 0;

    volatile uint32_t* src_ptr = framebuffer->address + (src_y * (framebuffer->pitch / 4) + src_x) * sizeof(uint32_t);
    volatile uint32_t* dst_ptr = framebuffer->address + (dst_y * (framebuffer->pitch / 4) + dst_x) * sizeof(uint32_t);

    // Calculate the total number of bytes to move (width * height pixels)
    size_t total_size = framebuffer->width * (framebuffer->height - ssfn_src->height) * sizeof(uint32_t);
    // This line causes a crash. Reason? 
    memmove(dst_ptr, src_ptr, total_size);

    // Clear last line
    ssfn_dst.y = ssfn_dst.h - (1 * ssfn_src->height);
    ssfn_dst.x = 0;
    for (int y = framebuffer->height - ssfn_src->height; y < framebuffer->height; y++) {
        for (int x = 0; x < framebuffer->width; x++) {
            volatile uint32_t *fb_ptr = framebuffer->address;
            // This line also causes a crash too.
            fb_ptr[x + y * framebuffer->width] = COLORS_BG;
        }
    }

    // Set new cursor position, which should be at last row, column 0.
    ssfn_dst.x = 0;
	ssfn_dst.y = ssfn_dst.h - ssfn_src->height;
}

void terminal_putchar(uint8_t c) {
	uint8_t uc = c;

	if(c == '\n') {
		ssfn_dst.x = 0;
		ssfn_dst.y += ssfn_src->height;

	} else {	
        ssfn_putc(c);
		if (ssfn_dst.x >= ssfn_dst.w) { // If we reach the end of line, wrap text around
          ssfn_dst.x = 0;
          ssfn_dst.y += ssfn_src->height;
          if (ssfn_dst.y == ssfn_dst.h){
            ssfn_dst.y = 0;
          }
            
        }
	}

	if(ssfn_dst.y >= ssfn_dst.h) {
		terminal_scroll();
	}	
}

void terminal_write(const uint8_t* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const uint8_t* data) {
	terminal_write(data, strlen(data));
}

void terminal_set_text_color(uint32_t c) {
    ssfn_dst.fg = c;
}

void terminal_reset_text_color(void) {
    ssfn_dst.fg = COLORS_TEXT_FG;
}

void terminal_set_background_color(uint32_t c) {
    ssfn_dst.bg = c;
}

void terminal_reset_background_color(void) {
    ssfn_dst.bg = COLORS_BG;
}