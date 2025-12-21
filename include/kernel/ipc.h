#ifndef KERNEL_IPC_H
#define KERNEL_IPC_H

#include <stdint.h>

void ipc_init(void);
int ipc_send(int32_t dest_id, const void* data, uint32_t size);
int ipc_receive(int32_t* sender_id, void* buffer, uint32_t size);

#endif /* KERNEL_IPC_H */
