#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#include <kernel/limine.h>

// These are only specific to x32, since we have SSFN to help handling cursor position and putchar
#if defined (__i386__)
/* The I/O ports */
#define TTY_COMMAND_PORT         0x3D4
#define TTY_DATA_PORT            0x3D5
/* The I/O commands */
#define TTY_HIGH_BYTE_COMMAND    0x0E
#define TTY_LOW_BYTE_COMMAND     0x0F

void terminal_setcursor(int x, int y);
uint16_t terminal_getcursor(void);
#endif

#if defined(__x86_64__)
void terminal_initialize(struct limine_framebuffer *framebuffer);
void terminal_write(const uint8_t* data, size_t size);
void terminal_writestring(const uint8_t* data);
void terminal_putchar(uint8_t c);
#endif

void terminal_scroll(void);

#endif
