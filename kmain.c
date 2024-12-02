#include "drivers/framebuffer.h"

int main() {
    char welcome[] = "Welcome to HippOS";

    fb_write(welcome);

    return 0;
}

