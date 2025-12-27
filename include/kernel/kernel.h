#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include <stdint.h>
#include "boot/boot.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef u32 addr_t;

#define phys_to_virt(addr) ((addr) + HEADER_OFFSET)

void kernel_init(void);

void kernel_main(void);

#endif /* KERNEL_KERNEL_H */
