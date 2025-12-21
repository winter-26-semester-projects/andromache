#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "modules/task.h"

typedef enum {
    SCHED_RR,
    SCHED_HPF,
    SCHED_SPN
} sched_algo_t;

void scheduler_init(sched_algo_t algo);
void scheduler_add_task(Task* task);
void schedule(void);
void scheduler_tick(void); // Called by timer ISR

#endif
