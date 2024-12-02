#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/* Starting address for VGA video memory */
#define VIDEO_ADDRESS           0x000B8000
/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5
/* The I/O commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15
/* Screen size */
#define MAX_ROWS                25
#define MAX_COLS                80

/* Background and foreground colors can be set independently */
extern unsigned short fg_color;
extern unsigned short bg_color;

/* VGA Color mapping */
enum FB_COLORS {
    FB_BLACK,
    FB_BLUE,
    FB_GREEN,
    FB_CYAN,
    FB_RED,
    FB_MAGENTA,        
    FB_BROWN,          
    FB_LIGHT_GREY,     
    FB_DARK_GREY,      
    FB_LIGHT_BLUE,     
    FB_LIGHT_GREEN,     
    FB_LIGHT_CYAN,      
    FB_LIGHT_RED,       
    FB_LIGHT_MAGENTA,   
    FB_LIGHT_BROWN,     
    FB_WHITE           
};


void fb_write_cell (unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_set_cursor(unsigned short pos);
int fb_get_cursor();
int fb_get_row_from_pos(unsigned short pos);
int fb_get_col_from_pos(unsigned short pos);
int fb_get_pos_from_coord(int col, int row);
int fb_move_pos_to_newline(int pos);
int fb_write(char *buf);

#endif /* FRAMEBUFFER_H*/