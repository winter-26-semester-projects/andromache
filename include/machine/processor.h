#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include<stdint.h>
#include "kernel/kernel.h"
#include "pt.h"

#define CR3_ADDR_MASK 0xffffffffU
#define CR3_PCID_MASK 0
#define CR3_NOFLUSH   0

struct task_frame {
	u32 flags;

	u32 si;

	u32 di;

	u32 bx;

	u32 bp;	
};

struct tss_struct {

	_Alignas(PAGE_SIZE) struct hw_tss tss;

	struct io_bitmap io_bitmap;
};

struct __task_struct {
	u32 sp;
	u32 ip;
	u32 sp0;
};

static inline void load_cr3(u32 pgdir_phys_addr)
{
	__asm__ volatile("movl %0, %%cr3
			: : "r" (pgdir_phys_addr) : "memory"
			);
}

#endif /* _PROCESSOR_H_ */
