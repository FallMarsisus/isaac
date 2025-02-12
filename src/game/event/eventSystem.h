#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "eventList.h"

typedef enum {
    EVENT_NONE,
    EVENT_PLAYER_MOVED,
    EVENT_PLAYER_DAMAGED,
    EVENT_CHEST_OPENED,
    EVENT_STATE_CHANGE,
    EVENT_COLLISION,
    EVENT_ENTITY_REMOVED,
    EVENT_MAX // Safeguard to know the number of event types
} EventType;

typedef struct {
    EventType type;
    void* data;
    bool free_data; // Whether to free data after processing
} Event;

// Function run when an event is fired
typedef void (*EventListener)(Event event);

typedef struct {
    EventListener* listeners;
    int listener_count;
    int listener_capacity;
} EventSystem;

typedef struct {
    Event* events;
    int front;
    int rear;
    int size;
    int capacity;
} EventQueue;

// Initialize the event system
void init_event_system();

// Free the event system
void free_event_system();

// Register a listener for a specific event type
void register_listener(EventType type, EventListener listener);

// Unregister a listener for a specific event type
void unregister_listener(EventType type, EventListener listener);

/*
Add an event to the event queue
- `type`: The type of event.
- `data`: The event data (must be allocated by the caller).
- `free_data`: Whether the event system should free the data after processing.
*/
void trigger_event(EventType type, void* data, bool free_data);

// Process all pending events
void call_events();

// Helper functions
EventQueue* create_event_queue(int initial_capacity);
void free_event_queue(EventQueue* queue);
void enqueue_event(EventQueue* queue, EventType type, void* data, bool free_data);
Event dequeue_event(EventQueue* queue);