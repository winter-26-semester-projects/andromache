#ifndef _KERNEL_IPC_H_
#define _KERNEL_IPC_H_

#include <stdint.h>

void ipc_init(void);

/* Sends a message to the destination task. Returns 0 on success, -1 on failure. */
int ipc_send(int32_t dest_id, const void* data, uint32_t size);

/* Receives a message. Returns 0 on success, -1 if no message available.
   sender_id is output. */
int ipc_receive(int32_t* sender_id, void* buffer, uint32_t size);

#endif /* _KERNEL_IPC_H_ */
