#include "kernel/kernel.h"
#include "config.h"

void kernel_init(void) 
{
        (void) CONFIG_MAX_THREADS;
}

void kernel_main(void) 
{
        for (;;) {

            __asm__ volatile ("hlt");
        }
}
