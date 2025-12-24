cmake_minimum_required(VERSION 3.15)

project(Andromache LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -m32")

function(get_crt_files name out)
    execute_process(
        COMMAND ${CMAKE_C_COMPILER} -m32 -print-file-name=${name}
        OUTPUT_VARIABLE _out
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${out} "${_out}" PARENT_SCOPE)
endfunction()

get_crt_files(crtbegin.o CRTBEGIN_PATH)
get_crt_files(crtend.o CRTEND_PATH)

add_library(crt_i OBJECT include/crti.S)
target_compile_options(crt_i PRIVATE -m32)

add_library(crt_n OBJECT include/crtn.S)
target_compile_options(crt_n PRIVATE -m32)

add_library(kernel_objs OBJECT
    include/boot/head.S
    include/tty/serial/serial.c
    include/boot.c
    src/kernel.c
    src/init_task.c
)

target_include_directories(kernel_objs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(kernel_objs PRIVATE -ffreestanding -fno-stack-protector -fno-pic -m32)

add_subdirectory(src/modules/ipc)

add_executable(kernel.elf)
target_compile_definitions(kernel.elf PRIVATE __is_kernel)

target_link_options(kernel.elf PRIVATE
    -m32
    -nostdlib
    -no-pie
    -static
    -ffreestanding
    -T ${CMAKE_CURRENT_SOURCE_DIR}/include/boot/kernel.ld
    -Wl,-Map=kernel.map
    -Wl,--build-id=none
    "$<TARGET_OBJECTS:crt_i>"
    "${CRTBEGIN_PATH}"
)

target_link_libraries(kernel.elf
    PRIVATE
        kernel_objs
        ipc
        "${CRTEND_PATH}"
        "$<TARGET_OBJECTS:crt_n>"
)

target_compile_options(ipc PRIVATE
    -ffreestanding -fno-stack-protector -fno-pic -fno-exceptions -fno-rtti -m32
)

set_target_properties(kernel.elf PROPERTIES
    LINKER_LANGUAGE C
    LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/boot/kernel.ld
)
