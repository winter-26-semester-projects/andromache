#ifndef MM_MMU_H
#define MM_MMU_H

#include<stdbool.h>
#include "kernel/types.h"

static inline kcs_t kcs(u16 idx)
{
	u16 tmp = 0;

	tmp = BIT_PACK16(tmp, CS_IDX_W, CS_IDX_B, idx);
	tmp = BIT_PACK16(tmp, CS_TI_W, CS_TI_B, 0);
	tmp = BIT_PACK16(tmp, CS_RPL_W, CS_RPL_B, 0x00);

	return tmp;
}

static inline cs_t cs(u16 idx)
{
	u16 tmp = 0;

	tmp = BIT_PACK16(tmp, CS_IDX_W, CS_IDX_B, idx);
	tmp = BIT_PACK16(tmp, CS_TI_W, CS_TI_B, 0);
	tmp = BIT_PACK16(tmp, CS_RPL_W, CS_RPL_B, 0x03);

	return tmp;
}

typedef union {
	kcs_t __kcs;
	u16   __rkcs;
} cs_desc_t;

static inline void load_ds(u16 idx)
{
	cs_desc_t tmp;
	tmp.__kcs = kcs(idx);

	__asm__ volatile("mov %0, %%ds"
		: : "r"(tmp.__kcs));
}

static inline void switch_cs(u16 cs)
{
	__asm__ volatile(
			"pushl %0\n\t"
			"pushl $1f\n\t"
			"lret\n\t"
			"1:\n\t"
			: : "r"((u32)cs) : "memory");
};

enum mmu_flags {
	MMU_RO   = 0,
	MMU_RW   = 1 << 1,
	MMU_EXEC = 1 << 2,
	MMU_USER = 1 << 3,
};

enum mmu_err {
	/**
	 * FAIL IN CASE OF SHARED MEM ACCESS (SAVES CPU CYCLES)
	 * INSUFFICENT MEMROY IN CASE OF A BAD MAP
	 * SUCCESS IN CASE OF A NO-FAULT PAGE
	 * */
	FAIL 	= -1,
	ISM 	= 0,
	SUCC 	= 1,
};

enum mmu_err mmu_map(uintptr_t virt, uintptr_t phys, mmu_flags_t flags);

#endif /* MM_MMU_H */
