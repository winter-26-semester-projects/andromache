#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "kernel/types.h"
#include "gdt.h"

struct task_frame {
	u32 flags;
	u32 si;
	u32 di;
	u32 bx;
	u32 bp;	
};

struct tss_struct {
	_Alignas(PAGE_SIZE) struct tss hw;
	u32 io_bitmap[2048];
} __packed __aligned(PAGE_SIZE);

struct __task_struct {
	u32 sp;
	u32 ip;
	u32 sp0;
};

static inline void load_cr3(u32 pgdir_phys_addr)
{
	__asm__ volatile("movl %0, %%cr3"
			: : "r" (pgdir_phys_addr) : "memory"
			);
}

static inline void update_tss_stack(struct tss_struct *tss, u32 kstack)
{
	WRITE_ONCE(tss->hw.esp0, kstack);
}

#endif /* _PROCESSOR_H_ */
