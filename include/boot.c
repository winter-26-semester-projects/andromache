#include "boot/boot.h"
#include "tty/serial/serial.h"

#define HEADER_OFFSET 0xC0000000U
extern uint8_t _header_checksum;

extern void _start(void);

struct start header_t = {
    .magic = KERNEL_MAGIC,                              // ‘‘ANDR’’ in ASCII
    .kernel_entry = (uintptr_t)_start,                  // kernel entry point
    .flags = 0x00000001,                                // ‘0’ to enable tty, ‘1’ to disable
    .checksum = (uint32_t)(uintptr_t)&_header_checksum, // magic number + kernel entry addr + flag
    .hgr_mem = 0x1000,
    .cmdline = 0
};

#define PHYS(ptr) ((__typeof__(ptr))(uintptr_t)((uintptr_t)(ptr) - 0xC0000000U))

__attribute__((aligned(4096))) uint32_t boot_page_directory[1024];
__attribute__((aligned(4096))) uint32_t boot_page_table1[1024];

void setup_handoff_paging() {
    uint32_t *pd = PHYS(boot_page_directory);
    uint32_t *pt = PHYS(boot_page_table1);

    uintptr_t phys_pd = (uintptr_t)pd;
    uintptr_t phys_pt = (uintptr_t)pt;

    // Identity map 0-4MB
    pd[0] = phys_pt | 0x3;

    // Map Higher Half (0xC0000000 / 4MB = index 768)
    pd[768] = phys_pt | 0x3; 

    // Fill PT physically
    for (int i = 0; i < 1024; i++) {
        pt[i] = (i * 4096) | 0x3;
    }

    __asm__ volatile("mov %0, %%cr3" : : "r"(phys_pd));

    // Enable Paging (Set PG bit in CR0)
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}

int early_kernel_init(struct start *hdr) {

        /* Checks if the loaded kernel header matches after boot */
        
        if (hdr->magic != KERNEL_MAGIC) return 0;
        
        uint32_t chk = hdr->magic + (uint32_t)hdr->kernel_entry + hdr->flags;
        if (hdr->checksum != 0 && chk != hdr->checksum) return 0;

        if (hdr->hgr_mem < 0x1000) return 0;
        
        return 1;
}

void early_kernel_main() {

        struct start *hdr = (struct start *)((uintptr_t)&header_t - HEADER_OFFSET);

        if (!early_kernel_init(hdr)) {

                for (;;) {

                        __asm__("hlt"); 
                }
        }

        if (hdr->flags & 0x1) {

                early_serial_init(UART_PORT_COM1);

                serial_write(UART_PORT_COM1, PHYS((char*)"ANDROMACHE Boot: Header validated.\r\n"));
        }

        setup_handoff_paging();
        
        uintptr_t entr = (uintptr_t)hdr->kernel_entry;
        
        void (*kernel_main)(void) = (void (*)(void))entr;
        
        serial_write(UART_PORT_COM1, PHYS((char*)"ANDROMACHE: Jumping to Higher Half (0xC0000000)...\r\n"));
        kernel_main();
}

