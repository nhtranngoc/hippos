#include <stdio.h>
#include <string.h>
#include <kernel/klog.h>
#include <kernel/ssfn.h>

// In the future, we want to:
// 1) Decouple printf function to only one output and return it to standard UNIX compat (without the pipe argument)
void print_success(void) {
    printf("[");
    ssfn_dst.fg = 0x00ff00;
    printf(" OK ");
    ssfn_dst.fg = 0xffffff;
    printf("] ");
}

void print_debug(void) {
    printf("[");
    ssfn_dst.fg = 0xffffff;
    printf(" DEBUG ");
    ssfn_dst.fg = 0xffffff;
    printf("] ");
}

void print_error(void) {
    printf("[");
    ssfn_dst.fg = 0xff0000;
    printf(" ERROR ");
    ssfn_dst.fg = 0xffffff;
    printf("] ");
}

void klog(char * format) {
    print_debug();
    printf(format);
}

void kerror(char * format) {
    print_error();
    printf(format);
}

void ksuccess(char * format) {
    print_success();
    printf(format);
}