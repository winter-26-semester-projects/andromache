#ifndef _GDT_H_
#define _GDT_H_

#include "kernel/types.h"
#include "compiler.h"

#define GDT_LIMIT_LOW_B   0  
#define GDT_LIMIT_LOW_W   16
#define GDT_BASE_LOW_B    16  
#define GDT_BASE_LOW_W    16

#define GDT_BASE_MID_B    0   
#define GDT_BASE_MID_W    8
#define GDT_ACCESS_B      8   
#define GDT_ACCESS_W      8
#define GDT_LIMIT_HIGH_B  16  
#define GDT_LIMIT_HIGH_W  4
#define GDT_FLAGS_B       20  
#define GDT_FLAGS_W       4
#define GDT_BASE_HIGH_B   24  
#define GDT_BASE_HIGH_W   8

struct gdte {
	u16 lim_l;
	u16 base_l;
	u8  base_m;
	u8  access;
	u8  granularity;
	u8  base_h;
} __packed;

typedef union {
	struct gdte entry;
	uint64_t __gdt;	
} gdte_t;

extern gdte_t gdt[6];

struct tss {
	u32 prv_tss;
	u32 esp0;
	u32 ss0;
	u32 esp1, ss1, esp2, ss2;
	u32 cr3;
	u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	u32 es, cs, ss, ds, fs, gs;
	u32 ldt;
	u16 trap;
	u16 iomap_base;
} __packed;

static inline void __init set_tss_desc(gdte_t *gde, struct tss *tss_struct)
{
	addr_t base = (addr_t)tss_struct;
	u32 lim = sizeof(struct tss) - 1;

	u32 *gd = (u32*)gde;

	u32 low = 0;
	low = BIT_PACK32(low, GDT_LIMIT_LOW_W, GDT_LIMIT_LOW_B, (lim & 0xFFFF));
	low = BIT_PACK32(low, GDT_BASE_LOW_W, GDT_BASE_LOW_B, (base & 0xFFFF));	
	gd[0] = low;

	u32 high = 0;
	high = BIT_PACK32(high, GDT_BASE_MID_W, GDT_BASE_MID_B, (base >> 16) & 0xFF);
	high = BIT_PACK32(high, GDT_ACCESS_W, GDT_ACCESS_B, 0x89);
	high = BIT_PACK32(high, GDT_LIMIT_HIGH_W, GDT_LIMIT_HIGH_B, (lim >> 16) & 0x0F);
	high = BIT_PACK32(high, GDT_FLAGS_W, GDT_FLAGS_B, 0x00);
	high = BIT_PACK32(high, GDT_BASE_HIGH_W, GDT_BASE_HIGH_B, (base >> 24) & 0xFF);
	gd[1] = high;
}

static inline void load_tr(u16 stss)
{
	__asm__ volatile("ltr %0" :  :  "r"(stss));
}

void gdt_init(void);

#endif /* _GDT_H_ */
