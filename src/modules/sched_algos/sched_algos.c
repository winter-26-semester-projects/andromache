#include "../../../include/kernel/sched_algos.h"
#include "private/_sched_algos.h"
#include <stddef.h>
#include "/home/debian/andromache/andromache/include/kernel/list.h"

struct list_head ready_queue;
extern struct task_struct *current;
extern struct task_struct idle_task;

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
    /*checking for task existing state*/
    if (!task || task->state != READY)
        return;

    /*adding task to ready queue*/
    list_add_tail(&task->run_list, &ready_queue);

    /*HPF and SDN preemption check*/
    if (!current || current == &idle_task)
        return;

    switch (task->policy)
    {
    case HPF:
        if (current->policy != HPF || task->priority > current->priority)
            yield(); /*next task policy isn't HPF || higher priority arrived*/
        break;
    case SDN:
        if (current->policy != SDN || task->remaining_time < current->remaining_time)
            yield(); /*next task policy isn't SDN || shorter remaining time arrived*/
        break;

    default:
        break;
    }
}

struct task_struct *sched_pick_next_task(void)
{
    struct task_struct *task, *best = NULL;

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