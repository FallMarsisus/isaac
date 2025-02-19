#include "bossStates.h"

AttackBossStateVars* create_attack_boss_vars(uint32_t player) {
    AttackBossStateVars* vars = malloc(sizeof(AttackBossStateVars));
    vars->player = player;

    vars->last_sleep = SDL_GetTicks();
    vars->sleep_time = 1000;

    vars->nb_attacks = 3;
    vars->last_attack = SDL_GetTicks();
    vars->time_between_attacks = 1000;
    return vars;
}

void on_attack_boss_enter(State* state, uint32_t id) {
    AttackBossStateVars* vars = (AttackBossStateVars*)state->vars;
    vars->last_attack = SDL_GetTicks();
    vars->last_sleep = SDL_GetTicks();
    vars->sleep_time = 5 * 1000;
    vars->nb_attacks = 0;
    vars->time_between_attacks = 1000;
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

    int current_time = SDL_GetTicks();

    //Check if wakes up
    if(vars->nb_attacks > 3 && current_time > vars->sleep_time + vars->last_sleep) {
        vars->nb_attacks = 0;
    }
    else if(vars->nb_attacks <= 3 && current_time > vars->time_between_attacks + vars->last_attack) {
        printf("Attacking\n");
        vars->nb_attacks++;
        vars->last_attack = current_time;

        if(vars->nb_attacks > 3) {
            //Make him sleep
            vars->last_sleep = current_time;
            printf("Sleeping\n");
        }

        // Calculate direction vector from position to player
        float dx = player_position->x - position->x + (player_sprite->width - sprite->width) / 2;
        float dy = player_position->y - position->y + (player_sprite->height - sprite->height) / 2;

        float angle = atan2(dy, dx) * 180 / PI;
        for(int i = -2; i < 3; i++) {
            float angle_rad = ((angle + i * 10) * PI / 180.0);
            uint32_t flame = add_flame(
                position->x + (sprite->width - 64) / 2, 
                position->y + (sprite->height - 64) / 2, 
                cos(angle_rad) * 6, sin(angle_rad) * 6
            );
            SpriteComponent* flame_sprite = ECS_GetComponent(flame, SPRITE);
            if(flame_sprite) {
                flame_sprite->angle = 90 + angle + i * 10;
            }
        }
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