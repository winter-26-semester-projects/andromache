#ifndef MMU_MMU_H
#define MMU_MMU_H

#include<stdint.h>
#include<stdbool.h>
#include "kernel/kernel.h"
#include "machine/pt.h"

#define mmu_flags_t u32

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

#endif /* MMU_MMU_H */
