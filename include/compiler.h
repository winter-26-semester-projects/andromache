#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "kernel/types.h"
#include "machine/barrier.h"

#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#define __aligned(x) __attribute__((aligned(x)))
// this is just to silence a warning, write stdint later...
#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif
#define __packed __attribute__((packed))

#define static_assert _Static_assert

#define __is_constexpr(x) \
	(sizeof int == sizeof(*(7 ? (*(void*)((long)(x) * 0l) : (int *)8))))

#define __section(x) __attribute__((__section__(#x)))

#define __init __section(.init.text)
#define __initdata __section(.init.data)
#define __text __section(.text)

#define arr_size(arr) (sizeof(arr) / sizeof((arr)[0]))

#include<stddef.h>

// this is also just to silence the warning
#ifndef offsetof
#define offsetof(type, member) ( (size_t) &((type *)0)->member)
#endif
#define container_of(ptr, type, member) \
({ \
	const __typeof__( ((type *)0)->member ) *__tmptr = (ptr); \
	(type *)( (char *)__tmptr - offsetof(type,member) ); \
})

#define READ_ONCE(x) \
({ \
    	union { __typeof__(x) __val; char __c[1]; } __u; \
    	__read_once_size(&(x), __u.__c, sizeof(x)); \
    	__u.__val; \
})

#define WRITE_ONCE(x, val) \
({ \
    	union { __typeof__(x) __val; char __c[1]; } __u = { .__val = (__typeof__(x))(val) }; \
    	__write_once_size(&(x), __u.__c, sizeof(x)); \
    	__u.__val; \
})

static __always_inline void __read_once_size(const volatile void *p, void *res, int size)
{
	switch (size) {
    		case 1: *(uint8_t *)res = *(const volatile uint8_t *)p; break;
    		case 2: *(uint16_t *)res = *(const volatile uint16_t *)p; break;
    		case 4: *(uint32_t *)res = *(const volatile uint32_t *)p; break;
    		default:
        		barrier();
        		__builtin_memcpy(res, (const void *)p, size);
        		barrier();
    	}
}

static __always_inline void __write_once_size(volatile void *p, void *res, int size)
{
	switch (size) {
    		case 1: *(volatile uint8_t *)p = *(uint8_t *)res; break;
    		case 2: *(volatile uint16_t *)p = *(uint16_t *)res; break;
    		case 4: *(volatile uint32_t *)p = *(uint32_t *)res; break;
    		default:
        		barrier();
        		__builtin_memcpy((void *)p, (const void *)res, size);
        		barrier();
    	}
}

#endif /* _COMPILER_H_ */
