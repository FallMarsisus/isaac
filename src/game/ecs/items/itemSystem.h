#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "itemData.h"
#include "./itemComponent.h"
#include "../inventory/inventoryComponent.h"
#include "./itemsFunctions.h"

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity);

SDL_Texture* get_texture_from_Id(enum ItemID id);
Action* get_item_actions(enum ItemID id, slots slot);

void timer_make_item_grabbable_by_dropper(Timer* self, void* arguments);

/**
 * @brief Creates an item entity at specified coordinates
 * @param x X coordinate for item placement
 * @param y Y coordinate for item placement
 * @param itemType Data of item to create
 * @param dropper Entity ID of the entity that dropped the item (-1 if doesn't exist)
 * @param playerLocked Boolean indicating if the item is locked to the player
 * @return Entity ID of the created item entity
 */
uint32_t add_item_entity(float x, float y, ItemData itemType, uint32_t dropper, bool playerLocked);

bool update_item(uint32_t entity);
bool handle_collision_item(uint32_t entity1, uint32_t entity2);
void free_action(Action* act);