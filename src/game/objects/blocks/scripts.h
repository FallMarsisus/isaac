#pragma once

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

void init_teleporter(ScriptComponent* script, int posX, int posY);
void init_trap(ScriptComponent* script);

void update_teleporter(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount);
void update_trap(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount);