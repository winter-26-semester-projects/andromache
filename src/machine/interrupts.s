.global isr_stub_table
.extern isr_handler

.section .text

# Macros to create ISR stubs
.macro ISR_NOERRCODE num
    .global isr\num
    isr\num:
        cli
        push $0
        push $\num
        jmp isr_common_stub
.endm

.macro ISR_ERRCODE num
    .global isr\num
    isr\num:
        cli
        push $\num
        jmp isr_common_stub
.endm

isr_common_stub:
    pusha
    mov %ds, %ax
    push %eax
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Push pointer to stack (ESP points to regs)
    push %esp
    call isr_handler
    add $4, %esp

    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    popa
    add $8, %esp
    sti
    iret

# Define a few critical ISRs
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 32 # Timer
ISR_NOERRCODE 128 # Syscall

# Table of pointers to ISRs
.section .data
isr_stub_table:
    .long isr0
    .long isr1
    # ... fill gaps ...
    .long isr32
    .long isr128
