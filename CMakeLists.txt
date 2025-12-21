cmake_minimum_required(VERSION 3.10)
project(AndromacheOS C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -m32")
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -m32 -nostdlib -no-pie")

include_directories(include)
include_directories(extern/linux/include)

# Gather source files
file(GLOB_RECURSE SOURCES
    "src/boot/*.s"
    "src/machine/*.c"
    "src/machine/*.s"
    "src/kernel.c"
    "src/libc.c"
    "src/modules/scheduler/*.c"
    "src/modules/task/*.c"
    "src/modules/memory/*.c"
    "src/modules/ipc/*.c"
    "src/modules/child_handler/*.c"
    "src/modules/signal_handler/*.c"
    "src/modules/shell/*.c"
    "src/modules/daemon/*.c"
)

add_executable(andromache.bin ${SOURCES})

# Custom target for ISO (requires grub-mkrescue)
add_custom_target(iso
    COMMAND mkdir -p isodir/boot/grub
    COMMAND cp andromache.bin isodir/boot/andromache.bin
    COMMAND cp ${CMAKE_SOURCE_DIR}/boot/grub/grub.cfg isodir/boot/grub/grub.cfg
    COMMAND grub-mkrescue -o andromache.iso isodir
    DEPENDS andromache.bin
)
