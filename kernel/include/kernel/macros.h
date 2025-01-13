#ifndef KERNEL_MACROS_H
#define KERNEL_MACROS_H

#define min(a,b)                    (((a) < (b)) ? (a) : (b))
#define max(a,b)                    (((a) > (b)) ? (a) : (b))

// #define PFN2ADDR(frame)             ((frame) << FRAME_WIDTH)
// #define ADDR2PFN(addr)              ((addr) >> FRAME_WIDTH)

// #define FRAMES2SIZE(frames)         ((frames) << FRAME_WIDTH)
// #define SIZE2FRAMES(size)           (((size) == 0) ? 0 : ((((size) - 1) >> FRAME_WIDTH) + 1))

// Align address up (and down) to the nearest size (must be a power of 2)
#define ALIGN_UP(a, s)              (((s) + ((a) - 1)) & ~((a) - 1))
#define ALIGN_DOWN(a, s)            ((s) & ~((a) - 1))
#define IS_ALIGNED(a, s)`             (ALIGN_UP((s), (a)) == (s))


#endif // KERNEL_MACROS_H_