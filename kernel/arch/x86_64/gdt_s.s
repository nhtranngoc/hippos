.global gdt_flush
gdt_flush:
    lgdt (%rdi)             /* Load GDT from RDI register */
    mov $0x30, %ax          /* Why 0x30? */
    mov %ax, %ds            /* Update segment registers */
    mov %ax, %es        
    mov %ax, %fs        
    mov %ax, %gs        
    mov %ax, %ss        
    pop %rdi
    mov $0x28, %rax         /* TSS stuff? */
    push %rax
    push %rdi
    retfq                   /* Long jump */ 