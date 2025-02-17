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
    sword->renderer = get_sprites()->sword_texture; // TEMP
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

    // Display sword sprite next to the player
    
    if (!pos1) {
        printf("Position component not found for entity %u\n", entity);
        return SDL_MAX_UINT32;
    }

    // Display sword sprite next to the player
    
    int sword_x = pos1->x;
    int sword_y = pos1->y;
    float angle = 0.0f;
    int w = 25;
    int h = 38;

    if (pos1->vx >= 1 && pos1->vy == 0) {
        sword_x += 1920 / 64;
        sword->renderer = get_sprites()->sword_texture;
        angle = 0.0f; // Right
    } else if (pos1->vx <= -1 && pos1->vy == 0) {
        sword_x -= 1920 / 64;
        sword->renderer = get_sprites()->sword_mirror_texture;
        angle = 180.0f; // Left
    } else {
        w = 38;
        h = 25;
        if (pos1->vx == 0 && pos1->vy >= 1) {
        sword_y += 1920 / 64;
        sword->renderer = get_sprites()->sword_down_texture;
        angle = 90.0f; // Down
        } else if (pos1->vx == 0 && pos1->vy <= -1) {
            sword_y -= 1920 / 64;
            angle = 270.0f; // Up
            sword->renderer = get_sprites()->sword_up_texture;
        } else if (pos1->vx >= 1 && pos1->vy >= 1) {
            sword_x += 1920 / 64;
            sword_y += 1920 / 64;
        
            sword->renderer = get_sprites()->sword_down_texture;
            angle = 45.0f; // Down-Right
        } else if (pos1->vx <= -1 && pos1->vy <= -1) {
            sword_x -= 1920 / 64;
            sword_y -= 1920 / 64;
            sword->renderer = get_sprites()->sword_up_texture;
            angle = 225.0f; // Up-Left
        } else if (pos1->vx >= 1 && pos1->vy <= -1) {
            sword_x += 1920 / 64;
            sword_y -= 1920 / 64;
            sword->renderer = get_sprites()->sword_up_texture;
            angle = 315.0f; // Up-Right
        } else if (pos1->vx <= -1 && pos1->vy >= 1) {
            sword_x -= 1920 / 64;
            sword_y += 1920 / 64;
            sword->renderer = get_sprites()->sword_down_texture;
            angle = 135.0f; // Down-Left
        }
    }


    uint32_t sword_temp = add_effect(sword_x, sword_y, sword->renderer);
    AnimationComponent* animation = ECS_AddComponent(sword_temp, ANIMATION, sizeof(AnimationComponent));
    init_anim_component(animation, w, h);
    add_anim(animation, 0.1, 6);
    set_active_anim(animation, 0);
    play_anim(animation);



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
