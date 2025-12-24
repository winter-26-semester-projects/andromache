#ifndef _KERNEL_IPC_H_
#define _KERNEL_IPC_H_

#include <stddef.h>
#include <stdint.h>
#include <kernel/kernel.h>

/*
 * IPC endpoint identifier.
 * This represents an opaque kernel-managed communication object.
 * A value of zero is always considered invalid.
 */
typedef uint32_t ipc_endpoint_t;

/*
 * IPC operation result codes.
 * These values are returned directly by IPC functions.
 */
typedef enum
{
    IPC_OK = 0,
    IPC_ERR_INVALID,
    IPC_ERR_NO_MEMORY,
    IPC_ERR_PERMISSION,
    IPC_ERR_WOULD_BLOCK,
    IPC_ERR_CLOSED
} ipc_status_t;

/*
 * IPC message descriptor.
 * The kernel copies the payload described by this structure.
 */
typedef struct ipc_message
{
    const void *data;
    size_t length;
} ipc_message_t;

/*
 * IPC operation flags.
 * These flags control blocking behavior.
 */
#define IPC_BLOCKING (1U << 0)
#define IPC_NONBLOCKING (1U << 1)

/*
 * Create a new IPC endpoint.
 * The endpoint is owned by the calling process.
 *
 * Returns a valid endpoint identifier on success.
 * Returns zero on failure.
 */
ipc_endpoint_t ipc_endpoint_create(void);

/*
 * Destroy an IPC endpoint.
 * All blocked senders and receivers are woken and fail with IPC_ERR_CLOSED.
 */
ipc_status_t ipc_endpoint_destroy(ipc_endpoint_t endpoint);

/*
 * Send a message to an IPC endpoint.
 * The behavior depends on the supplied flags.
 */
ipc_status_t ipc_send(
    ipc_endpoint_t endpoint,
    const ipc_message_t *message,
    uint32_t flags);

/*
 * Receive a message from an IPC endpoint.
 * The kernel copies the message into the provided buffer.
 */
ipc_status_t ipc_receive(
    ipc_endpoint_t endpoint,
    ipc_message_t *message,
    uint32_t flags);

#endif /* _KERNEL_IPC_H_ */
