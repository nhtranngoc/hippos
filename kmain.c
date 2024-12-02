#include "drivers/framebuffer.h"

int main() {
    // Here is an interesting problem: String length is limited to 64 characters. This might be a problem with how memory is accessed? Need to investigate.
    // This would work (with line wrapping after col 80).
    char welcome[] = "Welcome to HippOS! After 10 year";
    char welcome2[] = "s of development, we hope it ";
    char welcome3[] = "has been worth the weight (of hippo)";

    // This would not work.
    // char unwelcome[] = "Welcome to HippOS! After 10 years of development, we hope it has been worth the weight (of hippo)";

    fb_write(welcome);
    fb_write(welcome2);
    fb_write(welcome3);

    return 0;
}

