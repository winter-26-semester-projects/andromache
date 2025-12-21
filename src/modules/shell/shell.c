#include "modules/shell.h"
#include <stddef.h>
#include <stdint.h>

// Mock I/O functions
void kprint(const char* str);
char kgetc(); // blocking get char

void shell_start(void) {
    kprint("AndromacheOS Shell > ");

    // In a real implementation, this would loop and read input
    // For now, it's just an entry point task.
    while(1) {
        // Do nothing / yield
        // yield();
    }
}

// Temporary stubs
void kprint(const char* str) {
    // Write to VGA buffer (0xB8000)
    static uint16_t* vga_buffer = (uint16_t*)0xB8000;
    static int cursor = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        vga_buffer[cursor++] = (uint16_t)str[i] | 0x0700;
        if (cursor >= 80*25) cursor = 0;
    }
}
