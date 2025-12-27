#ifndef _KERNEL_TASK_H
#define _KERNEL_TASK_H

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
#endif /* _KERNEL_TASK_h */
