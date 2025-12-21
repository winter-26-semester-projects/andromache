SECTIONS {
    . = 1M;
    .text : {
        KEEP(*(.multiboot))
        *(.text)
    }
}
