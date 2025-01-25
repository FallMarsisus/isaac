#pragma once

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