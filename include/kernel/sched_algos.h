#ifndef _KERNEL_SCHED_ALGOS_H_
#define _KERNEL_SCHED_ALGOS_H_
#include <kernel/task.h>

/*sched policies*/
typedef enum
{
    FCFS,
    RR,
    HPF,
    SDN
} sched_policy_t;

/*
initialize scheduler algorithm,
and it's called only once during system boot
*/
void sched_algos_init(void);

void sched_enqueue_task(struct task_struct *task); // enqueue a task to the scheduler

/*
pick next task according to sched_policy_t,
returns NULL if no task is ready
*/
struct task_struct *sched_dequeue_task(sched_policy_t policy);

int sched_has_ready_tasks(void); // unecessary but to avoid peaking into internals (much cleaner)

#endif /* _KERNEL_SCHED_ALGOS_H_ */
