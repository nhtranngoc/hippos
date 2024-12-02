#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5
/* The I/O commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Framebuffer memory map + colors */
extern char *fb;     // Starting position of framebuffer in memory
#define FB_GREEN     2
#define FB_DARK_GREY 8

/** outb:
    *  Sends the given data to the given I/O port. Defined in io.s
    *
    *  @param port The I/O port to send the data to
    *  @param data The data to send to the I/O port
*/
void outb(unsigned short port, unsigned char data);

void fb_write_cell (unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_move_cursor(unsigned short pos);

#endif /* INCLUDE_IO_H */