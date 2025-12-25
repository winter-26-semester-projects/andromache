#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include <stdint.h>
#include "../boot/boot.h"

#define LOGICAL(addr) ((addr) + HEADER_OFFSET)

void kernel_init(void);

void kernel_main(void);

#endif /* KERNEL_KERNEL_H */
