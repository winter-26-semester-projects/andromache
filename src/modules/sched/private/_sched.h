#ifndef _SCHED_PRIV_H_
#define _SCHED_PRIV_H_
#include "kernel/task.h"
#include "kernel/sched_algos.h"

/*currently running task*/
extern struct task_struct *current;

/*idle task (when no ready task is available)*/
extern struct task_struct *idle_task;

/*active sched policy*/
extern sched_policy_t curr_sched_policy;

/*
placeholder context switch for future implementation
ik it doesn't make any sense but just to build sched skeleton
*/
void context_switch(struct task_struct *prev, struct task_struct *next);

#endif /* _SCHED_PRIV_H_ */
