#ifndef _PT_H_
#define _PT_H_

#include<stdint.h>
#include<stdbool.h>
#include "kernel/kernel.h"

extern u8 LOGIC_BASE;

#define PAGE_SIZE 4096

#define addr(var) ((uintptr_t)(void*)&(var))

#define phys(ptr) ((__typeof__(ptr))(uintptr_t)((uintptr_t)ptr - addr(LOGIC_BASE))) 

_Alignas(PAGE_SIZE) u32 boot_page_directory[1024];
_Alignas(PAGE_SIZE) u32 boot_page_table1[1024];

uintptr_t get_pde_index(addr_t);
uintptr_t get_pte_index(addr_t);

uintptr_t get_offset(addr_t);

uintptr_t make_pde(addr_t, u32);

bool is_valid_page(u32);

void setup_page_tables(void);

#endif /* _PT_H_ */
