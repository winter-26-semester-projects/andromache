#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include <stdint.h>
#include "boot/boot.h"

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

typedef u32 addr_t;

#define phys_to_virt(addr) ((addr) + HEADER_OFFSET)

void kernel_init(void);

void kernel_main(void);

#endif /* KERNEL_KERNEL_H */
