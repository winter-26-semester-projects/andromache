#ifndef IPC_H
#define IPC_H

#include <stdint.h>

// Simple message passing
typedef struct {
    int sender_pid;
    int receiver_pid;
    char message[128];
} Message;

int ipc_send(int receiver_pid, const char* msg);
int ipc_receive(int sender_pid, char* buffer, int size);

#endif
