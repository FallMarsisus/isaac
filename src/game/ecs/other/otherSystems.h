#pragma once

#include "../components.h"
#include "../ecs.h"
#include "../../../utils/id_array.h"

void init_timer_component(TimerComponent* timer, float chrono);
void init_script_component(ScriptComponent* script, void (*update)(uint32_t entity, ECS_Manager* ecs));
void init_parent_component(ParentComponent* parent);
void init_child_component(ChildComponent* child, float x, float y, bool relative, uint32_t parent);

void add_child(ParentComponent* parent, uint32_t id);

void free_parent_component(ParentComponent* parent);
void free_all_other_components(ECS_Manager* ecs, uint32_t id);

void update_others(uint32_t id, ECS_Manager* ecs);