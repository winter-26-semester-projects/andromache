#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Mock task_struct */
typedef enum { FCFS, RR, HPF, SDN } sched_policy_t;

typedef struct task_struct {
    int pid;
    sched_policy_t policy;
    int priority;       // for HPF
    int remaining_time; // for SDN
    int time_slice;     // for RR
    struct task_struct *next; // for ready queue
} task_struct;

/* Mock globals */
task_struct *current = NULL;
task_struct idle_task;

/* Ready queue implemented as linked list */
task_struct *ready_queue = NULL;

/* Add task to ready queue */
void sched_enqueue_task(task_struct *task) {
    task_struct **ptr = &ready_queue;
    while (*ptr) ptr = &(*ptr)->next;
    task->next = NULL;
    *ptr = task;

    // Preemption for HPF and SDN
    if (!current || current == &idle_task) return;

    if (task->policy == HPF && (current->policy != HPF || task->priority > current->priority)) {
        printf("[Preemption] Task %d preempts Task %d (HPF)\n", task->pid, current->pid);
        current = NULL; // force scheduler to pick next
    }
    if (task->policy == SDN && (current->policy != SDN || task->remaining_time < current->remaining_time)) {
        printf("[Preemption] Task %d preempts Task %d (SDN)\n", task->pid, current->pid);
        current = NULL;
    }
}

/* Pick next task according to policy */
task_struct* sched_pick_next_task() {
    if (!ready_queue) return &idle_task;

    task_struct *best = NULL, *prev = NULL, *iter = ready_queue, *prev_iter = NULL;

    // HPF
    while (iter) {
        if (iter->policy == HPF && (!best || iter->priority > best->priority)) {
            best = iter;
            prev = prev_iter;
        }
        prev_iter = iter;
        iter = iter->next;
    }
    if (best) goto dequeue;

    // RR
    iter = ready_queue;
    prev_iter = NULL;
    while (iter) {
        if (iter->policy == RR) { best = iter; prev = prev_iter; break; }
        prev_iter = iter;
        iter = iter->next;
    }
    if (best) goto dequeue;

    // FCFS
    iter = ready_queue;
    prev_iter = NULL;
    while (iter) {
        if (iter->policy == FCFS) { best = iter; prev = prev_iter; break; }
        prev_iter = iter;
        iter = iter->next;
    }
    if (best) goto dequeue;

    // SDN
    iter = ready_queue;
    prev_iter = NULL;
    while (iter) {
        if (iter->policy == SDN && (!best || iter->remaining_time < best->remaining_time)) {
            best = iter;
            prev = prev_iter;
        }
        prev_iter = iter;
        iter = iter->next;
    }

dequeue:
    if (!best) return &idle_task;
    // remove from ready queue
    if (prev) prev->next = best->next;
    else ready_queue = best->next;
    best->next = NULL;
    return best;
}

/* Simulated scheduler */
void schedule() {
    current = sched_pick_next_task();
    printf("[Schedule] Running Task %d (Policy %d)\n", current->pid, current->policy);
}

/* Simulated yield */
void yield() {
    if (current && current != &idle_task) sched_enqueue_task(current);
    schedule();
}

/* Simulated RR tick */
void rr_tick() {
    if (!current || current == &idle_task || current->policy != RR) return;
    current->time_slice--;
    if (current->time_slice <= 0) {
        printf("[RR Tick] Time slice expired for Task %d\n", current->pid);
        current->time_slice = 5; // reset quantum
        yield();
    }
}

/* Helper to create task */
task_struct* create_task(int pid, sched_policy_t policy, int priority, int remaining_time) {
    task_struct *t = malloc(sizeof(task_struct));
    t->pid = pid;
    t->policy = policy;
    t->priority = priority;
    t->remaining_time = remaining_time;
    t->time_slice = 5; // default RR quantum
    t->next = NULL;
    return t;
}

/* Test all policies */
int main() {
    printf("=== Scheduler Test ===\n");

    idle_task.pid = 0;
    idle_task.policy = FCFS;

    // Create tasks
    task_struct *t1 = create_task(1, FCFS, 0, 0);
    task_struct *t2 = create_task(2, RR, 0, 0);
    task_struct *t3 = create_task(3, HPF, 5, 0);
    task_struct *t4 = create_task(4, SDN, 0, 3);

    // Enqueue tasks
    sched_enqueue_task(t1);
    sched_enqueue_task(t2);
    sched_enqueue_task(t3);
    sched_enqueue_task(t4);

    // Run scheduler manually
    for (int i = 0; i < 10; i++) {
        schedule();
        rr_tick(); // simulate RR timer
    }

    return 0;
}
