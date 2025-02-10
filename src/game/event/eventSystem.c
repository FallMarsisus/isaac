#include "eventSystem.h"

EventSystem* event_system;
EventQueue* event_queue;

EventQueue* create_event_queue(int initial_capacity) {
    EventQueue* queue = (EventQueue*)malloc(sizeof(EventQueue));
    if (!queue) {
        printf("Failed to allocate memory for event queue\n");
        exit(1);
    }

    queue->events = (Event*)malloc(initial_capacity * sizeof(Event));
    if (!queue->events) {
        printf("Failed to allocate memory for events\n");
        free(queue);
        exit(1);
    }

    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = initial_capacity;
    return queue;
}
void enqueue_event(EventQueue* queue, EventType type, void* data) {
    if (queue->size == queue->capacity) {
        // Double the capacity
        queue->capacity *= 2;
        queue->events = (Event*)realloc(queue->events, queue->capacity * sizeof(Event));
        if (!queue->events) {
            printf("Failed to resize event queue\n");
            exit(1);
        }

        // Adjust for circular behavior after resizing
        if (queue->front > queue->rear) {
            for (int i = 0; i < queue->front; i++) {
                queue->events[queue->size + i] = queue->events[i];
            }
            queue->rear = queue->size + queue->front - 1;
        }
    }

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->events[queue->rear].type = type;
    queue->events[queue->rear].data = data;
    queue->size++;
}
Event dequeue_event(EventQueue* queue) {
    if (queue->size == 0) {
        printf("Event queue is empty, cannot dequeue\n");
        return (Event){EVENT_NONE, NULL};
    }

    Event event = queue->events[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return event;
}
void free_event_queue(EventQueue* queue) {
    if (queue) {
        for(int i = 0; i < queue->size; i++) {
            if(queue->events[queue->front + i].data != NULL) {
                free(queue->events[queue->front + i].data);
            }
        }
        free(queue->events);
        free(queue);
    }
}

void init_event_system() {
    event_system = malloc(EVENT_MAX * sizeof(EventSystem));
    if (!event_system) {
        printf("Failed to allocate memory for event system\n");
        exit(1);
    }

    for (int i = 0; i < EVENT_MAX; ++i) {
        event_system[i].listener_count = 0;
        event_system[i].listener_capacity = 4;
        event_system[i].listeners = malloc(event_system[i].listener_capacity * sizeof(EventListener));
        if (!event_system[i].listeners) {
            printf("Failed to allocate memory for event listeners\n");
            exit(1);
        }
    }

    event_queue = create_event_queue(10);
    if (!event_queue) {
        printf("Failed to allocate memory for event queue\n");
        exit(1);
    }
}

void free_event_system() {
    for (int i = 0; i < EVENT_MAX; ++i) {
        free(event_system[i].listeners);
    }
    free_event_queue(event_queue);
    free(event_system);
    event_system = NULL;
}

// Register a listener
void register_listener(EventType type, EventListener listener) {
    if (!event_system || type >= EVENT_MAX) {
        printf("Invalid event type %d\n", type);
        return;
    }

    EventSystem* system = &event_system[type];

    if (system->listener_count >= system->listener_capacity) {
        system->listener_capacity *= 2;
        system->listeners = (EventListener*)realloc(system->listeners, system->listener_capacity * sizeof(EventListener));
        if (!system->listeners) {
            printf("Failed to resize event listeners\n");
            exit(1);
        }
    }

    system->listeners[system->listener_count++] = listener;
}

// Unregister a listener
void unregister_listener(EventType type, EventListener listener) {
    if (!event_system || type >= EVENT_MAX) {
        printf("Invalid event type %d\n", type);
        return;
    }

    EventSystem* system = &event_system[type];

    for (int i = 0; i < system->listener_count; ++i) {
        if (system->listeners[i] == listener) {
            for (int j = i; j < system->listener_count - 1; ++j) {
                system->listeners[j] = system->listeners[j + 1];
            }
            system->listener_count--;
            return;
        }
    }

    printf("Listener not found for event type %d\n", type);
}

void trigger_event(EventType type, void* data) {
    if (!event_queue) {
        printf("Event queue not initialized\n");
        return;
    }

    enqueue_event(event_queue, type, data);
}

void call_events() {
    while (event_queue->size > 0) {
        Event event = dequeue_event(event_queue);

        // Call listeners for the event type
        EventSystem* system = &event_system[event.type];
        if(!system) continue;
        for (int i = 0; i < system->listener_count; ++i) {
            system->listeners[i](event);
        }

        if(event.data != NULL) {
            free(event.data);
            event.data = NULL;
        }
    }
}
