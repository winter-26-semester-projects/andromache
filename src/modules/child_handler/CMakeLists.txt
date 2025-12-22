cmake_minimum_required(VERSION 3.15)

set(MODULE_NAME "child_handler")
set(MODULE_LIB "${MODULE_NAME}")

set(MODULE_SRCS
    child_handler.c
    private/_child_handler.cpp
)

set(MODULE_PRIVATE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/private")

add_library(${MODULE_LIB} STATIC ${MODULE_SRCS})

target_include_directories(${MODULE_LIB}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${MODULE_PRIVATE_INCLUDE}
)

target_compile_options(${MODULE_LIB} PRIVATE
    -ffreestanding
    -fno-stack-protector
    -fno-pic
    -fno-exceptions
    -fno-rtti
    -m32
)
