#pragma once

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../ecs/inventory/inventorySystem.h"
#include "../../ecs/inventory/inventoryUI.h"
#include "../../ecs/sword/swordSystem.h"
#include "../enemies/enemies.h"

#include "../../../menu/menuManager.h"

typedef struct {
    float speed;
    Vector direction;
	int win_width;
	int true_width;
} PlayerData;

void init_player(ScriptComponent* script, int win_width, int true_width);

void update_player(u_int32_t player, SDL_Rect cam, uint32_t* entities, int amount);