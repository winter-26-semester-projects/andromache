#ifndef KERNEL_IPC_MESSAGE_H
#define KERNEL_IPC_MESSAGE_H

#include <stdint.h>
// #include <stddef.h>

// DOCTOR SAID TO ADD THIS STRUCTURE FOR IPC MESSAGES
#define MSG_MAX_DATA_SIZE 256

// SHERIF SAID TO ADD msg_t nameconvention
typedef struct msg_t
{
    uint32_t sender_id;
    uint32_t receiver_id;
    uint32_t type;
    uint32_t length;
    uint8_t data[MSG_MAX_DATA_SIZE];
} msg_t;

// just init. the a new message
static inline void msg_init(msg_t *msg)
{
    if (msg)
    {
        msg->sender_id = 0;
        msg->receiver_id = 0;
        msg->type = 0;
        msg->length = 0;
        for (size_t i = 0; i < MSG_MAX_DATA_SIZE; ++i)
            msg->data[i] = 0;
    }
}

#endif // KERNEL_IPC_MESSAGE_H

// Developed with love by @Mahros