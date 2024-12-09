#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/io.h>
#include <kernel/limine.h>
#include <kernel/ssfn.h>

#include <kernel/serial.h>
#include <stdio.h>
#include <stdlib.h>

struct limine_framebuffer *framebuffer;

void terminal_initialize(struct limine_framebuffer *fb) {
    // @TODO: Implement check to see if framebuffer is ready. Exit if not
    framebuffer = fb;

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
    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */
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
    int src_x = 0;
    int src_y = ssfn_src->height;
    int dst_x = 0;
    int dst_y = 0;
    uint32_t* src_ptr = framebuffer->address + (src_y * (framebuffer->pitch / 4) + src_x) * sizeof(uint32_t);
    uint32_t* dst_ptr = framebuffer->address + (dst_y * (framebuffer->pitch / 4) + dst_x) * sizeof(uint32_t);

    // Calculate the total number of bytes to move (width * height pixels)
    size_t total_size = framebuffer->width * (framebuffer->height - ssfn_src->height) * sizeof(uint32_t);
    memmove(dst_ptr, src_ptr, total_size);

    // // Clear last line
    ssfn_dst.y = ssfn_dst.h - (1 * ssfn_src->height);
    ssfn_dst.x = 0;
    for (int y = framebuffer->height - ssfn_src->height; y < framebuffer->height; y++) {
        for (int x = 0; x < framebuffer->width; x++) {
            volatile uint32_t *fb_ptr = framebuffer->address;
            fb_ptr[x + y * framebuffer->width] = 0x000000;
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

	if(ssfn_dst.y == ssfn_dst.h) {
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
