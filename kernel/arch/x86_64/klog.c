#include <stdio.h>
#include <string.h>
#include <kernel/klog.h>
#include <kernel/ssfn.h>

// In the future, we want to:
// 1) Decouple printf function to only one output and return it to standard UNIX compat (without the pipe argument)
void print_success(void) {
    printf(PIPE_TERMINAL, "[");
    ssfn_dst.fg = 0x00ff00;
    printf(PIPE_TERMINAL, " OK ");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, "] ");
}

void print_debug(void) {
    printf(PIPE_TERMINAL, "[");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, " DEBUG ");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, "] ");
}

void print_error(void) {
    printf(PIPE_TERMINAL, "[");
    ssfn_dst.fg = 0xff0000;
    printf(PIPE_TERMINAL, " ERROR ");
    ssfn_dst.fg = 0xffffff;
    printf(PIPE_TERMINAL, "] ");
}

void klog(char * format) {
    print_debug();
    printf(PIPE_TERMINAL, format);
}

void kerror(char * format) {
    print_error();
    printf(PIPE_TERMINAL, format);
}

void ksuccess(char * format) {
    print_success();
    printf(PIPE_TERMINAL, format);
}