#include <kernel/ipc.h>
#include <kernel/child_handler.h> /* For getting current task ID */
#include "private/_ipc.h"
#include <string.h> /* For memcpy */

static struct ipc_internal ipc_data;

void ipc_init(void) {
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        ipc_data.messages[i].used = 0;
    }
}

int ipc_send(int32_t dest_id, const void* data, uint32_t size) {
    if (size > MAX_MSG_SIZE) {
        return -1;
    }

    /* Find a free slot */
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        if (!ipc_data.messages[i].used) {
            ipc_data.messages[i].used = 1;
            ipc_data.messages[i].dest_id = dest_id;
            ipc_data.messages[i].sender_id = get_current_task_id();
            ipc_data.messages[i].size = size;

            /* Simple memcpy - assuming freestanding env has it or we implement it.
               Usually gcc provides builtins or we link against a lib.
               If not, I should implement a simple copy loop. */
            const uint8_t* d = (const uint8_t*)data;
            for (uint32_t k = 0; k < size; ++k) {
                ipc_data.messages[i].data[k] = d[k];
            }

            return 0;
        }
    }
    return -1; /* No buffer space */
}

int ipc_receive(int32_t* sender_id, void* buffer, uint32_t size) {
    int32_t current_id = get_current_task_id();

    for (int i = 0; i < MAX_MESSAGES; ++i) {
        if (ipc_data.messages[i].used && ipc_data.messages[i].dest_id == current_id) {
            if (size < ipc_data.messages[i].size) {
                return -1; /* Buffer too small */
            }

            *sender_id = ipc_data.messages[i].sender_id;

            uint8_t* b = (uint8_t*)buffer;
            for (uint32_t k = 0; k < ipc_data.messages[i].size; ++k) {
                b[k] = ipc_data.messages[i].data[k];
            }

            ipc_data.messages[i].used = 0; /* Free the slot */
            return 0;
        }
    }
    return -1; /* No message found */
}
