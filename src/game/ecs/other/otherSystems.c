#include "otherSystems.h"

void init_timer_component(TimerComponent* timer, float chrono) {
    timer->last = SDL_GetTicks();
    timer->time = chrono;
}
void init_script_component(ScriptComponent* script, void (*update)(uint32_t entity, SDL_Rect cam)) {
    script->update = update;
}
void init_parent_component(ParentComponent* parent) {
    parent->children = create_id_array();
}
void init_child_component(ChildComponent* child, float x, float y, bool relative, uint32_t parent) {
    child->offsetX = x;
    child->offsetY = y;
    child->is_relative = relative;
    child->parent = parent;
}

void add_child(ParentComponent* parent, uint32_t id) {
    add_id(parent->children, id);
}

void free_parent_component(ParentComponent* parent) {
    if(parent) free_id_array(parent->children);
}
void free_all_other_components(uint32_t id) {
    ParentComponent* parent = ECS_GetComponent(id, PARENT);
    if(parent) free_parent_component(parent);
}

void update_others(uint32_t id, SDL_Rect cam) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    ScriptComponent* script = ECS_GetComponent(id, SCRIPT);

    ChildComponent* childComp = ECS_GetComponent(id, CHILD);

    if(childComp && position) {
        PositionComponent* posParent = ECS_GetComponent(childComp->parent, POSITION);
        if(posParent && childComp->is_relative) {
            position->x = posParent->x + childComp->offsetX;
            position->y = posParent->y + childComp->offsetY;
        }
    }

    if(script) {
        script->update(id, cam);
    }
}