#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "eventList.h"

typedef enum {
    EVENT_NONE,
    EVENT_PLAYER_MOVED,
    EVENT_PLAYER_DAMAGED,
    EVENT_CHEST_OPENED,
    EVENT_STATE_CHANGE,
    EVENT_MAX // Safeguard to know the number of event types
} EventType;

typedef struct {
    EventType type;
    void* data;
} Event;

//Function run when an event is fired
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
WARNING : ALWAYS MALLOC THE DATA FIRST
*/
void trigger_event(EventType type, void* data);

//Dequeues all pending events
void call_events();