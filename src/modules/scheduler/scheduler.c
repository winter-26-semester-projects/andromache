#include "private/_scheduler.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Global Scheduler Instance */
SchedulerGlobal g_scheduler_sys;

/* Static Process Pool (No kmalloc yet) */
static Process g_process_pool[SCHEDULER_MAX_PROCESSES];
static pid_t g_next_pid = 1;

/* Aging Config */
#define SCHED_AGING_THRESHOLD_TICKS                                            \
  1000 /* Boost priority every 1000 ticks of wait */

/* --- Bit Manipulation Helpers --- */

static inline int __builtin_clz_wrapper(uint32_t x) {
#if defined(__GNUC__) || defined(__clang__)
  return __builtin_clz(x);
#else
  int n = 0;
  if (!(x & 0xFFFF0000)) {
    n += 16;
    x <<= 16;
  }
  if (!(x & 0xFF000000)) {
    n += 8;
    x <<= 8;
  }
  if (!(x & 0xF0000000)) {
    n += 4;
    x <<= 4;
  }
  if (!(x & 0xC0000000)) {
    n += 2;
    x <<= 2;
  }
  if (!(x & 0x80000000)) {
    n += 1;
  }
  return n;
#endif
}

static inline int sched_find_first_bit(uint32_t word) {
  if (word == 0)
    return -1;
  return 31 - __builtin_clz_wrapper(word);
}

/* --- Queue Management --- */

void _sched_enqueue(CpuRunQueue *rq, Process *p) {
  if (p->priority < 0)
    p->priority = 0;
  if (p->priority >= SCHED_PRIO_LEVELS)
    p->priority = SCHED_PRIO_LEVELS - 1;

  priority_t prio = p->priority;
  PrioQueue *q = &rq->queues[prio];

  p->next = NULL;
  p->prev = q->tail;

  if (q->tail) {
    q->tail->next = p;
  } else {
    q->head = p;
  }
  q->tail = p;
  q->count++;

  int word_idx = prio / 32;
  int bit_idx = prio % 32;
  rq->prio_bitmap[word_idx] |= (1 << bit_idx);
}

void _sched_remove(CpuRunQueue *rq, Process *p) {
  priority_t prio = p->priority;
  PrioQueue *q = &rq->queues[prio];

  if (p->prev)
    p->prev->next = p->next;
  else
    q->head = p->next;

  if (p->next)
    p->next->prev = p->prev;
  else
    q->tail = p->prev;

  q->count--;

  if (q->count == 0) {
    int word_idx = prio / 32;
    int bit_idx = prio % 32;
    rq->prio_bitmap[word_idx] &= ~(1 << bit_idx);
  }

  p->next = NULL;
  p->prev = NULL;
}

Process *_sched_pick_next(CpuRunQueue *rq) {
  for (int i = 0; i < 5; i++) {
    if (rq->prio_bitmap[i] != 0) {
      int bit_idx = sched_find_first_bit(rq->prio_bitmap[i]);
      if (bit_idx < 0)
        continue;

      priority_t prio = i * 32 + bit_idx;
      PrioQueue *q = &rq->queues[prio];
      return q->head;
    }
  }
  return rq->idle_task;
}

/* --- Core Init --- */

void scheduler_init(void) {
  g_scheduler_sys.enabled = false;
  g_scheduler_sys.active_cpus = 1;

  CpuRunQueue *rq = &g_scheduler_sys.cpu_queues[0];
  rq->cpu_id = 0;
  rq->running = NULL;
  rq->switches = 0;
  rq->total_ticks = 0;

  for (int i = 0; i < SCHED_PRIO_LEVELS; i++) {
    rq->queues[i].head = NULL;
    rq->queues[i].tail = NULL;
    rq->queues[i].count = 0;
  }

  for (int i = 0; i < 5; i++)
    rq->prio_bitmap[i] = 0;

  /* Init process pool (all zero/null) */
  for (int i = 0; i < SCHEDULER_MAX_PROCESSES; i++) {
    g_process_pool[i].pid = 0; /* 0 means free slot */
    g_process_pool[i].state = PROCESS_STATE_TERMINATED;
  }

  rq->idle_task = NULL; /* Idle task usually created explicitly as PID 0 */
}

/* --- Process Management --- */

Process *scheduler_create_process(const char *name, void (*entry_point)(void),
                                  priority_t prio) {
  for (int i = 0; i < SCHEDULER_MAX_PROCESSES; i++) {
    if (g_process_pool[i].pid == 0) {
      Process *p = &g_process_pool[i];
      p->pid = g_next_pid++;
      p->name = name;
      p->priority = prio;
      p->dynamic_priority = prio;
      p->state = PROCESS_STATE_NEW;
      p->cpu_affinity = 0xFFFFFFFF; /* Run anywhere */
      p->time_slice = SCHED_DEFAULT_TIMESLICE_MS;

      /* Stack and Context setup would go here (arch dependent) */
      /* p->context.rip = (uint64_t)entry_point; */
      /* p->kernel_stack = ... */

      return p;
    }
  }
  return NULL; /* Out of processes */
}

void scheduler_add_process(Process *process) {
  if (!process)
    return;

  process->cpu_id = 0;
  process->state = PROCESS_STATE_READY;
  if (process->time_slice == 0)
    process->time_slice = SCHED_DEFAULT_TIMESLICE_MS;

  CpuRunQueue *rq = _sched_get_cpu_rq(0);
  _sched_enqueue(rq, process);
}

