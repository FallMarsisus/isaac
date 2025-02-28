#pragma once

#include <stdint.h>

typedef struct state_s State;

typedef enum {
    EVENT_NONE,
    EVENT_PLAYER_MOVED,
    EVENT_PLAYER_DAMAGED,
    EVENT_CHEST_OPENED,
    EVENT_STATE_CHANGE,
    EVENT_COLLISION,
    EVENT_ENTITY_CREATED,
    EVENT_ENTITY_REMOVED,
    EVENT_GAME_OVER,
    EVENT_MENU_CHANGE,
    EVENT_MAX // Safeguard to know the number of event types
} EventType;

typedef struct {
    int player_id;
    float new_x, new_y;
} PlayerMovedEvent;

typedef struct {
    int player_id;
    int damage_taken;
} PlayerDamagedEvent;

typedef struct {
    int chest_id;
    int player_id;
    float x, y;
} ChestOpenedEvent;

typedef struct {
    int id;
    char* new_state;
} StateChangeEvent;

typedef struct {
    uint32_t entity1;
    uint32_t entity2;
} CollisionEvent;

typedef struct {
    uint32_t entity;
} EntityCreatedEvent;

typedef struct {
    int entity;
} EntityRemovedEvent;

typedef struct {
    int player_id;
} GameOverEvent;
