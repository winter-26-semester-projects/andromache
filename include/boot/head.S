.set MAGIC, 0xe85250d6
.set PROTECTION_MODE, 0
.set HEADER_LENGTH, 32
.set CHECKSUM, -(MAGIC + PROTECTION_MODE + HEADER_LENGTH)

.section .multiboot
.balign 8
header_start:
    .long MAGIC
    .long PROTECTION_MODE
    .long HEADER_LENGTH
    .long CHECKSUM

    .balign 8
    .short 1
    .short 0
    .long 8

    .balign 8
    .short 0
    .short 0
    .long 8
header_end:

.section .text
.global _start
_start:
    movl $stack_top, %esp
    subl $0xC0000000, %esp

    movl $early_kernel_init, %eax
    subl $0xC0000000, %eax
    call *%eax
    cli
.1:
    hlt
    jmp .1

.section .bss
.balign 16
stack_top:
    .skip 16384
