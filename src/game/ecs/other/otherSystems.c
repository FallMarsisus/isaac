#include "otherSystems.h"

void init_parent_component(ParentComponent* parent) {
    parent->children = create_id_array();
}
void init_child_component(ChildComponent* child, float x, float y, bool relative, uint32_t parent) {
    child->offsetX = x;
    child->offsetY = y;
    child->is_relative = relative;
    child->parent = parent;
}
void init_effect_component(EffectComponent* effect, uint32_t id, float timeLeft) {
    effect->id = id;

    effect->time = timeLeft * 1000;
    effect->end_time = SDL_GetTicks() + effect->time;
    effect->has_physics = false;
}

void add_child(ParentComponent* parent, uint32_t id) {
    add_id(parent->children, id);
}
void remove_child(ParentComponent* parent, uint32_t id) {
    remove_id(parent->children, id);
}

void free_script_component(ScriptComponent* script) {
    if(script) {
        free(script->data);
        script->data = NULL;
		script->update = NULL;
    }
}
void free_parent_component(ParentComponent* parent) {
    if(parent) free_id_array(parent->children);
}
void free_all_other_components(uint32_t id) {
    ParentComponent* parent = ECS_GetComponent(id, PARENT);
    if(parent) {
        free_parent_component(parent);
    }
    ScriptComponent* script = ECS_GetComponent(id, SCRIPT);
    if(script) {
        free_script_component(script);
    }
}

void update_others(uint32_t id, SDL_Rect cam, uint32_t* entities, int amount) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    ChildComponent* childComp = ECS_GetComponent(id, CHILD);
    if(childComp && position) {
        PositionComponent* posParent = ECS_GetComponent(childComp->parent, POSITION);
        if(posParent && childComp->is_relative) {
            position->x = posParent->x + childComp->offsetX;
            position->y = posParent->y + childComp->offsetY;
        }
    }

    ScriptComponent* script = ECS_GetComponent(id, SCRIPT);
    if(script && script->update) {
        script->update(id, cam, entities, amount);
    }

    EffectComponent* effectComp = ECS_GetComponent(id, EFFECT);
    if(effectComp) {
        if(SDL_GetTicks() > effectComp->end_time) {
            ECS_RemoveEntity(id);
        }
    }
}