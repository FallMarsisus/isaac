#include "swordSystem.h"
#include <stdio.h>
#include "../../../display.h"
#include "../../objects/blocks/blocks.h"

bool create_sword(SwordComponent* sword, enum ItemID id, int damage, int range, int cooldown)
{
    if(!sword) return false;

    sword->id = id;
    sword->damage = damage;
    sword->range = range;
    sword->cooldown = cooldown;
    sword->renderer = get_sprites()->sword_item_texture; // TEMP
    return true;
}



uint32_t use_sword(uint32_t entity, uint32_t enemy)
{
    SwordComponent* sword = ECS_GetComponent(entity, SWORD_C);
    if (!sword) {
        printf("Sword component not found for entity %u\n", entity);
        return SDL_MAX_UINT32;
    }

    PositionComponent* pos1 = ECS_GetComponent(entity, POSITION);
    if (!pos1) {
        printf("Position component not found for entity %u\n", entity);
        return SDL_MAX_UINT32;
    }

    // Display sword sprite next to the player
    
    int sword_x = pos1->x + floor(pos1->vx)*(1280/64);
    int sword_y = pos1->y + floor(pos1->vy)*(1280/64);

    uint32_t sword_temp = add_block_without_rigidbody(sword_x, sword_y, sword->renderer);

    if(enemy == -1) return sword_temp;
    PositionComponent* pos2 = ECS_GetComponent(enemy, POSITION);
    if (!pos2) {
        printf("Position component not found for enemy %u\n", enemy);
        return sword_temp;
    }

    int dx = pos2->x - pos1->x;
    int dy = pos2->y - pos1->y;
    int distance = sqrt(dx * dx + dy * dy);

    printf("Enemy %u is at distance %d from entity %u\n", enemy, distance, entity);

    if (distance <= sword->range) {
        printf("Enemy %u is within range of entity %u\n", enemy, entity);
        // Collision detected, apply damage or other logic
        HealthComponent* health = ECS_GetComponent(enemy, HEALTH);
        if (health) {
            printf("Applying %d damage to enemy %u\n", sword->damage, enemy);
            damage(enemy, sword->damage);
        } else {
            printf("Health component not found for enemy %u\n", enemy);
        }
    }
    
    return sword_temp;
}



void free_sword_component(SwordComponent *sword)
{
    //// TODO ^^
}
