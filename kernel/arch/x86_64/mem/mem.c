#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <kernel/mem/mem.h>
#include <kernel/limine.h>
#include <kernel/klog.h>
#include <kernel/mem/hhdm.h>
#include <kernel/utils.h>

// Let's request Memmap from limine and poke around a bit.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void mem_initialize(void) {
    uint64_t entry_count = memmap_request.response->entry_count;
    struct limine_memmap_entry **entries = memmap_request.response->entries;

    print_memmap(entries, entry_count);

    // Count how much total memory we have in RAM
    for (uint64_t i = 0; i < entry_count; i++){
        if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
            g_total_memory += entries[i]->length;
        }
    }  
  
    g_num_of_pages = g_total_memory / PAGE_SIZE;
    printf(PIPE_TERMINAL, "Total usable memory: %d bytes, or %d pages available\n", g_total_memory, g_num_of_pages);

    // Now that we know how many pages we have, let's find a place in memory to put our page directory/bitmap in
    // Size of bitmap = g_num_of_pages / BITS_PER_ROW; 1 page = 1 bit?
    uint16_t bitmap_size = g_num_of_pages / BITS_PER_ROW;
    uint8_t bitmap_found = 0;
    for(int i = 0; i < entry_count; i++) {
        struct limine_memmap_entry *entry = entries[i];

        // Keep searching until we find one that's suitable
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitmap_size) {
            // Nestle the bitmap in the suitable spot, and set everything to taken.
            g_bitmap = (uint64_t *)(entry->base + g_hhdm_offset);
            memset(g_bitmap, FREE, bitmap_size * 8);
            // Readjust the Limine memmap entry to show that it's used.
            entry->base += bitmap_size;
            entry->length -= bitmap_size;

            bitmap_found++;
            break;
        }
    }

    if (!bitmap_found) {
        kerror("Could not allocate bitmap in RAM.\n");
        hcf();
    }

    printf(PIPE_TERMINAL, "Bitmap created with size %d\n", bitmap_size);

    // Let's test our bitmap
    // First, print out our bitmap at location 10
    printf(PIPE_TERMINAL, "At location 10, our bitmap looks like %b\n", g_bitmap[10]);
    // Then reserve 20 pages
    // Print it out
    reserve_pages(640, 10);
    printf(PIPE_TERMINAL, "At location 10, our bitmap looks like %b\n", g_bitmap[10]);
    // Then free it
    // Print it out
    free_pages(640, 20);
    printf(PIPE_TERMINAL, "At location 10, our bitmap looks like %b\n", g_bitmap[10]);
}

void print_memmap(struct limine_memmap_entry **entries, uint64_t entry_count) {
    printf(PIPE_TERMINAL, "Memory: Found %d memory entries from Limine.\n", entry_count);
    for (uint8_t i = 0; i < entry_count; i++) {
        uint64_t type = entries[i]->type;
        char *type_buf ="";
        switch(type){
            case LIMINE_MEMMAP_USABLE:
            type_buf = "USABLE";
            break;
            case LIMINE_MEMMAP_RESERVED:
            type_buf = "RESERVED";
            break;
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            type_buf = "ACPI RECLAIMABLE";
            break;
            case LIMINE_MEMMAP_ACPI_NVS:
            type_buf = "ACPI NVS";
            break;
            case LIMINE_MEMMAP_BAD_MEMORY:
            type_buf = "BAD MEMORY";
            break;
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            type_buf = "BOOTLOADER RECLAIMABLE";
            break;
            case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            type_buf = "EXECUTABLES & MODULES";
            break;
            case LIMINE_MEMMAP_FRAMEBUFFER:
            type_buf = "FRAMEBUFFER";
            break;
        }
        printf(PIPE_TERMINAL, "[*] Base 0x%x, length 0x%x, %s.\n", entries[i]->base, entries[i]->length, type_buf);
    }
}

void reserve_pages(uint64_t offset, uint64_t length) {
    for(uint64_t i = offset; i < offset + length; i++) {
        bitmap_use(i);
    }
}

void free_pages(uint64_t offset, uint64_t length) {
    for(uint64_t i = offset; i < offset + length; i++) {
        bitmap_free(i);
    }
}