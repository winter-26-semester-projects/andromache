#ifndef _TYPES_H_
#define _TYPES_H_

// special consts
#define PAGE_SIZE     4096

#define HEADER_OFFSET 0xc0000000U
#define KERNEL_MAGIC  0x414e4452

#define CR3_ADDR_MASK 0xffffffffU
#define CR3_PCID_MASK 0
#define CR3_NOFLUSH   0

#define CS_RPL_B 0
#define CS_TI_B  2
#define CS_IDX_B 3
#define CS_RPL_W 2
#define CS_TI_W  1
#define CS_IDX_W 0x0d

#define VGA_PHYS_ADDR 0xb8000
#define VGA_VIRT_BASE phys_to_virt(VGA_PHYS_ADDR)

#define VGA_HEIGHT 20
#define VGA_WIDTH  80

#define UART_PORT_COM1 0x3f8

#define UART_TX  0
#define UART_IER 1
#define UART_FCR 2
#define UART_LCR 3
#define UART_MCR 4
#define UART_LSR 5

#define UART_LCR_DLAB 0x80
#define UART_LSR_THRE 0x20

#define IPC_PRIORITY_LOW    0
#define IPC_PRIORITY_NORMAL 1
#define IPC_PRIORITY_HIGH   2

#define IPC_MAX_MESSAGE_SIZE 4096
#define IPC_MAX_QUEUE_DEPTH  64  

#define IPC_MAX_ENDPOINTS 128

#define WNOHANG 	    0
#define WEXITSTATUS(status) ((status) & 0xff)
#define WIFEXITED(status)   1

// int size types
#include<stdint.h>

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define mmu_flags_t u32
#define ipc_endpoint_t u32

// alias types
typedef u32 addr_t;
typedef u16 kcs_t;
typedef u16 cs_t;
typedef u32 pid_t;

// linker vars
#ifdef __is_kernel
extern u8 LOGIC_BASE;

extern char __kernel_start[];
extern char __kernel_end[];
extern char __bss_start[];
extern char __bss_end[];

extern void _header_checksum;
#endif

// utility macros
#define phys_to_virt(addr) ((addr) + HEADER_OFFSET)

#define addr(var) ((uintptr_t)(void*)&(var))
#define phys(ptr) ((__typeof__(ptr))(uintptr_t)((uintptr_t)ptr - addr(LOGIC_BASE)))
 
#define BIT_MASK(W) ((1UL << (W)) - 1)

#define BIT_PACK16(n, W, B, k) \
	((u16)((n) & ~(BIT_MASK(W) << (B))) | ((k) << (B)))

#define BIT_PACK32(n, W, B, k) \
	((u32)((n) & ~(BIT_MASK(W) << (B))) | ((k) << (B)))

#endif /* _TYPES_H_ */
