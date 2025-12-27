#ifndef _KERNEL_TASK_H
#define _KERNEL_TASK_H
#include <linux/list.h>

void task_init(void);

typedef enum
{
    RUNNING,
    READY,
    BLOCKED,
    TERMINATED
} task_state_t;

struct cpu_context
{
    /*
    This function basically acts as a place holder for context switching until further implementation
    */
    void *sp;
    void *pc;
};

struct task_struct
{
    /*Basic standard task structure*/
    int pid;
    task_state_t state;
    struct cpu_context context; // placeholder for cpu context for future implementation

    /*Actuall sched algos related metadata*/
    int priority;
    int time_slice;
    int remaining_time;
    int burst_time;

    struct list_head ready_queue;
} task_t;

#endif /* _KERNEL_TASK_h */
