#include "enemies.h"

uint32_t add_enemy(float x, float y, uint32_t pl) {
    uint32_t enemy = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(enemy, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(enemy, SPRITE, sizeof(SpriteComponent));
    AnimationComponent* animation = ECS_AddComponent(enemy, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(enemy, BODY, sizeof(RigidbodyComponent));
    StateMachineComponent* sm = ECS_AddComponent(enemy, STATE_MACHINE, sizeof(StateMachineComponent));
    HealthComponent* health = ECS_AddComponent(enemy, HEALTH, sizeof(HealthComponent));
    
    init_health_component(enemy, 1, 10, 0);
    init_state_machine(sm, enemy);

    State* idle_state = create_state("idle", on_idle_enter, on_idle_update, on_idle_exit, on_idle_free);
    idle_state->vars = create_idle_vars(pl);
    add_state(sm , idle_state);

    State* chase_state = create_state("chase", on_chase_enter, on_chase_update, on_chase_exit, on_chase_free);
    chase_state->vars = create_chase_vars(pl);
    add_state(sm, chase_state);

    switch_state(sm, "idle");
    
    create_damager(enemy, (DamagerComponent) {1, 0, false, 0});
    // Initialize components
    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_rigidbody_component(body, 2, 2, 60, 60);
    body->is_dynamic = true;

    init_sprite_component(sprite, 64, 64, get_sprites()->goblin_texture);
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    play_anim(animation);

    return enemy;
}

uint32_t get_nearest_enemy(uint32_t entity)
{
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    int min_dist = 1000;
    int nearest_enemy = -1;

    for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
        Node* current = ECS_GetManager()->st->dict->array[i];
        while (current) {
            if(current->key == entity) {
                current = current->next;
                continue;
            }
            
            PositionComponent* enemy_pos = ECS_GetComponent(current->key, POSITION);
            if (enemy_pos) {
                int dist = sqrt(pow(pos->x - enemy_pos->x, 2) + pow(pos->y - enemy_pos->y, 2));
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_enemy = current->key;
                }
            }
            current = current->next;
        }
    }
    return nearest_enemy;
}

bool is_colliding_with_enemy(uint32_t entity) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    RigidbodyComponent* body = ECS_GetComponent(entity, BODY);
    
    for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
        Node* current = ECS_GetManager()->st->dict->array[i];
        while (current) {
            if(current->key == entity) {
                current = current->next;
                continue;
            }
            PositionComponent* enemy_pos = ECS_GetComponent(current->key, POSITION);
            RigidbodyComponent* enemy_body = ECS_GetComponent(current->key, BODY);
            DamagerComponent* damager = ECS_GetComponent(current->key, DAMAGER);
            if( damager == NULL) {
                current = current->next;
                continue;
            }
            if (isColliding(pos, body, enemy_pos, enemy_body)) return true;
            
            current = current->next;
        }
    }
    return false ;
}
