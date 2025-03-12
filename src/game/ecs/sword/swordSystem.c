#include "swordSystem.h"
#include <stdio.h>
#include "../../../display.h"
#include "../../objects/blocks/blocks.h"
#include "../../../utils/ouputColors.h"

bool create_sword(SwordComponent* sword, enum ItemID id, int damage, int range, int cooldown)
{
    if(!sword) return false;

    sword->id = id;
    sword->damage = damage;
    sword->range = range;
    sword->cooldown = cooldown;
    sword->renderer = get_sprites()->sword_texture; // TEMP
    return true;
}


uint32_t use_sword(uint32_t entity, float offsetX, float offsetY) {
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

    uint32_t sword_temp = add_sword(entity, sword, offsetX, offsetY);
    
    return sword_temp;
    /*
    PositionComponent* pos2 = ECS_GetComponent(enemy, POSITION);
    if (!pos2) {
        printf("Position component not found for enemy %u\n", enemy);
        return sword_temp;
    }

    int dx = pos2->x - pos1->x;
    int dy = pos2->y - pos1->y;
    int distance = sqrt(dx * dx + dy * dy);
    // printf("Enemy %u is at distance %d from entity %u\n", enemy, distance, entity);

    if (distance <= sword->range) {
        // printf("Enemy %u is within range of entity %u\n", enemy, entity);
        // Collision detected, apply damage or other logic
        HealthComponent* health = ECS_GetComponent(enemy, HEALTH);
        if (health) {
            printf("Applying %d damage to enemy %u\n", sword->damage, enemy);
            
            // Calculer la direction du knockback basée sur la différence de position
            float dx = pos2->x - pos1->x;
            float dy = pos2->y - pos1->y;
            // Normaliser le vecteur
            float len = sqrt(dx*dx + dy*dy);
            if (len > 0) {
                dx /= len;
                dy /= len;
            }
            
            float* knockbackArgs = malloc(sizeof(float) * 3);
            knockbackArgs[0] = dx * 2.0f;       // Vitesse X réduite
            knockbackArgs[1] = dy * 2.0f;       // Vitesse Y réduite
            knockbackArgs[2] = 500.0f;         // Force réduite
            
            RigidbodyComponent* enemyBody = ECS_GetComponent(enemy, BODY);
            PositionComponent* enemyPosition = ECS_GetComponent(enemy, POSITION);
            if (enemyBody) {
                enemyBody->is_dynamic = true;
                enemyPosition->vx = 0;  // Réinitialiser la vitesse actuelle
                enemyPosition->vy = 0;
                Force* knockback = create_force(knockback_force, knockbackArgs);
                add_force(enemy, knockback);
            }

            damage(enemy, sword->damage);
            
            // Mettre à jour le stun existant au lieu d'en créer un nouveau
            StunComponent* stun = ECS_GetComponent(enemy, STUN);
            if (stun) {
                stun->duration = 500;  // Stun de 500ms
                stun->start_time = SDL_GetTicks();
            }
        } else {
            printf(RED "Health component not found for enemy %u\n" RESET, enemy);
        }
    }
    
    return sword_temp;
    */
}



void free_sword_component(SwordComponent *sword)
{
    //// TODO ^^
}
