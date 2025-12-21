.set MAGIC, 0xe85250d6
.set PROTECTION_MODE, 0
.set HEADER_LENGTH, header_end - header_start
.set CHECKSUM, -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

.section .multiboot
align 8
    .long MAGIC
    .long PROTECTION_MODE                     
    .long HEADER_LENGTH
    .long CHECKSUM

    .word 0    
    .word 0  
    .long 8 
header_end:

.section .text
.global _start
_start:
    mov $stack_top, %esp

    push %ebx
    push %eax

    call kernel_main
    
    cli
1:  hlt
    jmp 1b

.section .bss
.align 16
    .skip 16384
stack_top:
