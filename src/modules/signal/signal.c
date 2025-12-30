#include <kernel/signal.h>
#include "private/_signal.h"

void signal_init(void) 
{
    (void)sizeof(struct signal_internal);
}
