#ifndef _KERNEL_SCHED_H_
#define _KERNEL_SCHED_H_
#include "kernel/task.h"

/*called once during kernel boot to initialize the scheduler*/
void sched_init(void);

/*min scheduler function*/
void schedule(void);

/*voluntarily yield the CPU*/
void yield(void);

#endif /* _KERNEL_SCHED_H_ */
