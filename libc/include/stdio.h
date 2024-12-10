#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

enum OUTPUT_PIPES {
    PIPE_SERIAL = 0,
    PIPE_TERMINAL = 1
};

int printf(uint8_t output, const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
