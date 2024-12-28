.global gdt_flush
gdt_flush:
    lgdt (%rdi)             /* Load GDT from RDI register */
/* Reload Segment Registers */
    mov $0x10, %ax          /* 0x30: Code segment */
    mov %ax, %ds            /* Update segment registers */
    mov %ax, %es        
    mov %ax, %fs        
    mov %ax, %gs        
    mov %ax, %ss        
/* Reload CS Registers */    
    pop %rdi
    mov $0x08, %rax         /* 0x28: Data segment */
    push %rax
    push %rdi
    retfq                   /* Long jump */ 