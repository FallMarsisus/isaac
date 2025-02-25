#pragma once

#include "../../../utils/vector.h"
#include "../../../utils/utils.h"

#include "../../stateMachine/stateMachine.h"

#include "../../event/eventSystem.h"

#include "../../ecs/ecs.h"
#include "../../ecs/components.h"

#include "../../ecs/render/renderSystems.h"
#include "../../ecs/physics/physicsSystems.h"

#include "../../../utils/betterArrays/queue.h"

#include "../others/others.h"

#include "../../../utils/pi.h"

typedef struct {
    uint32_t player;
    float speed;

    int last_sleep;
    int sleep_time;

    int nb_attacks;
    int last_attack;
    int time_between_attacks;

    float target_x;        // Add target position for wandering
    float target_y;
    float wander_radius;   // Maximum distance from center
    int next_target_time;  // When to pick a new target
    
    float center_x;
    float center_y;
    float movement_speed;   // Adjusted for smoother movement
} AttackBossStateVars;

AttackBossStateVars* create_attack_boss_vars(uint32_t player);
void on_attack_boss_enter(State* state, uint32_t id);
void on_attack_boss_update(State* state, uint32_t id);
void on_attack_boss_exit(State* state, uint32_t id);
void on_attack_boss_free(State* state, uint32_t id);