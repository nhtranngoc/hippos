#include <stdio.h>
#include <string.h>

#include <kernel/log/klog.h>
#include <kernel/log/ulog.h>
#include <kernel/ssfn.h>
#include <kernel/io/serial.h>
#include <kernel/framebuffer/tty.h>
#include <colors/colors.h>

#define SERIAL_LOG_BUF_SIZE 256

void serial_log(ulog_level_t severity, const char *msg) {
    if(!is_serial_initialized()) {
        return;
    }

    char serial_buf[SERIAL_LOG_BUF_SIZE];
    snprintf(serial_buf, SERIAL_LOG_BUF_SIZE, "[%s]: %s\n", ulog_level_name(severity), msg);
    serial_write(serial_buf, SERIAL_COM1);
}

void kernel_log(ulog_level_t severity, const char *msg) {
    // Handle terminal colors
    switch (severity)
    {
    case ULOG_INFO_LEVEL:
        terminal_set_text_color(COLORS_GREEN);
        break;
    case ULOG_WARNING_LEVEL:
        terminal_set_text_color(COLORS_YELLOW);
        break;
    case ULOG_DEBUG_LEVEL:
        terminal_set_text_color(COLORS_BLUE);
        break;
    case ULOG_CRITICAL_LEVEL:
        terminal_set_text_color(COLORS_RED);
        break;
    default:
        break;
    }

    printf("[%s]: ", ulog_level_name(severity));

    terminal_reset_text_color();
    printf("%s\n", msg);
}

void log_initialize(void) {
    ULOG_INIT();

    ULOG_SUBSCRIBE((ulog_function_t) kernel_log, ULOG_INFO_LEVEL);
    ULOG_SUBSCRIBE((ulog_function_t) serial_log, ULOG_DEBUG_LEVEL);
}
