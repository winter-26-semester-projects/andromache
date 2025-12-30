#ifndef _KERNEL_TASK_H
#define _KERNEL_TASK_H

#include "kernel.h"
#include "machine/processor.h"

/**
 * I shall henceforth, forward-declare a great many declarations...
 * */
<<<<<<< HEAD
<<<<<<< HEAD

enum pd_state { _=0,};
=======
enum pd_state {_=0,};
>>>>>>> 09bcd267cd5c15384bcb0c1871c3e700763a0037
=======
enum pd_state {_=0,};
>>>>>>> 09bcd267cd5c15384bcb0c1871c3e700763a0037
struct mm_struct;

struct task_struct {
	u16 pid;
	enum pd_state state;
	u32 flags;
	void *stack;
	struct mm_struct *mm;
	struct __task_struct __task;
};

void task_init(void);

#endif /* _KERNEL_TASK_h */

