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
 * IPC operation flags.
 */
#define IPC_MAX_MESSAGE_SIZE 4096 // 4 KiB
#define IPC_MAX_QUEUE_DEPTH 64    // 64 messages

#endif /* _KERNEL_IPC_H_ */
