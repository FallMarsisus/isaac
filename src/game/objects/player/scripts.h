#pragma once

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../ecs/inventory/inventorySystem.h"
#include "../../ecs/inventory/inventoryUI.h"
#include "../../ecs/sword/swordSystem.h"
#include "../enemies/enemies.h"

typedef struct {
    float speed;
    Vector direction;
} PlayerData;

void init_player(ScriptComponent* script);

void update_player(u_int32_t player, SDL_Rect cam);