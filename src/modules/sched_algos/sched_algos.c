#include "../../../include/kernel/sched_algos.h"
#include "private/_sched_algos.h"
#include <stddef.h>

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

struct task_struct *sched_pick_next_task(sched_policy_t policy)
{
    struct task_struct *task;

    struct task_struct *task;
    struct task_struct *iter;
    struct task_struct *best;

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
    case RR:
        task = list_first_entry(
            &ready_queue,
            struct task_struct,
            run_list);

        list_del(&task->run_list);
        task->state = RUNNING;
        return task;
    case HPF:
        /*start by assuming that the first task has the highest priority*/
        best = list_first_entry(
            &ready_queue,
            struct task_struct,
            run_list);

        /*iterate and find the highest priority task*/
        list_for_each_entry(iter, &ready_queue, run_list)
        {
            if (iter->priority > best->priority)
            {
                best = iter;
            }
            list_del(&best->run_list);
            best->state = RUNNING;
            return best;
        }
    case SDN:
        /*start by assuming that the first task has the least burst time*/
        best = list_first_entry(
            &ready_queue,
            struct task_struct,
            run_list);
        /*iterate and find the task with the least burst time*/
        list_for_each_entry(iter, &ready_queue, run_list)
        {
            if (iter->burst_time < best->burst_time)
            {
                best = iter;
            }
            list_del(&best->run_list);
            best->state = RUNNING;
            return best;
        }
    default:
        return NULL;
    }
}

// check for any ready task
static inline int sched_has_ready_tasks(void)
{
    return !list_empty(&ready_queue);
}