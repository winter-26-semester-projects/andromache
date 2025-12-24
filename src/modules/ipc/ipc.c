#include <kernel/ipc.h>
#include "private/_ipc.h"

// why this here? ipc_internal?
void ipc_init(void)
{

    (void)sizeof(struct ipc_internal);
}

#include <kernel/ipc.h>
#include <kernel/kernel.h>
#include "private/_ipc.h"

/*
 * Create a new IPC endpoint.
 * This stub returns an invalid endpoint.
 */
ipc_endpoint_t ipc_endpoint_create(void)
{
    return 0;
}

/*
 * Destroy an IPC endpoint.
 * This stub reports an invalid operation.
 */
ipc_status_t ipc_endpoint_destroy(ipc_endpoint_t endpoint)
{
    (void)endpoint;
    return IPC_ERR_INVALID;
}

/*
 * Send a message to an IPC endpoint.
 * This stub reports an invalid operation.
 */
ipc_status_t ipc_send(
    ipc_endpoint_t endpoint,
    const ipc_message_t *message,
    uint32_t flags)
{
    (void)endpoint;
    (void)message;
    (void)flags;
    return IPC_ERR_INVALID;
}

/*
 * Receive a message from an IPC endpoint.
 * This stub reports an invalid operation.
 */
ipc_status_t ipc_receive(
    ipc_endpoint_t endpoint,
    ipc_message_t *message,
    uint32_t flags)
{
    (void)endpoint;
    (void)message;
    (void)flags;
    return IPC_ERR_INVALID;
}
