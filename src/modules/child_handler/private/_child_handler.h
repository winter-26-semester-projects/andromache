#ifndef _CHILD_HANDLER_PRIV_H_
#define _CHILD_HANDLER_PRIV_H_

#include <stdint.h>
#include "config.h"

enum task_state {
    TASK_FREE = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_WAITING
};

struct task {
    int32_t id;
    enum task_state state;
    uintptr_t stack_ptr;
    void (*entry)(void);
};

struct child_handler_internal {
    struct task tasks[CONFIG_MAX_THREADS];
    int current_task_index;
};

#endif /* _CHILD_HANDLER_PRIV_H_ */
