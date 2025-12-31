#include <kernel/sched.h>
#include "private/_sched.h"
#include "kernel/sched_algos.h"
#include "kernel/task.h"
#include <stdint.h>
#include "../../../include/kernel/time.h"

#define RR_QUANTUM 5
/*Global states*/
struct task_struct *current;
struct task_struct idle_task;

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
    idle_task.policy = FCFS;
    current = &idle_task;

    INIT_LIST_HEAD(&idle_task.run_list);
}

/*main scheduler*/
void schedule(void)
{
    struct task_struct *next;

    next = sched_pick_next_task(curr_sched_policy);

    /* no task is ready */
    if (!next)
        next = &idle_task;

    /* no switch needed */
    if (current == next)
        return;

    /* update task states */
    if (current && current != &idle_task)
        current->state = READY;

    next->state = RUNNING;

    /* reset RR quantum */
    if (next->policy == RR)
        next->time_slice = RR_QUANTUM;

    context_switch(current, next);
    current = next;
}

/*voluntary yield*/
void yield(void)
{
    if (current && current != &idle_task)
    {
        current->state = READY;
        sched_enqueue_task(current);
    }
    schedule();
}

void sched_tick(void)
{
    /*never preempt idle task*/
    if (!current || current == &idle_task)
        return;

    switch (current->policy)
    {
    case RR:
        current->time_slice--;
        if (current->time_slice <= 0)
        {
            current->time_slice = RR_QUANTUM;
            yield();
        }
        break;
    case SDN:
        current->remaining_time--;

    /*HPF is not timer based preemptive and FCFS is non-preemptive */
    default:
        break;
    }
}