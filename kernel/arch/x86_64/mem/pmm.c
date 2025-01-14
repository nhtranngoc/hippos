#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <kernel/mem/pmm.h>
#include <kernel/limine.h>
#include <kernel/klog.h>
#include <kernel/mem/hhdm.h>
#include <kernel/utils.h>
#include <kernel/ssfn.h>
#include <kernel/macros.h>

// Let's request Memmap from limine and poke around a bit.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

struct pmm_bitmap_t pmm_bitmap = {
    .map = NULL,
    .size = 0,
    .total_pages = 0,
    .used_pages = 0,
    .lowest_address = 0xffffffff,
    .highest_address = 0,
};

void pmm_initialize(void) {
    struct limine_memmap_entry **entries = memmap_request.response->entries;
    uint64_t entry_count = memmap_request.response->entry_count;
    size_t current_page_top = 0;

    // Count how much total memory we have in RAM
    for (uint64_t i = 0; i < entry_count; i++){
        if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
            current_page_top = entries[i]->base + entries[i]->length;

            if (current_page_top > pmm_bitmap.highest_address) {
                pmm_bitmap.highest_address = current_page_top;
            }

            if(entries[i]->base < pmm_bitmap.lowest_address) {
                pmm_bitmap.lowest_address = entries[i]->base;
            }
        }
    }  
  
    pmm_bitmap.total_pages = (pmm_bitmap.highest_address - pmm_bitmap.lowest_address) / PAGE_SIZE;
    printf("Total usable memory: %d bytes, or %d pages available\n", pmm_bitmap.highest_address - pmm_bitmap.lowest_address, pmm_bitmap.total_pages);
    printf("Highest usable address at memory 0x%08x, lowest at 0x%08x.\n", pmm_bitmap.highest_address, pmm_bitmap.lowest_address);

    // Now that we know how many pages we have, let's find a place in memory to put our page directory/bitmap in
    // Size of bitmap = pmm_bitmap.total_pages / BITS_PER_ROW; 1 page = 1 bit?
    pmm_bitmap.size = ALIGN_UP(pmm_bitmap.total_pages / BITS_PER_ROW, PAGE_SIZE);
    for(size_t i = 0; i < entry_count; i++) {
        struct limine_memmap_entry *entry = entries[i];

        // Keep searching until we find one that's suitable
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= pmm_bitmap.size) {
            // Nestle the bitmap in the suitable spot, and set everything to taken.
            pmm_bitmap.map = (uint64_t *)(entry->base + g_hhdm_offset);
            printf("Current PMM Bitmap stored between 0x%.8lx and 0x%.8lx.\n", entry->base, entry->base + pmm_bitmap.size - 1);

            memset(pmm_bitmap.map, USED, pmm_bitmap.size * 8);
            // Readjust the Limine memmap entry to show that it's used.
            entry->base += pmm_bitmap.size;
            entry->length -= pmm_bitmap.size;
            // Re-update the pmm_bitmap object page_count to reflect the used pages to store itself
            pmm_bitmap.total_pages -= (pmm_bitmap.size / PAGE_SIZE);
            pmm_bitmap.used_pages = pmm_bitmap.total_pages;
            break;
        }
    }

    if (pmm_bitmap.map == NULL) {
        kerror("Could not allocate bitmap in RAM.\n");
        hcf();
    }

    print_memmap(entries, entry_count);

    // Go through memmap entries again, only free usable pages.
    for(size_t i = 0; i < entry_count; i++) {
        struct limine_memmap_entry *entry = entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        kfree((void *)entry->base + g_hhdm_offset, entry->length / PAGE_SIZE);
    }

    // Reserve page 0 for null pointer
    bitmap_use(&pmm_bitmap, 0);

    printf("Bitmap created with size %d. Used %d/%d pages.\n", pmm_bitmap.size, pmm_bitmap.used_pages, pmm_bitmap.total_pages);
    printf("==========\n");

    // Test physical frame allocator by allocating the first page, then freeing it
    pmm_memdump_rows(0, 5);
    void *page = kalloc(76);
    pmm_memdump_rows(0, 5);
    kfree(page, 76);
    pmm_memdump_rows(0, 5);
    // kfree(page2, 65);
    // pmm_memdump_rows(0, 2);
}

