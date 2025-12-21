#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "machine/isr.h"

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_TERMINATED
} task_state_t;

typedef struct Task {
    int id;
    uint32_t esp; // Stack pointer
    uint32_t stack_base;
    task_state_t state;
    int priority;
    int expected_time; // For SPN
    struct Task* next;
} Task;

void task_init(void);
Task* task_create(void (*entry_point)(), int priority, int expected_time);
void task_switch(Task* next);

#endif
