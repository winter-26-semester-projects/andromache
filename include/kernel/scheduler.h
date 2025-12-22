#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* --- Constants & Configuration --- */

/* Max priorities: 0-99 (Realtime), 100-139 (Normal/CFS-like) */
#define SCHED_PRIORITY_MAX 140
#define SCHED_PRIORITY_REALTIME_MAX 99
#define SCHED_PRIORITY_NORMAL_MIN 100
#define SCHED_PRIO_DEFAULT 120

/* Time slice defaults (in ms or ticks) */
#define SCHED_DEFAULT_TIMESLICE_MS 10
#define SCHED_MIN_TIMESLICE_MS 1

/* --- Types --- */

typedef int32_t pid_t;
typedef int32_t tid_t;
typedef int16_t cpu_id_t;
typedef int32_t priority_t;

/* Process/Thread States */
typedef enum {
  PROCESS_STATE_NEW = 0,
  PROCESS_STATE_READY,
  PROCESS_STATE_RUNNING,
  PROCESS_STATE_BLOCKED,   /* Blocked on I/O or Synchronization */
  PROCESS_STATE_SUSPENDED, /* Explicitly suspended */
  PROCESS_STATE_ZOMBIE,    /* Exited, waiting for parent wait() */
  PROCESS_STATE_TERMINATED /* Fully dead */
} ProcessState;

/* Scheduling Policies */
typedef enum {
  SCHED_POLICY_FIFO = 0, /* First-In-First-Out (Realtime) */
  SCHED_POLICY_RR,       /* Round Robin (Realtime) */
  SCHED_POLICY_NORMAL,   /* Standard Time-Sharing (CFS-like) */
  SCHED_POLICY_BATCH,    /* Low priority, throughput oriented */
  SCHED_POLICY_IDLE      /* Run only when nothing else runs */
} SchedPolicy;

/* Context structure (Architecture Dependent - Placeholder) */
/* In a real scenario, this would include GP registers, IP, SP, FLAGS, FPU state
 */
typedef struct {
  uint64_t rip;
  uint64_t rsp;
  uint64_t rbp;
  uint64_t rflags;
  /* ... other registers ... */
} ArchContext;

/* Process Control Block (PCB) / Task Control Block (TCB) */
typedef struct Process {
  /* Identifiers */
  pid_t pid;  /* Process ID */
  pid_t ppid; /* Parent Process ID */
  tid_t tid;  /* Thread ID (if supporting threads) */

  /* State */
  ProcessState state;
  SchedPolicy policy;
  const char *name; /* Debug name */

  /* Scheduling Attributes */
  priority_t priority;         /* Static priority */
  priority_t dynamic_priority; /* Current priority (after aging/boosting) */
  uint32_t time_slice;         /* Remaining time slice in ticks */
  uint64_t total_runtime;      /* Total CPU cycles consumed */
  uint64_t sleep_start;        /* Time when sleep started (for wakeup) */

  /* SMP / Affinity */
  cpu_id_t cpu_id;       /* Current CPU */
  uint32_t cpu_affinity; /* Bitmask of allowed CPUs */

  /* Memory / Resources */
  void *mm_struct;     /* Pointer to memory map (page directory) */
  void *kernel_stack;  /* Kernel stack pointer */
  ArchContext context; /* Saved hardware context */

  /* List Nodes (for RunQueue, WaitQueue, Lists) */
  struct Process *next;
  struct Process *prev;

  /* IPC / Synchronization */
  int32_t exit_code;
  void *waiting_on; /* Generic pointer to resource triggering block */

  /* Security / Capabilities (Microkernel style) */
  uint32_t capabilities;

} Process;

/* --- API --- */

/**
 * @brief Initializes the core scheduler subsystems.
 *        Called early in kernel boot.
 */
void scheduler_init(void);

/**
 * @brief Starts the scheduler on the current CPU.
 *        DOES NOT RETURN.
 */
void scheduler_start(void);

/* Task Management */
Process *scheduler_create_process(const char *name, void (*entry_point)(void),
                                  priority_t prio);
void scheduler_add_process(Process *proc);
void scheduler_terminate_process(Process *proc);
void scheduler_exit_current(int code);
void scheduler_fork(void); /* Placeholder for fork implementation */

/* Scheduling Control */
void scheduler_yield(void);
void scheduler_sleep(uint32_t ms);
void scheduler_wake_up(pid_t pid);
void scheduler_suspend(pid_t pid);
void scheduler_resume(pid_t pid);

/* Configuration */
void scheduler_set_policy(pid_t pid, SchedPolicy policy);
void scheduler_set_priority(pid_t pid, priority_t prio);
void scheduler_set_affinity(pid_t pid, uint32_t cpu_mask);

/* Info */
Process *scheduler_get_current_process(void);
cpu_id_t scheduler_get_current_cpu(void);

/* Lock Scheduler (Disable Preemption) - Critical Sections */
void scheduler_lock(void);
void scheduler_unlock(void);

#endif /* KERNEL_SCHEDULER_H */