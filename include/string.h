#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
size_t strlen(const char* str);

#endif /* STRING_H */
