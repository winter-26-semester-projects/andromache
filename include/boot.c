#include "boot/boot.h"
#include "kernel/kernel.h"
#include "tty/serial/serial.h"
#include "machine/pt.c"

extern void _header_checksum;

extern void kernel_main(void);

struct start header_t = {
        /**
        * kernel header:
        * .magic        : magic number is ‘‘ANDR’’ in ASCII
        * .kernel_entry : physical entry point of the kernel
        * .flag         : serial tty flag; ‘0’ to enable tty, ‘1’ to disable
        * .checksum     : sum is equal to magic number + kernel entry addr + flag
        * .hgh_mem      : start of higher memory region
        * .cmdline	: tty0 flag; ‘1’ to enable tty0, ‘0’ to disable
        */

        .magic        = KERNEL_MAGIC,                          
        .kernel_entry = (uintptr_t)kernel_main,                
        .flags        = 0x00000001,                            
        .checksum     = (uint32_t)addr(_header_checksum),
        .hgr_mem      = 0x1000,
        .cmdline      = 0
};

int early_kernel_init(struct start *hdr)
{
        /* checks if the loaded kernel header matches */
        
        if (hdr->magic != KERNEL_MAGIC) return 0;
        
        uint32_t chk = hdr->magic + (uint32_t)hdr->kernel_entry + hdr->flags;
        if (hdr->checksum != 0 && chk != hdr->checksum) return 0;

        if (hdr->hgr_mem < 0x1000) return 0;
        
        return 1;
}

void early_kernel_main()
{
        /**
         * makes sure CPU does not execute any instructions in case the header is incorrect,
         * and enables tty if flag is on, and enables paging before handoff
         */

        struct start *hdr = (struct start *)(addr(header_t) - HEADER_OFFSET);

        if (!early_kernel_init(hdr)) {

                for (;;) {

                        __asm__("hlt");
                }
        }

	char* ok = phys((char*)"systemd : [ OK ] headers validated\n");

        if (hdr->flags & 0x1) {

                early_serial_init(UART_PORT_COM1);

                serial_write(UART_PORT_COM1, ok);
        }

<<<<<<< HEAD
#ifdef _TTY_TTY0_H	
=======
#ifdef _TTY_TTY0_H
>>>>>>> 09bcd267cd5c15384bcb0c1871c3e700763a0037
	if (hdr->cmdline & 0x1) {
		
		tty_init();

		tty_writeline(ok);
	}
#endif
        setup_page_tables();
}

