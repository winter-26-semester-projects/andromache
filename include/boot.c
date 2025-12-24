#include "boot/boot.h"
#include "tty/serial/serial.h"

#define HEADER_OFFSET 0xC0000000

extern struct start header_t;

void early_kernel_init() {

        /* Grabbing header_t entry point */
        struct start *hdr = (struct start *)((uint32_t)&header_t - HEADER_OFFSET);

        if (hdr->magic != KERNEL_MAGIC) {

                /* It *must* do nothing. */
                for (;;) {

                        __asm__("hlt");
                }
        }

        uint32_t chk = hdr->magic + hdr->kernel_entry + hdr->flags;
        if (chk != hdr->checksum) {

                /* It again, *must* do nothing. */
                for (;;) {

                        __asm__("hlt");
                }
        }

        if (hdr->flags & 0x1) {

                early_serial_init(0x3F8);
                serial_write(0x3F8, "ANDROMACHE Boot: Header Validated.\n");
        }

        if (hdr->hgr_mem < 0x1000) {

                serial_write(0x3F8, "ANDROMACHE Boot: Insufficient Memory.\n");

                /* It again, *must* do nothing. */
                for(;;) {

                        __asm__("hlt");
                }
        }

        int32_t entr = hdr->kernel_entry - HEADER_OFFSET;

        /* Grabbing the actual entry point */
        void (*kernel_main)(void) = (void (*)(void))entr;

        kernel_main();
}

