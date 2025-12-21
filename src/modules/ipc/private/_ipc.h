#ifndef _IPC_PRIV_H_
#define _IPC_PRIV_H_

#include <stdint.h>

#define MAX_MESSAGES 128
#define MAX_MSG_SIZE 64

struct ipc_message {
    int32_t sender_id;
    int32_t dest_id;
    uint8_t data[MAX_MSG_SIZE];
    uint32_t size;
    int used; /* 1 if this slot is used, 0 otherwise */
};

struct ipc_internal {
    struct ipc_message messages[MAX_MESSAGES];
};

#endif /* _IPC_PRIV_H_ */
