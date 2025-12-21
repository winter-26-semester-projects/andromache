#include <stddef.h>
#include <stdint.h>
#include "machine/gdt.h"
#include "machine/idt.h"
#include "machine/timer.h"
#include "modules/memory.h"
#include "modules/scheduler.h"
#include "modules/task.h"
#include "modules/shell.h"

// Forward declaration for daemon
void daemon_start(void);

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_addr) {
    (void)multiboot_magic;
    (void)multiboot_addr;

    // 1. Initialize Machine Layer
    gdt_init();
    idt_init();
    timer_init(50); // 50 Hz

    // 2. Initialize Memory
    // Assuming we have some memory, e.g., starting at 2MB, size 4MB
    memory_init(0x200000, 0x400000);

    // 3. Initialize Scheduler
    scheduler_init(SCHED_RR);

    // 4. Create Initial Tasks
    // Shell (User interface)
    Task* shell_task = task_create(shell_start, 1, 100);
    if (shell_task) scheduler_add_task(shell_task);

    // Daemon (Background process)
    Task* daemon_task = task_create(daemon_start, 2, 200);
    if (daemon_task) scheduler_add_task(daemon_task);

    // 5. Enter infinite loop (idle task)
    // Enable interrupts to let scheduler take over
    __asm__ volatile("sti");

    while (1) {
        // Halt CPU until next interrupt
        __asm__ volatile("hlt");
    }
}
