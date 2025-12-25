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
    IPC_ERR_CLOSED,
    IPC_ERR_TIMEOUT
} ipc_status_t;

/*
 * IPC message priorities.
 */
#define IPC_PRIORITY_LOW 0
#define IPC_PRIORITY_NORMAL 1
#define IPC_PRIORITY_HIGH 2

/*
 * IPC message descriptor.
 */
struct ipc_message
{
    const void *data;
    size_t length;
    uint8_t priority; // Message priority
};

/*
 * IPC operation flags.
 */
#define IPC_MAX_MESSAGE_SIZE 4096 // 4 KiB
#define IPC_MAX_QUEUE_DEPTH 64    // 64 messages

/*
 * IPC function prototypes.
 */
ipc_status_t ipc_send(ipc_endpoint_t endpoint, const struct ipc_message *message, uint32_t timeout_ms);
ipc_status_t ipc_receive(ipc_endpoint_t endpoint, struct ipc_message *message, uint32_t timeout_ms);
ipc_status_t ipc_broadcast(const ipc_endpoint_t *endpoints, size_t count, const struct ipc_message *message);

#endif /* _KERNEL_IPC_H_ */
