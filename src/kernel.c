#include "kernel/types.h"
#include "kernel/kernel.h"
#include "kernel/task.h"
#include "compiler.h"
#include "machine/processor.h"
#include "machine/gdt.h"
#include "config.h"
#include "mm/mmu.h"

struct tss_struct ktss = {0};

void task_init(void)
{	
	gdt_init();

	set_tss_desc(&gdt[5], &ktss.hw);

	ktss.hw.ss0 = kcs(2);

	load_tr(kcs(5));
}

void kernel_main(void) 
{
	volatile unsigned short* vga_buffer = (unsigned short*)phys_to_virt(0xB8000);

	for (int i = 0; i < 80 * 25; i++) {
		vga_buffer[i] = (0x07 << 8) | ' ';
	}

	const char* str = "Hello, world.";
	for (int i = 0; str[i] != '\0'; i++) {
		vga_buffer[i] = (0x0F << 8) | str[i];
	}
	
	task_init();

	for (;;) {
		__asm__ volatile ("hlt");
	}
}

