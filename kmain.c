#include "drivers/framebuffer.h"
#include "drivers/serialport.h"

int main() {
    // Here is an interesting problem: String length is limited to 64 characters. This might be a problem with how memory is accessed? Need to investigate.
    // This would work (with line wrapping after col 80).
    char welcome[] = "Welcome to HippOS! After 10 yea\n";
    char welcome2[] = "rs of development, we hope it\n ";
    // char welcome3[] = "has been worth the weight (of hippo)\n";
    // char test_serial[] = "abcd";

    // This would not work.
    // char unwelcome[] = "Welcome to HippOS! After 10 years of development, we hope it has been worth the weight (of hippo)";

    sp_setup(COM1);
    sp_write(welcome, COM1);
    sp_write(welcome2, COM1);

    fb_clear();
    // Test scroll function: print ASCII characters 
    for(int i = 65; i < 126; i++) {
        char str[3];
        str[0] = (char) i;
        str[1] = '\n';
        str[2] = '\0';
        fb_write(str);
    }
    return 0;
}

