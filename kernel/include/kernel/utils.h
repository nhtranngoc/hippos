#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

#endif // KERNEL_UTILS_H