void print_memmap(struct limine_memmap_entry **entries, uint64_t entry_count) {
    printf("Memory: Found %d memory entries from Limine.\n", entry_count);
    for (uint8_t i = 0; i < entry_count; i++) {
        uint64_t type = entries[i]->type;
        char *type_buf ="";
        switch(type){
            case LIMINE_MEMMAP_USABLE:
            ssfn_dst.fg = 0x00ff00;
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
        printf("[*] Base 0x%08x | Length 0x%08x (%04d pages) | %s.\n", entries[i]->base, entries[i]->length, entries[i]->length / PAGE_SIZE, type_buf);
        ssfn_dst.fg = 0xffffff;
    }
}

void *kalloc(size_t page_count) {
    // If our allocation pointer exceed available memory, we couldn't find anything
    if (pmm_bitmap.used_pages >= pmm_bitmap.total_pages) {
        // Todo: Implement proper log levels
        printf("ERROR: OUT OF MEMORY.\n");
        return NULL;
    }

    void *pointer = find_first_free_page_range(page_count);

    if (pointer == NULL) {
        return NULL;
    }

    uint64_t index = PAGE_TO_BIT(pointer);
    for (size_t i = 0; i < page_count; i++) {
        bitmap_use(&pmm_bitmap, index + i);
    }

    pmm_bitmap.used_pages += page_count;

    return (void *)(pmm_bitmap.lowest_address + BIT_TO_PAGE(index) + g_hhdm_offset);
}

// Allocate n number of pages, return a pointer to the first contiguous page
void *find_first_free_page_range(size_t page_count) {
    // For each page/bit
    for (uint64_t bit_i = 0; bit_i < pmm_bitmap.total_pages; bit_i++) {
        if(!bitmap_check(&pmm_bitmap, bit_i)) {
            // Look ahead page_count bits
            // If we find a range, return the address
            for(size_t page_count_i = 0; page_count_i < page_count; page_count_i++) {
                if(bitmap_check(&pmm_bitmap, bit_i + page_count_i)) {
                    break;
                }

                if(page_count_i == page_count - 1) {
                    return (void *)BIT_TO_PAGE(bit_i);
                }
            }
            
        }
    }
    // Otherwise, return NULL pointer
    return NULL;

    // Old implementation, trying for optimized, but not working quite well
    // // Divide available memory into 64-bit chunks, which is easy, since our bitmap size is already a chunk.
    // pmm_bitmap.size = pmm_bitmap.total_pages / BITS_PER_ROW;
    // size_t page_count_i = 0;
    // for (size_t row_i = 0; row_i < pmm_bitmap.size; row_i++) {
    //     printf("Kalloc: Searching row %d\n", row_i);
    //     uint64_t chunk = pmm_bitmap.map[row_i];
    
    //     // If chunk is full, NEXT!!
    //     if (chunk == UINT64_MAX) {
    //         printf("Chunk is full, skipping \n");
    //         page_count_i = 0;
    //         continue;
    //     }

    //     // For each of the 64 bits
    //     for (uint8_t bit_i = 0; bit_i < 64; bit_i++) {
    //         // If page is already used, NEXT!!
    //         // printf("Kalloc: Searching bit %d", bit_i);
    //         if (bitmap_check(&pmm_bitmap, row_i * BITS_PER_ROW + bit_i)) {
    //             // printf(", taken\n");
    //             continue;
    //         }

    //         // printf(", free\n");
    //         // Otherwise, let's start counting
    //         for (uint8_t bit_j = bit_i;page_count_i < page_count; bit_j++) {
    //             // printf("Starting from %d, looking ahead at bit %d, bit count %d\n ", bit_i, bit_j, page_count_i);
    //             // Look ahead, if there's an used bit and we are not reaching desired page count, skip.
    //             if (bitmap_check(&pmm_bitmap, row_i * BITS_PER_ROW + bit_j)) {
    //                 // printf("Not enough free bits, discard\n");
    //                 page_count_i = 0;
    //                 break;
    //             }

    //             // Otherwise, increment continuous page counter
    //             page_count_i++;
    //         }

    //         if (page_count_i >= page_count) {
    //             // Return pointer
    //             size_t page_index  = (row_i * BITS_PER_ROW + bit_i) - (page_count / BITS_PER_ROW);
    //             printf("Found suitable page starting at bitmap index %d\n", page_index);
    //             return (void *)(BIT_TO_PAGE(page_index));
    //         }
    //     }

    //     // If we get to this point, we have searched everything and can't find anything.
    //     return NULL;
    // }
}

void kfree(void *pointer, size_t page_count) {
    uint64_t index = PAGE_TO_BIT((uintptr_t)(pointer - g_hhdm_offset - pmm_bitmap.lowest_address));
    printf("Freeing %d pages starting at index %d\n", page_count, index);
    for (size_t i = 0; i < page_count; i++) {
        bitmap_free(&pmm_bitmap, index + i);
    }

    pmm_bitmap.used_pages -= page_count;
}

// Dump bitmap memory starting at row index, with count number of rows
void pmm_memdump_rows(size_t row, size_t row_count) {
    printf("===== MEMMAP =====\n");
    for(size_t i = row; i < row + row_count; i++) {
        ssfn_dst.fg = 0xFF00FF;
        printf("[ %05d - %05d ] ", i * BITS_PER_ROW + BITS_PER_ROW - 1, i * BITS_PER_ROW);
        ssfn_dst.fg = 0xFFFFFF;
        printf(
           PRINTF_BINARY_PATTERN_INT64 "\n",
           PRINTF_BYTE_TO_BINARY_INT64(pmm_bitmap.map[i]));
    }
    printf("==================\n");
}