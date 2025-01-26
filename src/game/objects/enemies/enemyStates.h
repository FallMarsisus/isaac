#pragma once

#include "../../../utils/vector.h"
#include "../../../utils/utils.h"

#include "../../stateMachine/stateMachine.h"

#include "../../event/eventSystem.h"

#include "../../ecs/ecs.h"
#include "../../ecs/components.h"

#include "../../ecs/render/renderSystems.h"

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