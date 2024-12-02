#include "framebuffer.h"
#include "io.h"

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
    // Starting memory address for VGA text mode
    char * fb = (char *) VIDEO_ADDRESS;

    fb[i] = c;
    fb[i+1] = ((bg & 0x0F) << 4 | (fg & 0x0F));
}

/** fb_set_cursor:
    *  Moves the cursor of the framebuffer to the given position
    *
    *  @param pos The new position of the cursor
*/
void fb_set_cursor(unsigned short pos) {
    pos /= 2; // Since memory pos is twice the cursor pos, we want to change that before/after modifying memory
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
    pos |= ((int)inb(FB_DATA_PORT)) << 8;
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    pos |= inb(FB_DATA_PORT);

    return pos * 2; // Same here, see fb_set_cursor()
}

/** fb_get_row_from_pos
 *      Calculate cursor row from given position
 * 
 *      @param pos  Current position
 *      @return int Current row number
 */
int fb_get_row_from_pos(unsigned short pos) {
    return (pos / 2) / MAX_COLS;
}

/** fb_get_col_from_pos
 *      Calculate cursor column from given position
 * 
 *      @param pos  Current position
 *      @return int Current column number
 */
int fb_get_col_from_pos(unsigned short pos) {
    return (pos / 2) % MAX_COLS;
}

/** fb_get_pos_from_coord
 *      Calculate cursor position given a row and column
 * 
 *      @param col  Current column
 *      @param row  Current row
 *      @return int Current position 
 */
int fb_get_pos_from_coord(int col, int row) {
    return 2 * (row * MAX_COLS) + col;
}

/** fb_move_pos_to_newline
 *      Given a position, calculate the new position to set to new line
 *      @param pos Current position
 *      @return int New position to move to
 */
int fb_move_pos_to_newline(int pos) {
    return fb_get_pos_from_coord(0, fb_get_row_from_pos(pos) + 1);
}

/** fb_write
    *   Write a string with a given length to the framebuffer
    *   
    *   @param *buf pointer to the string
*/
int fb_write(char *buf) {
    int pos = fb_get_cursor();
    int i = 0;
    while (buf[i] != 0) {
        if (buf[i] == '\n' || fb_get_col_from_pos(pos) >= MAX_COLS) {
            pos = fb_move_pos_to_newline(pos);
        } else {
            fb_write_cell(pos, buf[i], fg_color, bg_color);
            pos += 2;
        }
        i++;
    }
    fb_set_cursor(pos);

    return 0;
}