#ifndef MM_PT_H
#define MM_PT_H

#include<stdbool.h>
#include "kernel/types.h"
 
_Alignas(PAGE_SIZE) u32 boot_page_directory[1024];
_Alignas(PAGE_SIZE) u32 boot_page_table1[1024];

uintptr_t get_pde_index(addr_t);
uintptr_t get_pte_index(addr_t);

uintptr_t get_offset(addr_t);

uintptr_t make_pde(addr_t, u32);

bool is_valid_page(u32);

void setup_page_tables();

#endif /* MM_PT_H */
