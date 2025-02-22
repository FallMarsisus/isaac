#include "damagerSystem.h"

bool create_damager(uint32_t entity, DamagerComponent damagerC)
{
    DamagerComponent* damager = ECS_AddComponent(entity, DAMAGER, sizeof(DamagerComponent));
    damager->damage = damagerC.damage;
    damager->knockback = damagerC.knockback;
    damager->is_poisonnous = damagerC.is_poisonnous;

    return true;
}

bool destroy_damager(uint32_t entity, DamagerComponent damager)
{
    return false;
}

bool apply_damage(uint32_t entity, uint32_t target)
{
    DamagerComponent* damager = ECS_GetComponent(entity, DAMAGER);
    HealthComponent* health = ECS_GetComponent(target, HEALTH);

    if (damager == NULL|| health == NULL) {
        return false;
    }

    damage(target, damager->damage);

    return true;
}