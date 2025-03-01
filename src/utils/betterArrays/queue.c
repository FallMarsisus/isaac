#include "queue.h"

// Queue node structure
typedef struct queue_node_s {
    void *data;
    struct queue_node_s *next;
} QueueNode;

// Queue structure
typedef struct queue_s {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

// Function to create a new queue
Queue* create_queue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q) return NULL;

    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

// Function to check if the queue is empty
bool queue_is_empty(Queue *q) {
    if (!q) return true; // Treat NULL queue as empty
    return q->size == 0;
}

// Function to enqueue an element
bool queue_enqueue(Queue *q, void *data, int data_size) {
    if (!q || !data || data_size == 0) return false;

    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    if (!new_node) return false;

    new_node->data = malloc(data_size);
    if (!new_node->data) {
        free(new_node);
        return false;
    }
    
    memcpy(new_node->data, data, data_size);
    new_node->next = NULL;

    if (q->rear) {
        q->rear->next = new_node;
    } else {
        q->front = new_node;
    }
    q->rear = new_node;
    q->size++;
    return true;
}

// Function to dequeue an element
bool queue_dequeue(Queue *q, void *out_data, int data_size) {
    if (!q || !out_data || data_size == 0 || queue_is_empty(q)) return false;

    QueueNode* temp = q->front;
    memcpy(out_data, temp->data, data_size);
    
    q->front = temp->next;
    if (!q->front) {
        q->rear = NULL; // Queue is now empty
    }

    free(temp->data);
    free(temp);
    q->size--;
    return true;
}

// Function to peek at the front element
bool queue_peek(Queue *q, void *out_data, int data_size) {
    if (!q || !out_data || data_size == 0 || queue_is_empty(q)) return false;

    memcpy(out_data, q->front->data, data_size);
    return true;
}

// Function to get the queue size
int queue_size(Queue *q) {
    if (!q) return 0; // Treat NULL queue as having size 0
    return q->size;
}

// Function to clear the queue
void queue_clear(Queue *q) {
    if (!q) return;

    while (q->front) {
        QueueNode* temp = q->front;
        q->front = q->front->next;
        free(temp->data);
        free(temp);
    }
    q->rear = NULL;
    q->size = 0;
}

// Function to destroy the queue
void queue_destroy(Queue *q) {
    if (!q) return;

    queue_clear(q);
    free(q);
}

QueueNode* get_first_queue_node(Queue* q) {
    return q->front;
}

QueueNode* get_next_queue_node(QueueNode* node) {
    return node->next;
}

void* get_data_queue_node(QueueNode* node) {
    return node->data;
}
