#ifndef KERNEL_KLOG_H
#define KERNEL_KLOG_H

#include <kernel/log/ulog.h>

void log_initialize(void);

void kernel_log(ulog_level_t level, const char *msg);
void serial_log(ulog_level_t level, const char *msg);
#endif // KERNEL_KLOG_H