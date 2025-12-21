#ifndef KERNEL_CHILD_HANDLER_H
#define KERNEL_CHILD_HANDLER_H

#include <stdint.h>

void child_handler_init(void);
int32_t create_task(void (*entry)(void));
void schedule(void);
int32_t get_current_task_id(void);

#endif /* KERNEL_CHILD_HANDLER_H */
