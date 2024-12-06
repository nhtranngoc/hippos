#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

/* The I/O ports */
#define TTY_COMMAND_PORT         0x3D4
#define TTY_DATA_PORT            0x3D5
/* The I/O commands */
#define TTY_HIGH_BYTE_COMMAND    0x0E
#define TTY_LOW_BYTE_COMMAND     0x0F

void terminal_setcursor(int x, int y);
uint16_t terminal_getcursor(void);
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);


#endif
