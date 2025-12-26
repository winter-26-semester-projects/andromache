.set MAGIC, 0xe85250d6
.set PROTECTION_MODE, 0
.set HEADER_LENGTH, 32
.set CHECKSUM, -(MAGIC + PROTECTION_MODE + HEADER_LENGTH)

.set VIRTUAL_BASE, $0xC0000000

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
    subl VIRTUAL_BASE, %esp

    movl $_init, %eax
    subl VIRTUAL_BASE, %eax
    call *%eax

    movl $early_kernel_main, %eax
    subl VIRTUAL_BASE, %eax
    call *%eax

    movl $boot_page_directory, %eax
    subl VIRTUAL_BASE, %eax
    movl %eax, %cr3

    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    addl VIRTUAL_BASE, %esp
    movl $kernel_main, %eax
    jmp *%eax
    cli
.1:
    hlt
    jmp .1

.section .bss
.balign 16
stack_top:
    .skip 16384
