#include "drivers/framebuffer.h"
#include "drivers/serialport.h"

char title_card[13][60] = 
{
    "\n",
    "   888      d8b                    .d88888b.   .d8888b.  \n",
    "   888      Y8P                   d88P\" \"Y88b d88P  Y88b \n",
    "   888                            888     888 Y88b.      \n",
    "   88888b.  888 88888b.  88888b.  888     888  \"Y888b.   \n",
    "   888 \"88b 888 888 \"88b 888 \"88b 888     888     \"Y88b. \n",
    "   888  888 888 888  888 888  888 888     888       \"888 \n",
    "   888  888 888 888 d88P 888 d88P Y88b. .d88P Y88b  d88P \n",
    "   888  888 888 88888P\"  88888P\"   \"Y88888P\"   \"Y8888P\"  \n",
    "                 888      888                             \n",
    "                 888      888                             \n",
    "                 888      888                             \n"
    "\n"
};

void display_title_card() {
    fb_clear();
    for(int i = 0; i < 11; i++) {
        fb_set_fg(i+1);
        fb_write(title_card[i]);
    }
    fb_set_default_colors();
}

int main() {
    // Here is an interesting problem: String length is limited to 64 characters. This might be a problem with how memory is accessed? Need to investigate.
    // This would work (with line wrapping after col 80).
    // char welcome[] = "Welcome to HippOS! After 10 yea\n";
    // char welcome2[] = "rs of development, we hope it\n ";
    // char welcome3[] = "has been worth the weight (of hippo)\n";
    // char test_serial[] = "abcd";

    // This would not work.
    // char unwelcome[] = "Welcome to HippOS! After 10 years of development, we hope it has been worth the weight (of hippo)";

    sp_setup(COM1);
    fb_clear();
    display_title_card();
    // Currently, string literals doesn't work - pending linker refactor
    // fb_write("Hello!");
    
    return 0;
}

