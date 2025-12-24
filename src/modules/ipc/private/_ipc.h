#ifndef _IPC_PRIV_H_
#define _IPC_PRIV_H_

/** @sherif
 * why this here? ipc_internal?
 *
 */

struct ipc_internal
{
    int placeholder;

} __TASK_;

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/kernel.h>
#include <kernel/ipc.h>
#include <pthread_compat.h>

/*
 * Maximum size of a single IPC message payload.
 * Messages larger than this value must be rejected.
 */
#define IPC_MAX_MESSAGE_SIZE 4096 // 4 KiB: is this proper value? and how to justify it?

/*
 * Maximum number of queued messages per endpoint.
 * This bound prevents unbounded kernel memory usage.
 */
#define IPC_MAX_QUEUE_DEPTH 64 // 64 messages

/*
 * Internal IPC message representation.
 * The payload buffer is owned by the kernel.
 */
struct ipc_message_internal
{
    void *data;
    size_t length;
};

/*
 * Internal IPC message queue node.
 * Messages are ordered in FIFO order.
 */
struct ipc_message_node
{
    struct ipc_message_internal message;
    struct ipc_message_node *next;
};

/*
 * Internal IPC endpoint structure.
 * This structure represents a single communication endpoint.
 */
struct ipc_endpoint
{
    ipc_endpoint_t id;
    pid_t owner;
    struct ipc_message_node *queue_head;
    struct ipc_message_node *queue_tail;
    size_t queue_depth;
    bool closed;
    spinlock_t lock;
    wait_queue_t wait_queue;
};

/*
 * Validate an IPC endpoint identifier.
 * Returns a pointer to the internal endpoint structure on success.
 * Returns NULL if the endpoint is invalid or destroyed.
 */
struct ipc_endpoint *ipc_endpoint_lookup(ipc_endpoint_t id);

/*
 * Wake all threads blocked on an endpoint.
 * All woken threads must observe IPC_ERR_CLOSED.
 */
void ipc_endpoint_wake_all(struct ipc_endpoint *endpoint);

/*
 * Free all queued messages associated with an endpoint.
 * This function assumes the endpoint lock is held.
 */
void ipc_endpoint_clear_queue(struct ipc_endpoint *endpoint);

#endif /* _IPC_PRIV_H_ */
