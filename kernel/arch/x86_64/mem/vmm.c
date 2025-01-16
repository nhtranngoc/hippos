#include <kernel/mem/vmm.h>
#include <kernel/mem/pmm.h>

void* vmm_pt_root;

void vmm_initialize(void) {
    // Allocate a physical page for the vmm_pt_root pointer (at this point a function to do that should be present)
    
    // Map the physical address into the virtual address vmm_pt_root.
}

void* vmm_alloc(size_t length, size_t flags, void* arg) {
    
}
