#ifndef _BOOT_H_
#define _BOOT_H_

#include "kernel/types.h"
#include "compiler.h"

struct start {
        uint32_t magic;
        uintptr_t kernel_entry;
        uint32_t flags;
        uint32_t checksum;
        uint32_t hgr_mem;
        uint32_t cmdline;
} __packed;

int early_kernel_init(struct start*);

void early_kernel_main(void);

#endif /* _BOOT_H_ */

