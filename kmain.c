#include "drivers/framebuffer.h"

int main() {
    fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
    fb_move_cursor(2);
    fb_write_cell(2, 'B', FB_GREEN, FB_DARK_GREY);
    
    return 0;
}

