#ifndef _SCHED_ALGOS_PRIV_H_
#define _SCHED_ALGOS_PRIV_H_
#include <include/kernel/task.h>
#include <linux/list.h>

#define TIME_SLICE 5

/*A global ready queue for all algos*/
extern struct list_head ready_queue;

static inline void enqueue_task(struct task_struct *task)
{
    task->state = READY;
    list_add_tail(&task->run_list, &ready_queue);
}

static inline void dequeue_task_internal(struct task_struct *task)
{
    list_del(&task->run_list);
}

static inline int ready_queue_empty(void)
{
    return list_empty(&ready_queue);
}

#endif /* _SCHED_ALGOS_PRIV_H_ */
