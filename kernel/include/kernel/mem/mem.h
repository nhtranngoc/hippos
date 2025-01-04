#ifndef KERNEL_MEM_MEM_H
#define KERNEL_MEM_MEM_H

#include <stdint.h>
#include <stddef.h>   
#include <stdbool.h>

#include <kernel/limine.h>

// We use 4096 bytes per page, since Limine memory map is aligned at 4kB
#define PAGE_SIZE 0x1000
#define BITS_PER_ROW 64

#define FREE 0x00
#define USED 0xff

static uint32_t g_num_of_pages = 0;
static uint64_t *g_bitmap = NULL;
static uint64_t g_total_memory = 0;

void mem_initialize(void);
void print_memmap(struct limine_memmap_entry **entries, uint64_t entry_count);

// Check if an address in bitmap is used or not
static inline bool bitmap_check(size_t addr) {
    /*
    Napkin math:
    - Let's say we want to set bit 7 of bitmap[3]
    bitmap[3] 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0100 0000

    addr in this case would be (64 * 3 + 7) * 4096;
    */

    // Given an address, check if it's used or not
    // For that we need to calculate the bit_number
    uint64_t bit_number = addr / PAGE_SIZE;
    uint32_t row = bit_number / BITS_PER_ROW;
    uint32_t column = bit_number % BITS_PER_ROW;

    return (g_bitmap[row] >> column) & 1;
}

// Given a bit_number offset, set the page as USED
static inline void bitmap_use(size_t bit_number) {
    g_bitmap[bit_number / BITS_PER_ROW] |= 1 << (bit_number % BITS_PER_ROW);
}

// Free a page at offset
static inline void bitmap_free(size_t bit_number) {
    g_bitmap[bit_number / BITS_PER_ROW] &= ~ (1 << bit_number % BITS_PER_ROW);
}

void reserve_pages(uint64_t offset, uint64_t length);
void free_pages(uint64_t offset, uint64_t length);

#endif // KERNEL_MEM_MEM_H