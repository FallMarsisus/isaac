#include "swordSystem.h"
#include <stdio.h>

bool create_sword(uint32_t entity, enum ItemID id, int damage, int range, int cooldown)
{
    SwordComponent* sword = ECS_AddComponent(entity, SWORD_C, sizeof(SwordComponent));
    sword->id = id;
    sword->damage = damage;
    sword->range = range;
    sword->cooldown = cooldown;
    return true;

}
bool use_sword(uint32_t entity, uint32_t enemy)
{
    SwordComponent* sword = ECS_GetComponent(entity, SWORD_C);
    if (!sword) {
        printf("Sword component not found for entity %u\n", entity);
        return false;
    }

    PositionComponent* pos1 = ECS_GetComponent(entity, POSITION);
    if (!pos1) {
        printf("Position component not found for entity %u\n", entity);
        return false;
    }

    if(enemy == -1) return false;
    PositionComponent* pos2 = ECS_GetComponent(enemy, POSITION);
    if (!pos2) {
        printf("Position component not found for enemy %u\n", enemy);
        return false;
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

    return true;
}

void free_sword_component(SwordComponent *sword)
{
    //// TODO ^^
}
