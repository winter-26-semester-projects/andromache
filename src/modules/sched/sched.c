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

/*Idle task*/
static void idle_task_func(void)
{
    for (;;)
    {
        __asm__ volatile(
            "sti\n" /*added sti to make sure interrupts are enabled before halting to prevent deadlock*/
            "hlt\n");
    }
}

/*sched initializaiton*/
void sched_init(void)
{
    sched_algos_init();
    idle_task.pid = 0;
    idle_task.state = RUNNING;
    idle_task.entry = idle_task_func;
    current = &idle_task;
}