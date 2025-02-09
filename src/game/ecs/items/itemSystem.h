#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "itemData.h"
#include "itemComponent.h"
#include "../inventory/inventoryComponent.h"
#include "./itemsFunctions.h"

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity);

SDL_Texture* get_texture_from_Id(enum ItemID id);
Action* get_item_actions(enum ItemID id);

bool update_item(uint32_t entity, uint32_t player, uint32_t* other_entities, int nb_entities);
bool handle_collision_item(uint32_t entity1, uint32_t entity2);
void freeAction(Action* act);