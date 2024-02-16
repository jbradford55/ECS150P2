#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct node {
    void *data;
    struct node *next;
};
struct queue {
	struct node *head;
    struct node *tail;
    int length;
};

queue_t queue_create(void)
{
	struct queue *new_queue = malloc(sizeof(struct queue *));
    if (new_queue == NULL)
        return NULL;
    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->length = 0;
    return new_queue;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->length != 0)
        return -1; // Queue is NULL or not empty

    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL)
        return -1; // Queue or data is NULL

    struct node *new_node = (struct node *)malloc(sizeof(struct node*));
    if (new_node == NULL)
        return -1; // Memory allocation error

    new_node->data = data;
    new_node->next = NULL;

    if (queue->tail == NULL) {
        queue->head = new_node;
        
    } else {
        queue->tail->next = new_node;
    }

    queue->tail = new_node;
    queue->length++;

    return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || data == NULL || queue->length == 0)
        return -1; // Queue is NULL, data is NULL, or queue is empty

    *data = queue->head->data;
    struct node *temp = queue->head;
    queue->head = queue->head->next;
    free(temp);

    if (queue->head == NULL)
        queue->tail = NULL;

    queue->length--;

    return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL || queue->length == 0)
        return -1; // Queue is NULL, data is NULL, or queue is empty

    struct node *prev = NULL;
    struct node *curr = queue->head;

    while (curr != NULL) {
        if (curr->data == data) {
            if (prev == NULL) {
                queue->head = curr->next;
            } else {
                prev->next = curr->next;
            }

            if (queue->tail == curr) {
                queue->tail = prev;
            }

            //free(curr);
            queue->length--;
            return 0;
        }

        prev = curr;
        curr = curr->next;
    }

    return -1; // Data not found
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if (queue == NULL || func == NULL)
        return -1; // Queue or function is NULL

    struct node *current = queue->head;
    while (current != NULL) {
        int *a = (int*)(current->data);
        (*func)(queue, current->data);
        current = current->next;
    }

    return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL)
        return -1; // Queue is NULL

    return queue->length;
}
