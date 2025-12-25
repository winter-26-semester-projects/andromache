#ifndef _IPC_PRIV_H_
#define _IPC_PRIV_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/kernel.h>
#include <kernel/ipc.h>
#include <sys/types.h>

/*
 * Internal IPC message representation.
 * The payload buffer is owned by the kernel.
 */
struct ipc_message
{
    void *data;
    size_t length;
};

/*
 * IPC endpoint structure.
 */
struct ipc_endpoint
{
    pid_t owner;
    ipc_endpoint_t id;
    struct ipc_message *queue_head;
    struct ipc_message *queue_tail;
    size_t queue_depth;
    uint32_t closed; // bit mask
};

#endif /* _IPC_PRIV_H_ */
