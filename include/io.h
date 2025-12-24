#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

/* Output a byte to a hardware port */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

/* Input a byte from a hardware port */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

/* Optional: I/O Wait (needed for some older hardware compatibility) */
static inline void io_wait(void) {
    /* Port 0x80 is used for checkpoints during POST; safe for small delays */
    outb(0x80, 0);
}

#endif
