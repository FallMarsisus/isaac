#pragma once

typedef struct state_s State;

typedef struct {
    u_int32_t player_id;
    float new_x, new_y;
} PlayerMovedEvent;

typedef struct {
    u_int32_t player_id;
    int damage_taken;
} PlayerDamagedEvent;

typedef struct {
    u_int32_t chest_id;
    u_int32_t player_id;
    float x, y;
} ChestOpenedEvent;

typedef struct {
    u_int32_t id;
    char* new_state;
} StateChangeEvent;