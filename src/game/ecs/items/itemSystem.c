#include "itemSystem.h"

bool create_item(ECS_Manager *manager, uint32_t entity, ItemComponent itemC) {
    ItemComponent *item = ECS_AddComponent(manager, entity, ITEM, sizeof(ItemComponent));
    item->id = itemC.id; ;
    item->name = itemC.name;
    item->description = itemC.description;
    item->value = itemC.value;
    return true;
}

bool destroy_item(ECS_Manager *manager, uint32_t entity, ItemComponent itemC)
{
    // TODO: Implement this function
    return false;    
}

ItemComponent get_item_by_id(ECS_Manager *manager, uint32_t entity, int id)
{
    ItemComponent *item = ECS_GetComponent(manager, entity, ITEM);
    for (int i = 0; i < 50; i++)
    {
        if (item->id == id)
        {
            return *item;
        }
    }
    return (ItemComponent){-1, "NULL", "NULL", 0};
}
