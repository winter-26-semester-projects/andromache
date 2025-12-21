#include "machine/isr.h"
#include "machine/idt.h"
#include <stddef.h>

isr_t interrupt_handlers[256];

void isr_install_handler(int n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t* r) {
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
}

// Stub for ISRs (usually implemented in ASM, but we need C bindings)
// We need a real ASM file to handle the interrupt entry/exit and call `isr_handler`
// For now, I will create a minimal ASM file for interrupts.
