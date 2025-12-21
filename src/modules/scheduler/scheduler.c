#include "modules/scheduler.h"
#include <stddef.h>

static sched_algo_t current_algo = SCHED_RR;
static Task* head = NULL;
static Task* current_task = NULL;
static int quantum = 10;
static int time_slice = 0;

extern void perform_task_switch(uint32_t old_esp_ptr, uint32_t new_esp);

void scheduler_init(sched_algo_t algo) {
    current_algo = algo;
    // Create a dummy task for the currently running kernel code (pid 0)
    // In a real implementation, we'd properly set up the kernel task struct
    // For now, we assume scheduler_add_task handles the first real tasks.
}

void scheduler_add_task(Task* task) {
    if (!head) {
        head = task;
        task->next = task; // Circular for RR
    } else {
        Task* t = head;
        while (t->next != head) {
            t = t->next;
        }
        t->next = task;
        task->next = head;
    }
}

static Task* schedule_rr(void) {
    if (!head) return NULL;

    Task* start = current_task ? current_task->next : head;
    Task* t = start;

    do {
        if (t->state == TASK_READY || t->state == TASK_RUNNING) {
             return t;
        }
        t = t->next;
    } while (t != start);

    return NULL; // No ready task found
}

static Task* schedule_hpf(void) {
    if (!head) return NULL;

    Task* best = head;
    Task* t = head->next;

    // Scan one full loop
    if (t != head) {
        do {
            if (t->state == TASK_READY) {
                if (t->priority > best->priority) {
                    best = t;
                }
            }
            t = t->next;
        } while (t != head);
    }

    return best;
}

static Task* schedule_spn(void) {
     if (!head) return NULL;

    Task* best = head;
    Task* t = head->next;

    if (t != head) {
        do {
            if (t->state == TASK_READY) {
                if (t->expected_time < best->expected_time) {
                    best = t;
                }
            }
            t = t->next;
        } while (t != head);
    }

    return best;
}

void schedule(void) {
    Task* next = NULL;
    if (current_algo == SCHED_RR) next = schedule_rr();
    else if (current_algo == SCHED_HPF) next = schedule_hpf();
    else if (current_algo == SCHED_SPN) next = schedule_spn();

    if (next && next != current_task) {
        Task* prev = current_task;
        current_task = next;

        // If we have a previous task, switch.
        // Note: First switch from kernel main might need special handling if prev is NULL or dummy.
        // For simplicity, we assume we always have a valid current_task context after init.
        if (prev) {
            perform_task_switch((uint32_t)&prev->esp, current_task->esp);
        } else {
            // Initial task switch (first time enabling multitasking)
            // We just load the new ESP and pop the context.
            // We cannot return to the caller (kernel_main loop) easily because we discard its stack.
            // But since perform_task_switch logic is: save old, load new, pop, ret.
            // We can mimic the "load new, pop, ret" part.
            __asm__ volatile (
                "mov %0, %%esp\n"
                "pop %%edi\n"
                "pop %%esi\n"
                "pop %%ebx\n"
                "pop %%ebp\n"
                "ret\n"
                : : "r"(current_task->esp) : "memory"
            );
        }
    }
}

void scheduler_tick(void) {
    if (current_algo == SCHED_RR) {
        time_slice++;
        if (time_slice >= quantum) {
            time_slice = 0;
            schedule();
        }
    }
    // HPF and SPN are typically non-preemptive in basic forms, or preempt on new arrival.
    // Here we just let RR preempt.
}
