/**
 * @file test_ipc.c
 * @brief Unit tests for IPC module.
 */
#include <kernel/ipc.h>
#include <assert.h>
#include <stdio.h>

void test_ipc_create_destroy()
{
    ipc_init();

    ipc_endpoint_t ep1 = ipc_endpoint_create();
    ipc_endpoint_t ep2 = ipc_endpoint_create();

    assert(ep1 != 0);
    assert(ep2 != 0);
    assert(ep1 != ep2);

    assert(ipc_endpoint_destroy(ep1) == IPC_OK);
    assert(ipc_endpoint_destroy(ep2) == IPC_OK);
    assert(ipc_endpoint_destroy(ep1) == IPC_ERR_INVALID);

    printf("test_ipc_create_destroy passed\n");
}

void test_ipc_send_receive()
{
    ipc_init();

    ipc_endpoint_t ep = ipc_endpoint_create();
    assert(ep != 0);

    struct ipc_message msg = {
        .data = "Hello, IPC!",
        .length = 12,
        .priority = IPC_PRIORITY_NORMAL};

    assert(ipc_send(ep, &msg, 0) == IPC_OK);

    struct ipc_message received_msg;
    assert(ipc_receive(ep, &received_msg, 0) == IPC_OK);
    assert(received_msg.length == msg.length);
    assert(received_msg.priority == msg.priority);
    assert(memcmp(received_msg.data, msg.data, msg.length) == 0);

    printf("test_ipc_send_receive passed\n");
}

int main()
{
    test_ipc_create_destroy();
    test_ipc_send_receive();

    printf("All tests passed!\n");
    return 0;
}

/**
 * Created with &hearts; by 0xQ4B4S
 */