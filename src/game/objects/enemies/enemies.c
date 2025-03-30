#include "enemies.h"

uint32_t add_standard_enemy(float x, float y, int width, int height, uint32_t pl, SDL_Texture* texture) {
    uint32_t enemy = ECS_CreateEntity();


	if (enemy == 137) {
		printf("enemy ID is 137\n");
	}

    PositionComponent* position = ECS_AddComponent(enemy, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(enemy, SPRITE, sizeof(SpriteComponent));
    RigidbodyComponent* body = ECS_AddComponent(enemy, BODY, sizeof(RigidbodyComponent));
    StateMachineComponent* sm = ECS_AddComponent(enemy, STATE_MACHINE, sizeof(StateMachineComponent));
    HealthComponent* health = ECS_AddComponent(enemy, HEALTH, sizeof(HealthComponent));
    StunComponent* stun = ECS_AddComponent(enemy, STUN, sizeof(StunComponent));
    DamagerComponent* damager = ECS_AddComponent(enemy, DAMAGER, sizeof(DamagerComponent));

    init_health_component(health, 10);
    init_damager_component(damager, 2, true);

    stun->duration = 0;
    stun->start_time = 0;
    
    init_state_machine(sm, enemy);
    init_position_component(position, x, y);
    
    State* idle_state = create_state("idle", on_idle_enter, on_idle_update, on_idle_exit, on_idle_free);
    idle_state->vars = create_idle_vars(pl);
    add_state(sm , idle_state);

    State* chase_state = create_state("chase", on_chase_enter, on_chase_update, on_chase_exit, on_chase_free);
    chase_state->vars = create_chase_vars(pl);
    add_state(sm, chase_state);
    
    State* follow_state = create_state("follow", on_follow_enter, on_follow_update, on_follow_exit, on_follow_free);
    follow_state->vars = create_follow_vars(pl);
    add_state(sm, follow_state);
    
    switch_state(sm, "idle");

    ID_array* col_layers_mask = create_id_array();
    add_id(col_layers_mask, 0);
    add_id(col_layers_mask, 2);
    add_id(col_layers_mask, 3);
    init_rigidbody_component(body, 2, 2, width - 4, height - 4, 1, col_layers_mask);
    body->is_dynamic = true;

    init_sprite_component(sprite, width, height, texture);
    sprite->layer = 1;

    return enemy;
}
uint32_t add_goblin(float x, float y, uint32_t pl) {
    uint32_t goblin = add_standard_enemy(x, y, 64, 64, pl, get_sprites()->goblin_texture);


	if (goblin == 137) {
		printf("goblin ID is 137\n");
	}

    AnimationComponent* animation = ECS_AddComponent(goblin, ANIMATION, sizeof(AnimationComponent));
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    return goblin;
}
uint32_t add_slime(float x, float y, uint32_t pl) {
    uint32_t slime = add_standard_enemy(x, y, 64, 64, pl, get_sprites()->slime_texture);

	if (slime == 137) {
		printf("slime ID is 137\n");
	}

    AnimationComponent* animation = ECS_AddComponent(slime, ANIMATION, sizeof(AnimationComponent));
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 4);

    set_active_anim(animation, 0);
    return slime;
}
uint32_t add_alien(float x, float y, uint32_t pl) {
    uint32_t alien = add_standard_enemy(x, y, 64, 64, pl, get_sprites()->alien_texture);


	if (alien == 137) {
		printf("alin ID is 137\n");
	}

    AnimationComponent* animation = ECS_AddComponent(alien, ANIMATION, sizeof(AnimationComponent));
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    return alien;
}

uint32_t add_boss(float x, float y, uint32_t pl) {
    uint32_t boss = ECS_CreateEntity();

    int width = 150;
    int height = 150;

    PositionComponent* position = ECS_AddComponent(boss, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(boss, SPRITE, sizeof(SpriteComponent));
    RigidbodyComponent* body = ECS_AddComponent(boss, BODY, sizeof(RigidbodyComponent));
    StateMachineComponent* sm = ECS_AddComponent(boss, STATE_MACHINE, sizeof(StateMachineComponent));
    HealthComponent* health = ECS_AddComponent(boss, HEALTH, sizeof(HealthComponent));
    AnimationComponent* animation = ECS_AddComponent(boss, ANIMATION, sizeof(AnimationComponent));
    StunComponent* stun = ECS_AddComponent(boss, STUN, sizeof(StunComponent));
    DamagerComponent* damager = ECS_AddComponent(boss, DAMAGER, sizeof(DamagerComponent));

    init_health_component(health, 100);
    init_damager_component(damager, 4, true);

    stun->duration = 0;
    stun->start_time = 0;
    
    init_position_component(position, x, y);
    
    init_state_machine(sm, boss);

    State* attack_state = create_state("attack", on_attack_boss_enter, on_attack_boss_update, on_attack_boss_exit, on_attack_boss_free);
    attack_state->vars = create_attack_boss_vars(pl);
    add_state(sm, attack_state);
    
    switch_state(sm, "attack");

    ID_array* col_layers_mask = create_id_array();
    add_id(col_layers_mask, 0);
    add_id(col_layers_mask, 2);
    add_id(col_layers_mask, 3);
    init_rigidbody_component(body, 2, 2, width - 4, height - 4, 1, col_layers_mask);
    body->is_dynamic = true;

    init_sprite_component(sprite, width, height, get_sprites()->boss_texture);
    init_anim_component(animation, 50, 50);

    add_anim(animation, 0.1, 5);

    set_active_anim(animation, 0);
    play_anim(animation);
    return boss;
}