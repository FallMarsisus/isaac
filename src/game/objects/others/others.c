#include "others.h"

uint32_t add_effect(float x, float y, float time, int width, int height, SDL_Texture* texture) {
    uint32_t effect = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(effect, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(effect, SPRITE, sizeof(SpriteComponent));
    EffectComponent* effectComp = ECS_AddComponent(effect, EFFECT, sizeof(EffectComponent));
    RigidbodyComponent* body = ECS_AddComponent(effect, BODY, sizeof(RigidbodyComponent));
    
    init_rigidbody_component(body, 0, 0, width, height);
    body->is_dynamic = true;
    body->mass = 1;

    init_effect_component(effectComp, effect, time);
    init_position_component(position, x, y);
    init_sprite_component(sprite, width, height, texture);

    return effect;
}

uint32_t add_projectile(float x, float y, float vx, float vy, float time, SDL_Texture* texture) {
    uint32_t projectile = add_effect(x, y, time, 64, 64, texture);

    EffectComponent* effect = ECS_GetComponent(projectile, EFFECT);
    effect->has_physics = true;

    RigidbodyComponent* body = ECS_GetComponent(projectile, BODY);
    apply_one_force(body, vx * 5, vy * 5);

    return projectile;
}

uint32_t add_flame(float x, float y, float vx, float vy) {
    uint32_t flame = add_projectile(x, y, vx, vy, 5, get_sprites()->flame_texture);
    AnimationComponent* animation = ECS_AddComponent(flame, ANIMATION, sizeof(AnimationComponent));

    create_damager(flame, (DamagerComponent) {1, 0, false, 0});
    
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 3);
    set_active_anim(animation, 0);
    play_anim(animation);
    
    return flame;
}

uint32_t add_sword(uint32_t player, SwordComponent* sword_component) {
    //Player relative components
    PlayerMovementComponent* player_movement = ECS_GetComponent(player, PLAYER);
    PositionComponent* player_position = ECS_GetComponent(player, POSITION);
    SpriteComponent* player_sprite = ECS_GetComponent(player, SPRITE);
    ParentComponent* parent = ECS_GetComponent(player, PARENT);

    if (!player_position || !player_movement) {
        printf("Position or movement component not found for entity %u\n", player);
        return SDL_MAX_UINT32;
    }

    int width = 128;
    int height = 128;
    float offsetX = player_movement->direction.x * 24;
    float offsetY = player_movement->direction.y * 24;

    //Sword relative components
    uint32_t sword = add_effect(
        player_position->x + offsetX + (player_sprite->width - width) / 2, 
        player_position->y + offsetY + (player_sprite->height - height) / 2, 
        0.15, width, height, sword_component->renderer
    );
    
    SpriteComponent* sprite = ECS_GetComponent(sword, SPRITE);
    AnimationComponent* animation = ECS_AddComponent(sword, ANIMATION, sizeof(AnimationComponent));

    sprite->angle = atan2(player_movement->direction.y, player_movement->direction.x) * 180 / PI;

    init_anim_component(animation, 32, 32);
    add_anim(animation, 0.05, 3);
    set_active_anim(animation, 0);
    play_anim(animation);

    if(parent) {
        ChildComponent* child = ECS_AddComponent(sword, CHILD, sizeof(ChildComponent));
        init_child_component(child, 
            offsetX + (player_sprite->width - width) / 2, 
            offsetY + (player_sprite->height - height) / 2, 
            true, 
            player
        );
        add_child(parent, sword);
    }
}