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

// check for any ready task
int sched_has_ready_tasks(void)
{
    return !list_empty(&ready_queue);
}