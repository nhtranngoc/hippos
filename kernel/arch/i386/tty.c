#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/io.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Local variables to store cursor position and color
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

/** terminal_setcursor:
    *  Moves the cursor of the framebuffer to the given position
    *
    *  @param x     X coordinate of the cursor
    *  @param y     Y coordinate of the cursor
*/
void terminal_setcursor(int x, int y) {
    const uint16_t index = (y * VGA_WIDTH + x);
	outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_COMMAND);
    outb(TTY_DATA_PORT, (uint8_t) (index & 0xFF));
    outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_COMMAND);
    outb(TTY_DATA_PORT, ((uint8_t) (index >> 8 & 0xFF)));
}

/** terminal_getcursor
 *      Read the position of the cursor from memory
 *
 *      @return uint16_t The given position of the cursor in memory;
 */
uint16_t terminal_getcursor(void) {
    uint16_t pos = 0;
    outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_COMMAND);
    pos |= inb(TTY_DATA_PORT);
	outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_COMMAND);
    pos |= (uint16_t)(inb(TTY_DATA_PORT) << 8);

    return pos;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

/** terminal_scroll
 *      Scroll the screen to a new line by moving the buffer of row 1 up to 0, then clear the last row, then readjust cursor positioning
 *      
 *      @param pos Current position of the cursor 
 *      @return New position of the cursor
*/
void terminal_scroll(void) {
    // Move the screen up by 1
    memmove(
        (char *) terminal_buffer,
        (char *) terminal_buffer + 2 * VGA_WIDTH,
        VGA_WIDTH * (VGA_HEIGHT - 1) * 2
    );

    // Clear last line
    for (uint16_t x = 0; x < VGA_WIDTH - 1; x++) {
		terminal_putentryat(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK), x, VGA_HEIGHT - 1);
    }

    // Set new cursor position, which should be at last row, column 0.
	terminal_row = VGA_HEIGHT - 1;
	terminal_column = 0;
	terminal_setcursor(terminal_column, terminal_row);
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	if(c == '\n') {
		terminal_column = 0;
		terminal_row++;
		terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
		terminal_setcursor(terminal_column, terminal_row);
		// return;	
	} else {	
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		terminal_setcursor(terminal_column + 1, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
          terminal_column = 0;
          if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
        }
	}

	if(terminal_row >= VGA_HEIGHT) {
		terminal_scroll();
	}
	
	terminal_setcursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	// Update row and column from cursor memory
	uint16_t pos = terminal_getcursor();
	terminal_row = pos / VGA_WIDTH;
	terminal_column = pos % VGA_WIDTH;
	terminal_write(data, strlen(data));
	// After writing, update it to memory
	terminal_setcursor(terminal_column, terminal_row);
}
