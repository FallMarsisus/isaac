#include "eventSystem.h"

// Global event system and queue
EventSystem* event_system;
EventQueue* event_queue;

// Implementation
EventQueue* create_event_queue(int initial_capacity) {
    EventQueue* queue = (EventQueue*)malloc(sizeof(EventQueue));
    if (!queue) {
        fprintf(stderr, "Failed to allocate memory for event queue\n");
        return NULL;
    }

    queue->events = (Event*)calloc(initial_capacity, sizeof(Event));
    if (!queue->events) {
        fprintf(stderr, "Failed to allocate memory for events\n");
        free(queue);
        return NULL;
    }

    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = initial_capacity;
    return queue;
}

void free_event_queue(EventQueue* queue) {
    if (queue) {
        for (int i = 0; i < queue->size; i++) {
            int index = (queue->front + i) % queue->capacity;
            if (queue->events[index].free_data && queue->events[index].data) {
                free(queue->events[index].data);
            }
        }
        free(queue->events);
        free(queue);
    }
}

void enqueue_event(EventQueue* queue, EventType type, void* data, bool free_data) {
    if (!queue) {
        fprintf(stderr, "Event queue is NULL\n");
        return;
    }

    if (queue->size == queue->capacity) {
        // Double the capacity
        int new_capacity = queue->capacity * 2;
        Event* new_events = (Event*)realloc(queue->events, new_capacity * sizeof(Event));
        if (!new_events) {
            fprintf(stderr, "Failed to resize event queue\n");
            return;
        }

        // Adjust for circular behavior after resizing
        if (queue->front > queue->rear) {
            for (int i = 0; i < queue->front; i++) {
                new_events[queue->size + i] = new_events[i];
            }
            queue->rear = queue->size + queue->front - 1;
        }

        queue->events = new_events;
        queue->capacity = new_capacity;
    }

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->events[queue->rear].type = type;
    queue->events[queue->rear].data = data;
    queue->events[queue->rear].free_data = free_data;
    queue->size++;
}

Event dequeue_event(EventQueue* queue) {
    if (!queue) {
        fprintf(stderr, "Event queue is NULL\n");
        return (Event){EVENT_NONE, NULL, false};
    }

    if (queue->size == 0) {
        fprintf(stderr, "Event queue is empty, cannot dequeue\n");
        return (Event){EVENT_NONE, NULL, false};
    }

    Event event = queue->events[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    return event;
}

void init_event_system() {
    event_system = (EventSystem*)calloc(EVENT_MAX, sizeof(EventSystem));
    if (!event_system) {
        fprintf(stderr, "Failed to allocate memory for event system\n");
        return;
    }

    for (int i = 0; i < EVENT_MAX; ++i) {
        event_system[i].listener_count = 0;
        event_system[i].listener_capacity = 4;
        event_system[i].listeners = (EventListener*)calloc(event_system[i].listener_capacity, sizeof(EventListener));
        if (!event_system[i].listeners) {
            fprintf(stderr, "Failed to allocate memory for event listeners\n");
            // Free previously allocated listeners
            for (int j = 0; j < i; ++j) {
                free(event_system[j].listeners);
            }
            free(event_system);
            return;
        }
    }

    event_queue = create_event_queue(10);
    if (!event_queue) {
        fprintf(stderr, "Failed to allocate memory for event queue\n");
        for (int i = 0; i < EVENT_MAX; ++i) {
            free(event_system[i].listeners);
        }
        free(event_system);
        return;
    }
}

void free_event_system() {
    if (event_system) {
        for (int i = 0; i < EVENT_MAX; ++i) {
            free(event_system[i].listeners);
        }
        free(event_system);
        event_system = NULL;
    }
    free_event_queue(event_queue);
    event_queue = NULL;
}

void register_listener(EventType type, EventListener listener) {
    if (!event_system || type >= EVENT_MAX) {
        fprintf(stderr, "Invalid event type %d\n", type);
        return;
    }

    if (!listener) {
        fprintf(stderr, "Invalid listener (NULL)\n");
        return;
    }

    EventSystem* system = &event_system[type];

    if (system->listener_count >= system->listener_capacity) {
        system->listener_capacity *= 2;
        system->listeners = (EventListener*)realloc(system->listeners, system->listener_capacity * sizeof(EventListener));
        if (!system->listeners) {
            fprintf(stderr, "Failed to resize event listeners\n");
            return;
        }
    }

    system->listeners[system->listener_count++] = listener;
}

void unregister_listener(EventType type, EventListener listener) {
    if (!event_system || type >= EVENT_MAX) {
        fprintf(stderr, "Invalid event type %d\n", type);
        return;
    }

    if (!listener) {
        fprintf(stderr, "Invalid listener (NULL)\n");
        return;
    }

    EventSystem* system = &event_system[type];

    for (int i = 0; i < system->listener_count; ++i) {
        if (system->listeners[i] == listener) {
            system->listeners[i] = system->listeners[system->listener_count - 1];
            system->listener_count--;
            return;
        }
    }

    fprintf(stderr, "Listener not found for event type %d\n", type);
}

void trigger_event(EventType type, void* data, bool free_data) {
    if (!event_queue) {
        fprintf(stderr, "Event queue not initialized\n");
        return;
    }

    if (!data) {
        fprintf(stderr, "Warning: Attempted to trigger event with NULL data\n");
        return;
    }

    enqueue_event(event_queue, type, data, free_data);
}

void call_events() {
    if (!event_queue) {
        fprintf(stderr, "Event queue not initialized\n");
        return;
    }

    while(event_queue->size > 0) {
        Event event = dequeue_event(event_queue);

        // Call listeners for the event type
        EventSystem* system = &event_system[event.type];
        if (!system) continue;

        for (int i = 0; i < system->listener_count; i++) {
            if(system->listeners[i]) {
                system->listeners[i](event);
            }
        }

        // Free event data if required
        if (event.free_data && event.data) {
            free(event.data);
            event.data = NULL;
        }
    }
}