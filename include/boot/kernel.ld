ENTRY(_start)

LOGIC_BASE = 0xC0000000;
PHYS_BASE = 1M;

SECTIONS
{
    . = LOGIC_BASE + PHYS_BASE;
    __kernel_start = .;

    .text ALIGN(4K) : AT(ADDR(.text) - LOGIC_BASE) {
        KEEP(*(.multiboot))
        *(.text*)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - LOGIC_BASE) {
        *(.rodata*)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - LOGIC_BASE) {
        *(.data*)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - LOGIC_BASE) {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    _header_checksum = 0x414E4452 + kernel_main + 0x1;

    __kernel_end = .;
}
