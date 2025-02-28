#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "eventList.h"

typedef struct {
    EventType type;
    void* data;
    bool free_data;
} Event;

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

void init_event_system();
void free_event_system();

void register_listener(EventType type, EventListener listener);
void unregister_listener(EventType type, EventListener listener);

/*
Add an event to the event queue
- `type`: The type of event.
- `data`: The event data (must be allocated by the caller).
- `free_data`: Whether the event system should free the data after processing.
*/
void trigger_event(EventType type, void* data, bool free_data);

void call_events();

EventQueue* create_event_queue(int initial_capacity);
void free_event_queue(EventQueue* queue);
void enqueue_event(EventQueue* queue, EventType type, void* data, bool free_data);
Event dequeue_event(EventQueue* queue);