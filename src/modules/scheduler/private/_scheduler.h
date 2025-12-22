#ifndef MODULES_SCHEDULER_PRIVATE_SCHEDULER_H
#define MODULES_SCHEDULER_PRIVATE_SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Use relative path to ensure visibility without compiler flags in this context
 */
#include "../../../../include/kernel/scheduler.h"

/* --- Constants --- */

#define SCHED_MAX_CPUS 4             /* Max CPUs supported for now */
#define SCHEDULER_MAX_PROCESSES 1024 /* Max processes in static pool */

/* O(1) Scheduler Config */
/* We maintain an array of queues for each priority level */
#define SCHED_PRIO_LEVELS SCHED_PRIORITY_MAX

/* --- Macros --- */

/* Helper to align structure to cache line to prevent false sharing in SMP */
#define __cacheline_aligned __attribute__((aligned(64)))

/* --- Internal Structures --- */

/**
 * @brief A runqueue for a specific priority level.
 */
typedef struct {
  Process *head;
  Process *tail;
  uint32_t count;
} PrioQueue;

/**
 * @brief Per-CPU Scheduler Context.
 *        This structure is hot and should be aligned.
 */
typedef struct {
  /* Locks */
  volatile int lock; /* Spinlock for this runqueue */

  /* Current State */
  cpu_id_t cpu_id;
  Process *running;
  Process *idle_task; /* Pointer to the idle task for this CPU */

  /* Queues */
  /* Active and Expired arrays could be used for O(1) scheduler (Linux 2.4/2.6
   * style) */
  /* For now, just a single priority array */
  PrioQueue queues[SCHED_PRIO_LEVELS];

  /* Bitmaps for fast finding of first set bit (highest priority) */
  /* 140 priorities fit in 5x32-bit words */
  uint32_t prio_bitmap[5];

  /* Statistics */
  uint64_t switches;
  uint64_t total_ticks;

} __cacheline_aligned CpuRunQueue;

/**
 * @brief Global Scheduler Data
 */
typedef struct {
  CpuRunQueue cpu_queues[SCHED_MAX_CPUS];
  volatile bool enabled;
  uint32_t active_cpus;
} SchedulerGlobal;

extern SchedulerGlobal g_scheduler_sys;

/* --- Internal Functions --- */

/* Queue Management */
void _sched_enqueue(CpuRunQueue *rq, Process *p);
Process *_sched_dequeue(CpuRunQueue *rq);
Process *_sched_pick_next(CpuRunQueue *rq);
void _sched_remove(CpuRunQueue *rq, Process *p);

/* Context Switching (ASM) */
extern void _arch_context_switch(ArchContext *prev, ArchContext *next);

/* Load Balancing */
void _sched_balance_load(void);

/* Helper */
static inline CpuRunQueue *_sched_get_cpu_rq(cpu_id_t cpu) {
  if (cpu >= SCHED_MAX_CPUS)
    return NULL;
  return &g_scheduler_sys.cpu_queues[cpu];
}

#endif /* MODULES_SCHEDULER_PRIVATE_SCHEDULER_H */
