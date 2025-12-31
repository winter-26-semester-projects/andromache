#include "../../../include/kernel/sched_algos.h"
#include "private/_sched_algos.h"
#include <stddef.h>
#include "/home/debian/andromache/andromache/include/kernel/list.h"

struct list_head ready_queue;

void sched_algos_init(void)
{
    INIT_LIST_HEAD(&ready_queue);
}

void wakeup_task(struct task_struct *task)
{
    task->state = READY;

    sched_enqueue_task(task);
}

void sched_enqueue_task(struct task_struct *task)
{
    /**
     * modify enqueue to actually check for task’s existing state
     **/
    if (!task || task->state != READY)
        return;

    list_add_tail(&task->run_list, &ready_queue);
}

// FCFS implementation

struct task_struct *sched_pick_next_task(void)
{
    struct task_struct *task, *best = NULL;

    struct task_struct *task;
    struct task_struct *iter;

    if (list_empty(&ready_queue))
        return NULL;

    /*HPF*/
    list_for_each_entry(task, &ready_queue, run_list)
    {
        if (task->policy == HPF)
        {
            if (!best || task->priority > best->priority)
                best = task;
        }
    }
    if (best)
        goto found;

    /*RR*/
    list_for_each_entry(task, &ready_queue, run_list)
    {
        if (task->policy == RR)
        {
            best = task;
            goto found;
        }
    }

    /*FCFS*/
    list_for_each_entry(task, &ready_queue, run_list)
    {
        if (task->policy == FCFS)
        {
            best = task;
            goto found;
        }
    }

    /*SDN*/
    list_for_each_entry(task, &ready_queue, run_list)
    {
        if (task->policy == SDN)
        {
            if (!best || task->remaining_time < best->remaining_time)
                best = task;
        }
    }

found:
    if (!best)
        return NULL;
    list_del(&best->run_list);
    best->state = RUNNING;
    return best;
}

// check for any ready task
static inline int sched_has_ready_tasks(void)
{
    return !list_empty(&ready_queue);
}