cmake_minimum_required(VERSION 3.15)

set(MODULE_NAME "ipc")
set(MODULE_LIB "${MODULE_NAME}")

set(MODULE_SRCS
    ipc.c
)

set(MODULE_PRIVATE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/private")

add_library(${MODULE_LIB} STATIC ${MODULE_SRCS})

target_include_directories(${MODULE_LIB}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${MODULE_PRIVATE_INCLUDE}
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(${MODULE_LIB} PRIVATE
    -ffreestanding
    -fno-stack-protector
    -fno-pic
    -m32
    )
endif()