#include "others.h"

uint32_t add_effect(float x, float y, float time, int width, int height, SDL_Texture* texture) {
    uint32_t effect = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(effect, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(effect, SPRITE, sizeof(SpriteComponent));
    EffectComponent* effectComp = ECS_AddComponent(effect, EFFECT, sizeof(EffectComponent));
    
    init_effect_component(effectComp, effect, time);

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_sprite_component(sprite, width, height, texture);

    return effect;
}

uint32_t add_projectile(float x, float y, float vx, float vy, float time, SDL_Texture* texture) {
    uint32_t projectile = add_effect(x, y, time, 64, 64, texture);
    PositionComponent* position = ECS_GetComponent(projectile, POSITION);
    position->vx = vx; position->vy = vy;

    return projectile;
}

uint32_t add_flame(float x, float y, float vx, float vy, float time) {
    uint32_t flame = add_projectile(x, y, vx, vy, time, get_sprites()->flame_texture);
    AnimationComponent* animation = ECS_AddComponent(flame, ANIMATION, sizeof(AnimationComponent));

    create_damager(flame, (DamagerComponent) {1, 0, false, 0});
    
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 3);
    set_active_anim(animation, 0);
    play_anim(animation);
    
    return flame;
}
