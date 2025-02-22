#pragma once

#include "../components.h"
#include "../ecs.h"
#include "../../../utils/betterArrays/id_array.h"

void init_parent_component(ParentComponent* parent);
void init_child_component(ChildComponent* child, float x, float y, bool relative, uint32_t parent);
void init_effect_component(EffectComponent* effect, uint32_t id, float time);

void add_child(ParentComponent* parent, uint32_t id);
void remove_child(ParentComponent* parent, uint32_t id);

void free_parent_component(ParentComponent* parent);
void free_script_component(ScriptComponent* script);
void free_all_other_components(uint32_t id);

void update_others(uint32_t id, SDL_Rect cam);