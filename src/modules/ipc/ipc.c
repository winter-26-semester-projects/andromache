#include "modules/ipc.h"
#include <string.h>
#include <stddef.h>

// Simple buffer for demonstration (single message buffer per process would be better)
// Here we just use a global slot for simplicity.
static Message global_msg;
static int msg_pending = 0;

int ipc_send(int receiver_pid, const char* msg) {
    if (msg_pending) return -1; // Busy

    global_msg.sender_pid = 0; // Current PID
    global_msg.receiver_pid = receiver_pid;

    // safe copy
    size_t len = 0;
    while(msg[len] && len < 127) {
        global_msg.message[len] = msg[len];
        len++;
    }
    global_msg.message[len] = '\0';

    msg_pending = 1;
    return 0;
}

int ipc_receive(int sender_pid, char* buffer, int size) {
    if (!msg_pending) return -1;

    // Filter by sender if specified
    if (sender_pid != -1 && global_msg.sender_pid != sender_pid) return -1;

    size_t len = 0;
    while(global_msg.message[len] && len < (size_t)(size - 1)) {
        buffer[len] = global_msg.message[len];
        len++;
    }
    buffer[len] = '\0';

    msg_pending = 0;
    return len;
}
