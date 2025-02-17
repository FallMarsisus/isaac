#include "bossStates.h"

AttackBossStateVars* create_attack_boss_vars(uint32_t player) {
    AttackBossStateVars* vars = malloc(sizeof(AttackBossStateVars));
    vars->player = player;
    return vars;
}

void on_attack_boss_enter(State* state, uint32_t id) {
    AttackBossStateVars* vars = (AttackBossStateVars*)state->vars;
    vars->last_attack = SDL_GetTicks();
    vars->attack_cooldown = 500;
}
void on_attack_boss_update(State* state, uint32_t id) {
    AttackBossStateVars* vars = (AttackBossStateVars*)state->vars;
    if(!vars) return;

    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    if(!position || !sprite) return;

    PositionComponent* player_position = ECS_GetComponent(vars->player, POSITION);
    SpriteComponent* player_sprite = ECS_GetComponent(vars->player, SPRITE);
    if(!player_position || !player_sprite) return;

    if(SDL_GetTicks() - vars->last_attack > vars->attack_cooldown) {
        vars->last_attack = SDL_GetTicks();
        
        // Calculate direction vector from position to player
        float dx = player_position->x - position->x + (player_sprite->width - sprite->width) / 2;
        float dy = player_position->y - position->y + (player_sprite->height - sprite->height) / 2;
        
        // Normalize the direction vector
        float length = sqrt(dx*dx + dy*dy);
        float vx = dx/length;
        float vy = dy/length;

        add_flame(position->x + sprite->width / 2, position->y + sprite->height / 2, vx, vy);
    }
}
void on_attack_boss_exit(State* state, uint32_t id) {
    AttackBossStateVars* vars = (AttackBossStateVars*)state->vars;
    if(!vars) return;
}
void on_attack_boss_free(State* state, uint32_t id) {
    AttackBossStateVars* vars = (AttackBossStateVars*)state->vars;
    if(vars) return; free(vars);
}