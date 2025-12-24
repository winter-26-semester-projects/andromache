#include "boot/boot.h"
#include "tty/serial/serial.h"

#define HEADER_OFFSET 0xC0000000

extern void _start(void);

struct start header_t = {
    .magic = KERNEL_MAGIC,             // ‘‘ANDR’’ in ASCII
    .kernel_entry = (uintptr_t)_start, // kernel entry point
    .flags = 0x00000001,               // ‘0’ to enable tty, ‘1’ to disable
    .checksum = 0,                     // magic number + kernel entry addr + flag
    .hgr_mem = 0x1000,
    .cmdline = 0
};

void early_kernel_init() {

        /* Grabbing header_t entry point */
        struct start *hdr = (struct start *)((uintptr_t)&header_t - HEADER_OFFSET);

        early_serial_init(0x3F8);
        while(1) {
                serial_write(0x3F8, "STAYING IN EARLY INIT\r\n");
                for(int i = 0; i < 1000000; i++) __asm__("nop"); // Small delay
        }

        // if (hdr->magic != KERNEL_MAGIC) {

        //         /* It *must* do nothing. */
        //         for (;;) {

        //                 __asm__("hlt");
        //         }
        // }

        // uint32_t chk = hdr->magic + (uint32_t)hdr->kernel_entry + hdr->flags;
        // if (chk != hdr->checksum) {

        //         /* It again, *must* do nothing. */
        //         for (;;) {

        //                 __asm__("hlt");
        //         }
        // }

        // if (hdr->flags & 0x1) {

        //         early_serial_init(UART_PORT_COM1);
        //         serial_write(UART_PORT_COM1, "ANDROMACHE Boot: Header Validated.\r\n");
        // }

        // if (hdr->hgr_mem < 0x1000) {

        //         serial_write(UART_PORT_COM1, "ANDROMACHE Boot: Insufficient Memory.\r\n");

        //         /* It again, *must* do nothing. */
        //         for(;;) {

        //                 __asm__("hlt");
        //         }
        // }

        uint32_t entr = (uint32_t)hdr->kernel_entry - HEADER_OFFSET;

        /* Grabbing the actual entry point */
        void (*kernel_main)(void) = (void (*)(void))entr;

        kernel_main();
}

