#include "modules/task.h"
#include "modules/memory.h"
#include <stddef.h>

static int next_pid = 1;
extern void perform_task_switch(uint32_t old_esp_ptr, uint32_t new_esp);

void task_init(void) {
    // Initial task is the kernel itself, usually handled by setting current_task
}

Task* task_create(void (*entry_point)(), int priority, int expected_time) {
    Task* task = (Task*)kmalloc(sizeof(Task));
    if (!task) return NULL;

    task->id = next_pid++;
    task->state = TASK_READY;
    task->priority = priority;
    task->expected_time = expected_time;
    task->next = NULL;

    // Allocate stack
    uint32_t stack_size = 4096;
    task->stack_base = (uint32_t)kmalloc(stack_size);
    if (!task->stack_base) return NULL;

    // Setup stack for context switch
    uint32_t* stack = (uint32_t*)(task->stack_base + stack_size);

    // Mimic the registers pushed by pusha and ISR
    // We need to match the structure expected by `perform_task_switch` or `iret`
    // For simplicity, we can set up an IRET frame if we are jumping to user mode,
    // or a simple function call frame for kernel threads.

    // Pushing initial EIP
    extern void task_trampoline();
    *--stack = (uint32_t)task_trampoline;
    // Pushing initial EBP, EBX, ESI, EDI (callee saved)
    *--stack = 0;
    *--stack = (uint32_t)entry_point; // Store entry_point in EBX
    *--stack = 0;
    *--stack = 0;

    task->esp = (uint32_t)stack;

    return task;
}

__asm__(
    ".global task_trampoline\n"
    "task_trampoline:\n"
    "sti\n"
    "call *%ebx\n"
    "1: hlt\n"
    "jmp 1b\n"
);

// Simple ASM wrapper for context switch (cooperative/kernel threads)
// In a real OS, this interacts with the scheduler interrupt.
__asm__(
    ".global perform_task_switch\n"
    "perform_task_switch:\n"
    "mov 4(%esp), %eax\n" // old_esp_ptr
    "mov 8(%esp), %edx\n" // new_esp
    "push %ebp\n"
    "push %ebx\n"
    "push %esi\n"
    "push %edi\n"
    "mov %esp, (%eax)\n" // save old ESP
    "mov %edx, %esp\n"   // load new ESP
    "pop %edi\n"
    "pop %esi\n"
    "pop %ebx\n"
    "pop %ebp\n"
    "ret\n"
);