void scheduler_exit_current(int code) {
  Process *curr = scheduler_get_current_process();
  if (curr) {
    curr->exit_code = code;
    curr->state = PROCESS_STATE_ZOMBIE; /* Wait for parent */
    /* Zombie processes effectively leave the scheduler until cleaned up */
    /* We just yield, next pick shouldn't pick us because we aren't READY */
  }
  scheduler_yield();
}

/* --- Dispatcher --- */

void scheduler_yield(void) {
  CpuRunQueue *rq = _sched_get_cpu_rq(0);

  Process *prev = rq->running;
  Process *next = _sched_pick_next(rq);

  if (!next)
    return;

  if (prev != next) {
    rq->running = next;
    rq->switches++;
    next->state = PROCESS_STATE_RUNNING;

    if (prev && prev->state == PROCESS_STATE_RUNNING) {
      prev->state = PROCESS_STATE_READY;
      /* Rotate */
      _sched_remove(rq, prev);
      _sched_enqueue(rq, prev);
    }

    /* Low level switch */
    /* _arch_context_switch(&prev->context, &next->context); */
  }
}

/* --- Sleep / Wake --- */

void scheduler_sleep(uint32_t ms) {
  Process *curr = scheduler_get_current_process();
  if (!curr)
    return;

  curr->state = PROCESS_STATE_BLOCKED;

  /* Simple sleep: store target wake time.
     Usually we need a delta queue, but for now we iterate pool in tick. */
  /* Store expiry tick somehow. Let's abuse 'sleep_start' or use a new field.
     'sleep_start' was defined as start time. We'll interpret 'waiting_on' as
     timeout if needed or just assume time_slice/total_runtime usage. Actually
     let's just use a simplified approach: sleep_start = now + ms */

  CpuRunQueue *rq = _sched_get_cpu_rq(0);
  curr->sleep_start = rq->total_ticks + ms; /* ms ~ ticks mapping */

  /* Yield immediately */
  /* Note: We handle list removal in yield:
     yield checks if prev->state == RUNNING. It is now BLOCKED.
     So yield will NOT re-enqueue it. Perfect. */
  scheduler_yield();
}

void scheduler_wake_up(pid_t pid) {
  /* Find process - linear search */
  /* In a real OS, use a hash map */
  for (int i = 0; i < SCHEDULER_MAX_PROCESSES; i++) {
    if (g_process_pool[i].pid == pid &&
        g_process_pool[i].state == PROCESS_STATE_BLOCKED) {
      Process *p = &g_process_pool[i];
      p->sleep_start = 0; // Clear sleep timer
      p->state = PROCESS_STATE_READY;
      scheduler_add_process(p); /* Re-enqueue */
      return;
    }
  }
}

void scheduler_tick(void) {
  CpuRunQueue *rq = _sched_get_cpu_rq(0);
  if (!rq || !rq->running)
    return;

  rq->total_ticks++;
  Process *current = rq->running;

  /* 1. Time Slice Management */
  if (current->policy != SCHED_POLICY_FIFO) {
    if (current->time_slice > 0) {
      current->time_slice--;
    } else {
      current->time_slice = SCHED_DEFAULT_TIMESLICE_MS;
      /* Flag for preemption */
    }
  }

  /* 2. Wake up sleeping processes (Polling implementation) */
  /* Only do this every few ticks to save time? Or every tick for precision. */
  for (int i = 0; i < SCHEDULER_MAX_PROCESSES; i++) {
    Process *p = &g_process_pool[i];
    if (p->pid != 0 && p->state == PROCESS_STATE_BLOCKED &&
        p->sleep_start > 0) {
      if (rq->total_ticks >= p->sleep_start) {
        /* WAKE UP */
        p->sleep_start = 0;
        p->state = PROCESS_STATE_READY;
        _sched_enqueue(rq, p);
      }
    }
  }

  /* 3. Aging (Starvation Prevention) */
  if ((rq->total_ticks % SCHED_AGING_THRESHOLD_TICKS) == 0) {
    for (int i = 0; i < SCHEDULER_MAX_PROCESSES; i++) {
      Process *p = &g_process_pool[i];
      if (p->pid != 0 && p->state == PROCESS_STATE_READY &&
          p->priority < (SCHED_PRIORITY_MAX - 1)) {
        /* Bump priority */
        /* Requires removing from queue, changing prio, re-enqueuing */
        /* Optimization: direct manipulation or just lazy update? */
        /* Safe way: */
        if (p != current) { /* Don't mess with running */
          _sched_remove(rq, p);
          p->priority++;
          _sched_enqueue(rq, p);
        }
      }
    }
  }
}

Process *scheduler_get_current_process(void) {
  CpuRunQueue *rq = _sched_get_cpu_rq(0);
  return rq ? rq->running : NULL;
}

void scheduler_start(void) {
  g_scheduler_sys.enabled = true;

  CpuRunQueue *rq = _sched_get_cpu_rq(0);
  Process *initial = _sched_pick_next(rq);

  if (initial) {
    rq->running = initial;
    initial->state = PROCESS_STATE_RUNNING;
  }
}
