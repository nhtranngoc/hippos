#ifndef KERNEL_MACROS_H
#define KERNEL_MACROS_H

#define min(a,b)                    (((a) < (b)) ? (a) : (b))
#define max(a,b)                    (((a) > (b)) ? (a) : (b))

#define HIGHER_HALF_DATA	0xFFFF800000000000UL
#define HIGHER_HALF_CODE	0xFFFFFFFF80000000UL
#define HIGHER_HALF_END		0xFFFFFFFFFFFFFFFFUL

#define GiB 0x40000000UL

#define HEAP_MAX_SIZE	(4 * GiB)
#define HEAP_START_ADDR	0xFFFF900000000000

#define KB_TO_PAGES(kb)		    (((kb) * 1024) / PAGE_SIZE)
// Align address up (and down) to the nearest size (must be a power of 2)
#define ALIGN_DOWN(address, align)  ((address) & ~((align)-1))
#define ALIGN_UP(address, align)    (((address) + (align)-1) & ~((align)-1))

#define BIT_TO_PAGE(bit)    ((size_t)bit * PAGE_SIZE)
#define PAGE_TO_BIT(page)   ((size_t)page / PAGE_SIZE)

#define PHYS_TO_HIGHER_HALF_DATA(address)   (address + HIGHER_HALF_DATA)
#define PHYS_TO_HIGHER_HALF_CODE(address)   (address + HIGHER_HALF_CODE)
#define HIGHER_HALF_DATA_TO_PHYS(address)   (address - HIGHER_HALF_DATA)
#define HIGHER_HALF_CODE_TO_PHYS(address)   (address - HIGHER_HALF_CODE)

#define IS_ALIGNED(a, s)`             (ALIGN_UP((s), (a)) == (s))


#endif // KERNEL_MACROS_H_