#include <kernel/sched.h>
#include "private/_sched.h"
#include "kernel/sched_algos.h"
#include "kernel/task.h"
#include <stdint.h>

/*Global states*/
struct task_struct *current;
struct task_struct idle_task;
sched_policy_t curr_sched_policy = FCFS;

/*Again, justa a placeholder nothing fancy*/
void context_switch(struct task_struct *prev, struct task_struct *next)
{
    current = next;
}