#include "others.h"

uint32_t add_effect(float x, float y, float time, int width, int height, SDL_Texture* texture) {
    uint32_t effect = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(effect, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(effect, SPRITE, sizeof(SpriteComponent));
    EffectComponent* effectComp = ECS_AddComponent(effect, EFFECT, sizeof(EffectComponent));
    RigidbodyComponent* body = ECS_AddComponent(effect, BODY, sizeof(RigidbodyComponent));
    
    init_rigidbody_component(body, 2, 2, width - 4, height - 4, 3, NULL);
    
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
    init_anim_component(animation, 16, 16);

    DamagerComponent* damager = ECS_AddComponent(flame, DAMAGER, sizeof(DamagerComponent));
    init_damager_component(damager, 2, true);
    
    add_anim(animation, 0.1, 3);
    set_active_anim(animation, 0);
    play_anim(animation);

    RigidbodyComponent* body = ECS_GetComponent(flame, BODY);
    ID_array* col_layers_mask = create_id_array();
    add_id(col_layers_mask, 2); // Layer 2 is the player
    body->layer_that_collides_with = col_layers_mask;
    
    return flame;
}

uint32_t add_sword(uint32_t player, SwordComponent* sword_component, float offsetX, float offsetY) {
    //Player relative components
    PositionComponent* player_position = ECS_GetComponent(player, POSITION);
    if (!player_position) {
        printf("Position component not found for entity %u\n", player);
        return SDL_MAX_UINT32;
    }

    ParentComponent* parent = ECS_GetComponent(player, PARENT);
    SpriteComponent* player_sprite = ECS_GetComponent(player, SPRITE);

    int width = 128;
    int height = 128;

    //Sword relative components
    uint32_t sword = add_effect(
        player_position->x + offsetX + (player_sprite->width - width) / 2, 
        player_position->y + offsetY + (player_sprite->height - height) / 2, 
        0.15, width, height, sword_component->renderer
    );
    
    SpriteComponent* sprite = ECS_GetComponent(sword, SPRITE);

    //Animation
    AnimationComponent* animation = ECS_AddComponent(sword, ANIMATION, sizeof(AnimationComponent));
    
    init_anim_component(animation, 32, 32);
    sprite->angle = atan2(offsetY, offsetX) * 180 / PI;
    
    add_anim(animation, 0.05, 3);
    set_active_anim(animation, 0);
    play_anim(animation);

    //Child
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

    RigidbodyComponent* body = ECS_GetComponent(sword, BODY);
    
    ID_array* col_layers_mask = create_id_array();
    add_id(col_layers_mask, 1); // Layer 1 is the enemy
    body->layer_that_collides_with = col_layers_mask;

    DamagerComponent* damager = ECS_AddComponent(sword, DAMAGER, sizeof(DamagerComponent));
    init_damager_component(damager, 5, false);

    return sword;
}