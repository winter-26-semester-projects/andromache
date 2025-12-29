#include <kernel/sched.h>
#include "private/_sched.h"

void sched_init(void) {
    (void)sizeof(struct sched_internal);
}
