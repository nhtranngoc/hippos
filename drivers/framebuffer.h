#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5
/* The I/O commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Framebuffer memory map + colors */
extern char *fb;     // Starting position of framebuffer in memory
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
int fb_write(char *buf);

#endif /* FRAMEBUFFER_H*/