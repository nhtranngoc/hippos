#include "framebuffer.h"
#include "io.h"

// Starting memory address for VGA text mode
char * fb = (char *) 0x000B8000;

// Set default text colors, which is white on black
unsigned short fg_color = FB_WHITE;
unsigned short bg_color = FB_BLACK;

/** fb_write_cell:
    *  Writes a character with the given foreground and background to position i
    *  in the framebuffer.
    *
    *  @param i  The location in the framebuffer
    *  @param c  The character
    *  @param fg The foreground color
    *  @param bg The background color
*/
void fb_write_cell (unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i] = c;
    fb[i+1] = ((bg & 0x0F) << 4 | (fg & 0x0F));
}

/** fb_set_cursor:
    *  Moves the cursor of the framebuffer to the given position
    *
    *  @param pos The new position of the cursor
*/
void fb_set_cursor(unsigned short pos) {
    pos /= 2;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/** get_cursor
 *      Read the position of the cursor from memory
 *
 *      @return int The given position of the cursor in memory;
 */
int fb_get_cursor() {
    int pos = 0;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    pos |= inb(FB_DATA_PORT);
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    pos |= ((int)inb(FB_DATA_PORT)) << 8;

    return pos * 2;
}

/** fb_write
    *   Write a string with a given length to the framebuffer
    *   
    *   @param *buf pointer to the string
*/
int fb_write(char *buf) {
    int offset = fb_get_cursor();
    int i = 0;
    while (buf[i] != 0) {
        fb_write_cell(offset, buf[i], fg_color, bg_color);
        i++;
        offset += 2;
    }
    fb_set_cursor(offset);

    return 0;
}