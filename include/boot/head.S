.set MAGIC, 0xe85250d6
.set PROTECTION_MODE, 0
.set HEADER_LENGTH, header_end - header_start
.set CHECKSUM, -(MAGIC + PROTECTION_MODE + HEADER_LENGTH)

.section .multiboot
.balign 8
header_start:
    .long MAGIC
    .long PROTECTION_MODE
    .long HEADER_LENGTH
    .long CHECKSUM
header_end:

.section .text
.global _start
_start:
    movl $stack_top, %esp
    call kernel_main
    cli
.1:
    hlt
    jmp .1

.section .bss
.balign 16
stack_top:
    .skip 16384
