#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

void memory_init(uint32_t start_address, uint32_t size);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
