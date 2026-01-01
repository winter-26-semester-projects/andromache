#include "gdt.h"
#include "kernel/types.h"
#include "compiler.h"

gdte_t gdt[6];

struct gdt_ptr {
	u16 lim;
	u32 base;
} __packed;

void gdt_init(void)
{
	struct gdt_ptr *gd;

	gd->lim = (sizeof(gdt))-1;
	gd->base = (u32)&gdt;

	__asm__ volatile("lgdt %0" : : "m"(*gd));
}
