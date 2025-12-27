#include "boot/boot.h"
#include "kernel/kernel.h"
#include "tty/serial/serial.h"

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
        * 
        */

        .magic        = KERNEL_MAGIC,                          
        .kernel_entry = (uintptr_t)kernel_main,                
        .flags        = 0x00000001,                            
        .checksum     = (uint32_t)(uintptr_t)&_header_checksum,
        .hgr_mem      = 0x1000,
        .cmdline      = 0
};

#define phys(ptr) ((__typeof__(ptr))(uintptr_t)((uintptr_t)(ptr) - 0xC0000000U))

__attribute__((aligned(4096))) uint32_t boot_page_directory[1024];
__attribute__((aligned(4096))) uint32_t boot_page_table1[1024];

void setup_page_tables()
{
        uint32_t *pd = phys((uint32_t*)boot_page_directory);
        uint32_t *pt = phys((uint32_t*)boot_page_table1);

        uintptr_t phys_pd = (uintptr_t)pd;
        uintptr_t phys_pt = (uintptr_t)pt;

        for (int i = 0; i < 1024; i++) {
                pd[i] = 0;
                pt[i] = 0;
        }

        pd[0]   = phys_pt | 0x3;
        pd[768] = phys_pt | 0x3; 

        for (int i = 0; i < 1024; i++) {

                pt[i] = (i * 4096) | 0x3;
        }

	uint32_t vga_phys = 0xb8000;
	uint32_t vga_virt = (uint32_t)phys_to_virt(0xb8000);

	pt[vga_phys >> 12] = vga_phys | 0x3;
}

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

        struct start *hdr = (struct start *)((uintptr_t)&header_t - HEADER_OFFSET);

        if (!early_kernel_init(hdr)) {

                for (;;) {

                        __asm__("hlt");
                }
        }

        if (hdr->flags & 0x1) {

                early_serial_init(UART_PORT_COM1);

                serial_write(UART_PORT_COM1, phys((char*)"SysBoot: [X] Headers validated\n"));
        }

        setup_page_tables();
}

