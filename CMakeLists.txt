cmake_minimum_required(VERSION 3.15)
project(Andromache LANGUAGES C ASM)

set(LINUX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/linux" CACHE PATH "Linux Path")

set(KERNEL_SOURCES
    src/kernel.c
    src/boot.s
    src/libc.c
)

add_executable(kernel ${KERNEL_SOURCES})

target_include_directories(kernel
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${LINUX_PATH}/include
        ${LINUX_PATH}/arch/x86/include
        ${LINUX_PATH}/mm
        ${LINUX_PATH}/arch/x86/kernel
)

set(MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")

if(EXISTS "${MODULES_DIR}")
    file(GLOB MODULE_DIRS RELATIVE "${MODULES_DIR}" "${MODULES_DIR}/*")
    foreach(MOD ${MODULE_DIRS})
        if(IS_DIRECTORY "${MODULES_DIR}/${MOD}")
            add_subdirectory("${MODULES_DIR}/${MOD}")
            target_link_libraries(kernel PRIVATE ${MOD})
            message(STATUS "Added module: ${MOD}")
        endif()
    endforeach()
endif()

set_target_properties(kernel PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
)

# Force 32-bit and no-pie for freestanding kernel
target_compile_options(kernel PRIVATE
    -Wall -Wextra -Wno-missing-field-initializers
    -m32 -fno-pie
)

option(KERNEL_FREESTANDING "Compile as freestanding kernel" ON)
if(KERNEL_FREESTANDING)
    target_compile_options(kernel PRIVATE -ffreestanding)
    target_link_options(kernel PRIVATE
        -nostdlib
        -m32
        -no-pie
        -Wl,--build-id=none
        -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
    )
endif()

message(STATUS "Kernel include: ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Linux include: ${LINUX_PATH}")
