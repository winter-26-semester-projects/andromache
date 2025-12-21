#include "modules/signal.h"
#include <stddef.h>

static sighandler_t handlers[32];

void signal_register(int signum, sighandler_t handler) {
    if (signum >= 0 && signum < 32) {
        handlers[signum] = handler;
    }
}

void signal_send(int pid, int signum) {
    // In a real OS, this would modify the target process's pending signals
    // and the scheduler/ISR return path would invoke the handler.
    // Here we just call it directly if it's for the current process (simplified).
    (void)pid;
    if (signum >= 0 && signum < 32 && handlers[signum]) {
        handlers[signum](signum);
    }
}
