#include "kernel/kernel.h"
#include "config.h"
#include "kernel/ipc.h"
#include "kernel/child_handler.h"

/* Dummy task 1 */
void task_a(void) {
    /* Send a message to task 1 (which will be ID 1) */
    /* Assuming task IDs are 0 and 1 */
    const char* msg = "Hello from A";
    ipc_send(1, msg, 12); /* 12 is length of "Hello from A" + null terminator */
    /* Since we don't have real yielding, this will just return and scheduler will run next */
}

/* Dummy task 2 */
void task_b(void) {
    int32_t sender;
    char buffer[64];
    if (ipc_receive(&sender, buffer, 64) == 0) {
        /* Message received */
        /* In a real kernel we might print it or act on it */
        /* For now we just consume it */
        (void)sender;
        (void)buffer;
    }
}

void kernel_init(void) {
    (void) CONFIG_MAX_THREADS;

    ipc_init();
    child_handler_init();

    create_task(task_a);
    create_task(task_b);
}

void kernel_main(void) {
    for (;;) {
        schedule();
        /* If schedule returns (which it does in our simple implementation), we halt slightly or loop */
        /* In a real preemptive kernel, interrupts would drive this. */
        /* Here we cooperative multitask via function calls. */

        /* Loop delay to avoid tight spin if schedule returns immediately */
        for (volatile int i = 0; i < 10000; ++i) {}
    }
}
