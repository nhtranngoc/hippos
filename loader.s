global loader                           ; the entry symbol for ELF

MAGIC_NUMBER    equ     0x1BADB002      ; define the magic number constant
FLAGS           equ     0x0             ; multiboot flags
CHECKSUM        equ     -MAGIC_NUMBER   ; calculate the checksum = (magic number + checksum + flag) should equal 0

section .text:                          ; start of the text (code) section
align 4                                 ; the code must be 4 bytes aligned
    dd MAGIC_NUMBER                     ; write the magic number, flags, and checksum to the machine code (@note: what's that?)
    dd FLAGS                            
    dd CHECKSUM

loader:                                 ; loader label, defined as entry point in linker script
    mov eax, 0xCAFEBABE                 ; place the number CAFEBABE to eax register
.loop:
    jmp .loop                           ; loop forever
