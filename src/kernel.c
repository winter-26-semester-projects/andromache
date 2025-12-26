#include "kernel/kernel.h"
#include "config.h"

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

    for (;;) {
        __asm__ volatile ("hlt");
    }
}

