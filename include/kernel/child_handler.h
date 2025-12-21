#ifndef _KERNEL_CHILD_HANDLER_H_
#define _KERNEL_CHILD_HANDLER_H_

#include <stdint.h>

void child_handler_init(void);

/* Creates a task with the given entry point. Returns task ID. */
int32_t create_task(void (*entry)(void));

/* Runs the next task (Round Robin). */
void schedule(void);

/* Returns the ID of the currently running task. */
int32_t get_current_task_id(void);

#endif /* _KERNEL_CHILD_HANDLER_H_ */
