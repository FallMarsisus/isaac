#ifndef DYNAMIC_QUEUE_H
#define DYNAMIC_QUEUE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct queue_node_s QueueNode;

// Queue structure
typedef struct queue_s Queue;

// Function declarations
Queue* create_queue();
bool queue_is_empty(Queue *q);
bool queue_enqueue(Queue *q, void *data, int data_size);
bool queue_dequeue(Queue *q, void *out_data, int data_size);
bool queue_peek(Queue *q, void *out_data, int data_size);
int queue_size(Queue *q);
void queue_clear(Queue *q);
void queue_destroy(Queue *q);

QueueNode* get_first_queue_node(Queue* q);
QueueNode* get_next_queue_node(QueueNode* node);
void* get_data_queue_node(QueueNode* node);

#endif // DYNAMIC_QUEUE_H
