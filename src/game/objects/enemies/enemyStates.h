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

typedef struct {
    uint32_t player;
    Vector direction;
    float speed;

    float wander_time;
} IdleStateVars;

typedef struct {
    uint32_t player;
    float speed;
} ChaseStateVars;

typedef struct {
    QueueNode* currentGoal;

    float speed;
    uint32_t target;
} FollowStateVars;

IdleStateVars* create_idle_vars(uint32_t player);
void on_idle_enter(State* state, uint32_t id);
void on_idle_update(State* state, uint32_t id);
void on_idle_exit(State* state, uint32_t id);
void on_idle_free(State* state, uint32_t id);

ChaseStateVars* create_chase_vars(uint32_t player);
void on_chase_enter(State* state, uint32_t id);
void on_chase_update(State* state, uint32_t id);
void on_chase_exit(State* state, uint32_t id);
void on_chase_free(State* state, uint32_t id);

void init_player_positions(uint32_t id);
void free_player_positions();

void update_player_positions(uint32_t id);

FollowStateVars* create_follow_vars(uint32_t target);
void on_follow_enter(State* state, uint32_t id);
void on_follow_update(State* state, uint32_t id);
void on_follow_exit(State* state, uint32_t id);
void on_follow_free(State* state, uint32_t id);