#include "itemSystem.h"
#include "itemList.h"
#include "../../../display.h"
#include "../ecs.h"
#include "../inventory/inventoryComponent.h"
#include "../inventory/inventorySystem.h"

SDL_Texture* get_texture_from_Id(enum ItemID id) {
    switch (id)
    {
    case POTION:
        // A FAIRE
        break;
    
    case APPLE:
        return get_sprites()->apple_item_texture;
        break;

    case SWORD:
        return get_sprites()->sword_slash; // A CHANGER
        break;

    case KEY:
        // A FAIRE
        break;

    case SHIELD:
        // A FAIRE
        break;

    default:
        return get_sprites()->apple_item_texture;
        break;
    }

    // Pour que les cas pas encore faits ne fassent pas crash le jeu
    return get_sprites()->apple_item_texture;
}

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity) {
    ItemComponent* item = ECS_GetComponent(itemEntity, ITEM);

    if (!item) {
        return false;
    }

    return add_item_to_inventory(targetEntity, item->item);
}
