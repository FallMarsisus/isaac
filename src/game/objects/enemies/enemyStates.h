#pragma once

#include "../../../utils/vector.h"
#include "../../../utils/utils.h"
#include "../../stateMachine/stateMachine.h"
#include "../../event/eventSystem.h"
#include "../../ecs/ecs.h"
#include "../../ecs/components.h"

typedef struct {
    Vector direction;
    float speed;
    float wander_time;
} IdleStateVars;

IdleStateVars* create_idle_vars();
void on_idle_enter(State* state, uint32_t id);
void on_idle_update(State* state, uint32_t id);
void on_idle_exit(State* state, uint32_t id);
void on_idle_free(State* state, uint32_t id);
