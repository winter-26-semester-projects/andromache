#ifndef _BOOT_H_
#define _BOOT_H_

#include<stdint.h>

#define HEADER_OFFSET 0xC0000000U

#define KERNEL_MAGIC 0x414e4452

struct start {
        uint32_t magic;
        uintptr_t kernel_entry;
        uint32_t flags;
        uint32_t checksum;
        uint32_t hgr_mem;
        uint32_t cmdline;
} __attribute__((packed));

int early_kernel_init(struct start*);

void early_kernel_main(void);

#endif /* _BOOT_H_ */

