#include "damagerSystem.h"

bool create_damager(ECS_Manager *manager, uint32_t entity, DamagerComponent damagerC)
{
    DamagerComponent* damager = ECS_AddComponent(manager, entity, DAMAGER, sizeof(DamagerComponent));
    damager->damage = damagerC.damage;
    damager->knockback = damagerC.knockback;
    damager->is_poisonnous = damagerC.is_poisonnous;

    return true;
}

bool destroy_damager(ECS_Manager *manager, uint32_t entity, DamagerComponent damager)
{
    return false;
}

bool apply_damage(ECS_Manager *manager, uint32_t entity, uint32_t target)
{
    DamagerComponent* damager = ECS_GetComponent(manager, entity, DAMAGER);
    HealthComponent* health = ECS_GetComponent(manager, target, HEALTH);
    PlayerMovementComponent* player = ECS_GetComponent(manager, target, PLAYER);

    if (damager == NULL|| health == NULL || player == NULL) {
        return false;
    }

    damage(manager, target, damager->damage);
    // player->speed = -player->speed*2; NONONONONOONN

    return true;
}