#ifndef _IO_H_
#define _IO_H_

#include "kernel/types.h"

static inline void outb(u16 port, u8 val) 
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(u16 port) 
{
    u8 ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void io_wait(void) 
{
    outb(0x80, 0);
}

#endif /* _IO_H_ */
