#ifndef _BARRIER_H_
#define _BARRIER_H_

static inline void barrier(void)
{
	__asm__ volatile("" : : : "memory");
}

static inline void mb(void)
{
	__asm__ volatile("mfence" : : : "memory");
}

static inline void rmb(void)
{
	__asm__ volatile("lfence" : : : "memory");
}

static inline void wmb(void)
{
	__asm__ volatile("sfence" : : : "memory");
}


#endif /* _BARRIER_H_ */
