cmake_minimum_required(VERSION 3.15)

project(Andromache LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_library(kernel_objs OBJECT
    include/boot/head.S
    src/kernel.c
    src/init_task.c
)

set_source_files_properties(include/boot/head.S PROPERTIES LANGUAGE ASM)

target_include_directories(kernel_objs
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(kernel_objs
    PRIVATE
        -ffreestanding
        -fno-stack-protector
        -fno-pic
        -m32
)

add_subdirectory(src/modules/ipc)

add_executable(kernel.elf)

target_link_libraries(kernel.elf
    PRIVATE
        kernel_objs
        ipc
)

target_compile_options(ipc PRIVATE
    -ffreestanding
    -fno-stack-protector
    -fno-pic
    -fno-exceptions
    -fno-rtti
    -m32
)

target_link_options(kernel.elf PRIVATE
    -m32
    -nostdlib
    -ffreestanding
    -T ${CMAKE_CURRENT_SOURCE_DIR}/include/boot/kernel.ld
    -Wl,-Map=kernel.map
)

set_target_properties(kernel.elf PROPERTIES
    LINKER_LANGUAGE C
    LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/boot/kernel.ld
)
