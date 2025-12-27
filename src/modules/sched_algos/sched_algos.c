#include <kernel/sched_algos.h>
#include "private/_sched_algos.h"

void sched_algos_init(void) {
    (void)sizeof(struct sched_algos_internal);
}
