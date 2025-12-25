#include <kernel/ipc.h>
#include "private/_ipc.h"
#include <stdlib.h> // For memory allocation
#include <string.h> // For memset

#define MAX_ENDPOINTS 128

static struct ipc_endpoint endpoint_table[MAX_ENDPOINTS];

/* Placeholder for time management */
static uint32_t kernel_get_time_ms(void)
{
    // Replace with actual time management logic if available
    return 0;
}

static void kernel_yield(void)
{
    // Replace with actual yield logic if available
}

/* Retrieve endpoint by ID */
static struct ipc_endpoint *get_endpoint_by_id(ipc_endpoint_t id)
{
    for (size_t i = 0; i < MAX_ENDPOINTS; ++i)
    {
        if (endpoint_table[i].id == id && !endpoint_table[i].closed)
        {
            return &endpoint_table[i];
        }
    }
    return NULL;
}

/*
 * Initialize the IPC subsystem.
 */
void ipc_init(void)
{
    memset(endpoint_table, 0, sizeof(endpoint_table));
}

/*
 * Create a new IPC endpoint.
 */
ipc_endpoint_t ipc_endpoint_create(void)
{
    for (size_t i = 0; i < MAX_ENDPOINTS; ++i)
    {
        if (endpoint_table[i].id == 0)
        {
            endpoint_table[i].id = i + 1; // Assign a unique ID
            endpoint_table[i].queue_head = NULL;
            endpoint_table[i].queue_tail = NULL;
            endpoint_table[i].queue_depth = 0;
            endpoint_table[i].closed = 0;
            return endpoint_table[i].id;
        }
    }
    return 0; // No available slots
}

/*
 * Destroy an IPC endpoint.
 */
ipc_status_t ipc_endpoint_destroy(ipc_endpoint_t endpoint)
{
    struct ipc_endpoint *ep = get_endpoint_by_id(endpoint);
    if (!ep)
    {
        return IPC_ERR_INVALID;
    }

    // Free all queued messages
    struct ipc_message_internal *msg = ep->queue_head;
    while (msg)
    {
        struct ipc_message_internal *next = msg->next;
        free(msg);
        msg = next;
    }

    memset(ep, 0, sizeof(*ep)); // Clear the endpoint
    return IPC_OK;
}

/*
 * Insert a message into the endpoint's queue based on priority.
 */
static void ipc_insert_message(struct ipc_endpoint *endpoint, struct ipc_message_internal *new_message)
{
    if (!endpoint->queue_head || new_message->priority > endpoint->queue_head->priority)
    {
        // Insert at the head of the queue
        new_message->next = endpoint->queue_head;
        endpoint->queue_head = new_message;
        if (!endpoint->queue_tail)
        {
            endpoint->queue_tail = new_message;
        }
        return;
    }

    // Find the correct position based on priority
    struct ipc_message_internal *current = endpoint->queue_head;
    while (current->next && current->next->priority >= new_message->priority)
    {
        current = current->next;
    }

    // Insert the message
    new_message->next = current->next;
    current->next = new_message;
    if (!new_message->next)
    {
        endpoint->queue_tail = new_message;
    }
}

/*
 * Send a message to an IPC endpoint with priority.
 */
ipc_status_t ipc_send(
    ipc_endpoint_t endpoint,
    const struct ipc_message *message,
    uint32_t timeout_ms)
{
    if (!message || !message->data || message->length > IPC_MAX_MESSAGE_SIZE)
    {
        return IPC_ERR_INVALID;
    }

    struct ipc_endpoint *ep = get_endpoint_by_id(endpoint);
    if (!ep || ep->closed)
    {
        return IPC_ERR_INVALID;
    }

    struct ipc_message_internal *new_message = malloc(sizeof(struct ipc_message_internal));
    if (!new_message)
    {
        return IPC_ERR_NO_MEMORY;
    }

    new_message->data = (void *)message->data;
    new_message->length = message->length;
    new_message->priority = message->priority;
    new_message->next = NULL;

    ipc_insert_message(ep, new_message);

    return IPC_OK;
}

/*
 * Receive a message from an IPC endpoint.
 */
ipc_status_t ipc_receive(
    ipc_endpoint_t endpoint,
    struct ipc_message *message,
    uint32_t timeout_ms)
{
    if (!message)
    {
        return IPC_ERR_INVALID;
    }

    struct ipc_endpoint *ep = get_endpoint_by_id(endpoint);
    if (!ep || ep->closed || !ep->queue_head)
    {
        return IPC_ERR_WOULD_BLOCK;
    }

    struct ipc_message_internal *msg = ep->queue_head;
    ep->queue_head = msg->next;
    if (!ep->queue_head)
    {
        ep->queue_tail = NULL;
    }

    message->data = msg->data;
    message->length = msg->length;
    message->priority = msg->priority;

    free(msg);

    return IPC_OK;
}

/*
 * Broadcast a message to multiple IPC endpoints.
 */
ipc_status_t ipc_broadcast(
    const ipc_endpoint_t *endpoints,
    size_t count,
    const struct ipc_message *message)
{
    if (!endpoints || !message || !message->data || message->length > IPC_MAX_MESSAGE_SIZE)
    {
        return IPC_ERR_INVALID;
    }

    ipc_status_t status = IPC_OK;

    for (size_t i = 0; i < count; ++i)
    {
        ipc_status_t result = ipc_send(endpoints[i], message, 0);
        if (result != IPC_OK)
        {
            status = result;
        }
    }

    return status;
}




/**
 * Created with &hearts; by 0xQ4B4S
 */