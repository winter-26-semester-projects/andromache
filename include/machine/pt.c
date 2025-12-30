#include<stdint.h>

#include "kernel/kernel.h"
#include "pt.h"
#include "tty/vga.h"

void setup_page_tables()
{
	u32 *pd = phys((u32*)boot_page_directory);
	u32 *pt = phys((u32*)boot_page_table1);

	uintptr_t phys_pd = (uintptr_t)pd;
	uintptr_t phys_pt = (uintptr_t)pt;

	for (int i = 0; i < 1024; ++i) {

		pd[i] = 0;
		pt[i] = 0;
	}

	pd[0] 	= phys_pt | 0x3;
	pd[768] = phys_pt | 0x3;

	for (int i = 0; i < 1024; ++i) {
		
		pt[i] = (i * PAGE_SIZE) | 0x3;
	}

	vga_setup_page(pt);
}
