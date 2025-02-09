#ifndef DYNAMIC_QUEUE_H
#define DYNAMIC_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Queue structure
typedef struct queue_s Queue;

// Function declarations
Queue* create_queue();
bool queue_is_empty(Queue *q);
bool queue_enqueue(Queue *q, void *data, size_t data_size);
bool queue_dequeue(Queue *q, void *out_data, size_t data_size);
bool queue_peek(Queue *q, void *out_data, size_t data_size);
size_t queue_size(Queue *q);
void queue_clear(Queue *q);
void queue_destroy(Queue *q);

#endif // DYNAMIC_QUEUE_H
