#ifndef KERNEL_MEM_PMM_H
#define KERNEL_MEM_PMM_H

#include <stdint.h>
#include <stddef.h>   
#include <stdbool.h>

#include <kernel/limine.h>

// We use 4096 bytes per page, since Limine memory map is aligned at 4kB
#define PAGE_SIZE 0x1000
#define BITS_PER_ROW 64

#define FREE 0x00
#define USED 0xff

static uint64_t g_total_memory = 0;
static uint64_t g_allocation_index = 0;

struct pmm_bitmap_t {
    uint64_t *map;
    size_t size;
    size_t page_count;
    size_t highest_address;
    size_t lowest_address;
};

void pmm_initialize(void);
void print_memmap(struct limine_memmap_entry **entries, uint64_t entry_count);

// Check if an address in bitmap is used or not
static inline bool bitmap_check(struct pmm_bitmap_t *bitmap, uint64_t bit_number) {
    /*
    Napkin math:
    - Let's say we want to set bit 7 of bitmap[3]
    bitmap[3] 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0100 0000

    addr in this case would be (64 * 3 + 7) * 4096;
    */

    // Given an address, check if it's used or not
    // For that we need to calculate the bit_number
    uint32_t row = bit_number / BITS_PER_ROW;
    uint32_t column = bit_number % BITS_PER_ROW;

    return (bitmap->map[row] >> column) & 1;
}

// Given a bit_number offset, set the page as USED
// @NOTE: Since we are doing 64 bit bitmap rows, we have to use 1LL instead of 1, otherwise it'll implicitly cast.
static inline void bitmap_use(struct pmm_bitmap_t *bitmap, uint64_t bit_number) {
    bitmap->map[bit_number / BITS_PER_ROW] |= 1LL << (bit_number % BITS_PER_ROW);
}
// Free a page at offset
static inline void bitmap_free(struct pmm_bitmap_t *bitmap, uint64_t bit_number) {
    bitmap->map[bit_number / BITS_PER_ROW] &= ~ (1LL << bit_number % BITS_PER_ROW);
}

// size_t allocate_page(void);
// void free_page(size_t index);

void kfree(void *pointer, size_t page_count);
void *kalloc(size_t page_count);
void *find_first_free_page_range(size_t page_count);

void pmm_memdump_rows(size_t row, size_t row_count);

#endif // KERNEL_MEM_PMM_H