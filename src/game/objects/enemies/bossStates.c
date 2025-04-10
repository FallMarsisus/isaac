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

    vars->wander_radius = 300.0f;
    vars->movement_speed = 1.0f;
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

    vars->center_x = floor(position->x / 1920) * 1920 + 960 - 64;
    vars->center_y = floor(position->y / 1280) * 1280 + 640 - 64;

    printf("Id : %d / Target : %f %f / Position : %f %f\n", 
        id,
        vars->center_x, vars->center_y,
        position->x, position->y
    );

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

    // Calculate direction vector from player to boss (for fleeing)
    float flee_dx = position->x - player_position->x;
    float flee_dy = position->y - player_position->y;
    float flee_distance = sqrt(flee_dx * flee_dx + flee_dy * flee_dy);

    // Update target position based on player proximity
    if (flee_distance < vars->wander_radius * 1.5f) {
        // Flee from player while considering center position
        float flee_weight = 0.7f;  // Weight for flee direction
        float center_weight = 0.3f; // Weight for center attraction
        
        // Normalize flee direction
        float normalized_flee_dx = flee_dx / flee_distance;
        float normalized_flee_dy = flee_dy / flee_distance;
        
        // Calculate direction to center
        float center_dx = vars->center_x - position->x;
        float center_dy = vars->center_y - position->y;
        float center_distance = sqrt(center_dx * center_dx + center_dy * center_dy);
        float normalized_center_dx = center_dx / (center_distance + 0.0001f);
        float normalized_center_dy = center_dy / (center_distance + 0.0001f);
        
        // Combine flee and center vectors
        vars->target_x = position->x + (normalized_flee_dx * flee_weight + normalized_center_dx * center_weight) * vars->wander_radius;
        vars->target_y = position->y + (normalized_flee_dy * flee_weight + normalized_center_dy * center_weight) * vars->wander_radius;
        
        // Clamp target position to maximum distance from center
        float dx_from_center = vars->target_x - vars->center_x;
        float dy_from_center = vars->target_y - vars->center_y;
        float dist_from_center = sqrt(dx_from_center * dx_from_center + dy_from_center * dy_from_center);
        if (dist_from_center > vars->wander_radius) {
            float scale = vars->wander_radius / dist_from_center;
            vars->target_x = vars->center_x + dx_from_center * scale;
            vars->target_y = vars->center_y + dy_from_center * scale;
        }
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

    // Check if wakes up
    if(vars->nb_attacks > 3 && current_time > vars->sleep_time + vars->last_sleep) {
        vars->nb_attacks = 0;
    }
    else if(vars->nb_attacks <= 3 && current_time > vars->time_between_attacks + vars->last_attack) {
        //printf("Attacking\n");
        vars->nb_attacks++;
        vars->last_attack = current_time;

        if(vars->nb_attacks > 3) {
            // Make him sleep
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