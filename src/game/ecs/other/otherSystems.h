#pragma once

#include "../components.h"
#include "../ecs.h"

void init_timer_component(TimerComponent* timer, float chrono);
void init_target_component(TargetMovementComponent* targetComp, u_int32_t target);
void init_script_component(ScriptComponent* script, void (*update)(uint32_t entity, ECS_Manager* ecs));

void update_others(uint32_t id, ECS_Manager* ecs);