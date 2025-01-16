#ifndef KERNEL_VMM_H
#define KERNEL_VMM_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define VM_FLAG_NONE 0
#define VM_FLAG_WRITE (1 << 0)
#define VM_FLAG_EXEC (1 << 1)
#define VM_FLAG_USER (1 << 2)

#define PT_FLAG_WRITE (1 << 1)
#define PT_FLAG_USER (1 << 2)
#define PT_FLAG_NX (1LL << 63)

// typedef struct {
//     uintptr_t base;
//     size_t length;
//     size_t flags;
//     vm_object* next;
// } vm_object;

// Map this to the end of the kernel?
void vmm_initialize(void);
void* vmm_alloc(size_t length, size_t flags, void* arg);

extern uint32_t endkernel;

static inline uint64_t convert_x86_64_vm_flags(size_t flags) {
    uint64_t value = 0;
    if (flags & VM_FLAG_WRITE)
        value |= PT_FLAG_WRITE;
    if (flags & VM_FLAG_USER)
        value |= PT_FLAG_USER;
    if ((flags & VM_FLAG_EXEC) == 0)
        value |= PT_FLAG_NX;
    return value;
};

#endif // KERNEL_VMM_H