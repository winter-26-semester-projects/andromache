#ifndef _IRQFLAGS_H_
#define _IRQFLAGS_H_

#include "barrier.h"

static inline void enable(void) 
{
	__asm__ volatile("sti"
			: : : "memory"
			);
}

static inline void disable(void)
{
	__asm__ volatile("cli"
		: : : "memory"
		);
}

static inline unsigned long save_flags(void)
{
	unsigned long flags;

	__asm__ volatile("pushfd ; pop %0"
		: "=g"(flags) : : "memory"
		);

	return flags;
}

static inline void restore_flags(unsigned long flags)
{
	__asm__ volatile("push %0 ; popfd"
		: : "g"(flags) : "memory"
		);
}

static inline void save_irq_state(unsigned long *flags)
{
	*flags = save_flags();
	
	disable();
	barrier();
}

static inline void restore_irq_state(unsigned long *flags)
{
	barrier();

	restore_flags(*flags);
}

#endif /* _IRQFLAGS_H_ */

