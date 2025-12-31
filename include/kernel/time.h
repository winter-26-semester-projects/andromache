#ifndef _KERNEL_TIME_H_
#define _KERNEL_TIME_H_
#include <stdint.h>

typedef uint64_t ktime_t;

/* monotonic time */
ktime_t ktime_now(void);

/* logical ticks for scheduler */
ktime_t ktime_ticks(void);

/* called once at boot */
void time_init(void);

/* called periodically (later by timer interrupt) */
void time_tick(void);

#endif /* _KERNEL_TIME_H_ */
