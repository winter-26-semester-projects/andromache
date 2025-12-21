#include <kernel/child_handler.h>
#include "private/_child_handler.h"

static struct child_handler_internal scheduler_data;

void child_handler_init(void) {
    for (int i = 0; i < CONFIG_MAX_THREADS; ++i) {
        scheduler_data.tasks[i].state = TASK_FREE;
        scheduler_data.tasks[i].id = -1;
    }
    scheduler_data.current_task_index = -1;
}

int32_t create_task(void (*entry)(void)) {
    for (int i = 0; i < CONFIG_MAX_THREADS; ++i) {
        if (scheduler_data.tasks[i].state == TASK_FREE) {
            scheduler_data.tasks[i].state = TASK_READY;
            scheduler_data.tasks[i].id = i; /* Simple ID generation */
            scheduler_data.tasks[i].entry = entry;
            /* Stack setup would go here */
            return i;
        }
    }
    return -1;
}

void schedule(void) {
    /* If no tasks, just return */
    int start_index = scheduler_data.current_task_index;
    if (start_index == -1) start_index = 0;

    int next_index = (start_index + 1) % CONFIG_MAX_THREADS;

    /* Find next READY task */
    int loops = 0;
    while (loops < CONFIG_MAX_THREADS) {
        if (scheduler_data.tasks[next_index].state == TASK_READY ||
            scheduler_data.tasks[next_index].state == TASK_RUNNING) {

            /* Update states */
            if (scheduler_data.current_task_index != -1 &&
                scheduler_data.tasks[scheduler_data.current_task_index].state == TASK_RUNNING) {
                scheduler_data.tasks[scheduler_data.current_task_index].state = TASK_READY;
            }

            scheduler_data.current_task_index = next_index;
            scheduler_data.tasks[next_index].state = TASK_RUNNING;

            /* Context switch logic would go here.
               For "basis", we just call the entry point if it's the first run,
               or resume (which we can't do easily in C without assembly).
               Since this is a loop in kernel_main calling schedule,
               we will just call the function for now to demonstrate logic. */

            if (scheduler_data.tasks[next_index].entry) {
                scheduler_data.tasks[next_index].entry();
            }
            return;
        }
        next_index = (next_index + 1) % CONFIG_MAX_THREADS;
        loops++;
    }
}

int32_t get_current_task_id(void) {
    if (scheduler_data.current_task_index == -1) return -1;
    return scheduler_data.tasks[scheduler_data.current_task_index].id;
}
