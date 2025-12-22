ENTRY(_start)

SECTIONS
{
    . = 1M;
    __kernel_start = .;

    .text ALIGN(4K) : {
        KEEP(*(.multiboot))
        *(.text*)
    }

    .rodata ALIGN(4K) : {
        *(.rodata*)
    }

    .data ALIGN(4K) : {
        *(.data*)
    }

    .bss ALIGN(4K) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    __kernel_end = .;
}
