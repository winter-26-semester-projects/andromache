#include "../../../include/kernel/sched_algos.h"
#include "private/_sched_algos.h"
#include <stddef.h>

struct list_head ready_queue;

void sched_algos_init(void)
{
    INIT_LIST_HEAD(&ready_queue);
}

void sched_enqueue_task(struct task_struct *task)
{
    if (!task)
        return;

    task->state = READY;
    list_add_tail(&task->run_list, &ready_queue);
}

// FCFS implementation

struct task_struct *sched_pick_next_task(sched_policy_t policy)
{
    struct task_struct *task;

    if (list_empty(&ready_queue))
        return NULL;

    switch (policy)
    {
    case FCFS:
        task = list_first_entry(
            &ready_queue,
            struct task_struct,
            run_list);
        list_del(&task->run_list);
        task->state = RUNNING;
        return task;
    default:
        return NULL;
    }
}

// check for any ready task
int sched_has_ready_tasks(void)
{
    return !list_empty(&ready_queue);
}