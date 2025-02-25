#include "bossStates.h"

AttackBossStateVars* create_attack_boss_vars(uint32_t player) {
    AttackBossStateVars* vars = malloc(sizeof(AttackBossStateVars));
    vars->player = player;

    vars->center_x = 0;
    vars->center_y = 0;

    vars->sleep_time = 5000;
    vars->time_between_attacks = 1000;

    vars->nb_attacks = 3;
    vars->last_attack = SDL_GetTicks();
    vars->last_sleep = SDL_GetTicks();

    vars->wander_radius = 150.0f;  // Maximum distance from center to wander
    vars->movement_speed = 1.0f;   // Adjusted for smoother movement
    vars->target_x = 0;
    vars->target_y = 0;
    vars->next_target_time = SDL_GetTicks();
    return vars;
}

void on_attack_boss_enter(State* state, uint32_t id) {
    AttackBossStateVars* vars = (AttackBossStateVars*)state->vars;
    if(!vars) return;

    PositionComponent* position = ECS_GetComponent(id, POSITION);
    if(!position) return;

    vars->center_x = (0.5 + position->x / 1920) * 1920;
    vars->center_y = (0.5 + position->y / 1280) * 1280;

    vars->last_attack = SDL_GetTicks();
    vars->last_sleep = SDL_GetTicks();
    vars->nb_attacks = 0;

    vars->target_x = vars->center_x;
    vars->target_y = vars->center_y;
    vars->next_target_time = SDL_GetTicks();
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

    // Pick new target position every few seconds
    if (current_time > vars->next_target_time) {
        // Random angle and distance within wander radius
        float angle = (float)(rand() % 360) * PI / 180.0f;
        float distance = ((float)rand() / RAND_MAX) * vars->wander_radius;
        
        vars->target_x = vars->center_x + cos(angle) * distance;
        vars->target_y = vars->center_y + sin(angle) * distance;
        vars->next_target_time = current_time + 2000; // New target every 2 seconds
    }

    // Move towards target position
    float dx = vars->target_x - position->x;
    float dy = vars->target_y - position->y;
    float distance = sqrt(dx * dx + dy * dy);
    
    if (distance > 5.0f) {
        float normalized_dx = dx / distance;
        float normalized_dy = dy / distance;
        position->x += normalized_dx * vars->movement_speed;
        position->y += normalized_dy * vars->movement_speed;
    }

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
                cos(angle_rad) * 30, sin(angle_rad) * 30
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
    if(vars) free(vars);
}