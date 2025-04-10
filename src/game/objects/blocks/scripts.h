#pragma once

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

typedef struct {
    int posX;
    int posY;
} TeleporterData;

typedef struct {
    int last_change;
    int change_time;

    bool active;
} TrapData;

void init_teleporter(ScriptComponent* script, int posX, int posY);
void init_trap(ScriptComponent* script);

void update_teleporter(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount);
void update_trap(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount);