#include "machine/timer.h"
#include "machine/isr.h"
#include "modules/scheduler.h"
#include <stdint.h>

uint32_t tick = 0;

static void timer_callback(registers_t* regs) {
    (void)regs;
    tick++;
    scheduler_tick();
}

// Port I/O wrappers (inline for now)
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void timer_init(uint32_t frequency) {
    isr_install_handler(32, timer_callback);

    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );
    outb(0x40, l);
    outb(0x40, h);
}
